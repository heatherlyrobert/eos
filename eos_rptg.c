/*============================[[    beg-code    ]]============================*/
#include    "eos.h"


#define  MAX_Y      70
#define  MAX_X     300

char     s_pert [MAX_Y][MAX_X];
int      s_xcol   = 0;
int      s_xrow   = 0;


char
rptg__pert_clear        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   int         r, c;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = yDLST_list_by_cursor ('[', NULL, &x_group);
   DEBUG_LOOP   yLOG_complex ("head"      , "%3d, %p", rc, x_group);
   /*---(walk through groups)------------*/
   while (rc >= 0) {
      if (x_group != NULL) DEBUG_LOOP   yLOG_complex ("GROUP"     , "%3d, %p, %-15.15s, %2dr", rc, x_group, x_group->name, x_group->askd);
      if (x_group != NULL) x_group->row = x_group->col = 0;
      rc = yDLST_list_by_cursor ('>', NULL, &x_group);
   }
   /*---(graph)--------------------------*/
   for (r = 0; r < MAX_Y; ++r) {
      for (c = 0; c < MAX_X; ++c)  s_pert [r][c] = ' ';
      s_pert [r][MAX_X - 1] = '\0';
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP  yLOG_exit    (__FUNCTION__);
   return 0;
}
char
rptg__pert_col          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   tGROUP     *x_pred      = NULL;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = yDLST_list_by_cursor ('[', NULL, &x_group);
   DEBUG_LOOP   yLOG_complex ("head"      , "%3d, %p", rc, x_group);
   s_xcol = 0;
   /*---(walk through groups)------------*/
   while (rc >= 0) {
      if (x_group != NULL) {
         if (x_group != NULL)  DEBUG_LOOP   yLOG_complex ("GROUP"     , "%3d, %p, %-15.15s, %2dr", rc, x_group, x_group->name, x_group->askd);
         rc = yDLST_seq_by_cursor ('<', '[', NULL, NULL, &x_pred);
         while (rc >= 0) {
            if (x_group->col < 1)  x_group->col = 1;
            if (x_group->col > s_xcol)  s_xcol = x_group->col;
            if (x_pred != NULL) {
               if (x_pred->col + 1 > x_group->col)  x_group->col = x_pred->col + 1;
            }
            rc = yDLST_seq_by_cursor ('<', '>', NULL, NULL, &x_pred);
         }
      }
      rc = yDLST_list_by_cursor ('>', NULL, &x_group);
   }
   ++s_xcol;
   /*---(complete)-----------------------*/
   DEBUG_OUTP  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
rptg__pert_rower        (void *a_list, char a_row)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   void       *x_list      = NULL;
   tGROUP     *x_succ      = NULL;
   char        x_old       =    0;
   char        c           =    0;
   /*---(quick out)----------------------*/
   if (a_list == NULL)  return a_row;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(set group as current)-----------*/
   rc = yDLST_list_restore  (a_list);
   DEBUG_LOOP  yLOG_complex ("args"      , "%p, %d", a_list, a_row);
   /*---(head successor)-----------------*/
   rc = yDLST_seq_by_index ('>', c++, NULL, &x_list, &x_succ);
   DEBUG_LOOP  yLOG_complex ("by_index"  , "%3d, %p, %p", rc, x_list, x_succ);
   if (x_list == NULL) {
      DEBUG_OUTP  yLOG_exit    (__FUNCTION__);
      return a_row;
   }
   if (x_succ != NULL && x_succ->row <= 0)  x_succ->row = a_row;
   /*---(walk successors)----------------*/
   while (rc >= 0) {
      if (x_succ != NULL) {
         /*---(update)----------------------*/
         x_old = x_succ->row;
         if    (x_old <= 0)   x_succ->row = a_row;
         DEBUG_LOOP  yLOG_complex ("succ"      , "%-15.15s, %3d, %3d", x_succ->name, x_old, x_succ->row);
         /*---(recurse)---------------------*/
         a_row = rptg__pert_rower (x_list, a_row);
         DEBUG_LOOP  yLOG_value   ("a_row"     , a_row);
      }
      /*---(update position)-------------*/
      if (rc >= 0 && x_succ != NULL && x_succ->row == a_row) ++a_row;
      /*---(next successor)--------------*/
      rc = yDLST_list_restore  (a_list);
      DEBUG_LOOP  yLOG_value   ("by_ptr"    , rc);
      rc = yDLST_seq_by_index  ('>', c++, NULL, &x_list, &x_succ);
      DEBUG_LOOP  yLOG_complex ("by_index"  , "%3d, %p, %p", rc, x_list, x_succ);
      /*---(done)------------------------*/
   }
   if (a_row > s_xrow)  s_xrow = a_row;
   /*---(complete)-----------------------*/
   DEBUG_OUTP  yLOG_exit    (__FUNCTION__);
   return a_row;
}

