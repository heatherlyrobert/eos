/*===============================[[ beg-code ]]===============================*/
#include    "eos.h"


int              /* [------] main entry point --------------------------------*/
main               (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        xwhich      [100];
   int         xitem       = -1;
   tPROC      *xproc       = NULL;
   int         fd;
   int         boot_recd   = 0;
   char        backup2     [LEN_CMD];
   tTSPEC      x_dur;
   long        x_loop      = 0;
   int         i           = 0;
   /*---(initialize)---------------------*/
   if (rc >= 0)  rc = yURG_logger  (a_argc, a_argv);
   if (rc >= 0)  rc = yURG_urgs    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_init    (a_argv [0][0]);
   if (rc >= 0)  rc = PROG_args    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_begin   ();
   /*---(defense)------------------------*/
   DEBUG_PROG  yLOG_value   ("startup"   , rc);
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      PROG_end ();
      return rce;
   }
   /*---(read config)--------------------*/
   /*> if (rc == 0)  rc = CONF_daemon  ();                                             <*/
   /*> if (rc == 0)  rc = CONF_signal  ();                                            <*/
   /*> if (rc == 0)  rc = CONF_open    ();                                            <* 
    *> if (rc == 0)  rc = CONF_read    ();                                            <* 
    *> while (rc == 0) {                                                              <* 
    *>    rc = CONF_parse   ();                                                       <* 
    *>    if (rc == 0)  rc = CONF_read    ();                                         <* 
    *> }                                                                              <* 
    *> DEBUG_TOPS   yLOG_break   ();                                                  <* 
    *> if (rc > 0)   rc = 0;                                                          <* 
    *> if (rc == 0)  rc = CONF_list    ();                                            <* 
    *> if (rc == 0)  rc = CONF_report  ('b');                                         <* 
    *> if (rc == 0)  rc = CONF_close   ();                                            <* 
    *> if (rc == 0)  rc = CONF_tableview ();                                          <*/
   /*---(defense)------------------------*/
   if (rc != 0) {
      DEBUG_TOPS   yLOG_value   ("rc"        , rc);
      DEBUG_TOPS   yLOG_note    ("one of the CONF functions failed");
      PROG_end    ();
      exit (-2);
   }
   /*---(prepare)------------------------*/
   x_dur.tv_sec    = 0;
   /*> x_dur.tv_nsec   = 100000; // 0.001 second                                      <*/
   /*> x_dur.tv_nsec   = 100000000; // 0.1 second                                     <*/
   x_dur.tv_nsec   = my.loop_msec * 1000000;
   /*> yDLST_focus_on  (0);                                                           <*/
   /*---(enter main loop)----------------*/
   DEBUG_TOPS   yLOG_break   ();
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   while (my.done_done != 'y') {
      /*---(checking)--------------------*/
      DEBUG_LOOP   yLOG_break   ();
      exec_check    ();
      exec_finish   ();
      exec_start    ();
      exec_dispatch ();
      /*---(sleeping)--------------------*/
      nanosleep    (&x_dur, NULL);
      /*---(write sec ASAP)--------------*/
      if (boot_recd == 0 &&
            access (WTMP, W_OK) == 0 &&
            access (UTMP, W_OK) != 0) {
         ySEC_startup ();
         boot_recd = 1;
      }

      /*---(done)------------------------*/
      ++x_loop;
      DEBUG_LOOP   yLOG_value   ("loop#"      , x_loop);
      if (x_loop > my.loop_max)  break;
   }
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   /*---(report out)---------------------*/
   if (my.done_done == 'y') {
      DEBUG_TOPS   yLOG_note    ("ALL JOBS COMPLETE");
   } else {
      DEBUG_TOPS   yLOG_note    ("STOPPED WITHOUT ALL JOBS COMPLETE");
   }
   /*> CONF_report   ('a');                                                           <*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   /*---(pass the torch)-----------------*/
   strlcpy    (backup2, "/sbin/kharon --acheron --leisurely", LEN_CMD);
   strlargs   (backup2, LEN_CMD, 20, &my.argc, my.argv);
   printf ("arg count %d\n", my.argc);
   for (i = 0; i < my.argc; ++i) {
      printf ("arg %2d <<%s>>\n", i, my.argv [i]);
   }
   PROG_end   ();
   if (my.pid == 1)   rc = execvp (*my.argv, my.argv);
   /*---(complete)-----------------------*/
   return 0;
}



/*============================[[ end-of-code ]]===============================*/
