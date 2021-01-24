/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



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
   strlcpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strlcpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   strlcpy (t, "[cbang      ]", 15);
#elif  __HEPH__  > 0
   strlcpy (t, "[hephaestus ]", 18);
#else
   strlcpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}



/*====================------------------------------------====================*/
/*===----                      run-time behavior                       ----===*/
/*====================------------------------------------====================*/
static void      o___BEHAVIOR________________o (void) {;}

char*
PROG_iam           (char a_iam)
{
   switch (a_iam) {
   case IAM_EOS          :  return "eos-rhododactylos (rosy-fingered dawn)";  break;
   case IAM_NYX          :  return "nyx-melaina (goddess of night/darkness)"; break;
   case IAM_HYPNOS       :  return "hypnos-epidotes (giver of sleep)";        break;
   case IAM_HANNIBAL     :  return "hannibal-barca (father of strategy)";     break;
   default               :  return "unknown";                                 break;
   }
}

char*
PROG_runmode       (char a_run)
{
   switch (a_run) {
   case EOS_RUN_VERIFY   :  return "verification";                            break;
   case EOS_RUN_PRETEND  :  return "simulation";                              break;
   case EOS_RUN_NORMAL   :  return "foreground";                              break;
   case EOS_RUN_DAEMON   :  return "daemon";                                  break;
   default               :  return "unknown";                                 break;
   }
}

char
PROG_verbose       (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(default)------------------------*/
   my.verbose = '-';
   /*> printf ("PROG_verbose : ");                                                    <*/
   /*> printf ("%d", a_argc);                                                         <*/
   /*---(locate loud)--------------------*/
   for (i = 1; i < a_argc; ++i) {
      /*> printf (", %s", a_argv [i]);                                                <*/
      if (a_argv [i][0] != '-')       continue;
      if (a_argv [i][1] != '-')       continue;
      if (strcmp (a_argv [i], "--verbose") == 0)  my.verbose = 'y';
   }
   /*> printf (", %c", my.verbose);                                                   <*/
   /*> printf (", done\n");                                                           <*/
   /*---(complete)-----------------------*/
   return 0;
}


