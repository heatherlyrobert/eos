/*============================[[    beg-code    ]]============================*/
#include    "eos.h"






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
   strlcpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strlcpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   strlcpy (t, "[cbang      ]", 15);
#elif  __HEPH__  > 0
   strncpy (t, "[hephaestus ]", 18);
#else
   strlcpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, VER_NUM, VER_TXT);
   return verstring;
}

/*> char                                                                               <* 
 *> PROG_mountproc     (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(design notes)-------------------+/                                        <* 
 *>    /+ mount the /proc filesystem, if necessary  +/                                 <* 
 *>    /+---(locals)-----------+-----------+-+/                                        <* 
 *>    int         rc          = 0;             /+ generic return code            +/   <* 
 *>    FILE       *f           = NULL;          /+ generic file pointer           +/   <* 
 *>    /+---(try to open)--------------------+/                                        <* 
 *>    DEBUG_VIEW   printf        ("PROG_mountproc :");                                <* 
 *>    DEBUG_VIEW   printf (" /proc");                                                 <* 
 *>    f = fopen ("/proc/mounts", "r");                                                <* 
 *>    /+---(check mounting)-----------------+/                                        <* 
 *>    if (f != NULL) {                                                                <* 
 *>       DEBUG_VIEW   printf (", found, already mounted");                            <* 
 *>       my.status_proc    = 'a';  /+ already mounted +/                              <* 
 *>    }                                                                               <* 
 *>    /+---(if needed, mount)---------------+/                                        <* 
 *>    else {                                                                          <* 
 *>       DEBUG_VIEW   printf        (", mounting");                                   <* 
 *>       my.status_proc  = 'm';   /+ newly mounted +/                                 <* 
 *>       rc = mount ("proc"  , "/proc", "proc", 0, NULL);                             <* 
 *>       if (rc < 0)  {                                                               <* 
 *>          rc = errno;                                                               <* 
 *>          DEBUG_VIEW   printf (", error %d (%s), FATAL\n", rc, strerror (rc));      <* 
 *>          exit (-10);                                                               <* 
 *>       }                                                                            <* 
 *>       DEBUG_VIEW   printf        (", mounted");                                    <* 
 *>       f = fopen ("/proc/mounts", "r");                                             <* 
 *>       if (f == NULL) {                                                             <* 
 *>          printf (", error, no access /proc/mounts, FATAL\n");                      <* 
 *>          exit (-3);                                                                <* 
 *>       }                                                                            <* 
 *>    }                                                                               <* 
 *>    /+---(close)--------------------------+/                                        <* 
 *>    DEBUG_VIEW   printf (", close");                                                <* 
 *>    rc = fclose (f);                                                                <* 
 *>    if (rc < 0)  {                                                                  <* 
 *>       rc = errno;                                                                  <* 
 *>       DEBUG_VIEW   printf (", error %d (%s)", rc, strerror (rc));                  <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_VIEW   printf (", done\n");                                               <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char                                                                                     <* 
 *> PROG_logtest       (void)                                                                <* 
 *> {                                                                                        <* 
 *>    /+---(design notes)-------------------+/                                              <* 
 *>    /+                                                                                    <* 
 *>     *  make sure to respect existing mounts                                              <* 
 *>     +/                                                                                   <* 
 *>    /+---(locals)-----------+-----------+-+/                                              <* 
 *>    int         rc          = 0;             /+ generic return code            +/         <* 
 *>    FILE       *f           = NULL;          /+ generic file pointer           +/         <* 
 *>    char        x_recd      [LEN_RECD];      /+ file record entry              +/         <* 
 *>    /+---(try to open)--------------------+/                                              <* 
 *>    DEBUG_VIEW   printf ("PROG_logtest   :");                                             <* 
 *>    DEBUG_VIEW   printf (" find /var/log/yLOG");                                          <* 
 *>    f = fopen ("/proc/mounts", "r");                                                      <* 
 *>    /+---(prepare defaults)---------------+/                                              <* 
 *>    my.status_log     = '-';  /+ not mounted +/                                           <* 
 *>    /+---(check on log filesystem)--------+/                                              <* 
 *>    DEBUG_VIEW   printf (", searching /proc/mounts");                                     <* 
 *>    while (!feof (f)) {                                                                   <* 
 *>       fgets (x_recd, 450, f);                                                            <* 
 *>       if (x_recd[0] == 'v' && strncmp (x_recd, "varlog /var/log/yLOG ", 21) == 0) {      <* 
 *>          DEBUG_VIEW   printf (", tmpfs");                                                <* 
 *>          my.status_log = 'a';                                                            <* 
 *>          break;                                                                          <* 
 *>       }                                                                                  <* 
 *>       if (x_recd[0] == '/' && strncmp (x_recd, "/dev/sda3 /var/log/yLOG ", 24) == 0) {   <* 
 *>          DEBUG_VIEW   printf (", sda3");                                                 <* 
 *>          my.status_log = 'a';                                                            <* 
 *>          break;                                                                          <* 
 *>       }                                                                                  <* 
 *>    }                                                                                     <* 
 *>    /+---(summary)------------------------+/                                              <* 
 *>    if (my.status_log == '-') {                                                           <* 
 *>       DEBUG_VIEW   printf (", not mounted");                                             <* 
 *>    } else {                                                                              <* 
 *>       DEBUG_VIEW   printf (", mounted");                                                 <* 
 *>    }                                                                                     <* 
 *>    /+---(close)--------------------------+/                                              <* 
 *>    DEBUG_VIEW   printf (", close");                                                      <* 
 *>    rc = fclose (f);                                                                      <* 
 *>    if (rc < 0)  {                                                                        <* 
 *>       rc = errno;                                                                        <* 
 *>       DEBUG_VIEW   printf (", error %d (%s)", rc, strerror (rc));                        <* 
 *>    }                                                                                     <* 
 *>    /+---(complete)-----------------------+/                                              <* 
 *>    DEBUG_VIEW   printf (", done\n");                                                     <* 
 *>    return 0;                                                                             <* 
 *> }                                                                                        <*/

