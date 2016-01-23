/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



tDEBUG      debug;



/*====================------------------------------------====================*/
/*===----                   standard program functions                 ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char        verstring    [500];

char*        /*--: return versioning information ---------[ leaf-- [ ------ ]-*/
PROG_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strlcpy (t, "[tcc built]", 15);
#elif  __GNUC__  > 0
   strlcpy (t, "[gnu gcc  ]", 15);
#elif  __CBANG__  > 0
   strlcpy (t, "[cbang    ]", 15);
#else
   strlcpy (t, "[unknown  ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, VER_NUM, VER_TXT);
   return verstring;
}

char         /*--: dispaly usage information -------------[ leaf-- [ ------ ]-*/
PROG_usage         (void)
{
   printf ("\n");
   printf ("eos        : clean, light, consistent, reliable system initialization\n");
   printf ("\n");
   printf ("usage      : eos [URGENTS] [OPTIONS]\n");
   printf ("\n");
   printf ("OPTIONS...\n");
   printf ("   -d,--daemon  (*)\n");
   printf ("             execute in normal, daemon mode\n");
   printf ("   -f,--foreground\n");
   printf ("             execute in non-daemon or foreground mode\n");
   printf ("   -1,--init  (*)\n");
   printf ("             must execute as pid 1\n");
   printf ("   -a,--anypid\n");
   printf ("             allow execution as any pid, not just 1\n");
   printf ("   -h,--help\n");
   printf ("             display program usage/help\n");
   printf ("\n");
   printf ("URGENTS...\n");
   printf ("   @q        quiet execution with no logs\n");
   printf ("   @l,@ls    route logging to system logs  (*)\n");
   printf ("   @lp       route logging to personal logs\n");
   printf ("   @lo       route logging to stdout\n");
   printf ("\n");
   printf ("namesake   : goddess of daybreak, i.e., homer's rosy-fingered dawn\n");
   printf ("\n");
   exit (0);
}

