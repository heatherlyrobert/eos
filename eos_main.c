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
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(pre_startup)--------------------*/
   if (rc >= 0)  rc = PROG_prestart  (a_argc, a_argv, '-');
   DEBUG_PROG  yLOG_value   ("prestart"  , rc);
   if (rc >= 0)  rc = PROG_debugging (a_argc, a_argv, '-');
   DEBUG_PROG  yLOG_value   ("debugging" , rc);
   if (rc >= 0)  rc = PROG_startup   (a_argc, a_argv, '-');
   DEBUG_PROG  yLOG_value   ("startup"   , rc);
   /*---(defense)------------------------*/
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      wait_sec ("its over",   rc,  20);
      PROG_end ();
      return rce;
   }
   /*---(process action)-----------------*/
   /*> rc = yJOBS_driver (my.run_as, my.run_mode, P_ONELINE, my.run_file, my.m_who, my.m_uid, FILE_assimilate, BASE_execute);   <*/
   rc = yJOBS_driver (P_ONELINE, eos_yjobs);
   /*> switch (my.run_mode) {                                                                   <* 
    *> case CASE_VERIFY :                                                                       <* 
    *>    rptg_pert ();                                                                         <* 
    *>    break;                                                                                <* 
    *> case EOS_RPTG_VERBS  :                                                                   <* 
    *>    proc_verblist ();                                                                     <* 
    *>    break;                                                                                <* 
    *> case EOS_RPTG_CONTROL :                                                                  <* 
    *>    yEXEC_controls ();                                                                    <* 
    *>    break;                                                                                <* 
    *> }                                                                                        <*/
   /*---(wrapup)-------------------------*/
   if (my.run_as == IAM_EOS && my.pid == 1)  BASE_kharon ();
   IF_NOEND  ;
   else      rc = PROG_end ();
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================[[ end-of-code ]]===============================*/