/*> char         /+--: look for screen debugging -------------[ leaf   [ ------ ]-+/   <* 
 *> PROG_urgview       (int a_argc, char *a_argv[])                                    <* 
 *> {                                                                                  <* 
 *>    /+---(design)-------------------------+/                                        <* 
 *>    /+ view mode is special and not included in @@full or @@kitchen or @@quiet +/   <* 
 *>    /+---(locals)-------------------------+/                                        <* 
 *>    int         i           = 0;             /+ loop iterator -- arguments     +/   <* 
 *>    char       *a           = NULL;          /+ current argument               +/   <* 
 *>    /+---(logger preprocessing)-----------+/                                        <* 
 *>    for (i = 1; i < a_argc; ++i) {                                                  <* 
 *>       a = a_argv[i];                                                               <* 
 *>       if (a[0] != '@')  continue;                                                  <* 
 *>       if       (strcmp ("@v"         , a) == 0)    debug.view    = 'y';            <* 
 *>       else if  (strcmp ("@@view"     , a) == 0)    debug.view    = 'y';            <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char         /+--> before even logger --------------------[ leaf-- [ ------ ]-+/   <* 
 *> PROG_preinit       (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-----------+-----------+-+/                                        <* 
 *>    int         rc          = 0;             /+ generic return code            +/   <* 
 *>    PROG_mountproc ();                                                              <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

char         /*--: pre-argument initialization -----------[ leaf-- [ ------ ]-*/
PROG_init          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   FILE       *f           = NULL;          /* generic file pointer           */
   char        x_recd      [LEN_RECD];      /* file record entry              */
   /*---(log header)---------------------*/
   DEBUG_TOPS   yLOG_info    ("purpose" , "simple, reliable, and transparent system initialization");
   DEBUG_TOPS   yLOG_info    ("namesake", "eos-rhododactylos (rosy-fingered)");
   DEBUG_TOPS   yLOG_info    ("heritage", "titaness of daybreak, opens the gates of heaven for the sun");
   DEBUG_TOPS   yLOG_info    ("imagery" , "radiant winged woman with golden arms and rosy fingers");
   DEBUG_TOPS   yLOG_info    ("eos"     , PROG_version    ());
   DEBUG_TOPS   yLOG_info    ("yPARSE"  , yPARSE_version  ());
   DEBUG_TOPS   yLOG_info    ("yDLST"   , yDLST_version   ());
   DEBUG_TOPS   yLOG_info    ("yEXEC"   , yEXEC_version   ());
   DEBUG_TOPS   yLOG_info    ("ySTR"    , ySTR_version    ());
   DEBUG_TOPS   yLOG_info    ("yLOG"    , yLOG_version    ());
   DEBUG_TOPS   yLOG_info    ("yURG"    , yURG_version    ());
   DEBUG_VIEW   printf       ("EOS-RHODODACTYLOS : simple, reliable, and transparent system initialization");
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW   printf       ("EOS_init     :");
   /*---(set globals)--------------------*/
   my.daemon         = 'y';
   my.init           = 'y';
   my.test           = '-';
   my.loop_msec      = 100;
   my.loop_max       = 500;
   /*---(call whoami)--------------------*/
   rc = yEXEC_whoami (&my.pid, &my.ppid, &my.uid, NULL, &my.who, 'n');
   DEBUG_TOPS   yLOG_value   ("whoami"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for root)-----------------*/
   DEBUG_TOPS   yLOG_value   ("my.uid"    , my.uid);
   --rce;  if (my.uid != 0) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defenses : pid)-----------------*/
   DEBUG_TOPS   yLOG_value   ("my.pid"    , my.pid);
   --rce;  if (my.pid != 1) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ARGS   yLOG_note    ("boot run confirmed");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
   /*---(check on proc filesystem)-------*/
   /*> PROG_mountproc ();                                                             <*/
   /*---(check on log filesystem)--------*/
   /*> PROG_logtest   ();                                                             <*/
   /*---(set file names)-----------------*/
   snprintf (my.name_conf   , 200, "%s%s", DIR_ETC  , FILE_CONF );
   snprintf (my.name_exec   , 200, "%s%s", DIR_YHIST, FILE_EXEC );
   snprintf (my.name_perf   , 200, "%s%s", DIR_YHIST, FILE_PERF );
   /*---(complete)-----------------------*/
   DEBUG_VIEW   printf        (", done\n");
   return 0;
}

