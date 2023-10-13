/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



/*====================------------------------------------====================*/
/*===----                     command-line support                     ----===*/
/*====================------------------------------------====================*/
static void  o___CLI_____________o () { return; }

static llong  s_time  =  0;

llong             /* PURPOSE : timestamp in milliseconds      */
BASE_msec               (void)
{
   /* second
    * millisecond  ms  0.001 sec
    * microsecond  us  0.000001 sec
    * nanosecond   ns  0.000000001 sec
    * picosecond   ps  0.000000000001 sec
    */
   llong       a           =   0;
   tTSPEC      t;
   clock_gettime (CLOCK_MONOTONIC, &t);
   a += (llong) t.tv_sec  * 1000;
   a += (llong) t.tv_nsec / 1000000;
   if (s_time == 0)  s_time  = a;
   else              a      -= s_time;
   return a;
}

/*> char                                                                              <* 
 *> BASE_file_verify        (uchar *a_name)                                           <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    int         rci         =    0;                                                <* 
 *>    tSTAT       st;                                                                <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    if (a_name == NULL)                   return 0;                                <* 
 *>    if (strcmp (a_name, "") == 0)         return 0;                                <* 
 *>    /+---(check for existance)------------+/                                       <* 
 *>    rci = lstat (a_name, &st);                                                     <* 
 *>    DEBUG_FILE   yLOG_value   ("lstat"     , rci);                                 <* 
 *>    --rce; if (rci < 0) {                                                          <* 
 *>       DEBUG_FILE   yLOG_note    ("file does not exist, can not read");            <* 
 *>       DEBUG_FILE   yLOG_exit    (__FUNCTION__);                                   <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(check for regular file)---------+/                                       <* 
 *>    --rce;  if (!S_ISREG (st.st_mode)) {                                           <* 
 *>       DEBUG_FILE   yLOG_note    ("not a regular file, rejected");                 <* 
 *>       DEBUG_FILE   yLOG_exit    (__FUNCTION__);                                   <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(output)-------------------------+/                                       <* 
 *>    DEBUG_FILE   yLOG_note    ("confirmed as existing and is a regular file");     <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    return 1;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                                                   <* 
 *> BASE_file_cli           (char *a_terse, char *a_name)                                                  <* 
 *> {                                                                                                      <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                            <* 
 *>    char        rce         =  -10;                                                                     <* 
 *>    char        rc          =    0;                                                                     <* 
 *>    char        x_recd      [LEN_RECD]  = "";                                                           <* 
 *>    int         l           =    0;                                                                     <* 
 *>    char       *x_valid     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_/.";      <* 
 *>    int         i           =    0;                                                                     <* 
 *>    /+---(header)-------------------------+/                                                            <* 
 *>    DEBUG_FILE   yLOG_enter   (__FUNCTION__);                                                           <* 
 *>    /+---(defense)------------------------+/                                                            <* 
 *>    DEBUG_ARGS  yLOG_point   ("a_terse"   , a_terse);                                                   <* 
 *>    --rce;  if (a_terse == NULL) {                                                                      <* 
 *>       yURG_err ('f', "option can not be null");                                                        <* 
 *>       DEBUG_PROG  yLOG_exitr (__FUNCTION__, rce);                                                      <* 
 *>       return rce;                                                                                      <* 
 *>    }                                                                                                   <* 
 *>    DEBUG_ARGS  yLOG_info    ("a_terse"   , a_terse);                                                   <* 
 *>    DEBUG_ARGS  yLOG_point   ("a_name"    , a_name);                                                    <* 
 *>    --rce;  if (a_name == NULL) {                                                                       <* 
 *>       yURG_err ('f', "%s <name>, name can not be null", a_terse);                                      <* 
 *>       DEBUG_PROG  yLOG_exitr (__FUNCTION__, rce);                                                      <* 
 *>       return rce;                                                                                      <* 
 *>    }                                                                                                   <* 
 *>    DEBUG_ARGS  yLOG_info    ("a_name"    , a_name);                                                    <* 
 *>    ystrlcpy (x_recd, a_name, LEN_RECD);                                                                 <* 
 *>    /+---(check length)-------------------+/                                                            <* 
 *>    l = strlen (x_recd);                                                                                <* 
 *>    DEBUG_ARGS  yLOG_value   ("l"         , l);                                                         <* 
 *>    --rce;  if (l <= 0) {                                                                               <* 
 *>       yURG_err ('f', "%s <name>, name can not be blank/empty", a_terse);                               <* 
 *>       DEBUG_PROG  yLOG_exitr (__FUNCTION__, rce);                                                      <* 
 *>       return rce;                                                                                      <* 
 *>    }                                                                                                   <* 
 *>    /+---(check characters)---------------+/                                                            <* 
 *>    --rce;  for (i = 0; i < l; ++i) {                                                                   <* 
 *>       if (strchr (x_valid, x_recd [i]) != NULL)  continue;                                             <* 
 *>       yURG_err ('f', "%s <name>, name can not have a <%c> at character %d", a_terse, x_recd [i], i);   <* 
 *>       DEBUG_PROG  yLOG_char  ("bad char"  , x_recd [i]);                                               <* 
 *>       DEBUG_PROG  yLOG_exitr (__FUNCTION__, rce);                                                      <* 
 *>       return rce;                                                                                      <* 
 *>    }                                                                                                   <* 
 *>    /+---(copy)---------------------------+/                                                            <* 
 *>    --rce;  switch (a_terse [0]) {                                                                      <* 
 *>    case 'c' :                                                                                          <* 
 *>       rc = BASE_file_verify (x_recd);                                                                  <* 
 *>       if (rc < 0) {                                                                                    <* 
 *>          yURG_err ('f', "%s <name>, name can not be found", a_terse);                                  <* 
 *>          DEBUG_PROG  yLOG_exitr (__FUNCTION__, rce);                                                   <* 
 *>          return rce;                                                                                   <* 
 *>       }                                                                                                <* 
 *>       ystrlcpy (my.n_conf, x_recd, LEN_FULL);                                                           <* 
 *>       DEBUG_ARGS  yLOG_info    ("conf"      , my.n_conf);                                              <* 
 *>       break;                                                                                           <* 
 *>    case 'e' :                                                                                          <* 
 *>       ystrlcpy (my.n_exec, x_recd, LEN_FULL);                                                           <* 
 *>       DEBUG_ARGS  yLOG_info    ("exec"      , my.n_exec);                                              <* 
 *>       break;                                                                                           <* 
 *>    case 'p' :                                                                                          <* 
 *>       ystrlcpy (my.n_perf, x_recd, LEN_FULL);                                                           <* 
 *>       DEBUG_ARGS  yLOG_info    ("perf"      , my.n_perf);                                              <* 
 *>       break;                                                                                           <* 
 *>    }                                                                                                   <* 
 *>    /+---(complete)-----------------------+/                                                            <* 
 *>    DEBUG_FILE   yLOG_exit    (__FUNCTION__);                                                           <* 
 *>    return 0;                                                                                           <* 
 *> }                                                                                                      <*/


