/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



/*====================------------------------------------====================*/
/*===----                        pert charting                         ----===*/
/*====================------------------------------------====================*/
static void      o___PERT____________________o (void) {;}


#define  MAX_Y      70
#define  MAX_X     300

#define  OFF_X      30
#define  OFF_Y       7

#define  END_X       2
#define  END_Y       2

static char     s_pert [MAX_Y][MAX_X];
static int      s_xcol   = 0;
static int      s_xrow   = 0;

static int      s_nalpha = 0;
static int      s_calpha = 0;
static int      s_nomega = 0;
static int      s_comega = 0;

static int      s_maxy   = 0;
static int      s_maxx   = 0;

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
   int         x_ngroup    =    0;
   tGROUP     *x_pred      = NULL;
   int         x_npred     =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = yDLST_list_by_index  (x_ngroup, NULL, &x_group);
   DEBUG_LOOP   yLOG_complex ("head"      , "%3d, %p", rc, x_group);
   s_xcol = 0;
   /*---(walk through groups)------------*/
   while (rc >= 0) {
      if (x_group != NULL) {
         DEBUG_LOOP   yLOG_complex ("GROUP"     , "%3d, %p, %-15.15s, %2dr", rc, x_group, x_group->name, x_group->askd);
         x_npred = 0;
         rc = yDLST_seq_by_index  ('<', x_npred, NULL, NULL, &x_pred);
         while (rc >= 0) {
            if (x_group->col < 1)  x_group->col = 1;
            if (x_pred != NULL) {
               if (x_pred->col + 1 > x_group->col)  x_group->col = x_pred->col + 1;
            }
            if (x_group->col > s_xcol)  s_xcol = x_group->col;
            rc = yDLST_seq_by_index  ('<', ++x_npred, NULL, NULL, &x_pred);
         }
      }
      rc = yDLST_list_by_index  (++x_ngroup, NULL, &x_group);
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
   return 0;
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
      if      (a_ch == (uchar) '·' && i == a_top)   s_pert [i][x] = '³';
      else if (a_ch == (uchar) '·' && i == a_bot)   s_pert [i][x] = '³';
      else                                          s_pert [i][x] = a_ch;
   }
   return 0;
}

char
rptg__pert_horzer       (int y, int a_lef, int a_rig, uchar a_ch)
{
   int         i           =    0;
   for (i = a_lef; i <= a_rig; ++i) s_pert [y][i] = a_ch;
   return 0;
}

char rptg__pert_horz   (int y, int a_lef, int a_rig) { return rptg__pert_horzer (y, a_lef, a_rig, '€'); }
char rptg__pert_vert   (int a_top, int a_bot, int x) { return rptg__pert_verter (a_top, a_bot, x, ''); }
char rptg__pert_bound  (int a_top, int a_bot, int x) { return rptg__pert_verter (a_top, a_bot, x, '·'); }

char
rptg__pert_end          (int y, int x, uchar a_name)
{
   char       *x_top       = "ƒ€€€‚";
   char       *x_mid       = "   ";
   char       *x_bot       = "„€€€…";
   char        t           [LEN_TERSE] = "";
   char        o           = 14;
   if (a_name == (uchar) 'ÿ') o = 0;
   rptg__pert_merge (y + 0, x + o, x_top);
   rptg__pert_merge (y + 1, x + o, x_mid);
   rptg__pert_merge (y + 2, x + o, x_mid);
   rptg__pert_merge (y + 3, x + o, x_mid);
   rptg__pert_merge (y + 4, x + o, x_bot);
   sprintf (t, "%c"     , a_name);
   rptg__pert_merge (y    , x + o + 2, t);
   if (a_name == (uchar) 'ÿ') {
      switch (s_nomega) {
      case 0  :  strcpy (t, "À");  break;
      case 1  :  strcpy (t, "Á");  break;
      case 2  :  strcpy (t, "Â");  break;
      case 3  :  strcpy (t, "Ã");  break;
      case 4  :  strcpy (t, "Ä");  break;
      default :  strcpy (t, "Å");  break;
      }
      rptg__pert_merge (y + 2, x + o + 1, t);
      rptg__pert_merge (y + 1, x + o + 4, "");
      rptg__pert_merge (y + 2, x + o + 4, "");
      rptg__pert_merge (y + 3, x + o + 4, "");
   } else {
      switch (s_nalpha) {
      case 0  :  strcpy (t, "À");  break;
      case 1  :  strcpy (t, "Á");  break;
      case 2  :  strcpy (t, "Â");  break;
      case 3  :  strcpy (t, "Ã");  break;
      case 4  :  strcpy (t, "Ä");  break;
      default :  strcpy (t, "Å");  break;
      }
      rptg__pert_merge (y + 2, x + o + 3, t);
      rptg__pert_merge (y + 1, x + o    , "");
      rptg__pert_merge (y + 2, x + o    , "");
      rptg__pert_merge (y + 3, x + o    , "");
   }
   return 0;
}

