/*============================[[    beg-code    ]]============================*/
#include    "eos.h"


tACCESSOR my;

/*====================------------------------------------====================*/
/*===----                      supporting functions                    ----===*/
/*====================------------------------------------====================*/
static void      o___SUUPPORT________________o (void) {;}

char        verstring    [500];

char*        /*--: return versioning information ---------[ leaf-- [ ------ ]-*/
PROG_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   ystrlcpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   ystrlcpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   ystrlcpy (t, "[cbang      ]", 15);
#elif  __HEPH__  > 0
   ystrlcpy (t, "[hephaestus ]", 18);
#else
   ystrlcpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

char       /* PURPOSE : display usage help information -----------------------*/
PROG__usage             (void)
{
   printf ("see man pages for a better understanding of eos...\n");
   printf ("  man 1 eos    command-line initiation, use, and options\n");
   printf ("  man 5 eos    structure of config, files, and streams\n");
   printf ("  man 7 eos    decision rationale, scope, and objectives\n");
   exit   (0);
}



/*====================------------------------------------====================*/
/*===----                     pre-startup functions                    ----===*/
/*====================------------------------------------====================*/
static void      o___PRESTART________________o (void) {;}

char
PROG__verbose           (int a_argc, char *a_argv[], char a_unit, int a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        x_loud      =  '-';
   /*---(default)------------------------*/
   my.verbose = '-';
   yURG_msg_std ();  yURG_msg_mute ();
   yURG_err_std ();  yURG_err_mute ();
   /*---(handle unit test)---------------*/
   if (a_unit == 'y' || a_unit == 'k') {
      yURG_msg_tmp ();
      yURG_err_tmp ();
   }
   /*---(check for pre-loud)-------------*/
   for (i = 1; i < a_argc; ++i) {
      /*> printf (", %s", a_argv [i]);                                                <*/
      if (a_argv [i][0] != '-')       continue;
      if (a_argv [i][1] != '-')       continue;
      if      (strcmp (a_argv [i], "--vdaemon" ) == 0)   x_loud = 'y';
      else if (strcmp (a_argv [i], "--vprickly") == 0)   x_loud = 'y';
      else if (strcmp (a_argv [i], "--vnormal" ) == 0)   x_loud = 'y';
      else if (strcmp (a_argv [i], "--vstrict" ) == 0)   x_loud = 'y';
   }
   /*---(set loud)-----------------------*/
   if (x_loud == 'y' && a_rpid == 1) {
      yURG_msg_live ();
      my.verbose = 'y';
   }
   yURG_err_live ();
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--> detirmine behavior --------------------[ leaf-- [ ------ ]-*/
PROG__runas             (char *a_name)
{
   char        rce         =  -10;
   char        rc          =    0;
   char        s           [LEN_HUND]  = "";
   /*---(defaults)-----------------------*/
   my.run_as         = '-';
   my.run_mode       = '-';
   /*---(specific)-----------------------*/
   yURG_msg ('>', "%s %s", P_NAMESAKE, P_SUBJECT);
   /* must also trap _debug versions */
   /*> yJOBS_args_init         (&(my.run_as), &(my.run_mode), my.run_file);           <*/
   rc = yJOBS_runas (a_name, &(my.run_as), P_HEADERS, NULL);
   --rce;  if (rc < 0) {
      yURG_err ('f', "not a recognized prog name å%sæ", a_name);
      yURG_msg (' ', "");
      return rce;
   }
   yURG_msg ('-', "run as (%c) %s", my.run_as, s);
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--> before even logger --------------------[ leaf-- [ ------ ]-*/
PROG__boot              (int a_argc, char *a_argv[], int a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         i           =    0;
   char       *a           = NULL;          /* current argument               */
   FILE       *f           = NULL;          /* generic file pointer           */
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   int         c           =    0;
   int         x_tries     =    0;
   char        x_ylog      =  '-';
   char        x_noteos    =  '-';
   /*---(quick out)----------------------*/
   for (i = 1; i < a_argc; ++i) {
      a = a_argv[i];
      if (a == NULL) return rce;
      if (a[0] == '@')       continue;
      if (strcmp (a, "--eos") == 0) {
         my.run_as    = IAM_EOS;
         x_noteos     = 'y';
      }
   }
   if (my.run_as != IAM_EOS) return 0;
   if (a_rpid != 1)          return 0;
   /*---(header)-------------------------*/
   yURG_msg ('>', "eos boot preparation...");
   if (x_noteos == 'y')  yURG_msg ('-', "forced into eos by --eos option");
   /*---(remount /)----------------------*/
   rc = mount ("/dev/sda4", "/", "ext4", MS_REMOUNT | MS_NOATIME, NULL);
   yURG_msg ('-', "remounted /dev/sda4 (%d)", rc);
   /*---(check on /proc)-----------------*/
   /*> mkdir ("/proc", 0755);                                                         <*/
   while (1) {
      f = fopen ("/proc/mounts", "rt");
      if (f != NULL)    break;
      if (x_tries > 3)  break;
      rc = mount ("proc"  , "/proc", "proc", 0, NULL);
      ++x_tries;
   }
   --rce;  if (f == NULL) {
      yURG_err ('f', "/proc could not be mounted in 3 tries");
      return rce;
   }
   if (x_tries == 0)  yURG_msg ('-', "/proc already successfully mounted");
   else               yURG_msg ('-', "/proc successfully mounted after (%d) tries", x_tries);
   /*---(show /proc/mounts)--------------*/
   while (1) {
      fgets (x_recd, 450, f);
      if (feof (f))  break;
      x_len = strlen (x_recd);
      if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
      /*> printf ("%2d [%s]\n", c, x_recd);                                           <*/
      ++c;
      if (strstr (x_recd, "yLOG") != NULL) {
         yURG_msg ('-', "/var/log/yLOG already successfully mounted");
         x_ylog = 'y';
      }
   }
   fclose (f);
   if (x_ylog == '-')  yURG_msg ('-', "/var/log/yLOG not yet mounted");
   /*---(check on yLOG)------------------*/
   if (x_ylog != 'y') {
      rc = mount ("varlog", "/var/log/yLOG", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME, "size=500m");
      yURG_msg ('-', "/var/log/yLOG mounted successfully (%d)", rc);
   }
   yURG_msg (' ', "");
   /*---(close)--------------------------*/
   return 0;
}

char
PROG_prestart           (int a_argc, char *a_argv[], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   int         x_rpid      =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(startup)------------------------*/
   x_rpid = getpid ();
   if (rc >= 0)  rc = PROG__verbose   (a_argc, a_argv, a_unit, x_rpid);
   if (rc >= 0)  rc = PROG__runas     (a_argv [0]);
   if (rc >= 0)  rc = PROG__boot      (a_argc, a_argv, x_rpid);
   DEBUG_PROG  yLOG_value   ("prestart"  , rc);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                    debugging startup                         ----===*/
/*====================------------------------------------====================*/
static void      o___DEBUGGING_______________o (void) {;}

char
PROG_debugging          (int a_argc, char *a_argv[], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(startup)------------------------*/
   if (rc >= 0)  rc = yURG_logger  (a_argc, a_argv);
   if (rc >= 0)  rc = yURG_urgs    (a_argc, a_argv);
   DEBUG_PROG  yLOG_value   ("debugging" , rc);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      startup functions                       ----===*/
/*====================------------------------------------====================*/
static void      o___STARTUP_________________o (void) {;}

char         /*--: pre-argument initialization -----------[ leaf-- [ ------ ]-*/
PROG__init              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   FILE       *f           = NULL;          /* generic file pointer           */
   int         x_pos       =   -1;
   /*---(log header)---------------------*/
   DEBUG_PROG   yLOG_info    ("namesake", P_NAMESAKE);
   DEBUG_PROG   yLOG_info    ("heritage", P_HERITAGE);
   DEBUG_PROG   yLOG_info    ("imagery" , P_IMAGERY);
   DEBUG_PROG   yLOG_info    ("purpose" , P_PURPOSE);
   DEBUG_PROG   yLOG_note    ("BASE SOFTWARE");
   DEBUG_PROG   yLOG_info    ("eos"     , PROG_version    ());
   DEBUG_PROG   yLOG_note    ("EVERYWHERE LIBRARIES");
   DEBUG_PROG   yLOG_info    ("yLOG"    , yLOGS_version   ());
   DEBUG_PROG   yLOG_info    ("yURG"    , yURG_version    ());
   DEBUG_PROG   yLOG_note    ("COMMON LIBRARIES");
   DEBUG_PROG   yLOG_info    ("ySTR"    , ySTR_version    ());
   DEBUG_PROG   yLOG_info    ("yPARSE"  , yPARSE_version  ());
   DEBUG_PROG   yLOG_note    ("SPECIFIC LIBRARIES");
   DEBUG_PROG   yLOG_info    ("yDLST"   , yDLST_version   ());
   DEBUG_PROG   yLOG_info    ("yEXEC"   , yEXEC_version   ());
   DEBUG_PROG   yLOG_info    ("yJOBS"   , yJOBS_version   ());
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "program initialization...");
   /*---(set globals)--------------------*/
   yURG_msg ('-', "set defaults for major globals");
   my.done_done      = '-';
   my.test           = '-';
   my.loop_msec      = 100;
   my.loop_max       = my.loop_msec * 10 * 240; /* four minutes */
   ystrlcpy (my.dev, "/dev/tty1", LEN_LABEL);
   /*> PROG__arg_load ();                                                             <*/
   /*---(call whoami)--------------------*/
   rc = yEXEC_whoami (&my.pid, &my.ppid, &my.m_uid, NULL, &my.m_who, 'n');
   DEBUG_PROG   yLOG_value   ("whoami"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "yEXEC_whoami failed (%d)", rc);
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "yEXEC_whoami returned %5d pid, %d uid, %s user", my.pid, my.m_uid, my.m_who);
   DEBUG_PROG   yLOG_value   ("pid"       , my.pid);
   DEBUG_PROG   yLOG_value   ("ppid"      , my.ppid);
   DEBUG_PROG   yLOG_value   ("uid"       , my.m_uid);
   DEBUG_PROG   yLOG_info    ("who"       , my.m_who);
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: evaluate command line arguments -------[ leaf   [ ------ ]-*/
PROG__args              (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char       *a           = NULL;          /* current argument               */
   char       *b           = NULL;          /* next argument                  */
   int         x_len       =    0;          /* argument length                */
   int         x_total     =    0;          /* total argument count           */
   int         x_args      =    0;          /* argument count                 */
   int         x_num       =    0;          /* numeric argument holder        */
   char        x_two       =    0;
   char        s           [LEN_HUND]  = "";
   /*---(process)------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "command line arguments handling...");
   yURG_msg ('-', "total of %d arguments, including name", a_argc);
   /*---(defaults)------------------------------*/
   /*> PROG__arg_clearmode ();                                                        <*/
   /*---(walk args)-----------------------------*/
   for (i = 1; i < a_argc; ++i) {
      /*---(prepare)---------------------*/
      a = a_argv [i];
      if (a == NULL) {
         yURG_err ('f', "arg %d is NULL", i);
         DEBUG_PROG   yLOG_note    ("FATAL, found a null argument, really bad news");
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (i < a_argc - 1)  b = a_argv [i + 1];
      else                 b = NULL;
      /*---(debugging--------------------*/
      if (a [0] == '@')       continue;
      /*---(two arg check)---------------*/
      ++x_args;
      DEBUG_ARGS  yLOG_info     ("argument"  , a);
      /*> rc = PROG__arg_handle (&i, a, b);                                           <*/
      rc = yJOBS_argument (&i, a, b, &(my.run_as), &(my.run_mode), my.run_file);
      DEBUG_ARGS  yLOG_value    ("handle"    , rc);
      if (rc < 0)  break;
   }
   /*---(check for default normal)-------*/
   if (x_args == 0) {
      rc = yJOBS_argument (&i, "--normal", NULL, &(my.run_as), &(my.run_mode), my.run_file);
      DEBUG_ARGS  yLOG_value    ("single"    , rc);
   }
   /*---(verify)-------------------------*/
   /*> yJOBS_iam  (my.run_as  , s);                                                   <* 
    *> yURG_msg ('-', "run as (%c) %s", my.run_as, s);                                <*/
   /*> yJOBS_mode (my.run_mode, s);                                                   <* 
    *> yURG_msg ('-', "mode   (%c) %s", my.run_mode, s);                              <* 
    *> yURG_msg ('-', "file   å%sæ", my.run_file);                                    <*/
   /*> yURG_msg ('-', "msec   %d", my.loop_msec);                                     <*/
   /*> yURG_msg ('-', "max    %d", my.loop_max);                                      <*/
   /*---(display urgents)----------------*/
   DEBUG_ARGS   yLOG_note    ("summarization of argument processing");
   DEBUG_ARGS   yLOG_value   ("entries"   , x_total);
   DEBUG_ARGS   yLOG_value   ("arguments" , x_args);
   DEBUG_ARGS   yLOG_char    ("test"      , my.test);
   DEBUG_ARGS   yLOG_value   ("loop_msec" , my.loop_msec);
   DEBUG_ARGS   yLOG_value   ("loop_max"  , my.loop_max);
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   if (rc > 0)  rc = 0;
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*--: final preparation for execution -------[ leaf   [ ------ ]-*/
PROG__begin             (void)
{
   /*---(locals)-----------+-------------*/
   char        rce         =  -10;
   char        rc          =    0;
   char        s           [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "prepare the program for run-time...");
   /*---(check permission)---------------*/
   yURG_msg ('-', "checking permissions");
   --rce;  switch (my.run_mode) {
   case CASE_DAEMON  :
   case CASE_PRICKLY :
      yURG_err ('f', "eos can never be allowed to daemonize");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
      break;
   case CASE_NORMAL  :
   case CASE_STRICT  :
      if (my.m_uid != 0 && my.run_as != IAM_HERACLES) {
         yURG_err ('f', "can not run normal/strict mode (%c) without being root", my.run_mode);
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      break;
   case CASE_AUDIT   : case CASE_FIX     :
      if (my.m_uid != 0) {
         yURG_err ('f', "can not audit/fix (%c) without being root", my.run_mode);
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      break;
   }
   /*---(setup signals)-------------------------*/
   DEBUG_PROG   yLOG_note    ("signals set to soft for normal working");
   yURG_msg ('-', "requesting signals to normal/soft mode");
   rc = yEXEC_signal (YEXEC_SOFT, YEXEC_NO, YEXEC_WAIT, NULL, NULL);
   DEBUG_PROG   yLOG_value   ("signals"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could set signals properly (%d)", rc);
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(startup dlst library)------------------*/
   yURG_msg ('-', "setting up and initializing yDLST");
   DEBUG_ARGS   yLOG_info    ("yDLST"    ,"initializing");
   rc = yDLST_init ();
   --rce;  if (rc < 0) {
      yURG_err ('f', "yDLST init failed");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(startup yPARSE)----------------s-------*/
   yURG_msg ('-', "setting up and initializing yPARSE");
   DEBUG_ARGS   yLOG_info    ("yPARSE"   ,"initializing");
   rc = yPARSE_config  (YPARSE_MANUAL, NULL, YPARSE_ONETIME, YPARSE_FIELD);
   --rce;  if (rc < 0) {
      yURG_err ('f', "yPARSE config failed");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set file names)-----------------*/
   /*> DEBUG_ARGS   yLOG_note    ("setting file names");                              <*/
   /*---(complete)------------------------------*/
   yURG_msg (' ', "");
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG__final             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_char    ("run_mode"  , my.run_mode);
   /*---(set output routing)-------------*/
   yJOBS_final (my.m_uid);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG_startup            (int a_argc, char *a_argv[], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(pre-startup)--------------------*/
   my.msec = BASE_msec ();
   DEBUG_LOOP   yLOG_value   ("my.msec"    , my.msec);
   /*---(startup)------------------------*/
   if (rc >= 0)  rc = PROG__init   ();
   if (rc >= 0)  rc = PROG__args   (a_argc, a_argv);
   if (rc >= 0)  rc = PROG__begin  ();
   if (rc >= 0)  rc = PROG__final  ();
   DEBUG_PROG  yLOG_value   ("startup"   , rc);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      shutdown functions                      ----===*/
/*====================------------------------------------====================*/
static void      o___SHUTDOWN________________o (void) {;}

char         /*--: wrapup program execution --------------[ leaf   [ ------ ]-*/
PROG_end           (void)
{
   /*---(create utmp boot)----------------------*/
   yLOG_enter (__FUNCTION__);
   /*> rptg_dump  ();                                                                 <*/
   yLOG_info  ("logger",   "shutting down logger");
   yLOG_exit  (__FUNCTION__);
   yLOGS_end   ();
   PROG_shutdown ();
   return 0;
}

char
PROG_shutdown           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10; 
   FILE       *f           = NULL;
   int         rc          =    0;
   int         pid         =    0;
   /*---(quick-out)----------------------*/
   --rce;  if (my.run_as != IAM_ASTRAIOS) {
      return rce;
   }
   printf("astraios -- system shutdown\n");
   /*---(mark shutdown)------------------*/
   ySEC_shutdown ();
   /*---(clean up the drives)------------*/
   printf ("   - sync drives three times\n");
   sync   ();
   sync   ();
   sync   ();
   sleep  (1);
   /*---(lock up this program)-----------*/
   signal (SIGTERM, SIG_IGN);
   signal (SIGSTOP, SIG_IGN);
   signal (SIGKILL, SIG_IGN);
   /*---(shutdown processes)-------------*/
   /*> printf("   - send TERM message\n");                                            <* 
    *> kill(-1, SIGTERM);                                                             <* 
    *> sleep(1);                                                                      <*/
   /*> printf("   - send KILL message\n");                                            <* 
    *> kill(-1, SIGKILL);                                                             <* 
    *> sleep(1);                                                                      <*/
   /*---(kill slim)----------------------*/
   /*> f = fopen ("/var/run/slim.pid", "r");                                          <* 
    *> if (f != NULL) {                                                               <* 
    *>    rc = fscanf (f, "%d", &pid);                                                <* 
    *>    if (rc == 1) {                                                              <* 
    *>       kill (pid, SIGTERM);                                                     <* 
    *>    }                                                                           <* 
    *>    fclose (f);                                                                 <* 
    *> }                                                                              <*/
   /*---(call the shutdown)--------------*/
   switch (my.halt) {
   case 'H' :
      printf("   - halt\n");
      rc = reboot (RB_POWER_OFF);
      break;
   case 'R' :
   default  :
      printf("   - reboot\n");
      rc = reboot (RB_AUTOBOOT);
      break;
   }
   /*---(these calls shouldn't come back)*/
   return rc;
}


/*====================------------------------------------====================*/
/*===----                     signal handling                          ----===*/
/*====================------------------------------------====================*/
static void      o___SIGNALS_________________o (void) {;}

/*> void      PROG_signal       (int a_signal, siginfo_t *a_info, void *a_nada) {return;};   <*/

/*> void       /+----: handle signals --------------------------------------------+/                     <* 
 *> PROG_signal         (int a_signal, siginfo_t *a_info, void *a_nada)                                  <* 
 *> {                                                                                                    <* 
 *>    /+---(locals)-------*-----------------+/                                                          <* 
 *>    int       status    = 0;                                                                          <* 
 *>    int       xlink     = 0;                                                                          <* 
 *>    tPROC    *xdata     = NULL;                                                                       <* 
 *>    tRUSAGE   r_use;                                                                                  <* 
 *>    int       rc        = 0;                                                                          <* 
 *>    int       errsave   = 0;                                                                          <* 
 *>    DEBUG_SIGN   yLOG_note   ("CONF_comm called");                                                    <* 
 *>    DEBUG_SIGN   yLOG_value  ("a_signal"  , a_signal);                                                <* 
 *>    DEBUG_SIGN   yLOG_point  ("a_info"    , a_info);                                                  <* 
 *>    DEBUG_SIGN   yLOG_point  ("a_nada"    , a_nada);                                                  <* 
 *>    switch (a_signal) {                                                                               <* 
 *>    case  SIGCHLD:                                                                                    <* 
 *>       DEBUG_SIGN   yLOG_info    ("SIGNAL", "SIGCHLD means a child has terminated");                  <* 
 *>       xlink  = EXEC_find (a_info->si_pid);                                                           <* 
 *>       if (xlink < 0) break;                                                                          <* 
 *>       xdata = (tPROC*) yDLST_index (xlink);                                                          <* 
 *>       DEBUG_SIGN   yLOG_senter  ("sigchld");                                                         <* 
 *>       DEBUG_SIGN   yLOG_spoint  ((void *) a_info);                                                   <* 
 *>       DEBUG_SIGN   yLOG_svalue  ("pid"     , (int) a_info->si_pid);                                  <* 
 *>       DEBUG_SIGN   yLOG_snote   (xdata->name);                                                       <* 
 *>       xdata->rc = (int) a_info->si_status;                                                           <* 
 *>       DEBUG_SIGN   yLOG_svalue  ("rc"      , (int) a_info->si_status);                               <* 
 *>       DEBUG_SIGN   yLOG_snote   ("wait");                                                            <* 
 *>       rc = wait4(a_info->si_pid, &status, WNOHANG, &r_use);                                          <* 
 *>       if (rc <  0) errsave = errno;                                                                  <* 
 *>       DEBUG_SIGN   yLOG_svalue  ("wait_rc" , rc);                                                    <* 
 *>       DEBUG_SIGN   yLOG_snote   ("done");                                                            <* 
 *>       DEBUG_SIGN   yLOG_sexit   ("sigchld");                                                         <* 
 *>       xdata->status = 'c';                                                                           <* 
 *>       yDLST_active_off (xdata->dlst);                                                                <* 
 *>       xdata->end    = yLOG_time();;                                                                  <* 
 *>       if (rc >= 0) {                                                                                 <* 
 *>          if (strchr("abcm", xdata->type) != 0)  ++complete;                                          <* 
 *>          xdata->swaps  = (int) (r_use.ru_nvcsw + r_use.ru_nivcsw);                                   <* 
 *>          xdata->u_time = (int) ((r_use.ru_utime.tv_sec * 1000) + (r_use.ru_utime.tv_usec / 1000));   <* 
 *>          xdata->s_time = (int) ((r_use.ru_stime.tv_sec * 1000) + (r_use.ru_stime.tv_usec / 1000));   <* 
 *>       } else if (rc < 0) {                                                                           <* 
 *>          DEBUG_SIGN   yLOG_value   ("cerrno"    , errsave);                                          <* 
 *>          DEBUG_SIGN   yLOG_info    ("cerrstr"   , strerror(errsave));                                <* 
 *>       }                                                                                              <* 
 *>       EXEC_children (xdata->dlst);                                                                   <* 
 *>       break;                                                                                         <* 
 *>    case  SIGHUP:                                                                                     <* 
 *>       DEBUG_SIGN   yLOG_info  ("SIGNAL", "SIGHUP MEANS REFRESH");                                    <* 
 *>       break;                                                                                         <* 
 *>    case  SIGTERM:                                                                                    <* 
 *>       DEBUG_SIGN   yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon (but eos can't)");          <* 
 *>       break;                                                                                         <* 
 *>    case  SIGSEGV:                                                                                    <* 
 *>       DEBUG_SIGN   yLOG_info  ("SIGNAL", "SIGSEGV means daemon blew up (but eos can't)");            <* 
 *>       break;                                                                                         <* 
 *>    }                                                                                                 <* 
 *>    /+---(complete)------------------------------+/                                                   <* 
 *>    return;                                                                                           <* 
 *> }                                                                                                    <*/

/*> char       /+----: setup signal handling -------------------------------------+/   <* 
 *> CONF_signal          (void)                                                        <* 
 *> {                                                                                  <* 
 *>    /+---(design notes)-------------------+/                                        <* 
 *>    /+ options are...                                                               <* 
 *>     *    signal()        -- race conditions when they come fast (depricated)       <* 
 *>     *    sigaction()     -- preferred method and standard in c                     <* 
 *>     *    signalfd()      -- linux specific that treats it like a file              <* 
 *>     *                                                                              <* 
 *>     +/                                                                             <* 
 *>    /+---(set up structure)---------------+/                                        <* 
 *>    struct sigaction sa;                                                            <* 
 *>    /+---(begin)--------------------------+/                                        <* 
 *>    DEBUG_PROG   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    /+---(set actions)--------------------+/                                        <* 
 *>    sa.sa_handler   = NULL;                                                         <* 
 *>    sa.sa_sigaction = CONF_comm;                                                    <* 
 *>    sa.sa_flags     = SA_RESTART | SA_SIGINFO;                                      <* 
 *>    sigfillset (&sa.sa_mask);                                                       <* 
 *>    sa.sa_restorer  = NULL;                                                         <* 
 *>    DEBUG_SIGN   yLOG_info  ("children", "pay attention to children");              <* 
 *>    sigaction (SIGCHLD,  &sa      , NULL);       /+ watch for children         +/   <* 
 *>    DEBUG_SIGN   yLOG_info  ("program",  "look for HUP");                           <* 
 *>    sigaction (SIGHUP ,  &sa      , NULL);       /+ hangup means refresh       +/   <* 
 *>    DEBUG_SIGN   yLOG_info  ("baddies",  "handle SEGV and TERM");                   <* 
 *>    sigaction (SIGTERM,  &sa      , NULL);       /+ catch a kill               +/   <* 
 *>    sigaction (SIGSEGV,  &sa      , NULL);       /+ catch a segfault           +/   <* 
 *>    DEBUG_SIGN   yLOG_info  ("keyboard", "stop keyboard kills and pauses");         <* 
 *>    sigaction (SIGINT ,  &sa      , NULL);       /+ keyboard interrupt         +/   <* 
 *>    sigaction (SIGQUIT,  &sa      , NULL);       /+ keyboard quit              +/   <* 
 *>    sigaction (SIGTERM,  &sa      , NULL);       /+ terminate                  +/   <* 
 *>    sigaction (SIGCONT,  &sa      , NULL);       /+ continue                   +/   <* 
 *>    sigaction (SIGTSTP,  &sa      , NULL);       /+ keyboard stop              +/   <* 
 *>    DEBUG_SIGN   yLOG_info  ("terminal", "ignore stadin and stdout");               <* 
 *>    sigaction (SIGTTIN,  &sa      , NULL);       /+ tty input for background   +/   <* 
 *>    sigaction (SIGTTOU,  &sa      , NULL);       /+ tty output for background  +/   <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_PROG   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/



/*====================------------------------------------====================*/
/*===----                   helpers for unit testing                   ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}


char       /*----: set up program test file locations ------------------------*/
prog__unit_files   (void)
{
   char        x_cmd       [LEN_RECD];
   chdir    ("/tmp");
   sprintf  (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT);
   system   (x_cmd);
   rmdir    (DIR_UNIT);
   sprintf  (x_cmd, "mkdir %s   > /dev/null", DIR_UNIT);
   system   (x_cmd);
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_quiet   (void)
{
   int         x_argc      = 1;
   char       *x_argv [1]  = { "eos" };
   PROG_prestart   (x_argc, x_argv, 'y');
   PROG_debugging  (x_argc, x_argv, 'y');
   PROG_startup    (x_argc, x_argv, 'y');
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_loud    (void)
{
   int         x_argc      = 5;
   char       *x_argv [5]  = { "eos_debug", "@@kitchen", "@@yparse", "@@ydlst", "@@yexec"  };
   PROG_prestart   (x_argc, x_argv, 'y');
   PROG_debugging  (x_argc, x_argv, 'y');
   PROG_startup    (x_argc, x_argv, 'y');
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
prog__unit_end     (void)
{
   char        x_cmd       [LEN_RECD];
   chdir    ("/tmp");
   sprintf  (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT);
   system   (x_cmd);
   rmdir    (DIR_UNIT);
   PROG_end       ();
   return 0;
}

char*
prog__unit              (char *a_question)
{
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_HUND]  = "";
   char        u           [LEN_LABEL];
   /*---(prepare)------------------------*/
   ystrlcpy  (unit_answer, "RPTG             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "mode"    )        == 0) {
      /*> yJOBS_iam  (my.run_as  , s);                                                <*/
      /*> yJOBS_mode (my.run_mode, t);                                                <*/
      snprintf (unit_answer, LEN_RECD, "PROG mode        : iam (%c) %-18.18s, run (%c) %-18.18s, å%sæ", my.run_as, s, my.run_mode, t, my.run_file);
   }
   else if (strcmp (a_question, "action"        )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG action      : %c  %2då%sæ", my.run_mode, strlen (my.run_file), my.run_file);
   }
   /*> else if (strcmp (a_question, "args"          )  == 0) {                                      <* 
    *>    sprintf (s, "%2då%.15sæ", strlen (g_silent) , g_silent);                                  <* 
    *>    sprintf (t, "%2då%.15sæ", strlen (g_confirm), g_confirm);                                 <* 
    *>    sprintf (u, "%2då%.15sæ", strlen (g_verbose), g_verbose);                                 <* 
    *>    snprintf (unit_answer, LEN_HUND, "PROG args        : %-20.20s  %-20.20s  %s", s, t, u);   <* 
    *> }                                                                                            <*/
   /*---(complete)-----------------------*/
   return unit_answer;
}




/*============================[[ end-of-code ]]===============================*/
