/*===============================[[ beg-code ]]===============================*/
#include    "eos.h"

char
wait_sec           (char *a_func, char a_rc, int a_sec)
{
   int         i           =    0;
   EOS_VERBOSE printf ("%-12.12s  : returned %d\n", a_func, a_rc);
   for (i = 0; i < a_sec; ++i) {
      EOS_VERBOSE printf ("sleep %d\n", i);
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
   /*---(pre-startup)--------------------*/
   my.msec = BASE_msec ();
   DEBUG_LOOP   yLOG_value   ("my.msec"    , my.msec);
   /*---(pre_startup)--------------------*/
   if (rc >= 0)  rc = PROG_prestart  (a_argc, a_argv, '-');
   DEBUG_PROG  yLOG_value   ("prestart"  , rc);
   if (rc >= 0)  rc = PROG_debugging (a_argc, a_argv, '-');
   DEBUG_PROG  yLOG_value   ("debugging" , rc);
   if (rc >= 0)  rc = PROG_startup   (a_argc, a_argv, '-');
   DEBUG_PROG  yLOG_value   ("startup"   , rc);
   /*---(defense)------------------------*/
   DEBUG_PROG  yLOG_value   ("startup"   , rc);
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      wait_sec ("its over",   rc,  20);
      PROG_end ();
      return rce;
   }
   /*---(read config)--------------------*/
   /*> rc = base_config ();                                                           <* 
    *> DEBUG_PROG  yLOG_value   ("config"    , rc);                                   <* 
    *> --rce;  if (rc <  0) {                                                         <* 
    *>    DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);                               <* 
    *>    wait_sec ("its over",   rc,  20);                                           <* 
    *>    PROG_end ();                                                                <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(verify only)--------------------*/
   IF_VERIFY {
      yURG_msg ('>', "called with VERIFY only, stopping run now");
      rc = PROG_end ();
      return 0;
   }
   /*---(verify only)--------------------*/
   switch (my.run_mode) {
   case CASE_VERIFY :
      rptg_pert ();
      break;
   case EOS_RPTG_VERBS  :
      proc_verblist ();
      break;
   case EOS_RPTG_CONTROL :
      yEXEC_controls ();
      break;
   case CASE_NORMAL  :
   case CASE_DAEMON :
      rc = BASE_execute ();
      /*> rptg_pert  ();                                                              <*/
      /*> rptg_gantt ();                                                              <*/
      /*> rptg_dump  ();                                                              <*/
      break;
   }
   /*---(wrapup)-------------------------*/
   if (my.run_as == IAM_EOS && my.pid == 1)  BASE_kharon ();
   rc = PROG_end ();
   /*---(complete)-----------------------*/
   return 0;
}



/*============================[[ end-of-code ]]===============================*/
