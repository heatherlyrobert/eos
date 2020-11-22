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


   return 0;


   /*---(header)-------------------------*/
   DEBUG_OUTP  yLOG_enter   (__FUNCTION__);
   /*---(open report file)---------------*/
   DEBUG_OUTP  yLOG_info    ("n_perf" , my.n_perf);
   f = fopen (my.n_perf, "wt");
   DEBUG_OUTP  yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(header)-------------------------*/
   if (my.run_as == IAM_EOS) {
      fprintf (f, "##/sbin/eos (system initialization)\n");
      fprintf (f, "##   eos-rhododactylos (rosy-fingered) -- simple, reliable, and transparent system initialization\n");
   } else {
      fprintf (f, "##/sbin/astraios (system shutdown)\n");
      fprintf (f, "##   astraios-aiolos (sparkling) -- simple, reliable, and transparent system shutdown\n");
   }
   fprintf (f, "##   performance report with major tracking information\n");
   fprintf (f, "##\n");
   fprintf (f, "##---group----------  ---proc-------------  ---description--------------------------  t  -rpid-  y  ret  ---beg----  ---end----  --dur-  btic  etic  dtic \n");
   /*---(check all groups)---------------*/
   /*> for (x_group = yDLST_list_seek (YDLST_HEAD); x_group != NULL; x_group = yDLST_list_seek (YDLST_NEXT)) {   <*/
   for (yDLST_list_by_cursor ('[', NULL, &x_group); x_group != NULL; yDLST_list_by_cursor ('>', NULL, &x_group)) {
      /*> for (x_proc = yDLST_line_seek (YDLST_HEAD); x_proc != NULL; x_proc = yDLST_line_seek (YDLST_NEXT)) {   <*/
      for (yDLST_line_by_cursor ('-', '[', NULL, &x_proc); x_proc != NULL; yDLST_line_by_cursor ('-', '>', NULL, &x_proc)) {
         if (c % 5 == 0)  fprintf (f, "\n");
         fprintf (f, "%-20.20s  %-20.20s  %-40.40s  %c  %6d  %c  %3d  %-10d  %-10d  %6d  %4d  %4d  %4d \n",
               x_group->name, x_proc->name, x_proc->desc,
               x_proc->type, x_proc->rpid, x_proc->yexec, x_proc->rc,
               x_proc->beg, x_proc->end, x_proc->dur);
         ++c;
      }
   }
   /*---(footer)-------------------------*/
   fprintf (f, "\n");
   fprintf (f, "##---group----------  ---proc-------------  ---description--------------------------  t  -rpid-  y  ret  ---beg----  ---end----  --dur-  btic  etic  dtic \n");
   fprintf (f, "##\n");
   fprintf (f, "## complete with %d lines\n", c);
   /*---(close report file)--------------*/
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



