/*============================[[    beg-code    ]]============================*/
#include    "eos.h"




char
rptg_performance        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   tGROUP     *x_group     = NULL;
   tPROC      *x_proc      = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP  yLOG_enter   (__FUNCTION__);
   /*---(open report file)---------------*/
   DEBUG_OUTP  yLOG_info    ("name_perf" , my.name_perf);
   f = fopen (my.name_perf, "wt");
   DEBUG_OUTP  yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check all groups)---------------*/
   for (x_group = yDLST_list_seek (YDLST_HEAD); x_group != NULL; x_group = yDLST_list_seek (YDLST_NEXT)) {
      for (x_proc = yDLST_line_seek (YDLST_HEAD); x_proc != NULL; x_proc = yDLST_line_seek (YDLST_NEXT)) {
         fprintf (f, "%-20.20s  %-20.20s  %-40.40s  %c  %6d  %c  %3d ",
               x_group->name, x_proc->name, x_proc->desc,
               x_proc->type, x_proc->rpid, x_proc->yexec, x_proc->rc);
      }
   }
   /*---(open report file)---------------*/
   rc = fclose (f);
   DEBUG_OUTP  yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP  yLOG_exit    (__FUNCTION__);
   return 0;
}