char
rptg__pert_row          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   void       *x_list      = NULL;
   tGROUP     *x_group     = NULL;
   int         x_ngroup    =    0;
   int         i           =    0;
   tGROUP     *x_pred      = NULL;
   int         x_npred     =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   rc = yDLST_list_by_index  (x_ngroup, &x_list, &x_group);
   s_xrow = 0;
   while (rc >= 0) {
      /*---(prepare)---------------------*/
      DEBUG_LOOP  yLOG_complex ("by_index"  , "%2d, %3d, %p, %p, %s", x_ngroup, rc, x_list, x_group, (x_group == NULL) ? "" : x_group->name);
      rc = x_npred = i = 0;
      /*---(count real preds)------------*/
      while (rc >= 0) {
         rc = yDLST_seq_by_index ('<', i++, NULL, NULL, &x_pred);
         if (rc >= 0 && x_pred != NULL)  ++x_npred;
      }
      DEBUG_LOOP  yLOG_value   ("x_npred"   , x_npred);
      /*---(process)---------------------*/
      if (x_npred == 0) {
         x_group->row = 1;
         rc = rptg__pert_rower (x_list, 1);
      }
      /*---(next)------------------------*/
      rc = yDLST_list_by_index (++x_ngroup, &x_list, &x_group);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP  yLOG_exit    (__FUNCTION__);
   return  rc;
}

char
rptg__pert_merge        (int y, int x, char *a_str)
{
   int         x_len       =    0;
   int         i           =    0;
   x_len = strlen (a_str);
   for (i = 0; i < x_len; ++i) {
      s_pert [y][x + i] = a_str [i];
   }
}

char
rptg__pert_verter       (int a_top, int a_bot, int x, uchar a_ch)
{
   int         i           =    0;
   for (i = a_top; i <= a_bot; ++i) {
      if      (i == a_top)   s_pert [i][x] = '³';
      else if (i == a_bot)   s_pert [i][x] = '³';
      else                   s_pert [i][x] = a_ch;
   }
   return 0;
}

char rptg__pert_vert   (int a_top, int a_bot, int x) { return rptg__pert_verter (a_top, a_bot, x, '|'); }
char rptg__pert_bound  (int a_top, int a_bot, int x) { return rptg__pert_verter (a_top, a_bot, x, '·'); }

char
rptg__pert_end          (int y, int x, uchar a_name)
{
   char       *x_edge      = "´===´";
   char       *x_mid       = "¨   ¨";
   char        t           [LEN_TERSE] = "";
   char        o           = 14;
   if (a_name == (uchar) 'ÿ') o = 0;
   rptg__pert_merge (y - 2, x + o, x_edge);
   rptg__pert_merge (y - 1, x + o, x_mid);
   rptg__pert_merge (y - 0, x + o, x_edge);
   sprintf (t, "%c"     , a_name);
   rptg__pert_merge (y - 1, x + o + 2                  , t);
   return 0;
}

char
rptg__pert_box          (int y, int x, char *a_name, char a_procs, int a_beg, int a_end, int a_dur, char a_note, char a_pred, char a_succ)
{
   char       *x_edge      = "´=================´";
   char       *x_mid       = "¨                 ¨";
   char        t           [LEN_TERSE] = "";
   rptg__pert_merge (y + 0, x, x_edge);
   rptg__pert_merge (y + 1, x, x_mid);
   rptg__pert_merge (y + 2, x, x_mid);
   rptg__pert_merge (y + 3, x, x_edge);
   rptg__pert_merge (y + 0, x + 9 - strlen (a_name) / 2, a_name);
   sprintf (t, "[%02d]", a_procs);
   rptg__pert_merge (y + 3, x + 7                      , t);
   sprintf (t, "b%-5d" , a_beg);
   rptg__pert_merge (y + 1, x + 2                      , t);
   sprintf (t, "%5de"   , a_end);
   rptg__pert_merge (y + 1, x + 16 - 5                 , t);
   sprintf (t, "%dd"    , a_dur);
   rptg__pert_merge (y + 2, x + 9 - strlen (t) / 2     , t);
   sprintf (t, "%1d", a_pred);
   rptg__pert_merge (y + 2, x + 2                      , t);
   sprintf (t, "%1d", a_succ);
   rptg__pert_merge (y + 2, x + 16                     , t);
   sprintf (t, "%c"     , a_note);
   rptg__pert_merge (y + 1, x + 9                      , t);
   return 0;
}

