/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



/*====================------------------------------------====================*/
/*===----                        main driver                           ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
eos_yjobs               (cchar a_req, cchar *a_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG    yLOG_enter   (__FUNCTION__);
   DEBUG_PROG    yLOG_char    ("a_req"     , a_req);
   DEBUG_PROG    yLOG_info    ("a_data"    , a_data);
   /*---(handle)-------------------------*/
   switch (a_req) {
   case YJOBS_STATS    :
      DEBUG_PROG    yLOG_note    ("called for stats");
      break;
   case YJOBS_PULL     :
      DEBUG_PROG    yLOG_note    ("called for pull");
      rc = BASE_pull (a_data);
      break;
   case YJOBS_LOCALRPT : 
      DEBUG_PROG    yLOG_note    ("called for localrpt");
      break;
   case YJOBS_REPORT   :
      DEBUG_PROG    yLOG_note    ("called for report");
      /*> rc = RPTG_by_min ();                                                        <*/
      break;
   case YJOBS_PURGE    :
      DEBUG_PROG    yLOG_note    ("called for purge");
      rc = BASE_purge ();
      /*> rc = yDLST_purge ();                                                        <*/
      break;
   }
   DEBUG_PROG    yLOG_value   ("rc"        , rc);
   /*---(trouble)------------------------*/
   if (rc < 0) {
      DEBUG_PROG    yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG    yLOG_exit    (__FUNCTION__);
   return rc;
}