char         /*--: pre-argument initialization -----------[ leaf-- [ ------ ]-*/
PROG_init          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         rc          = 0;             /* generic return code            */
   FILE       *f           = NULL;          /* generic file pointer           */
   char        x_recd      [LEN_RECD];      /* file record entry              */
   /*---(set globals)--------------------*/
   my.daemon         = 'y';
   my.init           = 'y';
   my.test           = '-';
   /*---(check on proc filesystem)-------*/
   my.status_proc    = 'a';  /* already mounted */
   f = fopen ("/proc/mounts", "r");
   if (f == NULL) {
      my.status_proc  = 'm';   /* mounted */
      rc = mount("proc"  , "/proc", "proc", 0, NULL);
      if (rc != 0) {
         printf ("FATAL, can not mount /proc filesystem (rc = %d)\n", rc);
         exit (-10);
      }
      f = fopen ("/proc/mounts", "r");
      if (f == NULL) {
         printf ("FATAL, can not access /proc/mounts file\n");
         exit (-3);
      }
   }
   /*---(check on log filesystem)--------*/
   my.status_log     = '-';  /* not mounted */
   while (!feof (f)) {
      fgets (x_recd, 450, f);
      if (strncmp (x_recd, "varlog ", 7) == 0) {
         my.status_log = 'a';
         break;
      }
      if (strncmp (x_recd, "/dev/sda3 ", 9) == 0) {
         my.status_log = 'a';
         break;
      }
   }
   fclose (f);
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: wholesale urgents change --------------[ leaf   [ ------ ]-*/
PROG_urgsmass      (char a_set, char a_extra)
{
   /*---(overall)------------------------*/
   debug.tops     = a_set;
   debug.summ     = a_set;
   /*---(startup/shutdown)---------------*/
   debug.args     = a_set;
   debug.conf     = a_set;
   debug.prog     = a_set;
   /*---(file processing)----------------*/
   debug.inpt     = a_set;
   debug.inpt_mas = a_set;
   debug.outp     = a_set;
   debug.outp_mas = a_set;
   /*---(event handling)-----------------*/
   debug.loop     = a_set;
   debug.user     = a_set;
   debug.apis     = a_set;
   debug.sign     = a_set;
   debug.scrp     = a_set;
   debug.hist     = a_set;
   /*---(program)------------------------*/
   debug.graf     = a_set;
   debug.data     = a_set;
   debug.envi     = a_set;
   debug.envi_mas = a_set;
   /*---(specific)-----------------------*/
   if (a_extra == 'y') {
      ;;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: evaluate logger needs early -----------[ leaf   [ ------ ]-*/
PROG_logger        (int a_argc, char *a_argv[])
{
   /*---(locals)-------------------------*/
   int         rc          = 0;             /* generic return code            */
   int         i           = 0;             /* loop iterator -- arguments     */
   char       *a           = NULL;          /* current argument               */
   int         x_len       = 0;             /* argument length                */
   int         x_total     = 0;             /* total argument count           */
   int         x_urgs      = 0;             /* urgent count                   */
   /*---(default urgents)----------------*/
   PROG_urgsmass ('-', 'y');
   /*---(logger preprocessing)-----------*/
   debug.logger   = -1;
   debug.logtype  = '-';
   debug.sda3     = '-';
   for (i = 1; i < a_argc; ++i) {
      a = a_argv[i];
      if (a[0] != '@')  continue;
      if (my.logtype == '-')  my.logtype = 's';
      if       (strcmp ("@@sda3"    , a) == 0)  { debug.logtype = 'h'; debug.sda3    = '3'; }
      else if  (strcmp ("@@personal", a) == 0)    debug.logtype = 'p';
      else if  (strcmp ("@@system"  , a) == 0)    debug.logtype = 's';
      else if  (strcmp ("@@stdout"  , a) == 0)    debug.logtype = 'o';
      else if  (strcmp ("@@root"    , a) == 0)    debug.logtype = 'r';
      else if ((strcmp ("@q", a) == 0) || (strcmp ("@@quiet", a) == 0)) {
         debug.logtype = 'q';
         PROG_urgsmass ('-', 'y');
         break;
      }
   }
   /*---(setup log filesystem)-----------*/
   if (my.status_log == '-') {
      if (debug.sda3 == 'y') {
         rc = mount("/dev/sda3", "/var/log/yLOG", "ext4", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME, NULL);
         if (rc != 0) {
            printf ("FATAL, can not mount sda3 for logging (rc=%d)\n", rc);
            exit   (ERR_NO_TMPFS);
         }
         my.status_log = '3';
      } else {
         rc = mount("varlog", "/var/log/yLOG", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME, "size=500m");
         if (rc != 0) {
            printf ("FATAL, can not mount tmpfs for logging (rc=%d)\n", rc);
            exit   (ERR_NO_TMPFS);
         }
         my.status_log = 't';
      }
   }
   /*---(startup logging)----------------*/
   if (my.logtype == '-' || my.logtype == 'q') {
      debug.logger = yLOG_begin ("eos", yLOG_SYSTEM, yLOG_QUIET);
   } else {
      /*---(start)-----------------------*/
      debug.tops = 'y';
      /*---(initialize logging)-------------*/
      switch (my.logtype ) {
      case  'o' : debug.logger = yLOG_begin ("eos", yLOG_STDOUT    , yLOG_NOISE);  break;
      case  'p' : debug.logger = yLOG_begin ("eos", yLOG_PERSONAL  , yLOG_NOISE);  break;
      case  's' : debug.logger = yLOG_begin ("eos", yLOG_SYSTEM    , yLOG_NOISE);  break;
      case  'h' : debug.logger = yLOG_begin ("eos", yLOG_HISTORICAL, yLOG_NOISE);  break;
      case  'r' : debug.logger = yLOG_begin ("eos", yLOG_ROOT      , yLOG_NOISE);  break;
      default   : debug.logger = yLOG_begin ("eos", yLOG_SYSTEM    , yLOG_NOISE);  break;
      }
   }
   /*---(log header)------------------*/
   DEBUG_TOPS   yLOG_info     ("purpose",  "clean, light, reliable, consistent system initialization");
   DEBUG_TOPS   yLOG_info     ("namesake", "goddess of daybreak, rosy-fingered dawn");
   DEBUG_TOPS   yLOG_info     ("eos"     , PROG_version   ());
   DEBUG_TOPS   yLOG_info     ("yDLST"   , yDLST_version  ());
   DEBUG_TOPS   yLOG_info     ("ySEC"    , ySEC_version   ());
   DEBUG_TOPS   yLOG_info     ("yEXEC"   , yEXEC_version  ());
   DEBUG_TOPS   yLOG_info     ("ySTR"    , ySTR_version   ());
   DEBUG_TOPS   yLOG_info     ("yLOG"    , yLOG_version   ());
   switch (my.status_proc) {
   case 'a' :
      DEBUG_TOPS   yLOG_info     ("proc"    , "proc filesystem was already mounted");
      break;
   case 'm' :
      DEBUG_TOPS   yLOG_info     ("proc"    , "proc filesystem mounted by eos");
      break;
   }
   switch (my.status_log) {
   case 'a' :
      DEBUG_TOPS   yLOG_info     ("varlog"  , "log filesystem was already mounted");
      break;
   case 't' :
      DEBUG_TOPS   yLOG_info     ("varlog"  , "log filesystem mounted by eos as tmpfs");
      break;
   case '3' :
      DEBUG_TOPS   yLOG_info     ("varlog"  , "log filesystem mounted by eos on sda3");
      break;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: evaluate command line urgents ---------[ leaf   [ ------ ]-*/
PROG_urgs          (int a_argc, char *a_argv[])
{
   /*---(locals)-------------------------*/
   int         rc          = 0;             /* generic return code            */
   int         i           = 0;             /* loop iterator -- arguments     */
   char       *a           = NULL;          /* current argument               */
   int         x_len       = 0;             /* argument length                */
   int         x_total     = 0;             /* total argument count           */
   int         x_urgs      = 0;             /* urgent count                   */
   /*---(default urgents)----------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   PROG_urgsmass ('-', 'y');
   /*---(walk through urgents)-----------*/
   for (i = 1; i < a_argc; ++i) {
      /*---(prepare)---------------------*/
      a        = a_argv [i];
      x_len    = strllen (a, LEN_RECD);
      ++x_total;
      /*---(filter)----------------------*/
      if (a[0] != '@')  continue;
      ++x_urgs;
      DEBUG_ARGS  yLOG_info  ("urgent"    , a);
      /*---(overall)---------------------*/
      if      (strncmp(a, "@t"           ,10) == 0)  debug.tops = 'y';
      else if (strncmp(a, "@@top"        ,10) == 0)  debug.tops = 'y';
      else if (strncmp(a, "@s"           ,10) == 0)  debug.tops = debug.summ  = 'y';
      else if (strncmp(a, "@@summ"       ,10) == 0)  debug.tops = debug.summ  = 'y';
      /*---(startup/shutdown)------------*/
      else if (strncmp(a, "@a"           ,10) == 0)  debug.tops = debug.args  = 'y';
      else if (strncmp(a, "@@args"       ,10) == 0)  debug.tops = debug.args  = 'y';
      else if (strncmp(a, "@c"           ,10) == 0)  debug.tops = debug.conf  = 'y';
      else if (strncmp(a, "@@conf"       ,10) == 0)  debug.tops = debug.conf  = 'y';
      else if (strncmp(a, "@p"           ,10) == 0)  debug.tops = debug.prog  = 'y';
      else if (strncmp(a, "@@prog"       ,10) == 0)  debug.tops = debug.prog  = 'y';
      /*---(file processing)-------------*/
      else if (strncmp(a, "@i"           ,10) == 0)  debug.tops = debug.inpt  = 'y';
      else if (strncmp(a, "@@inpt"       ,10) == 0)  debug.tops = debug.inpt  = 'y';
      else if (strncmp(a, "@I"           ,10) == 0)  debug.tops = debug.inpt  = debug.inpt_mas = 'y';
      else if (strncmp(a, "@@INPT"       ,10) == 0)  debug.tops = debug.inpt  = debug.inpt_mas = 'y';
      else if (strncmp(a, "@o"           ,10) == 0)  debug.tops = debug.outp  = 'y';
      else if (strncmp(a, "@@outp"       ,10) == 0)  debug.tops = debug.outp  = 'y';
      else if (strncmp(a, "@O"           ,10) == 0)  debug.tops = debug.outp  = debug.outp_mas = 'y';
      else if (strncmp(a, "@@OUTP"       ,10) == 0)  debug.tops = debug.outp  = debug.outp_mas = 'y';
      /*---(processing)------------------*/
      else if (strncmp(a, "@l"           ,10) == 0)  debug.tops = debug.loop  = 'y';
      else if (strncmp(a, "@@loop"       ,10) == 0)  debug.tops = debug.loop  = 'y';
      else if (strncmp(a, "@u"           ,10) == 0)  debug.tops = debug.user  = 'y';
      else if (strncmp(a, "@@user"       ,10) == 0)  debug.tops = debug.user  = 'y';
      else if (strncmp(a, "@z"           ,10) == 0)  debug.tops = debug.apis  = 'y';
      else if (strncmp(a, "@@apis"       ,10) == 0)  debug.tops = debug.apis  = 'y';
      else if (strncmp(a, "@x"           ,10) == 0)  debug.tops = debug.sign  = 'y';
      else if (strncmp(a, "@@sign"       ,10) == 0)  debug.tops = debug.sign  = 'y';
      else if (strncmp(a, "@b"           ,10) == 0)  debug.tops = debug.scrp  = 'y';
      else if (strncmp(a, "@@scrp"       ,10) == 0)  debug.tops = debug.scrp  = 'y';
      else if (strncmp(a, "@h"           ,10) == 0)  debug.tops = debug.hist  = 'y';
      else if (strncmp(a, "@@hist"       ,10) == 0)  debug.tops = debug.hist  = 'y';
      /*---(program)---------------------*/
      else if (strncmp(a, "@g"           ,10) == 0)  debug.tops = debug.graf  = 'y';
      else if (strncmp(a, "@@graf"       ,10) == 0)  debug.tops = debug.graf  = 'y';
      else if (strncmp(a, "@d"           ,10) == 0)  debug.tops = debug.data  = 'y';
      else if (strncmp(a, "@@data"       ,10) == 0)  debug.tops = debug.data  = 'y';
      else if (strncmp(a, "@e"           ,10) == 0)  debug.tops = debug.envi  = 'y';
      else if (strncmp(a, "@@envi"       ,10) == 0)  debug.tops = debug.envi  = 'y';
      else if (strncmp(a, "@E"           ,10) == 0)  debug.tops = debug.envi  = debug.envi_mas = 'y';
      else if (strncmp(a, "@@ENVI"       ,10) == 0)  debug.tops = debug.envi  = debug.envi_mas = 'y';
      /*---(complex)---------------------*/
      else if (strncmp(a, "@f"           ,10) == 0)  PROG_urgsmass ('y', '-');
      else if (strncmp(a, "@@full"       ,10) == 0)  PROG_urgsmass ('y', '-');
      else if (strncmp(a, "@k"           ,10) == 0)  PROG_urgsmass ('y', 'y');
      else if (strncmp(a, "@@kitchen"    ,10) == 0)  PROG_urgsmass ('y', 'y');
      /*---(specific)--------------------*/
      /*---(unknown)--------------------*/
      else {
         DEBUG_ARGS  yLOG_note    ("urgent not understood");
      }
      /*---(done)-----------------------*/
   }
   /*---(display urgents)----------------*/
   DEBUG_ARGS   yLOG_note    ("summarization of urgent processing");
   DEBUG_ARGS   yLOG_value   ("entries"   , x_total);
   DEBUG_ARGS   yLOG_value   ("urgents"   , x_urgs);
   DEBUG_ARGS   yLOG_note    ("standard urgents");
   DEBUG_ARGS   yLOG_char    ("tops"      , debug.tops);
   DEBUG_ARGS   yLOG_char    ("summ"      , debug.summ);
   DEBUG_ARGS   yLOG_char    ("args"      , debug.args);
   DEBUG_ARGS   yLOG_char    ("conf"      , debug.conf);
   DEBUG_ARGS   yLOG_char    ("prog"      , debug.prog);
   DEBUG_ARGS   yLOG_char    ("inpt"      , debug.inpt);
   DEBUG_ARGS   yLOG_char    ("INPT"      , debug.inpt_mas);
   DEBUG_ARGS   yLOG_char    ("outp"      , debug.outp);
   DEBUG_ARGS   yLOG_char    ("OUTP"      , debug.outp_mas);
   DEBUG_ARGS   yLOG_char    ("loop"      , debug.loop);
   DEBUG_ARGS   yLOG_char    ("user"      , debug.user);
   DEBUG_ARGS   yLOG_char    ("apis"      , debug.apis);
   DEBUG_ARGS   yLOG_char    ("sign"      , debug.sign);
   DEBUG_ARGS   yLOG_char    ("scrp"      , debug.scrp);
   DEBUG_ARGS   yLOG_char    ("hist"      , debug.hist);
   DEBUG_ARGS   yLOG_char    ("graf"      , debug.graf);
   DEBUG_ARGS   yLOG_char    ("data"      , debug.data);
   DEBUG_ARGS   yLOG_char    ("envi"      , debug.envi);
   DEBUG_ARGS   yLOG_char    ("ENVI"      , debug.envi_mas);
   DEBUG_ARGS   yLOG_note    ("specialty urgents");
   DEBUG_ARGS   yLOG_note    ("none yet");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: determine user and permissions --------[ leaf   [ ------ ]-*/
PROG_whoami        (void)
{
   /*---(locals)-------------------------*/
   tPASSWD    *x_pass      = NULL;         /* passwd data structure               */
   int         x_len       = 0;            /* user name length                    */
   int         rc          = 0;            /* generic return code                 */
   char        rce         = -10;
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(get real uid)-------------------*/
   my.uid    = getuid();
   DEBUG_ARGS   yLOG_value   ("uid"       , my.uid);
   /*---(pull user name)-----------------*/
   x_pass    = getpwuid (my.uid);
   --rce;  if (x_pass == NULL) {
      yLOG_note    ("can not  retrieve user information from system");
      yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(check user name)----------------*/
   yLOG_info    ("user name" , x_pass->pw_name);
   x_len      = strllen (x_pass->pw_name, 20);
   yLOG_value   ("length"    , x_len);
   if (x_len < 1) {
      yLOG_note    ("user name can not be empty");
      yLOG_exit    (__FUNCTION__);
      return rce;
   }
   if (x_len > 20) {
      yLOG_note    ("user name is too long");
      yLOG_exit    (__FUNCTION__);
      return rce;
   }
   strlcpy (my.who, x_pass->pw_name, 20);
   /*---(log pid info)---------------------*/
   my.pid  = getpid();
   DEBUG_ARGS   yLOG_value   ("pid"       , my.pid);
   DEBUG_ARGS   printf("   current pid  = %d\n",    my.pid);
   my.ppid = getppid();
   DEBUG_ARGS   yLOG_value   ("ppid"      , my.ppid);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: evaluate command line arguments -------[ leaf   [ ------ ]-*/
PROG_args          (int a_argc, char *a_argv[])
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(locals)-------------------------*/
   int         i           = 0;             /* loop iterator -- arguments     */
   char       *a           = NULL;          /* current argument               */
   int       x         = 0;            /* count of arguments                  */
   int         x_len       = 0;             /* argument length                */
   int         x_total     = 0;             /* total argument count           */
   int         x_args      = 0;             /* argument count                 */
   /*---(process)------------------------*/
   for (i = 1; i < a_argc; ++i) {
      a = a_argv[i];
      if (a[0] == '@')       continue;
      ++x_args;
      if      (strncmp(a, "-d"          , 12) == 0)     my.daemon        = 'y';
      else if (strncmp(a, "--daemon"    , 12) == 0)     my.daemon        = 'y';
      else if (strncmp(a, "-f"          , 12) == 0)     my.daemon        = '-';
      else if (strncmp(a, "--foreground", 12) == 0)     my.daemon        = '-';
      else if (strncmp(a, "-1"          , 12) == 0)     my.init          = 'y';
      else if (strncmp(a, "--init"      , 12) == 0)     my.init          = 'y';
      else if (strncmp(a, "--debug"     , 12) == 0)   { my.init          = '-'; my.daemon   = '-'; }
      else if (strncmp(a, "-a"          , 12) == 0)     my.init          = '-';
      else if (strncmp(a, "--anypid"    , 12) == 0)     my.init          = '-';
      else if (strncmp(a, "-t"          , 12) == 0)   { my.init          = '-'; my.daemon   = '-'; my.test   = 'y'; }
      else if (strncmp(a, "--test"      , 12) == 0)   { my.init          = '-'; my.daemon   = '-'; my.test   = 'y'; }
      else if (strncmp(a, "-h"          , 12) == 0)     PROG_usage ();
      else if (strncmp(a, "--help"      , 12) == 0)     PROG_usage ();
   }
   /*---(summary)------------------------*/
   printf ("count  = %2d", x);
   printf (", init   = %c"  , my.init  );
   /*---(display urgents)----------------*/
   DEBUG_ARGS   yLOG_note    ("summarization of argument processing");
   DEBUG_ARGS   yLOG_value   ("entries"   , x_total);
   DEBUG_ARGS   yLOG_value   ("arguments" , x_args);
   DEBUG_ARGS   yLOG_char    ("daemon"    , my.daemon);
   DEBUG_ARGS   yLOG_char    ("init"      , my.init);
   DEBUG_ARGS   yLOG_char    ("test"      , my.test);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: final preparation for execution -------[ leaf   [ ------ ]-*/
PROG_begin         (void)
{
   /*---(locals)-------*-----------------*/
   int       rc        = 0;            /* generic return code                 */
   FILE     *output = NULL;
   long      now;                                 /* present datetime         */
   tTIME    *curr_time = NULL;
   char      msg[200];
   int         x_uid       = 0;             /* running user id                */
   int         x_pid       = 0;             /* running process id             */
   char        x_recd      [LEN_RECD];
   /*---(defense : uid)------------------*/
   x_uid = getuid ();
   printf ("uid = %d", x_uid);
   if (x_uid != 0) {
      printf (", never allowed, FATAL\n");
      exit   (ERR_NOT_ROOT);      /* boot is always superuser, so no worries  */
   }
   /*---(defenses : pid)-----------------*/
   x_pid = getpid ();
   printf (", pid = %d", x_pid);
   if (x_pid != 1) {  /* process number 1 only            */
      if (my.test == 'y') {
         printf (", test mode", x_pid);
      } else {
         printf (", not test mode, FATAL\n");
         exit (ERR_NOT_PID_ONE);     /* boot is always pid 1, so no worries      */
      }
   }
   /*---(process initd.conf)--------------------*/
   yLOG_info  ("UID",      "properly logged in as superuser");
   if (my.init   == 'y')  yLOG_info  ("PID",      "running as process id 1");
   else                   yLOG_info  ("PID",      "running in anypid mode");
   if (my.status_log    == 'y')  yLOG_info  ("tmpfs",    "yLOG tmpfs already mounted");
   else                   yLOG_info  ("tmpfs",    "mounted the yLOG tmpfs");
   yLOG_info  ("logger",   "heatherly custom logger up and running");
   /*---(startup dlst library)------------------*/
   yLOG_info  ("yDLST"    ,"starting");
   yDLST_begin ();
   /*---(clear feedback file)-------------------*/
   output = fopen(STUFF, "w");
   now = time(NULL);
   curr_time = localtime(&now);
   strftime(msg, 50, "%Ss %Mm %Hh %dd %mm  %ww", curr_time);
   fprintf(output, "eos start : %s\n",   msg);
   fclose (output);
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   printf ("\n");
   return 0;
}

char         /*--: wrapup program execution --------------[ leaf   [ ------ ]-*/
PROG_end           (void)
{
   printf ("   > PROG_end\n");
   /*---(create utmp boot)----------------------*/
   yLOG_enter (__FUNCTION__);
   yLOG_info  ("logger",   "shutting down logger");
   yLOG_exit  (__FUNCTION__);
   yLOG_end   ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   helpers for unit testing                   ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char       /*----: set up program test file locations ------------------------*/
PROG_testfiles     (void)
{
   /*> snprintf (my.name_pulser , 200, "%s%s", "/tmp/" , FILE_PULSE);                 <* 
    *> snprintf (my.name_watcher, 200, "%s%s", "/tmp/" , FILE_WATCH);                 <* 
    *> snprintf (my.name_locker , 200, "%s%s", "/tmp/" , FILE_LOCK);                  <* 
    *> snprintf (my.name_exec   , 200, "%s%s", "/tmp/" , FILE_EXEC);                  <* 
    *> snprintf (my.name_status , 200, "%s%s", "/tmp/" , FILE_STATUS);                <*/
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
PROG_testquiet     (void)
{
   /*> char       *x_args [1]  = { "eos" };                                           <* 
    *> PROG_urgs      (1, x_args);                                                    <* 
    *> PROG_init      ();                                                             <* 
    *> PROG_whoami    ();                                                             <* 
    *> PROG_testfiles ();                                                             <* 
    *> PROG_args      (1, x_args);                                                    <* 
    *> PROG_begin     ();                                                             <*/
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
PROG_testloud      (void)
{
   /*> char       *x_args [2]  = { "eos", "@@kitchen"    };                           <* 
    *> PROG_urgs      (2, x_args);                                                    <* 
    *> PROG_init      ();                                                             <* 
    *> PROG_whoami    ();                                                             <* 
    *> PROG_testfiles ();                                                             <* 
    *> PROG_args      (2, x_args);                                                    <* 
    *> PROG_begin     ();                                                             <*/
   return 0;
}




/*============================[[ end-of-code ]]===============================*/