char
rptg__pert_box          (int y, int x, char *a_name, char a_procs, int a_beg, int a_end, int a_dur, char a_note, char a_pred, char a_succ)
{
   char       *x_top       = "ƒ€€€€€€€€€€€€€€€€€‚";
   char       *x_mid       = "                 ";
   char       *x_bot       = "„€€€€€€€€€€€€€€€€€…";
   char        t           [LEN_TERSE] = "";
   rptg__pert_merge (y + 0, x    , x_top);
   rptg__pert_merge (y + 1, x    , x_mid);
   rptg__pert_merge (y + 2, x    , x_mid);
   rptg__pert_merge (y + 3, x    , x_mid);
   rptg__pert_merge (y + 4, x    , x_bot);
   rptg__pert_merge (y + 0, x + 9 - strlen (a_name) / 2, a_name);
   switch (a_procs) {
   case  0 : sprintf (t, "···· ····");  break;
   case  1 : sprintf (t, "····Ï····");  break;
   case  2 : sprintf (t, "···Ï·Ï···");  break;
   case  3 : sprintf (t, "··Ï·Ï·Ï··");  break;
   case  4 : sprintf (t, "·Ï·Ï·Ï·Ï·");  break;
   case  5 : sprintf (t, "Ï·Ï·Ï·Ï·Ï");  break;
   case  6 : sprintf (t, "·ÏÏÏ·ÏÏÏ·");  break;
   case  7 : sprintf (t, "ÏÏ·ÏÏÏ·ÏÏ");  break;
   case  8 : sprintf (t, "ÏÏÏÏ·ÏÏÏÏ");  break;
   case  9 : sprintf (t, "ÏÏÏÏÏÏÏÏÏ");  break;
   default : sprintf (t, "<ÏÏÏÏÏÏÏ>");  break;
   }
   rptg__pert_merge (y + 2, x + 5, t);
   /*> sprintf (t, "[%02d]", a_procs);                                                <* 
    *> rptg__pert_merge (y + 3, x + 7                      , t);                      <*/
   /*> sprintf (t, "b%-5d" , a_beg);                                                  <* 
    *> rptg__pert_merge (y + 1, x + 2                      , t);                      <*/
   /*> sprintf (t, "%5de"   , a_end);                                                 <* 
    *> rptg__pert_merge (y + 1, x + 16 - 5                 , t);                      <*/
   /*> sprintf (t, "%dd"    , a_dur);                                                 <* 
    *> rptg__pert_merge (y + 2, x + 9 - strlen (t) / 2     , t);                      <*/
   switch (a_pred) {
   case 0  :  strcpy (t, "À");  break;
   case 1  :  strcpy (t, "Á");  break;
   case 2  :  strcpy (t, "Â");  break;
   case 3  :  strcpy (t, "Ã");  break;
   case 4  :  strcpy (t, "Ä");  break;
   default :  strcpy (t, "Å");  break;
   }
   rptg__pert_merge (y + 2, x + 1                      , t);
   switch (a_succ) {
   case 0  :  strcpy (t, "À");  break;
   case 1  :  strcpy (t, "Á");  break;
   case 2  :  strcpy (t, "Â");  break;
   case 3  :  strcpy (t, "Ã");  break;
   case 4  :  strcpy (t, "Ä");  break;
   default :  strcpy (t, "Å");  break;
   }
   rptg__pert_merge (y + 2, x + 17                     , t);
   /*> sprintf (t, "%c"     , a_note);                                                <* 
    *> rptg__pert_merge (y + 1, x + 9                      , t);                      <*/
   return 0;
}