char         /*--: evaluate command line arguments -------[ leaf   [ ------ ]-*/
PROG_args          (int a_argc, char *a_argv[])
{
   /*---(locals)-------------------------*/
   int         i           = 0;             /* loop iterator -- arguments     */
   char       *a           = NULL;          /* current argument               */
   int         x_len       = 0;             /* argument length                */
   int         x_total     = 0;             /* total argument count           */
   int         x_args      = 0;             /* argument count                 */
   int         x_num       = 0;             /* numeric argument holder        */
   /*---(process)------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW   printf        ("EOS -- arguments, ");
   for (i = 1; i < a_argc; ++i) {
      a = a_argv[i];
      if (a[0] == '@')       continue;
      ++x_args;
      DEBUG_ARGS  yLOG_info  ("argument"  , a);
      DEBUG_VIEW  printf ("%s, ", a);
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
      /*---(complex)---------------------*/
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
      /*> else if (strcmp(a, "--msec"        ) == 0) {                                <* 
       *>    if (i + 1 < argc) {                                                      <* 
       *>       x_num = atoi (argv [i + i]);                                          <* 
       *>       if (x_num > 0) {                                                      <* 
       *>          my.loop_msec = x_num;                                              <* 
       *>          ++i;                                                               <* 
       *>       } else {                                                              <* 
       *>          DEBUG_ARGS   yLOG_note    ("--msec value not numeric");            <* 
       *>       }                                                                     <* 
       *>    } else {                                                                 <* 
       *>       DEBUG_ARGS   yLOG_note    ("--msec value not available");             <* 
       *>    }                                                                        <* 
       *> }                                                                           <*/
   }
   DEBUG_VIEW   printf        ("done\n");
   /*---(display urgents)----------------*/
   DEBUG_ARGS   yLOG_note    ("summarization of argument processing");
   DEBUG_ARGS   yLOG_value   ("entries"   , x_total);
   DEBUG_ARGS   yLOG_value   ("arguments" , x_args);
   DEBUG_ARGS   yLOG_char    ("daemon"    , my.daemon);
   DEBUG_ARGS   yLOG_char    ("init"      , my.init);
   DEBUG_ARGS   yLOG_char    ("test"      , my.test);
   DEBUG_ARGS   yLOG_value   ("loop_msec" , my.loop_msec);
   DEBUG_ARGS   yLOG_value   ("loop_max"  , my.loop_max);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: final preparation for execution -------[ leaf   [ ------ ]-*/
PROG_begin         (void)
{
   /*---(locals)-----------+-------------*/
   int         rc          = 0;             /* generic return code            */
   FILE       *f           = NULL;          /* local file handle              */
   long        x_now       = 0;             /* present datetime               */
   tTIME      *x_broke     = NULL;
   char        x_msg       [200];
   int         x_uid       = 0;             /* running user id                */
   int         x_pid       = 0;             /* running process id             */
   char        x_recd      [LEN_RECD];
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW   printf       ("EOS -- begin, ");
   /*---(process initd.conf)--------------------*/
   /*> yLOG_info  ("UID",      "properly logged in as superuser");                            <* 
    *> if (my.init   == 'y')  yLOG_info  ("PID",      "running as process id 1");             <* 
    *> else                   yLOG_info  ("PID",      "running in anypid mode");              <* 
    *> if (my.status_log    == 'y')  yLOG_info  ("tmpfs",    "yLOG tmpfs already mounted");   <* 
    *> else                   yLOG_info  ("tmpfs",    "mounted the yLOG tmpfs");              <* 
    *> yLOG_info  ("logger",   "heatherly custom logger up and running");                     <*/
   /*---(startup dlst library)------------------*/
   DEBUG_ARGS   yLOG_info    ("yDLST"    ,"starting");
   yDLST_init ();
   rc = yPARSE_init  ('-', NULL, '-');
   rc = yPARSE_delimiters  ("§");
   /*---(clear feedback file)-------------------*/
   f         = fopen     (my.name_exec, "w");
   x_now     = time      (NULL);
   x_broke   = localtime (&x_now);
   strftime  (x_msg, 50, "%Ss %Mm %Hh %dd %mm  %ww", x_broke);
   fprintf   (f, "eos start : %s\n", x_msg);
   fclose    (f);
   /*---(setup signals)-------------------------*/
   rc = yEXEC_signal (YEXEC_SOFT, YEXEC_NO, YEXEC_NO, NULL);
   /*> rc = yEXEC_signal (YEXEC_HARD, YEXEC_NO, YEXEC_NO, NULL);                      <*/
   /*---(complete)------------------------------*/
   DEBUG_VIEW   printf       ("done\n");
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: wrapup program execution --------------[ leaf   [ ------ ]-*/
PROG_end           (void)
{
   /*---(create utmp boot)----------------------*/
   yLOG_enter (__FUNCTION__);
   yLOG_info  ("logger",   "shutting down logger");
   yLOG_exit  (__FUNCTION__);
   yLOG_end   ();
   return 0;
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
   snprintf (my.name_conf   , 200, "%s%s", DIR_UNIT , FILE_CONF );
   snprintf (my.name_exec   , 200, "%s%s", DIR_UNIT , FILE_EXEC );
   snprintf (my.name_perf   , 200, "%s%s", DIR_UNIT , FILE_PERF );
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
   yURG_logger    (x_argc, x_argv);
   yURG_urgs      (x_argc, x_argv);
   PROG_init      ();
   prog__unit_files ();
   PROG_args      (x_argc, x_argv);
   PROG_begin     ();
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_loud    (void)
{
   int         x_argc      = 5;
   char       *x_argv [5]  = { "eos_unit", "@@kitchen", "@@yparse", "@@ydlst", "@@yexec"  };
   yURG_logger    (x_argc, x_argv);
   yURG_urgs      (x_argc, x_argv);
   PROG_init      ();
   prog__unit_files ();
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




/*============================[[ end-of-code ]]===============================*/