char
rptg__pert_line         (int y1, int x1, char n, int y2, int x2, char m)
{
   int         i           =    0;
   /*---(horizontal)---------------------*/
   if (y1 == y2) {
      for (i = x1; i <= x2; ++i)  s_pert [y1 + n][i] = '-';
   }
   /*---(downward)-----------------------*/
   else if (y1 < y2) {
      switch (n) {
      case 3 : 
         rptg__pert_merge (y1 + n, x1, "-³");
         rptg__pert_vert  (y1 + n, y2 + m, x1 + 1);
         rptg__pert_line  (y2 + m, x1 + 2, m, y2 + m, x2, m);
         break;
      case 2 :
         rptg__pert_merge (y1 + n, x1, "---³");
         rptg__pert_vert  (y1 + n, y2 + m, x1 + 3);
         rptg__pert_line  (y2 + m, x1 + 4, m, y2 + m, x2, m);
         break;
      case 1 :
         rptg__pert_merge (y1 + n, x1, "-----³");
         rptg__pert_vert  (y1 + n, y2 + m, x1 + 5);
         rptg__pert_line  (y2 + m, x1 + 6, m, y2 + m, x2, m);
         break;
      case 0 :
         rptg__pert_merge (y1 + n, x1, "-------³");
         rptg__pert_vert  (y1 + n, y2 + m, x1 + 7);
         rptg__pert_line  (y2 + m, x1 + 8, m, y2 + m, x2, m);
         break;
      }
   }
   /*---(upward)-------------------------*/
   else {
      switch (m) {
      case 0 :
         rptg__pert_line  (y1 + n, x1, n, y1 + n, x2 - 9, n);
         rptg__pert_vert  (y2 + m, y1 + n, x2 - 8);
         rptg__pert_merge (y2 + m, x2 - 8, "³-------");
         break;
      case 1 :
         rptg__pert_line  (y1 + n, x1, n, y1 + n, x2 - 7, n);
         rptg__pert_vert  (y2 + m, y1 + n, x2 - 6);
         rptg__pert_merge (y2 + m, x2 - 6, "³-----");
         break;
      case 2 :
         rptg__pert_line  (y1 + n, x1, n, y1 + n, x2 - 5, n);
         rptg__pert_vert  (y2 + m, y1 + n, x2 - 4);
         rptg__pert_merge (y2 + m, x2 - 4, "³---");
         break;
      case 3 : 
         rptg__pert_line  (y1 + n, x1, n, y1 + n, x2 - 3, n);
         rptg__pert_vert  (y2 + m, y1 + n, x2 - 2);
         rptg__pert_merge (y2 + m, x2 - 2, "³-");
         break;
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
rptg__pert_write        (void)
{
   char        rc          =    0;
   FILE       *f           = NULL;
   tGROUP     *x_group     = NULL;
   int         i           =    0;
   f = fopen ("/tmp/eos_pert.txt", "wt");
   fprintf (f, "#!/usr/local/bin/eos --pert\n");
   fprintf (f, "#  %s\n", P_ONELINE);
   fprintf (f, "#  report of group/proc configuration in PERT format\n");
   fprintf (f, "\n");
   /*> rc = yDLST_list_by_index  (i, NULL, &x_group);                                 <* 
    *> while (rc >= 0) {                                                              <* 
    *>    fprintf (f, "%p\n", x_group);                                               <* 
    *>    s_pert [i + 10][0] = '*';                                                   <* 
    *>    rc = yDLST_list_by_index (++i, NULL, &x_group);                             <* 
    *> }                                                                              <*/
   for (i = 0; i < MAX_Y; ++i)   fprintf (f, "%s\n", s_pert [i]);
   fclose (f);
   return 0;
}

char
rptg__pert_connect      (tGROUP *a_group)
{
   char        rc          =    0;
   void       *x_beg       = NULL;
   void       *x_end       = NULL;
   int         x_nsucc     =    0;
   tGROUP     *x_succ      = NULL;
   int         x_npred     =    0;
   tGROUP     *x_pred      = NULL;
   int         y1, x1, y2, x2;
   /*---(check start)--------------------*/
   if (a_group == NULL)  return 0;
   x_beg = yDLST_list_current ();
   /*---(set start position)-------------*/
   y1 = a_group->row *  7;
   x1 = a_group->col * 28 + 19;
   /*---(get first successor)------------*/
   rc = yDLST_seq_by_index  ('>', x_nsucc = 0, NULL, &x_end, &x_succ);
   while (rc >= 0) {
      /*---(handle successor)------------*/
      if (x_succ != NULL) {
         /*---(set end position)---------*/
         y2 = x_succ->row *  7;
         x2 = x_succ->col * 28;
         /*---(go to successor)----------*/
         yDLST_list_restore (x_end);
         /*---(go to first predecessor)--*/
         rc = yDLST_seq_by_index ('<', x_npred = 0, NULL, NULL, &x_pred);
         while (rc >= 0) {
            /*---(draw connector)-------*/
            if (x_pred == a_group)  rptg__pert_line (y1, x1, x_nsucc, y2, x2, x_npred);
            /*---(next predecessor)-----*/
            rc = yDLST_seq_by_index  ('<', ++x_npred, NULL, NULL, &x_pred);
            /*---(done)-----------------*/
         }
         /*---(done)---------------------*/
      }
      /*---(handle omega end)------------*/
      else {
         y2 = 7;
         x2 = s_xcol * 28;
         rptg__pert_line (y1, x1, 0, y2, x2, 0);
      }
      /*---(next)------------------------*/
      yDLST_list_restore (x_beg);
      rc = yDLST_seq_by_index  ('>', ++x_nsucc, NULL, &x_end, &x_succ);
      /*---(done)------------------------*/
   }
   /*---(handle alpha end)---------------*/
   if (a_group->col == 1) {
      y2 = y1;
      x2 = x1 - 20;
      y1 = 7;
      x1 = 19;
      rptg__pert_line (y1, x1, 0, y2, x2, 0);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
rptg__pert_groups       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   int         x_ngroup    =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   rptg__pert_end     (7               , 0                , 'è');
   rc = yDLST_list_by_index  (x_ngroup, NULL, &x_group);
   while (rc >= 0) {
      /*---(normal)----------------------*/
      if (x_group != NULL) {
         rptg__pert_box     (x_group->row * 7, x_group->col * 28, x_group->name, x_group->askd, x_group->beg, x_group->end, x_group->dur, x_group->note, yDLST_seq_count ('<'), yDLST_seq_count ('>'));
         rptg__pert_connect (x_group);
      }
      /*---(next)------------------------*/
      rc = yDLST_list_by_index (++x_ngroup, NULL, &x_group);
      /*---(done)------------------------*/
   }
   rptg__pert_end     (7               , s_xcol * 28   , 'ÿ');
   /*---(bounds)-------------------------*/
   rptg__pert_bound   (3, s_xrow * 7 - 2, 10);
   rptg__pert_bound   (3, s_xrow * 7 - 2, s_xcol * 28 + 8);
   /*---(complete)-----------------------*/
   DEBUG_OUTP  yLOG_exit    (__FUNCTION__);
   return  rc;
}

char
rptg_pert               (void)
{
   rptg__pert_clear  ();
   rptg__pert_col    ();
   rptg__pert_row    ();
   rptg__pert_groups ();
   rptg__pert_write  ();
   return 0;
}



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
   for (yDLST_list_by_cursor ('[', NULL, &x_group); x_group != NULL; yDLST_list_by_cursor ('>', NULL, &x_group)) {
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



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*
rptg__unit              (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        t           [LEN_RECD]  = "[ALPHA]";
   tGROUP     *x_group     = NULL;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "RPTG             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   yDLST_backup ();
   if      (strcmp (a_question, "pos"     )        == 0) {
      yDLST_list_by_index (a_num, NULL, &x_group);
      if (x_group != NULL) {
         sprintf (t, "%2d[%-.15s]", strlen (x_group->name), x_group->name);
         snprintf (unit_answer, LEN_RECD, "RPTG pos    (%2d) : %-19.19s    %3dc  %3dr    %3dx  %3dy", a_num, t, x_group->col, x_group->row, 0, 0);
      } else {
         snprintf (unit_answer, LEN_RECD, "RPTG pos    (%2d) :  -[]                     ·c    ·r      ·x    ·y", a_num);
      }
   }
   yDLST_restore ();
   /*---(complete)-----------------------*/
   return unit_answer;
}






