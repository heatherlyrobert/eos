/*===============================[[ beg-code ]]===============================*/
#include    "eos.h"

char
wait_sec           (char *a_func, char a_rc, int a_sec)
{
   int         i           =    0;
   printf ("%-12.12s  : returned %d\n", a_func, a_rc);
   for (i = 0; i < a_sec; ++i) {
      printf ("sleep %d\n", i);
      sleep (1);
   }
   return 0;
}

int              /* [------] main entry point --------------------------------*/
main               (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(initialize)---------------------*/
   my.msec = base_msec ();
   DEBUG_LOOP   yLOG_value   ("my.msec"    , my.msec);
   printf ("%s\n", P_ONELINE);
   if (rc >= 0)  rc = PROG_verbose    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_runas      (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_boot       ();
   if (rc >= 0)  rc = yURG_logger     (a_argc, a_argv);
   if (rc >= 0)  rc = yURG_urgs       (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_init       (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_args       (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_begin      ();
   /*---(defense)------------------------*/
   DEBUG_PROG  yLOG_value   ("startup"   , rc);
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      wait_sec ("its over",   rc,  20);
      PROG_end ();
      return rce;
   }
   /*---(read config)--------------------*/
   rc = base_config ();
   DEBUG_PROG  yLOG_value   ("config"    , rc);
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      wait_sec ("its over",   rc,  20);
      PROG_end ();
      return rce;
   }
   /*---(verify only)--------------------*/
   if (my.run_mode == EOS_RUN_VERIFY) {
      rc = PROG_end ();
      return 0;
   }
   /*---(verify only)--------------------*/
   switch (my.run_mode) {
   case EOS_RUN_VERIFY  :
      rptg_pert ();
      break;
   case EOS_RUN_NORMAL  :
   case EOS_RUN_DAEMON  :
      rc = base_execute ();
      rptg_pert  ();
      rptg_gantt ();
      rptg_dump  ();
      break;
   /*> case EOS_RUN_DAEMON  :                                                         <*/
      /*> rc = base_execute ();                                                       <* 
       *> rc = PROG_end ();                                                           <* 
       *> rc = base_kharon  ();                                                       <* 
       *> break;                                                                      <*/
   }
   if (my.run_as == IAM_EOS && my.pid == 1)  base_kharon ();
   /*---(wrapup)-------------------------*/
   rc = PROG_end ();
   /*---(complete)-----------------------*/
   return 0;
}



/*============================[[ end-of-code ]]===============================*/
