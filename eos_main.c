/*===============================[[ beg-code ]]===============================*/
#include    "eos.h"


int              /* [------] main entry point --------------------------------*/
main               (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;        /* return code (char)                  */
   char        xwhich      [100];
   int         xitem       = -1;
   tPROC      *xproc       = NULL;
   int         fd;
   int         boot_recd   = 0;
   /*---(start)--------------------------*/
   if (rc == 0)  rc = PROG_init    ();
   if (rc == 0)  rc = PROG_logger  (a_argc, a_argv);
   if (rc == 0)  rc = PROG_urgs    (a_argc, a_argv);
   if (rc == 0)  rc = PROG_whoami  ();
   if (rc == 0)  rc = PROG_args    (a_argc, a_argv);
   if (rc == 0)  rc = PROG_begin   ();
   /*---(defense)------------------------*/
   if (rc != 0) {
      yLOG_value   ("rc"        , rc);
      yLOG_note    ("one of the PROG functions failed");
      PROG_end    ();
      exit (-1);
   }


   yLOG_note    ("test ending");
   PROG_end    ();
   return 0;


   /*> if (rc == 0)  rc = CONF_daemon  ();                                             <*/
   if (rc == 0)  rc = CONF_signal  ();

   /*---(process eos.conf)---------------*/
   rc = CONF_open     ();
   /*---(defense)------------------------*/
   if (rc != 0) {
      PROG_end    ();
      exit (-1);
   }
   /*---(process eos.conf)---------------*/
   rc = CONF_read     ();
   while (rc == 0) {
      rc = CONF_parse    ();
      rc = CONF_read     ();
   }
   /*---(report out)---------------------*/
   yLOG_break   ();
   CONF_list     ();
   CONF_report   ('b');
   CONF_close    ();
   /*> if (mode_test == 'y') {                                                        <* 
    *>    printf ("DONE, test mode stops here\n");                                    <* 
    *>    exit (0);                                                                   <* 
    *> }                                                                              <*/
   /*---(wait for jobs to complete)------*/
   yDLST_focus_on  (0);
   while (1) {
      yLOG_break   ();
      EXEC_check   ();
      EXEC_launch  ();
      usleep       (100000);   /*  100,000 = 0.1 second  */
      /*---(write ASAP)------------------*/
      if (boot_recd == 0 &&
            access (WTMP, W_OK) == 0 &&
            access (UTMP, W_OK) != 0) {
         ySEC_startup ();
         boot_recd = 1;
      }
      if (requested == complete) break;
   }
   /*---(report out)---------------------*/
   if (requested == complete) {
      yLOG_note  ("ALL JOBS COMPLETE");
   }
   CONF_report   ('a');
   /*---(pass the torch)-----------------*/
   char    backup2[LEN_CMD];
   strncpy(backup2, "/sbin/kharon --acheron --leisurely", LEN_CMD);
   /*> str_parse  (backup2);                                                          <*/
   strlargs   (backup2, LEN_CMD, 20, &my.argc, my.argv);
   PROG_end   ();
   if (getpid() == 1)   rc = execvp (*my.argv, my.argv);
   /*> execvp("/bin/bash", NULL);                                                     <*/
   /*---(complete)-----------------------*/
   return 0;
}



/*============================[[ end-of-code ]]===============================*/