char
rptg__pert_line         (int y1, int x1, char a_asucc, char a_nsucc, int y2, int x2, char a_apred, char a_npred)
{
   int         i           =    0;
   char        s           =    2;
   char        f           =    2;
   char        l           =    0;
   /*---(exit)---------------------------*/
   switch (a_asucc) {
   case  1 :  s = 2;                 break;
   case  2 :  s = 1 + a_nsucc * 2;   break;
   case  3 :  s = 1 + a_nsucc;       break;
   case  4 :  s = a_nsucc;           break;
   case  5 :  s = a_nsucc;           break;
   }
   switch (a_apred) {
   case  1 :  f = 2;                 break;
   case  2 :  f = 1 + a_npred * 2;   break;
   case  3 :  f = 1 + a_npred;       break;
   case  4 :  f = a_npred;           break;
   case  5 :  f = a_npred;           break;
   }
   /*---(start marker)-------------------*/
   switch (s) {
   case 4 :
      rptg__pert_merge (y1 + s, x1 - 1, "ˆ");
      break;
   case 3 : case 2 : case 1 :
      rptg__pert_merge (y1 + s, x1 - 1, "‡");
      break;
   case 0 :
      rptg__pert_merge (y1 + s, x1 - 1, "‰");
      break;
   }
   /*---(horizontal)---------------------*/
   if (y1 + s == y2 + f) {
      for (i = x1; i < x2; ++i)  s_pert [y1 + s][i] = '€';
   }
   /*---(downward)-----------------------*/
   else if (y1 + s < y2 + f) {
      l = 8 - (s * 2);
      rptg__pert_horz  (y1 + s, x1    , x1 + l);
      rptg__pert_merge (y1 + s, x1 + l + 1, "‚");
      rptg__pert_vert  (y1 + s + 1, y2 + f - 1, x1 + l + 1);
      s_pert [y2 + f][x1 + l + 1] = '„';
      rptg__pert_horz  (y2 + f, x1 + l + 2, x2 - 1);
   }
   /*---(upward)-------------------------*/
   else {
      l = 8 - (f * 2);
      rptg__pert_horz  (y2 + f, x2 - l - 1, x2 - 1);
      rptg__pert_merge (y2 + f, x2 - l - 2, "ƒ");
      rptg__pert_vert  (y2 + f + 1, y1 + s - 1, x2 - l - 2);
      s_pert [y1 + s][x2 - l - 2] = '…';
      rptg__pert_horz  (y1 + s, x1, x2 - l - 3);
   }
   /*---(finish marker)------------------*/
   switch (f) {
   case 4 :
      rptg__pert_merge (y2 + f, x2 + 0, "‰");
      break;
   case 3 : case 2 : case 1 :
      rptg__pert_merge (y2 + f, x2 + 0, "†");
      break;
   case 0 :
      rptg__pert_merge (y2 + f, x2 + 0, "ˆ");
      break;
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
   for (i = 0; i < s_maxy; ++i)   fprintf (f, "%*.*s\n", s_maxx, s_maxx, s_pert [i]);
   fclose (f);
   return 0;
}

char
rptg__pert_connect      (tGROUP *a_group)
{
   char        rc          =    0;
   void       *x_beg       = NULL;
   void       *x_end       = NULL;
   int         x_asucc     =    0;
   int         x_nsucc     =    0;
   tGROUP     *x_succ      = NULL;
   int         x_apred     =    0;
   int         x_npred     =    0;
   tGROUP     *x_pred      = NULL;
   int         y1, x1, y2, x2;
   /*---(check start)--------------------*/
   if (a_group == NULL)  return 0;
   x_beg   = yDLST_list_current ();
   x_asucc = yDLST_seq_count ('>');
   /*---(set start position)-------------*/
   y1 = a_group->row * OFF_Y;
   x1 = a_group->col * OFF_X + 19;
   /*---(get first successor)------------*/
   rc = yDLST_seq_by_index  ('>', x_nsucc = 0, NULL, &x_end, &x_succ);
   while (rc >= 0) {
      /*---(handle successor)------------*/
      if (x_succ != NULL) {
         /*---(set end position)---------*/
         y2 = x_succ->row * OFF_Y;
         x2 = x_succ->col * OFF_X;
         /*---(go to successor)----------*/
         yDLST_list_restore (x_end);
         x_apred = yDLST_seq_count ('<');
         /*---(go to first predecessor)--*/
         rc = yDLST_seq_by_index ('<', x_npred = 0, NULL, NULL, &x_pred);
         while (rc >= 0) {
            /*---(draw connector)-------*/
            if (x_pred == a_group)  rptg__pert_line (y1, x1, x_asucc, x_nsucc, y2, x2, x_apred, x_npred);
            /*---(next predecessor)-----*/
            rc = yDLST_seq_by_index  ('<', ++x_npred, NULL, NULL, &x_pred);
            /*---(done)-----------------*/
         }
         /*---(done)---------------------*/
      }
      /*---(handle omega end)------------*/
      else {
         y2 = (s_xrow - 1) * OFF_Y;
         x2 = s_xcol * OFF_X;
         rptg__pert_line (y1, x1, 0, 0, y2, x2, s_nomega, s_comega++);
      }
      /*---(next)------------------------*/
      yDLST_list_restore (x_beg);
      rc = yDLST_seq_by_index  ('>', ++x_nsucc, NULL, &x_end, &x_succ);
      /*---(done)------------------------*/
   }
   /*---(handle alpha end)---------------*/
   if (a_group->col == 1) {
      y2 = y1;
      x2 = x1 - 19;
      y1 = 7;
      x1 = 19;
      rptg__pert_line (y1, x1, s_nalpha, s_calpha++, y2, x2, 0, 0);
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
   /*---(prepare)------------------------*/
   s_nalpha = yDLST_alpha_count ();
   s_nomega = yDLST_omega_count ();
   s_calpha = s_comega = 0;
   /*---(walk)---------------------------*/
   rc = yDLST_list_by_index  (x_ngroup, NULL, &x_group);
   while (rc >= 0) {
      if (x_group != NULL) rptg__pert_box     (x_group->row * OFF_Y, x_group->col * OFF_X, x_group->name, x_group->askd, x_group->beg, x_group->end, x_group->dur, x_group->note, yDLST_seq_count ('<'), yDLST_seq_count ('>'));
      rc = yDLST_list_by_index (++x_ngroup, NULL, &x_group);
   }
   /*---(ends)---------------------------*/
   rptg__pert_end     (OFF_Y           , 0                 , 'è');
   rptg__pert_end     ((s_xrow - 1) * OFF_Y, s_xcol * OFF_X, 'ÿ');
   /*---(bounds)-------------------------*/
   rptg__pert_bound   (3, s_xrow * OFF_Y - 2, 10);
   rptg__pert_bound   (3, s_xrow * OFF_Y - 2, s_xcol * OFF_X + 8);
   s_maxy = s_xrow * OFF_Y - 2 + 5;
   s_maxx = s_xcol * OFF_X + 8 + 5;
   /*---(complete)-----------------------*/
   DEBUG_OUTP  yLOG_exit    (__FUNCTION__);
   return  rc;
}

char
rptg__pert_network      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   int         x_ngroup    =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   rc = yDLST_list_by_index  (x_ngroup, NULL, &x_group);
   while (rc >= 0) {
      if (x_group != NULL) rptg__pert_connect (x_group);
      rc = yDLST_list_by_index (++x_ngroup, NULL, &x_group);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP  yLOG_exit    (__FUNCTION__);
   return  rc;
}

char
rptg_pert               (void)
{
   rptg__pert_clear   ();
   rptg__pert_col     ();
   rptg__pert_row     ();
   rptg__pert_groups  ();
   rptg__pert_network ();
   rptg__pert_write   ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        pert charting                         ----===*/
/*====================------------------------------------====================*/
static void      o___GANTT___________________o (void) {;}

static int   s_gmax  = 0;
static float s_scale = 0.0;

char
rptg__gantt_max         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   int         x_ngroup    =    0;
   /*---(prepare)------------------------*/
   rc = yDLST_list_by_index  (x_ngroup, NULL, &x_group);
   DEBUG_LOOP   yLOG_complex ("head"      , "%3d, %p", rc, x_group);
   s_gmax = 0;
   /*---(walk through groups)------------*/
   while (rc >= 0) {
      if (x_group != NULL) {
         DEBUG_LOOP   yLOG_complex ("GROUP"     , "%3d, %p, %-15.15s, %2dr", rc, x_group, x_group->name, x_group->askd);
         if (x_group->end > s_gmax)  s_gmax = x_group->end;
      }
      rc = yDLST_list_by_index  (++x_ngroup, NULL, &x_group);
   }
   s_scale = 200.0 / s_gmax;
   return 0;
}

char
rptg_gantt              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   int         x_ngroup    =    0;
   tPROC      *x_proc      = NULL;
   int         x_nproc     =    0;
   int         x_beg       =    0;
   int         x_end       =    0;
   int         i           =    0;
   /*---(prepare)------------------------*/
   rptg__gantt_max ();
   rc = yDLST_list_by_index  (x_ngroup, NULL, &x_group);
   DEBUG_LOOP   yLOG_complex ("head"      , "%3d, %p", rc, x_group);
   /*---(walk through groups)------------*/
   while (rc >= 0) {
      if (x_group != NULL) {
         DEBUG_LOOP   yLOG_complex ("GROUP"     , "%3d, %p, %-15.15s, %2dr", rc, x_group, x_group->name, x_group->askd);
         x_beg = x_group->beg * s_scale;
         x_end = x_group->end * s_scale;
         printf ("\n");
         for (i = 0    ; i < x_beg; i++)  printf (" ");
         printf ("%s\n", x_group->name);
         for (i = 0    ; i < x_beg; i++)  printf (" ");
         printf ("[");
         for (i = x_beg + 1; i < x_end - 1; i++)  printf ("=");
         printf ("]\n");
         rc = yDLST_line_by_index  ('-', x_nproc = 0, NULL, &x_proc);
         while (rc >= 0) {
            if (x_proc != NULL) {
               for (i = 0    ; i < x_beg; i++)  printf (" ");
               printf ("%s (%c)\n", x_proc->name, x_proc->yexec);
            }
            rc = yDLST_line_by_index  ('-', ++x_nproc, NULL, &x_proc);
         }
      }
      rc = yDLST_list_by_index  (++x_ngroup, NULL, &x_group);
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        pert charting                         ----===*/
/*====================------------------------------------====================*/
static void      o___DUMP____________________o (void) {;}



char
rptg_dump               (void)
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
      fprintf (f, "##   eos-rhododactylos (rosy-fingered dawn) -- simple, reliable, and transparent system initialization\n");
   } else {
      fprintf (f, "##/sbin/astraios (system shutdown)\n");
      fprintf (f, "##   astraios-aeolus (sparkling wind father) -- simple, reliable, and transparent system shutdown\n");
   }
   fprintf (f, "##   performance report with major tracking information\n");
   fprintf (f, "##\n");
   fprintf (f, "#num § ---group------- § ---proc-------- § ---description-------------------------- § t § ---user--- § uid- § -est-- § -minest- § -maxest- § --beg--- § -rpid- § y § ret- § --end--- § --dur- § ");
   fprintf (f, "\n");
   /*---(check all groups)---------------*/
   for (yDLST_list_by_cursor ('[', NULL, &x_group); x_group != NULL; yDLST_list_by_cursor ('>', NULL, &x_group)) {
      for (yDLST_line_by_cursor ('-', '[', NULL, &x_proc); x_proc != NULL; yDLST_line_by_cursor ('-', '>', NULL, &x_proc)) {
         if (c % 5 == 0)  fprintf (f, "\n");
         fprintf (f, "%4d § %-15.15s § %-15.15s § %-40.40s § %c § ",
               x_proc->line, x_group->name, x_proc->name, x_proc->desc, x_proc->type);
         fprintf (f, "%-10.10s § %4d § ",
               x_proc->user, x_proc->uid);
         fprintf (f, "%6d § %8d § %8d § ",
               x_proc->est , x_proc->minest, x_proc->maxest);
         fprintf (f, "%8d § %6d § %c § ",
               x_proc->beg, x_proc->rpid, x_proc->yexec);
         fprintf (f, "%4d § %8d § %6d § ",
               x_proc->rc, x_proc->end, x_proc->dur);
         fprintf (f, "\n");
         ++c;
      }
   }
   /*---(footer)-------------------------*/
   fprintf (f, "\n");
   fprintf (f, "#num § ---group------- § ---proc-------- § ---description-------------------------- § t § ---user--- § uid- § -est-- § -minest- § -maxest- § --beg--- § -rpid- § y § ret- § --end--- § --dur- § ");
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