char         /*--> detirmine behavior --------------------[ leaf-- [ ------ ]-*/
PROG_runas              (int a_argc, char *a_argv[])
{
   /*---(defaults)-----------------------*/
   my.run_as         = '-';
   my.run_mode       = EOS_RUN_NORMAL;
   /*---(specific)-----------------------*/
   /* must also trap _debug versions */
   if      (strncmp (a_argv [0], "eos"           ,  3) == 0)    my.run_as = IAM_EOS;
   else if (strncmp (a_argv [0], "/sbin/eos"     ,  9) == 0)    my.run_as = IAM_EOS;
   else if (strcmp  (a_argv [0], "init"              ) == 0)    my.run_as = IAM_EOS;
   else if (strncmp (a_argv [0], "nyx"           ,  3) == 0)    my.run_as = IAM_NYX;
   else if (strncmp (a_argv [0], "/sbin/nyx"     ,  9) == 0)    my.run_as = IAM_NYX;
   else if (strcmp  (a_argv [0], "shutdown"          ) == 0)    my.run_as = IAM_NYX;
   else if (strcmp  (a_argv [0], "halt"              ) == 0)    my.run_as = IAM_NYX;
   else if (strcmp  (a_argv [0], "restart"           ) == 0)    my.run_as = IAM_NYX;
   else if (strncmp (a_argv [0], "hypnos"        ,  6) == 0)    my.run_as = IAM_HYPNOS;
   else if (strncmp (a_argv [0], "/sbin/hypnos"  , 12) == 0)    my.run_as = IAM_HYPNOS;
   else if (strncmp (a_argv [0], "hannibal"      ,  8) == 0)    my.run_as = IAM_HANNIBAL;
   else if (strncmp (a_argv [0], "/sbin/hannibal", 14) == 0)    my.run_as = IAM_HANNIBAL;
   else if (strcmp  (a_argv [0], "@"                 ) == 0)    my.run_as = IAM_HANNIBAL;
   else if (strcmp  (a_argv [0], "/sbin/@"           ) == 0)    my.run_as = IAM_HANNIBAL;
   else {
      return -1;
   }
   EOS_VERBOSE  printf ("\n%s job execution framework\n\n", PROG_iam (my.run_as));
   if (strcmp (my.n_conf, "") == 0) {
      switch (my.run_as) {
      case IAM_EOS       : snprintf (my.n_conf   , 200, "%seos%s"     , DIR_ETC , FILE_CONF); break;
      case IAM_NYX       : snprintf (my.n_conf   , 200, "%snyx%s"     , DIR_ETC , FILE_CONF); break;
      case IAM_HYPNOS    : snprintf (my.n_conf   , 200, "%shypnos%s"  , DIR_ETC , FILE_CONF); break;
      case IAM_UNIT      : snprintf (my.n_conf   , 200, "%sunit%s"    , DIR_UNIT, FILE_CONF); break;
      }
   }
   if (strcmp (my.n_exec, "") == 0) {
      switch (my.run_as) {
      case IAM_EOS       : snprintf (my.n_exec   , 200, "%seos%s"     , DIR_YLOG, FILE_EXEC); break;
      case IAM_NYX       : snprintf (my.n_exec   , 200, "%snyx%s"     , DIR_YLOG, FILE_EXEC); break;
      case IAM_HYPNOS    : snprintf (my.n_exec   , 200, "%shypnos%s"  , DIR_YLOG, FILE_EXEC); break;
      case IAM_HANNIBAL  : snprintf (my.n_exec   , 200, "%shannibal%s", DIR_YLOG, FILE_EXEC); break;
      case IAM_UNIT      : snprintf (my.n_exec   , 200, "%sunit%s"    , DIR_UNIT, FILE_EXEC); break;
      }
   }
   if (strcmp (my.n_perf, "") == 0) {
      switch (my.run_as) {
      case IAM_EOS       : snprintf (my.n_perf   , 200, "%seos%s"     , DIR_YLOG, FILE_PERF); break;
      case IAM_NYX       : snprintf (my.n_perf   , 200, "%snyx%s"     , DIR_YLOG, FILE_PERF); break;
      case IAM_HYPNOS    : snprintf (my.n_perf   , 200, "%shypnos%s"  , DIR_YLOG, FILE_PERF); break;
      case IAM_HANNIBAL  : snprintf (my.n_perf   , 200, "%shannibal%s", DIR_YLOG, FILE_PERF); break;
      case IAM_UNIT      : snprintf (my.n_perf   , 200, "%sunit%s"    , DIR_UNIT, FILE_PERF); break;
      }
   }
   DEBUG_TOPS   yLOG_info    ("conf"      , my.n_conf);
   DEBUG_TOPS   yLOG_info    ("exec"      , my.n_exec);
   DEBUG_TOPS   yLOG_info    ("perf"      , my.n_perf);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      startup functions                       ----===*/
/*====================------------------------------------====================*/
static void      o___STARTUP_________________o (void) {;}

char         /*--> before even logger --------------------[ leaf-- [ ------ ]-*/
PROG_boot               (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   FILE       *f           = NULL;          /* generic file pointer           */
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   int         c           =    0;
   int         x_tries     =    0;
   char        x_ylog      =  '-';
   /*---(quick out)----------------------*/
   if (my.run_as != IAM_EOS)  return 0;
   /*---(remount /)----------------------*/
   EOS_VERBOSE  printf ("PROG_boot  : ");
   rc = mount ("/dev/sda4", "/", "ext4", MS_REMOUNT | MS_NOATIME, NULL);
   EOS_VERBOSE  printf ("/ remounted (%d)", rc);
   /*---(check on /proc)-----------------*/
   /*> mkdir ("/proc", 0755);                                                         <*/
   EOS_VERBOSE  printf ("/proc");
   while (1) {
      EOS_VERBOSE  printf (" (%d)", x_tries);
      f = fopen ("/proc/mounts", "rt");
      if (f != NULL)    break;
      if (x_tries > 3)  break;
      EOS_VERBOSE  printf ("failed");
      rc = mount ("proc"  , "/proc", "proc", 0, NULL);
      ++x_tries;
   }
   --rce;  if (f == NULL) {
      EOS_VERBOSE  printf ("FATAL\n");
      return rce;
   }
   EOS_VERBOSE  printf ("success");
   /*---(show /proc/mounts)--------------*/
   while (1) {
      fgets (x_recd, 450, f);
      if (feof (f))  break;
      x_len = strlen (x_recd);
      if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
      /*> printf ("%2d [%s]\n", c, x_recd);                                           <*/
      ++c;
      if (strstr (x_recd, "yLOG") != NULL) {
         /*> printf ("yLOG temporary logging ramdisk found\n");                       <*/
         x_ylog = 'y';
      }
   }
   fclose (f);
   /*---(check on yLOG)------------------*/
   EOS_VERBOSE  printf (", yLOG");
   if (x_ylog == 'y') {
      EOS_VERBOSE  printf (" already mounted");
      return 0;
   } else {
      rc = mount ("varlog", "/var/log/yLOG", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME, "size=500m");
      EOS_VERBOSE  printf (" mount %d", rc);
   }
   /*---(close)--------------------------*/
   EOS_VERBOSE  printf ("done\n");
   return 0;
}

char         /*--: pre-argument initialization -----------[ leaf-- [ ------ ]-*/
PROG_init          (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   FILE       *f           = NULL;          /* generic file pointer           */
   int         x_pos       =   -1;
   char        x_which     =  '-';
   char        x_base      [LEN_LABEL] = "";
   /*---(log header)---------------------*/
   DEBUG_TOPS   yLOG_info    ("namesake", P_NAMESAKE);
   DEBUG_TOPS   yLOG_info    ("heritage", P_HERITAGE);
   DEBUG_TOPS   yLOG_info    ("imagery" , P_IMAGERY);
   DEBUG_TOPS   yLOG_info    ("purpose" , P_PURPOSE);
   DEBUG_TOPS   yLOG_info    ("eos"     , PROG_version    ());
   DEBUG_TOPS   yLOG_info    ("yPARSE"  , yPARSE_version  ());
   DEBUG_TOPS   yLOG_info    ("yDLST"   , yDLST_version   ());
   DEBUG_TOPS   yLOG_info    ("yEXEC"   , yEXEC_version   ());
   DEBUG_TOPS   yLOG_info    ("ySTR"    , ySTR_version    ());
   DEBUG_TOPS   yLOG_info    ("yLOG"    , yLOGS_version   ());
   DEBUG_TOPS   yLOG_info    ("yURG"    , yURG_version    ());
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   EOS_VERBOSE  printf ("PROG_init    : ");
   /*---(set globals)--------------------*/
   EOS_VERBOSE  printf ("defaults");
   my.done_done      = '-';
   my.test           = '-';
   my.loop_msec      = 100;
   my.loop_max       = my.loop_msec * 10 * 240; /* four minutes */
   strlcpy (my.dev, "/dev/tty1", LEN_LABEL);
   /*---(check run as)-------------------*/
   x_pos = strlproj (a_argv [0], x_base);
   x_which = x_base [0];
   /*> printf ("%-20.20s (%c) %s\n", a_argv [0], x_which, x_base);                    <*/
   /*> DEBUG_TOPS   yLOG_value   ("x_pos"     , x_pos);                               <*/
   /*> if (x_pos < 0)   x_which = IAM_EOS;                                            <* 
    *> else             x_which = a_argv [0][x_pos];                                  <*/
   DEBUG_TOPS   yLOG_char    ("x_which"   , x_which);
   /*---(check run as)-------------------*/
   DEBUG_TOPS   yLOG_info    ("valid"     , IAM_VALID);
   --rce;  if (strchr (IAM_VALID, x_which) == NULL) {
      EOS_VERBOSE  printf       (", run as %c failed (%d), FATAL\n", x_which, rc);
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   my.run_as = x_which;
   DEBUG_TOPS   yLOG_char    (", run_as"    , my.run_as);
   EOS_VERBOSE  printf (", run as %c", my.run_as);
   /*---(call whoami)--------------------*/
   rc = yEXEC_whoami (&my.pid, &my.ppid, &my.uid, NULL, &my.who, 'n');
   DEBUG_TOPS   yLOG_value   ("whoami"    , rc);
   --rce;  if (rc < 0) {
      EOS_VERBOSE  printf       (", whoami failed (%d), FATAL\n", rc);
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   EOS_VERBOSE  printf (", uid %d, pid %d, ppid, %d", my.uid, my.pid, my.ppid);
   DEBUG_TOPS   yLOG_value   ("pid"       , my.pid);
   DEBUG_TOPS   yLOG_value   ("ppid"      , my.ppid);
   DEBUG_TOPS   yLOG_value   ("uid"       , my.uid);
   DEBUG_TOPS   yLOG_info    ("who"       , my.who);
   /*---(check for root)-----------------*/
   --rce;  if (my.uid != 0) {
      EOS_VERBOSE  printf       (", check root failed, FATAL\n");;
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   EOS_VERBOSE  printf        (", done\n");
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

#define  TWOARG  if (++i >= a_argc)  yURG_error ("FATAL, %s argument requires an additional string", a, --rc); else 

char         /*--: evaluate command line arguments -------[ leaf   [ ------ ]-*/
PROG_args          (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        rc          =    0;
   char       *a           = NULL;          /* current argument               */
   int         x_len       = 0;             /* argument length                */
   int         x_total     = 0;             /* total argument count           */
   int         x_args      = 0;             /* argument count                 */
   int         x_num       = 0;             /* numeric argument holder        */
   /*---(process)------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   EOS_VERBOSE  printf       ("PROG_args    : %d args", a_argc);
   for (i = 1; i < a_argc; ++i) {
      a = a_argv[i];
      if (a[0] == '@')       continue;
      ++x_args;
      DEBUG_ARGS  yLOG_info     ("argument"  , a);
      EOS_VERBOSE  printf (", %s", a);
      /*---(run as)----------------------*/
      if      (strcmp (a, "--eos"       ) == 0)  my.run_as    = IAM_EOS;
      else if (strcmp (a, "--nyx"       ) == 0)  my.run_as    = IAM_NYX;
      else if (strcmp (a, "--hypnos"    ) == 0)  my.run_as    = IAM_HYPNOS;
      else if (strcmp (a, "--hannibal"  ) == 0)  my.run_as    = IAM_HANNIBAL;
      /*---(run modes)-------------------*/
      else if (strcmp (a, "--verify"    ) == 0)  my.run_mode  = EOS_RUN_VERIFY;
      else if (strcmp (a, "--pretend"   ) == 0)  my.run_mode  = EOS_RUN_PRETEND;
      else if (strcmp (a, "--normal"    ) == 0)  my.run_mode  = EOS_RUN_NORMAL;
      else if (strcmp (a, "--daemon"    ) == 0)  my.run_mode  = EOS_RUN_DAEMON;
      /*---(reporting)-------------------*/
      else if (strcmp (a, "--show_verb" ) == 0)  my.run_mode  = EOS_RPTG_VERBS;
      else if (strcmp (a, "--show_cntl" ) == 0)  my.run_mode  = EOS_RPTG_CONTROL;
      /*---(files)-----------------------*/
      else if (strcmp (a, "--conf"      ) == 0)  TWOARG rc = base_file_cli ("conf", a_argv [i]);
      else if (strcmp (a, "--exec"      ) == 0)  TWOARG rc = base_file_cli ("exec", a_argv [i]);
      else if (strcmp (a, "--perf"      ) == 0)  TWOARG rc = base_file_cli ("perf", a_argv [i]);
      /*---(speeds)----------------------*/
      else if (strcmp(a, "--normal"      ) == 0) {
         my.loop_msec  = 10;
         my.loop_max   = 1000;
      }
      else if (strcmp(a, "--fast"        ) == 0) {
         my.loop_msec  = 1;
         my.loop_max   = 10000;
      }
      else if (strcmp(a, "--slow"        ) == 0) {
         my.loop_msec  = 100;
         my.loop_max   = 100;
      }
      else if (strcmp(a, "--short"       ) == 0) {
         my.loop_msec  = 100;
         my.loop_max   = 20;
      }
   }
   /*---(verify)-------------------------*/
   /*> if (my.run_as == IAM_EOS && my.pid == 1)    my.run_mode  = EOS_RUN_NORMAL;     <* 
    *> else if (my.run_mode == EOS_RUN_DAEMON && my.pid != 1) {                       <* 
    *>    my.run_mode = EOS_RUN_NORMAL;                                               <* 
    *> }                                                                              <*/
   EOS_VERBOSE  printf (", mode %c", my.run_mode);
   /*---(display urgents)----------------*/
   DEBUG_ARGS   yLOG_note    ("summarization of argument processing");
   DEBUG_ARGS   yLOG_value   ("entries"   , x_total);
   DEBUG_ARGS   yLOG_value   ("arguments" , x_args);
   DEBUG_ARGS   yLOG_char    ("test"      , my.test);
   DEBUG_ARGS   yLOG_value   ("loop_msec" , my.loop_msec);
   DEBUG_ARGS   yLOG_value   ("loop_max"  , my.loop_max);
   /*---(complete)-----------------------*/
   EOS_VERBOSE  printf        (", done\n");
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*--: final preparation for execution -------[ leaf   [ ------ ]-*/
PROG_begin         (void)
{
   /*---(locals)-----------+-------------*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   EOS_VERBOSE  printf       ("PROG_begin   : ");
   /*---(daemonize)-----------------------------*/
   if (my.run_mode == EOS_RUN_DAEMON) {
      rc = yEXEC_daemon (yLOGS_lognum (), &my.pid);
      EOS_VERBOSE  printf       ("daemonizing (%d)", rc);
      DEBUG_TOPS   yLOG_value   ("daemonize" , rc);
      --rce;  if (rc < 0) {
         DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_TOPS   yLOG_note    ("operating as a semi-daemon (process one)");
      /*---(update console)---------------------*/
      rc = ySEC_open (my.dev, NULL, YEXEC_STDOUT, YEXEC_NO, YEXEC_NO);
      EOS_VERBOSE  printf       (", tty_open (%d)", rc);
      DEBUG_TOPS   yLOG_value   ("console"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      EOS_VERBOSE  printf       (", semi-daemon");
   } else {
      EOS_VERBOSE  printf       ("not daemonizing");
      DEBUG_TOPS   yLOG_note    ("will not use semi-daemon mode");
   }
   /*---(setup signals)-------------------------*/
   if (my.run_mode == EOS_RUN_DAEMON) {
      EOS_VERBOSE  printf       (", bullet-proof");
      DEBUG_TOPS   yLOG_note    ("signals set to bullet-proof (dangerous)");
      /*> rc = yEXEC_signal (YEXEC_HARD, YEXEC_NO, YEXEC_WAIT, NULL, NULL);           <*/
      rc = yEXEC_signal (YEXEC_SOFT, YEXEC_NO, YEXEC_YES, NULL, NULL);
   } else {
      EOS_VERBOSE  printf       (", normal signals");
      DEBUG_TOPS   yLOG_note    ("signals set to soft for normal working");
      rc = yEXEC_signal (YEXEC_SOFT, YEXEC_NO, YEXEC_WAIT, NULL, NULL);
   }
   DEBUG_TOPS   yLOG_value   ("signals"   , rc);
   --rce;  if (rc < 0) {
      EOS_VERBOSE  printf       (" failed, FATAL\n");
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(startup dlst library)------------------*/
   DEBUG_ARGS   yLOG_info    ("yDLST"    ,"initializing");
   rc = yDLST_init ();
   DEBUG_ARGS   yLOG_info    ("yPARSE"   ,"initializing");
   rc = yPARSE_init  ('-', NULL, '-');
   rc = yPARSE_delimiters  ("§");
   /*---(set file names)-----------------*/
   DEBUG_ARGS   yLOG_note    ("setting file names");
   /*---(complete)------------------------------*/
   EOS_VERBOSE  printf       (", done\n");
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG_final              (void)
{
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
   return 0;
}

char
PROG_shutdown           (void)
{
   printf("nyx -- system shutdown\n");
   /*---(locals)-------------------------*/
   FILE     *f         = NULL;
   int       rc        = 0;
   int       pid       = 0;
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
   if  (my.halt  == 'H') {
      printf("   - halt\n");
      rc = reboot (RB_POWER_OFF);
   }
   if  (my.halt  == 'R') {
      printf("   - reboot\n");
      rc = reboot (RB_AUTOBOOT);
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
 *>    DEBUG_TOPS   yLOG_enter   (__FUNCTION__);                                       <* 
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
 *>    DEBUG_TOPS   yLOG_exit    (__FUNCTION__);                                       <* 
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
   PROG_runas     (x_argc, x_argv);
   yURG_logger    (x_argc, x_argv);
   yURG_urgs      (x_argc, x_argv);
   PROG_init      (x_argc, x_argv);
   /*> prog__unit_files ();                                                           <*/
   PROG_args      (x_argc, x_argv);
   PROG_begin     ();
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_loud    (void)
{
   int         x_argc      = 5;
   char       *x_argv [5]  = { "eos_unit", "@@kitchen", "@@yparse", "@@ydlst", "@@yexec"  };
   PROG_runas     (x_argc, x_argv);
   yURG_logger    (x_argc, x_argv);
   yURG_urgs      (x_argc, x_argv);
   PROG_init      (x_argc, x_argv);
   /*> prog__unit_files ();                                                           <*/
   PROG_args      (x_argc, x_argv);
   PROG_begin     ();
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
prog__unit              (char *a_question, int a_num)
{
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "RPTG             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "mode"    )        == 0) {
      snprintf (unit_answer, LEN_RECD, "PROG mode        : iam (%c) %-40.40s, run (%c) %s", my.run_as, PROG_iam (my.run_as), my.run_mode, PROG_runmode (my.run_mode));
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}




/*============================[[ end-of-code ]]===============================*/
