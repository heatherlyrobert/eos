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

char
BASE_file_verify        (uchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rci         =    0;
   tSTAT       st;
   /*---(defense)------------------------*/
   if (a_name == NULL)                   return 0;
   if (strcmp (a_name, "") == 0)         return 0;
   /*---(check for existance)------------*/
   rci = lstat (a_name, &st);
   DEBUG_FILE   yLOG_value   ("lstat"     , rci);
   --rce; if (rci < 0) {
      DEBUG_FILE   yLOG_note    ("file does not exist, can not read");
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(check for regular file)---------*/
   --rce;  if (!S_ISREG (st.st_mode)) {
      DEBUG_FILE   yLOG_note    ("not a regular file, rejected");
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(output)-------------------------*/
   DEBUG_FILE   yLOG_note    ("confirmed as existing and is a regular file");
   /*---(complete)-----------------------*/
   return 1;
}

char
BASE_file_cli           (char *a_terse, char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_RECD]  = "";
   int         l           =    0;
   char       *x_valid     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_/.";
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ARGS  yLOG_point   ("a_terse"   , a_terse);
   --rce;  if (a_terse == NULL) {
      yURG_err ('f', "option can not be null");
      DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ARGS  yLOG_info    ("a_terse"   , a_terse);
   DEBUG_ARGS  yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      yURG_err ('f', "%s <name>, name can not be null", a_terse);
      DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ARGS  yLOG_info    ("a_name"    , a_name);
   strlcpy (x_recd, a_name, LEN_RECD);
   /*---(check length)-------------------*/
   l = strlen (x_recd);
   DEBUG_ARGS  yLOG_value   ("l"         , l);
   --rce;  if (l <= 0) {
      yURG_err ('f', "%s <name>, name can not be blank/empty", a_terse);
      DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(check characters)---------------*/
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr (x_valid, x_recd [i]) != NULL)  continue;
      yURG_err ('f', "%s <name>, name can not have a <%c> at character %d", a_terse, x_recd [i], i);
      DEBUG_TOPS  yLOG_char  ("bad char"  , x_recd [i]);
      DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy)---------------------------*/
   --rce;  switch (a_terse [0]) {
   case 'c' :
      rc = BASE_file_verify (x_recd);
      if (rc < 0) {
         yURG_err ('f', "%s <name>, name can not be found", a_terse);
         DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rce);
         return rce;
      }
      strlcpy (my.n_conf, x_recd, LEN_FULL);
      DEBUG_ARGS  yLOG_info    ("conf"      , my.n_conf);
      break;
   case 'e' :
      strlcpy (my.n_exec, x_recd, LEN_FULL);
      DEBUG_ARGS  yLOG_info    ("exec"      , my.n_exec);
      break;
   case 'p' :
      strlcpy (my.n_perf, x_recd, LEN_FULL);
      DEBUG_ARGS  yLOG_info    ("perf"      , my.n_perf);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}


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
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(environment)--------------------*/
   rc = putenv ("TERM=console");
   printf (", term=%d", rc);
   DEBUG_TOPS   yLOG_value   ("putenv"    , rc);
   --rce;  if (rc != 0) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open term)----------------------*/
   x_fd = open ("/dev/tty1", O_RDWR | O_NOCTTY | O_NONBLOCK);
   printf (", open %d", x_fd);
   DEBUG_TOPS   yLOG_value   ("open"      , x_fd);
   --rce;  if (x_fd < 0) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get attributes)-----------------*/
   rc = tcgetattr (x_fd, &x_tty);
   printf (", get %d", rc);
   DEBUG_TOPS   yLOG_value   ("getattr"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
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
   DEBUG_TOPS   yLOG_value   ("setattr"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(flush data)---------------------*/
   rc = tcflush (x_fd, TCIOFLUSH);
   printf (", flush %d", rc);
   DEBUG_TOPS   yLOG_value   ("flush"     , x_fd);
   --rce;  if (x_fd < 0) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close term)---------------------*/
   rc = close (x_fd);
   printf (", close %d", rc);
   DEBUG_TOPS   yLOG_value   ("close"     , x_fd);
   --rce;  if (x_fd < 0) {
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set session id)-----------------*/
   x_sid = setsid ();
   printf (", x_sid %d", x_sid);
   DEBUG_TOPS   yLOG_value   ("x_sid"     , x_sid);
   /*> --rce;  if (x_sid < 0) {                                                       <* 
    *>    DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
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
   DEBUG_TOPS   yLOG_break   ();
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
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
   DEBUG_TOPS   yLOG_break   ();
   /*---(report out)---------------------*/
   if (my.done_done == 'y') {
      EOS_VERBOSE  printf       ("all jobs complete\n");
      DEBUG_TOPS   yLOG_note    ("ALL JOBS COMPLETE");
   } else {
      EOS_VERBOSE  printf       ("stopped without all jobs complete\n");
      DEBUG_TOPS   yLOG_note    ("STOPPED WITHOUT ALL JOBS COMPLETE");
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
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
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   EOS_VERBOSE  printf       ("BASE_kharon  : %c, %c, %d", my.run_as, my.run_mode, my.pid);
   DEBUG_LOOP   yLOG_char    ("run_mode"   , my.run_mode);
   /*> --rce;  IF_DAEMON {                                   <* 
    *>    DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    EOS_VERBOSE  printf       (", failed\n");                                   <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(pass the torch)-----------------*/
   /*> strlcpy    (x_args, "/sbin/kharon --acheron --leisurely --listen --abcdefghijklmnopqrstuvwxyz --abcdefghijklmnopqrstuvwxyz", LEN_FULL);   <*/
   /*> strlcpy    (x_args, "/sbin/kharon_debug @@kitchen @@yexec --acheron --leisurely --listen --abcdefghijklmnopqrstuvwxyz", LEN_FULL);   <*/
   /*> strlcpy    (x_args, "/sbin/kharon_debug @@kitchen --acheron --leisurely --listen", LEN_FULL);   <*/
   strlcpy    (x_args, "/sbin/kharon --acheron --leisurely --listen --abcdefghijklmnopqrstuvwxyz", LEN_FULL);
   strlparse  (x_args, NULL, x_final, 20, &my.argc, my.argv, LEN_FULL);
   DEBUG_LOOP   yLOG_value   ("argc"       , my.argc);
   DEBUG_VIEW   printf ("arg count %d\n", my.argc);
   for (i = 0; i < my.argc; ++i) {
      DEBUG_LOOP   yLOG_bullet  (i            , my.argv [i]);
      DEBUG_VIEW   printf ("arg %2d <<%s>>\n", i, my.argv [i]);
   }
   /*---(pass the torch)-----------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   PROG_end   ();
   if (my.pid == 1)   rc = execvp (*my.argv, my.argv);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_handler            (int n, uchar *a_verb, char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(dispatch)-----------------------*/
   switch (a_verb [0]) {
   case 'G' :
      rc = group_handler (n, a_verb);
      break;
   case 'A' :
      rc = after_handler (n, a_verb);
      break;
   default  :
      rc = proc_handler (n, a_verb);
      break;
   }
   ++my.f_lines;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_assimilate         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "read the configuration...");
   yURG_msg ('-', "purge yDLST to make sure of a fresh start");
   yDLST_purge ();
   yURG_msg ('-', "conf å%sæ", my.n_conf);
   DEBUG_INPT  yLOG_info    ("n_conf"     , my.n_conf);
   yURG_msg ('-', "start yPARSE auto-reader");
   rc = yPARSE_autoread (my.n_conf, NULL, BASE_handler);
   yURG_msg (' ', "");
   DEBUG_PROG  yLOG_value   ("config"    , rc);
   --rce;  if (rc <  0) {
      yURG_err ('f', "configration could not be properly read");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      /*> wait_sec ("its over",   rc,  20);                                           <*/
      /*> PROG_end ();                                                                <*/
      return rce;
   }
   yURG_msg ('>', "configuration reading complete");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FILE_assimilate         (cchar a_runas, cchar a_loc, cchar *a_name, char *r_user, char *r_desc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_lists     =    0;
   int         x_lines     =    0;
   int         x_seqs      =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   strcpy  (my.f_name, "");
   strcpy  (my.f_full, "");
   my.f_lines = 0;
   strcpy  (my.f_note, "");
   /*---(parse file)---------------------*/
   if      (a_loc == YJOBS_CENTRAL)   rc = yJOBS_central    (a_runas, a_name, my.f_user, my.f_uid, my.f_desc, my.f_dir);
   else if (a_loc == YJOBS_LOCAL  )   rc = yJOBS_acceptable (a_runas, a_name, my.f_user, my.f_uid, my.f_desc, my.f_dir);
   DEBUG_INPT   yLOG_value   ("parse"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save some data)-----------------*/
   strlcpy (my.f_name, a_name, LEN_HUND);
   sprintf (my.f_full, "%s%s", my.f_dir, my.f_name);
   yURG_msg ('-', "calling auto-reader");
   yURG_msg (' ', "");
   /*---(pull all lines)-----------------*/
   DEBUG_INPT  yLOG_info    ("f_full"     , my.f_full);
   rc = yPARSE_autoread (my.f_full, NULL, BASE_handler);
   DEBUG_PROG  yLOG_value   ("read"      , rc);
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_note    ("no file found");
      strlcpy (my.f_note, "NO FILE" , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_lists  = yDLST_list_count ();
   DEBUG_PROG  yLOG_value   ("x_lists"   , x_lists);
   x_lines  = yDLST_line_count (YDLST_GLOBAL);
   DEBUG_PROG  yLOG_value   ("x_lines"   , x_lines);
   x_seqs   = yDLST_seq_count  (YDLST_GLOBAL);
   DEBUG_PROG  yLOG_value   ("x_seqs"    , x_seqs);
   DEBUG_PROG  yLOG_value   ("f_lines"   , my.f_lines);
   yURG_msg ('>', "%d lines read, %d groups, %d procs, %%d seqs", my.f_lines, x_lists, x_lines, x_seqs);
   --rce;  if (my.f_lines <= 0) {
      yURG_msg (' ', "");
      yURG_err ('>', "NO LINES read");
      DEBUG_PROG  yLOG_note    ("no input lines found");
      strlcpy (my.f_note, "NO_INPUT" , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_lists <= 0) {
      yURG_msg (' ', "");
      yURG_err ('>', "NO GROUPS found");
      DEBUG_PROG  yLOG_note    ("no groups found");
      strlcpy (my.f_note, "NO_GROUPS"  , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_lines <= 0) {
      yURG_msg (' ', "");
      yURG_err ('>', "NO PROCS found");
      DEBUG_PROG  yLOG_note    ("no procs found");
      yURG_msg (' ', "");
      strlcpy (my.f_note, "NO_PROCS"  , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('>', "all lines read, SUCCESS, reviewed %d, accepted %d", my.f_lines, x_lines);
   yURG_msg (' ', "");
   strlcpy (my.f_note, "success"  , LEN_TERSE);
   /*---(save back)----------------------*/
   if (r_user != NULL)  strlcpy (r_user, my.f_user, LEN_LABEL);
   if (r_desc != NULL)  strlcpy (r_desc, my.f_desc, LEN_DESC);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



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
      rc = BASE_file_verify (my.n_conf);
      if      (rc >  0)  x_exist = 'y';
      else if (rc <= 0)  x_exist = '-';
      snprintf (unit_answer, LEN_RECD, "BASE conf        : %c  %2d[%s]",
            x_exist, strlen (my.n_conf), my.n_conf);
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