/*====================------------------------------------====================*/
/*===----                      daemons and signals                     ----===*/
/*====================------------------------------------====================*/
static void      o___DAEMON__________________o (void) {;}

char
BASE_console            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTERMIOS    x_tty;
   int         x_fd        =   -1;
   int         x_sid       =   -1;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(environment)--------------------*/
   rc = putenv ("TERM=console");
   printf (", term=%d", rc);
   DEBUG_PROG   yLOG_value   ("putenv"    , rc);
   --rce;  if (rc != 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open term)----------------------*/
   x_fd = open ("/dev/tty1", O_RDWR | O_NOCTTY | O_NONBLOCK);
   printf (", open %d", x_fd);
   DEBUG_PROG   yLOG_value   ("open"      , x_fd);
   --rce;  if (x_fd < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get attributes)-----------------*/
   rc = tcgetattr (x_fd, &x_tty);
   printf (", get %d", rc);
   DEBUG_PROG   yLOG_value   ("getattr"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(change attributes)--------------*/
   x_tty.c_cflag        &= CBAUD|CBAUDEX|CSIZE|CSTOPB|PARENB|PARODD;
   x_tty.c_cflag        |= HUPCL|CLOCAL|CREAD;
   x_tty.c_cc[VINTR]	    = CINTR;
   x_tty.c_cc[VQUIT]	    = CQUIT;
   x_tty.c_cc[VERASE]    = CERASE; /* ASCII DEL (0177) */
   x_tty.c_cc[VKILL]	    = CKILL;
   x_tty.c_cc[VEOF]	    = CEOF;
   x_tty.c_cc[VTIME]	    = 0;
   x_tty.c_cc[VMIN]	    = 1;
   x_tty.c_cc[VSWTC]	    = _POSIX_VDISABLE;
   x_tty.c_cc[VSTART]    = CSTART;
   x_tty.c_cc[VSTOP]	    = CSTOP;
   x_tty.c_cc[VSUSP]	    = CSUSP;
   x_tty.c_cc[VEOL]	    = _POSIX_VDISABLE;
   x_tty.c_cc[VREPRINT]  = CREPRINT;
   x_tty.c_cc[VDISCARD]  = CDISCARD;
   x_tty.c_cc[VWERASE]   = CWERASE;
   x_tty.c_cc[VLNEXT]    = CLNEXT;
   x_tty.c_cc[VEOL2]	    = _POSIX_VDISABLE;
   x_tty.c_iflag         = IGNPAR|ICRNL|IXON|IXANY;
   x_tty.c_oflag         = OPOST|ONLCR;
   x_tty.c_lflag         = ISIG|ICANON|ECHO|ECHOCTL|ECHOPRT|ECHOKE;
   x_tty.c_iflag        |=  IGNBRK;
   x_tty.c_iflag        &= ~(BRKINT|INLCR|IGNCR|IXON);
   x_tty.c_oflag        &= ~(OCRNL|ONLRET);
   /*---(set attributes)-----------------*/
   rc = tcsetattr (x_fd, TCSANOW, &x_tty);
   printf (", set %d", rc);
   DEBUG_PROG   yLOG_value   ("setattr"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(flush data)---------------------*/
   rc = tcflush (x_fd, TCIOFLUSH);
   printf (", flush %d", rc);
   DEBUG_PROG   yLOG_value   ("flush"     , x_fd);
   --rce;  if (x_fd < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close term)---------------------*/
   rc = close (x_fd);
   printf (", close %d", rc);
   DEBUG_PROG   yLOG_value   ("close"     , x_fd);
   --rce;  if (x_fd < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set session id)-----------------*/
   x_sid = setsid ();
   printf (", x_sid %d", x_sid);
   DEBUG_PROG   yLOG_value   ("x_sid"     , x_sid);
   /*> --rce;  if (x_sid < 0) {                                                       <* 
    *>    DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_execute            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTSPEC      x_dur;
   int         x_ifboot    =    0;
   char        x_args      [LEN_FULL];
   int         i           = 0;
   /*---(prepare)------------------------*/
   x_dur.tv_sec    = 0;
   x_dur.tv_nsec   = my.loop_msec * 1000000;
   /*---(enter main loop)----------------*/
   EOS_VERBOSE  printf       ("\nexecution begins\n");
   DEBUG_PROG   yLOG_break   ();
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   while (my.done_done != 'y') {
      /*---(looping)---------------------*/
      DEBUG_LOOP   yLOG_break   ();
      DEBUG_LOOP   yLOG_value   ("loop#"      , my.tic);
      my.msec = BASE_msec ();
      DEBUG_LOOP   yLOG_value   ("my.msec"    , my.msec);
      /*---(checking)--------------------*/
      rc = exec_check    (my.msec);
      rc = exec_finish   (my.msec);
      rc = exec_start    (my.msec);
      rc = exec_dispatch (my.msec);
      /*---(sleeping)--------------------*/
      nanosleep  (&x_dur, NULL);
      /*---(write sec ASAP)--------------*/
      if (x_ifboot == 0 &&
            access (WTMP, W_OK) == 0 &&
            access (UTMP, W_OK) != 0) {
         ySEC_startup ();
         x_ifboot = 1;
      }
      /*---(done)------------------------*/
      ++my.tic;
      if (my.tic > my.loop_max)  break;
   }
   DEBUG_PROG   yLOG_break   ();
   /*---(report out)---------------------*/
   if (my.done_done == 'y') {
      EOS_VERBOSE  printf       ("all jobs complete\n");
      DEBUG_PROG   yLOG_note    ("ALL JOBS COMPLETE");
   } else {
      EOS_VERBOSE  printf       ("stopped without all jobs complete\n");
      DEBUG_PROG   yLOG_note    ("STOPPED WITHOUT ALL JOBS COMPLETE");
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_kharon             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_args      [LEN_FULL];
   char        x_final     [LEN_FULL];
   int         i           = 0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   EOS_VERBOSE  printf       ("BASE_kharon  : %c, %c, %d", my.run_as, my.run_mode, my.pid);
   DEBUG_LOOP   yLOG_char    ("run_mode"   , my.run_mode);
   /*> --rce;  IF_DAEMON {                                   <* 
    *>    DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    EOS_VERBOSE  printf       (", failed\n");                                   <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(pass the torch)-----------------*/
   /*> ystrlcpy    (x_args, "/sbin/kharon --acheron --leisurely --listen --abcdefghijklmnopqrstuvwxyz --abcdefghijklmnopqrstuvwxyz", LEN_FULL);   <*/
   ystrlcpy    (x_args, "/sbin/kharon --acheron --leisurely --listen --abcdefghijklmnopqrst", LEN_FULL);
   /*> ystrlcpy    (x_args, "/sbin/kharon_debug @@kitchen @@yexec --acheron --leisurely --listen --abcdefghijklmnopqrstuvwxyz", LEN_FULL);   <*/
   /*> ystrlcpy    (x_args, "/sbin/kharon_debug @@kitchen --acheron --leisurely --listen", LEN_FULL);   <*/
                          /*123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789- */
   /*> ystrlcpy    (x_args, "/sbin/kharon --acheron --leisurely --listen --abcdefghijklmnopqrstu", LEN_FULL);   <*/
   /*> ystrlcpy    (x_args, "kharon-charopos (ferryman of dead souls) watching the acheron river", LEN_FULL);   <*/
   printf ("xargs  å%sæ\n", x_args);
   ystrlparse  (x_args, NULL, x_final, 20, &my.argc, my.argv, LEN_FULL);
   DEBUG_LOOP   yLOG_value   ("argc"       , my.argc);
   DEBUG_VIEW   printf ("arg count %d\n", my.argc);
   for (i = 0; i < my.argc; ++i) {
      DEBUG_LOOP   yLOG_bullet  (i            , my.argv [i]);
      DEBUG_VIEW   printf ("arg %2d <<%s>>\n", i, my.argv [i]);
   }
   /*---(pass the torch)-----------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   PROG_end   ();
   if (my.run_as == IAM_EOS && my.pid == 1)  {
      rc = execvp (*my.argv, my.argv);
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char                                                                                        <* 
 *> BASE_assimilate         (void)                                                              <* 
 *> {                                                                                           <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                 <* 
 *>    char        rce         =  -10;                                                          <* 
 *>    char        rc          =    0;                                                          <* 
 *>    /+---(header)-------------------------+/                                                 <* 
 *>    DEBUG_INPT  yLOG_enter   (__FUNCTION__);                                                 <* 
 *>    yURG_msg ('>', "read the configuration...");                                             <* 
 *>    yURG_msg ('-', "purge yDLST to make sure of a fresh start");                             <* 
 *>    yDLST_purge ();                                                                          <* 
 *>    yURG_msg ('-', "conf å%sæ", my.n_conf);                                                  <* 
 *>    DEBUG_INPT  yLOG_info    ("n_conf"     , my.n_conf);                                     <* 
 *>    yURG_msg ('-', "start yPARSE auto-reader");                                              <* 
 *>    rc = yPARSE_autoread (my.n_conf, NULL, BASE_handler);                                    <* 
 *>    yURG_msg (' ', "");                                                                      <* 
 *>    DEBUG_PROG  yLOG_value   ("config"    , rc);                                             <* 
 *>    --rce;  if (rc <  0) {                                                                   <* 
 *>       yURG_err ('f', "configration could not be properly read");                            <* 
 *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                         <* 
 *>       /+> wait_sec ("its over",   rc,  20);                                           <+/   <* 
 *>       /+> PROG_end ();                                                                <+/   <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    yURG_msg ('>', "configuration reading complete");                                        <* 
 *>    yURG_msg (' ', "");                                                                      <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    DEBUG_INPT  yLOG_exit    (__FUNCTION__);                                                 <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

/*> char                                                                                                                                    <* 
 *> FILE_assimilate_OLD     (cchar a_runas, cchar a_loc, cchar *a_name, char *r_user, char *r_desc)                                         <* 
 *> {                                                                                                                                       <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                                             <* 
 *>    char        rce         =  -10;                                                                                                      <* 
 *>    char        rc          =    0;                                                                                                      <* 
 *>    int         x_lists     =    0;                                                                                                      <* 
 *>    int         x_lines     =    0;                                                                                                      <* 
 *>    int         x_seqs      =    0;                                                                                                      <* 
 *>    /+---(header)-------------------------+/                                                                                             <* 
 *>    DEBUG_INPT  yLOG_enter   (__FUNCTION__);                                                                                             <* 
 *>    DEBUG_INPT  yLOG_info    ("a_name"     , a_name);                                                                                    <* 
 *>    /+---(default)------------------------+/                                                                                             <* 
 *>    yDLST_purge ();                                                                                                                      <* 
 *>    strcpy  (my.f_name, "");                                                                                                             <* 
 *>    strcpy  (my.f_full, "");                                                                                                             <* 
 *>    my.f_lines = 0;                                                                                                                      <* 
 *>    my.f_group = '-';                                                                                                                    <* 
 *>    my.f_gall  = my.f_gpass = my.f_gfail = 0;                                                                                            <* 
 *>    my.f_aall  = my.f_apass = my.f_afail = 0;                                                                                            <* 
 *>    my.f_pall  = my.f_ppass = my.f_pfail = 0;                                                                                            <* 
 *>    strcpy  (my.f_note, "");                                                                                                             <* 
 *>    /+---(parse file)---------------------+/                                                                                             <* 
 *>    /+> if      (a_loc == YJOBS_CENTRAL)   rc = yJOBS_central    (a_runas, a_name, my.f_user, &(my.f_uid), my.f_desc, my.f_dir);   <+/   <* 
 *>    /+> else if (a_loc == YJOBS_LOCAL  )   rc = yJOBS_acceptable (a_runas, a_name, my.f_user, &(my.f_uid), my.f_desc, my.f_dir);   <+/   <* 
 *>    DEBUG_INPT   yLOG_value   ("parse"     , rc);                                                                                        <* 
 *>    --rce;  if (rc < 0) {                                                                                                                <* 
 *>       DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
 *>       return rce;                                                                                                                       <* 
 *>    }                                                                                                                                    <* 
 *>    /+---(save some data)-----------------+/                                                                                             <* 
 *>    DEBUG_INPT  yLOG_info    ("f_dir"      , my.f_dir);                                                                                  <* 
 *>    DEBUG_INPT  yLOG_info    ("f_name"     , my.f_name);                                                                                 <* 
 *>    ystrlcpy (my.f_name, a_name, LEN_HUND);                                                                                               <* 
 *>    sprintf (my.f_full, "%s%s", my.f_dir, my.f_name);                                                                                    <* 
 *>    yURG_msg ('>', "calling AUTO-READER");                                                                                               <* 
 *>    /+---(pull all lines)-----------------+/                                                                                             <* 
 *>    DEBUG_INPT  yLOG_info    ("f_full"     , my.f_full);                                                                                 <* 
 *>    rc = yPARSE_autoread (my.f_full, NULL, BASE_handler);                                                                                <* 
 *>    DEBUG_PROG  yLOG_value   ("read"      , rc);                                                                                         <* 
 *>    --rce;  if (rc <  0) {                                                                                                               <* 
 *>       DEBUG_PROG  yLOG_note    ("no file found");                                                                                       <* 
 *>       yURG_msg (' ', "");                                                                                                               <* 
 *>       ystrlcpy (my.f_note, "NO FILE" , LEN_TERSE);                                                                                       <* 
 *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
 *>       return rce;                                                                                                                       <* 
 *>    }                                                                                                                                    <* 
 *>    x_lists  = yDLST_list_count ();                                                                                                      <* 
 *>    DEBUG_PROG  yLOG_value   ("x_lists"   , x_lists);                                                                                    <* 
 *>    x_lines  = yDLST_line_count (YDLST_GLOBAL);                                                                                          <* 
 *>    DEBUG_PROG  yLOG_value   ("x_lines"   , x_lines);                                                                                    <* 
 *>    x_seqs   = yDLST_seq_count  (YDLST_GLOBAL);                                                                                          <* 
 *>    DEBUG_PROG  yLOG_value   ("x_seqs"    , x_seqs);                                                                                     <* 
 *>    DEBUG_PROG  yLOG_value   ("f_lines"   , my.f_lines);                                                                                 <* 
 *>    yURG_msg (' ', "");                                                                                                                  <* 
 *>    yURG_msg ('>', "%d lines read, %d groups, %d procs, %d seqs", my.f_lines, x_lists, x_lines, x_seqs);                                 <* 
 *>    /+---(check empties)------------------+/                                                                                             <* 
 *>    --rce;  if (my.f_lines <= 0) {                                                                                                       <* 
 *>       yURG_err ('>', "NO LINES found");                                                                                                 <* 
 *>       DEBUG_PROG  yLOG_note    ("no input lines found");                                                                                <* 
 *>       ystrlcpy (my.f_note, "NO_INPUT" , LEN_TERSE);                                                                                      <* 
 *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
 *>       return rce;                                                                                                                       <* 
 *>    }                                                                                                                                    <* 
 *>    --rce;  if (x_lists <= 0) {                                                                                                          <* 
 *>       yURG_err ('>', "NO GROUPS created");                                                                                              <* 
 *>       DEBUG_PROG  yLOG_note    ("no groups created");                                                                                   <* 
 *>       ystrlcpy (my.f_note, "NO_GROUPS"  , LEN_TERSE);                                                                                    <* 
 *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
 *>       return rce;                                                                                                                       <* 
 *>    }                                                                                                                                    <* 
 *>    --rce;  if (x_lines <= 0) {                                                                                                          <* 
 *>       yURG_err ('>', "NO PROCS created");                                                                                               <* 
 *>       DEBUG_PROG  yLOG_note    ("no procs created");                                                                                    <* 
 *>       yURG_msg (' ', "");                                                                                                               <* 
 *>       ystrlcpy (my.f_note, "NO_PROCS"  , LEN_TERSE);                                                                                     <* 
 *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
 *>       return rce;                                                                                                                       <* 
 *>    }                                                                                                                                    <* 
 *>    /+---(save back)----------------------+/                                                                                             <* 
 *>    if (r_user != NULL)  ystrlcpy (r_user, my.f_user, LEN_LABEL);                                                                         <* 
 *>    if (r_desc != NULL)  ystrlcpy (r_desc, my.f_desc, LEN_DESC);                                                                          <* 
 *>    /+---(check trouble)------------------+/                                                                                             <* 
 *>    DEBUG_PROG  yLOG_complex ("fails"     , "%dg, %da, %dp", my.f_gfail, my.f_afail, my.f_pfail);                                        <* 
 *>    --rce;  if (my.f_gfail >  0) {                                                                                                       <* 
 *>       yURG_err ('F', "%d GROUPS failed", my.f_gfail);                                                                                   <* 
 *>       DEBUG_PROG  yLOG_note    ("some groups failed to read properly");                                                                 <* 
 *>       if (strcmp (my.f_note, "") == 0)  ystrlcpy (my.f_note, "BAD_GROUP" , LEN_TERSE);                                                   <* 
 *>    }                                                                                                                                    <* 
 *>    --rce;  if (my.f_afail >  0) {                                                                                                       <* 
 *>       yURG_err ('F', "%d AFTERS failed", my.f_afail);                                                                                   <* 
 *>       DEBUG_PROG  yLOG_note    ("some afters failed to read properly");                                                                 <* 
 *>       if (strcmp (my.f_note, "") == 0)  ystrlcpy (my.f_note, "BAD_AFTER" , LEN_TERSE);                                                   <* 
 *>    }                                                                                                                                    <* 
 *>    --rce;  if (my.f_pfail >  0) {                                                                                                       <* 
 *>       yURG_err ('F', "%d PROCS failed", my.f_pfail);                                                                                    <* 
 *>       DEBUG_PROG  yLOG_note    ("some procs failed to read properly");                                                                  <* 
 *>       if (strcmp (my.f_note, "") == 0)  ystrlcpy (my.f_note, "BAD_PROC"  , LEN_TERSE);                                                   <* 
 *>    }                                                                                                                                    <* 
 *>    if (my.f_gfail + my.f_afail + my.f_pfail >  0) {                                                                                     <* 
 *>       yURG_msg (' ', "");                                                                                                               <* 
 *>       DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                                                                                     <* 
 *>       return rce;                                                                                                                       <* 
 *>    }                                                                                                                                    <* 
 *>    yURG_msg ('>', "all read correctly, SUCCESS, reviewed %d, accepted %d", my.f_lines, x_lines);                                        <* 
 *>    yURG_msg (' ', "");                                                                                                                  <* 
 *>    ystrlcpy (my.f_note, "success"  , LEN_TERSE);                                                                                         <* 
 *>    /+---(complete)-----------------------+/                                                                                             <* 
 *>    DEBUG_INPT  yLOG_exit    (__FUNCTION__);                                                                                             <* 
 *>    return 0;                                                                                                                            <* 
 *> }                                                                                                                                       <*/


char
BASE_handler            (int n, uchar *a_verb, char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(dispatch)-----------------------*/
   ++my.f_lines;
   switch (a_verb [0]) {
   case 'G' :
      ++my.f_gall;
      rc = group_handler (n, a_verb);
      if (rc < 0) ++my.f_gfail; else  ++my.f_gpass;
      break;
   case 'A' :
      ++my.f_aall;
      rc = after_handler (n, a_verb);
      if (rc < 0) ++my.f_afail; else  ++my.f_apass;
      break;
   default  :
      ++my.f_pall;
      rc = proc_handler (n, a_verb);
      if (rc < 0) ++my.f_pfail; else  ++my.f_ppass;
      break;
   }
   DEBUG_INPT  yLOG_value   ("handler"   , rc);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
BASE__prepare           (cchar a_loc, cchar *a_full, cchar *a_fname, cchar *a_fuser, int a_fuid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(reset content)------------------*/
   yDLST_purge ();
   /*---(reset globals)------------------*/
   my.f_lines = 0;
   my.f_group = '-';
   my.f_gall  = my.f_gpass = my.f_gfail = 0;
   my.f_aall  = my.f_apass = my.f_afail = 0;
   my.f_pall  = my.f_ppass = my.f_pfail = 0;
   strcpy  (my.f_note, "");
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_full"    , a_full);
   --rce;  if (a_full == NULL) {
      yURG_err ('f', "file full name not provided", rc);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "full name  %2då%sæ", strlen (a_full), a_full);
   DEBUG_INPT   yLOG_info    ("a_full"    , a_full);
   ystrlcpy (my.f_full , a_full , LEN_PATH);
   DEBUG_INPT   yLOG_point   ("a_fname"   , a_fname);
   --rce;  if (a_fname == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "short name %2då%sæ", strlen (a_fname), a_fname);
   DEBUG_INPT   yLOG_info    ("a_fname"   , a_fname);
   ystrlcpy (my.f_name , a_fname, LEN_HUND);
   DEBUG_INPT   yLOG_point   ("a_fuser"   , a_fuser);
   --rce;  if (a_fuser == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "user name  %2då%sæ", strlen (a_fuser), a_fuser);
   DEBUG_INPT   yLOG_info    ("a_fuser"   , a_fuser);
   ystrlcpy (my.f_user , a_fuser, LEN_USER);
   DEBUG_INPT   yLOG_value   ("a_fuid"    , a_fuid);
   --rce;  if (a_fuid < 0 || a_fuid > 10000) {
      yURG_err ('f', "user id      %d, is unaccepable", a_fuid);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "user id      %d", a_fuid);
   my.f_uid  = a_fuid;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_pull_detail        (cchar a_loc, cchar *a_full, cchar *a_fname, cchar *a_fuser, int a_fuid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_lists     =    0;
   int         x_lines     =    0;
   int         x_seqs      =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = BASE__prepare  (a_loc, a_full, a_fname, a_fuser, a_fuid);
   DEBUG_INPT   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read all lines)-----------------*/
   yURG_msg ('-', "calling auto-reader in yPARSE");
   DEBUG_INPT  yLOG_info    ("f_full"     , a_full);
   rc = yPARSE_autoread (a_full, NULL, BASE_handler);
   DEBUG_PROG  yLOG_value   ("read"      , rc);
   /*---(trouble)------------------------*/
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_note    ("no file found");
      yURG_msg (' ', "");
      ystrlcpy (my.f_note, "NO FILE" , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get statistics)-----------------*/
   x_lists  = yDLST_list_count ();
   DEBUG_PROG  yLOG_value   ("x_lists"   , x_lists);
   x_lines  = yDLST_line_count (YDLST_GLOBAL);
   DEBUG_PROG  yLOG_value   ("x_lines"   , x_lines);
   x_seqs   = yDLST_seq_count  (YDLST_GLOBAL);
   DEBUG_PROG  yLOG_value   ("x_seqs"    , x_seqs);
   DEBUG_PROG  yLOG_value   ("f_lines"   , my.f_lines);
   yURG_msg (' ', "");
   yURG_msg ('>', "%d lines read, %d groups, %d procs, %d seqs", my.f_lines, x_lists, x_lines, x_seqs);
   /*---(verify the results)-------------*/
   --rce;  if (my.f_lines <= 0) {
      yURG_err ('>', "NO LINES found");
      DEBUG_PROG  yLOG_note    ("no input lines found");
      ystrlcpy (my.f_note, "NO_INPUT" , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_lists <= 0) {
      yURG_err ('>', "NO GROUPS created");
      DEBUG_PROG  yLOG_note    ("no groups created");
      ystrlcpy (my.f_note, "NO_GROUPS"  , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_lines <= 0) {
      yURG_err ('>', "NO PROCS created");
      DEBUG_PROG  yLOG_note    ("no procs created");
      yURG_msg (' ', "");
      ystrlcpy (my.f_note, "NO_PROCS"  , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check trouble)------------------*/
   DEBUG_PROG  yLOG_complex ("fails"     , "%dg, %da, %dp", my.f_gfail, my.f_afail, my.f_pfail);
   --rce;  if (my.f_gfail >  0) {
      yURG_err ('F', "%d GROUPS failed", my.f_gfail);
      DEBUG_PROG  yLOG_note    ("some groups failed to read properly");
      if (strcmp (my.f_note, "") == 0)  ystrlcpy (my.f_note, "BAD_GROUP" , LEN_TERSE);
   }
   --rce;  if (my.f_afail >  0) {
      yURG_err ('F', "%d AFTERS failed", my.f_afail);
      DEBUG_PROG  yLOG_note    ("some afters failed to read properly");
      if (strcmp (my.f_note, "") == 0)  ystrlcpy (my.f_note, "BAD_AFTER" , LEN_TERSE);
   }
   --rce;  if (my.f_pfail >  0) {
      yURG_err ('F', "%d PROCS failed", my.f_pfail);
      DEBUG_PROG  yLOG_note    ("some procs failed to read properly");
      if (strcmp (my.f_note, "") == 0)  ystrlcpy (my.f_note, "BAD_PROC"  , LEN_TERSE);
   }
   if (my.f_gfail + my.f_afail + my.f_pfail >  0) {
      yURG_msg (' ', "");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(show success)-------------------*/
   yURG_msg ('>', "all read correctly, SUCCESS, reviewed %d, accepted %d", my.f_lines, x_lines);
   yURG_msg (' ', "");
   ystrlcpy (my.f_note, "success"  , LEN_TERSE);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_pull               (cchar *a_fname)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_loc       =  '-';
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(begin feedback)-----------------*/
   yURG_msg ('>', "verify the contents of a configuation file (pull)...");
   DEBUG_INPT   yLOG_point   ("a_fname"   , a_fname);
   --rce;  if (a_fname == NULL) {
      yURG_err ('f', "file name not provided to function");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "requested file %då%sæ", strlen (a_fname), a_fname);
   DEBUG_INPT   yLOG_info    ("a_fname"   , a_fname);
   /*---(collect data)-------------------*/
   rc = yJOBS_filedata (&(my.run_as), &(my.run_mode), &x_loc, my.f_name, my.f_user, &(my.f_uid), my.f_desc, my.f_dir, my.f_full);
   DEBUG_INPT   yLOG_value   ("filedata"  , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not get file data from yJOBS (%d)", rc);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "file data request successful");
   /*> yURG_msg ('-', "short name %2då%sæ", strlen (my.f_name), my.f_name);           <* 
    *> yURG_msg ('-', "full name  %2då%sæ", strlen (my.f_full), my.f_full);           <* 
    *> yURG_msg ('-', "user name  %2då%sæ", strlen (my.f_user), my.f_user);           <* 
    *> yURG_msg ('-', "user id       %d"  , my.f_uid);                                <*/
   DEBUG_INPT   yLOG_char    ("run_as"    , my.run_as);
   DEBUG_INPT   yLOG_char    ("run_mode"  , my.run_mode);
   DEBUG_INPT   yLOG_char    ("x_loc"     , x_loc);
   DEBUG_INPT   yLOG_info    ("f_name"    , my.f_name);
   DEBUG_INPT   yLOG_info    ("f_user"    , my.f_user);
   DEBUG_INPT   yLOG_value   ("f_uid"     , my.f_uid);
   DEBUG_INPT   yLOG_info    ("f_desc"    , my.f_desc);
   DEBUG_INPT   yLOG_info    ("f_full"    , my.f_full);
   /*---(call detail)--------------------*/
   rc = BASE_pull_detail (x_loc, my.f_full, my.f_name, my.f_user, my.f_uid);
   DEBUG_INPT   yLOG_value   ("detail"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char                                                                              <* 
 *> BASE_purgeall           (void)                                                    <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    tGROUP     *x_group     = NULL;                                                <* 
 *>    tPROC      *x_proc      = NULL;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_INPT  yLOG_enter   (__FUNCTION__);                                       <* 
 *>    /+---(destroy procs)------------------+/                                       <* 
 *>    yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_DHEAD, NULL, &x_proc);               <* 
 *>    while (x_proc != NULL) {                                                       <* 
 *>       yDLST_line_destroy   (x_proc->tracker);                                     <* 
 *>       yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_DNEXT, NULL, &x_proc);            <* 
 *>    }                                                                              <* 
 *>    /+---(destroy groups)-----------------+/                                       <* 
 *>    yDLST_list_by_cursor (YDLST_DHEAD, NULL, &x_group);                            <* 
 *>    while (x_group != NULL) {                                                      <* 
 *>       yDLST_list_destroy   (x_group->name);                                       <* 
 *>       yDLST_list_by_cursor (YDLST_DNEXT, NULL, &x_group);                         <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_INPT  yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char        unit_answer [LEN_RECD];

char*        /*-> tbd --------------------------------[ light  [us.JC0.271.X1]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
base__unit              (char *a_question)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_exist     =  '-';
   int         n           =    0;
   /*---(defense)------------------------*/
   snprintf (unit_answer, LEN_RECD, "BASE unit        : question unknown");
   /*---(simple)-------------------------*/
   if      (strcmp (a_question, "conf"      )     == 0) {
      /*> rc = BASE_file_verify (my.n_conf);                                          <*/
      /*> if      (rc >  0)  x_exist = 'y';                                           <* 
       *> else if (rc <= 0)  x_exist = '-';                                           <*/
      snprintf (unit_answer, LEN_RECD, "BASE conf        : %c  %2d[%s]",
            x_exist, strlen (my.run_file), my.run_file);
   }
   else if (strcmp (a_question, "exec"      )     == 0) {
      snprintf (unit_answer, LEN_RECD, "BASE exec        : %c  %2d[%s]",
            x_exist, strlen (my.n_exec), my.n_exec);
   }
   else if (strcmp (a_question, "perf"      )     == 0) {
      snprintf (unit_answer, LEN_RECD, "BASE perf        : %c  %2d[%s]",
            x_exist, strlen (my.n_perf), my.n_perf);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}





/*============================[[ end-of-code ]]===============================*/
