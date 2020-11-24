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
   if (rc >= 0)  rc = PROG_preinit ();
   if (rc >= 0)  rc = yURG_logger  (a_argc, a_argv);
   if (rc >= 0)  rc = yURG_urgs    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_init    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_args    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_begin   ();
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
   /*---(modes)--------------------------*/
   switch (my.run_mode) {
   case MODE_VERIFY  :
      /*---(nothing more to do */
      break;
   case MODE_NORMAL  :
      rc = base_execute ();
      break;
   case MODE_DAEMON  :
      rc = base_execute ();
      rc = PROG_end ();
      rc = base_kharon  ();
      break;
   }
   /*---(wrapup)-------------------------*/
   rc = PROG_end ();
   /*---(complete)-----------------------*/
   return 0;
}



/*============================[[ end-of-code ]]===============================*/
