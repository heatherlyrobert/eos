/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



static char s_print     [LEN_RECD]  = "";



/*====================------------------------------------====================*/
/*===----                    cleansing and cleaning                    ----===*/
/*====================------------------------------------====================*/
static void  o___CLEAN___________o () { return; }

char
group__wipe             (tGROUP *a_group)
{
   /*---(master)-------------------------*/
   DEBUG_INPT   yLOG_snote   ("wipe master");
   a_group->line        =    0;
   a_group->name    [0] = '\0';
   a_group->desc    [0] = '\0';
   /*---(processing)---------------------*/
   DEBUG_INPT   yLOG_snote   ("wipe processing");
   a_group->status      = GROUP_READY;
   a_group->askd        =    0;
   a_group->done        =    0;
   /*---(timing)-------------------------*/
   a_group->beg         =    0;
   a_group->end         =    0;
   a_group->dur         =    0;
   a_group->note        =  '-';
   /*---(reporting)----------------------*/
   a_group->col         =    0;
   a_group->row         =    0;
   a_group->x           =    0;
   a_group->y           =    0;
   /*---(done)---------------------------*/
   return 1;
}

char*
group__memory           (tGROUP *a_group)
{
   int         i           =    0;
   ystrlcpy (s_print, "[___.___.____.__]", LEN_RECD);
   ++i;   if (a_group->line    != 0)             s_print [i] = 'X';
   ++i;   if (strlen (a_group->name) != 0)       s_print [i] = 'X';
   ++i;   if (strlen (a_group->desc) != 0)       s_print [i] = 'X';
   ++i;
   ++i;   if (a_group->status  != GROUP_READY)   s_print [i] = 'X';
   ++i;   if (a_group->askd    != 0)             s_print [i] = 'X';
   ++i;   if (a_group->done    != 0)             s_print [i] = 'X';
   ++i;
   ++i;   if (a_group->beg     != 0)             s_print [i] = 'X';
   ++i;   if (a_group->end     != 0)             s_print [i] = 'X';
   ++i;   if (a_group->dur     != 0)             s_print [i] = 'X';
   ++i;   if (a_group->note    != '-')           s_print [i] = 'X';
   ++i;
   ++i;   if (a_group->col     != 0)             s_print [i] = 'X';
   ++i;   if (a_group->row     != 0)             s_print [i] = 'X';
   return s_print;
}



/*====================------------------------------------====================*/
/*===----                     allocation and freeing                   ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
group__new                  (tGROUP **a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tGROUP     *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_INPT   yLOG_spoint  (a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_spoint  (*a_new);
   --rce;  if (*a_new != NULL) {
      DEBUG_INPT   yLOG_snote   ("already set");
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *a_new = NULL;
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tGROUP *) malloc (sizeof (tGROUP));
      if (x_tries > 3)   break;
   }
   DEBUG_INPT   yLOG_sint    (x_tries);
   DEBUG_INPT   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   group__wipe (x_new);
   /*---(save return)--------------------*/
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
group__free             (tGROUP **a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_DATA   yLOG_spoint  (a_old);
   --rce;  if (a_old == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (*a_old);
   --rce;  if (*a_old == NULL) {
      DEBUG_DATA   yLOG_snote   ("never set");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear and return)---------------*/
   free (*a_old);
   *a_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        major actions                         ----===*/
/*====================------------------------------------====================*/
static void  o___ACTIONS_________o () { return; }

char
group_handler           (int n, uchar *a_verb)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_label     [LEN_LABEL] = "";
   char        x_desc      [LEN_DESC]  = "";
   tGROUP     *x_new       = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   my.f_group = '-';
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL || strlen (a_verb) <= 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "group_handler called with null/empty verb");
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   --rce;  if (strcmp (a_verb, "GROUP") != 0) {
      yURG_err ('f', "group_handler called with å%sæ verb", a_verb);
      DEBUG_INPT  yLOG_note    ("incorrect verb handler called");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(parse fields)-------------------*/
   rc = yPARSE_ready (&c);
   DEBUG_INPT  yLOG_value   ("fields"    , c);
   --rce;  if (c < 2) {
      yURG_err ('F', "GROUP å%sæ on line %d with ONLY %d fields", x_label, n, c);
      DEBUG_INPT  yLOG_note    ("failed, only a verb");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   if (c > 3)  c = 3;
   switch (c) {
   case  2 :
      rc = yPARSE_scanf (a_verb, "L" , x_label);
      break;
   case  3 :
      rc = yPARSE_scanf (a_verb, "LD", x_label, x_desc);
      break;
   }
   DEBUG_INPT  yLOG_value   ("scanf"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('F', "GROUP å%sæ on line %d with %d fields, yPARSE_scanf failed (%d)", x_label, n, c, rc);
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(create data)--------------------*/
   rc = group__new (&x_new);
   DEBUG_INPT   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      yURG_msg ('F', "GROUP å%sæ on line %d with %d fields, malloc failed (%d)", x_label, n, c, rc);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   x_new->line = n;
   ystrlcpy (x_new->name, x_label, LEN_LABEL);
   ystrlcpy (x_new->desc, x_desc , LEN_DESC);
   /*---(create list)--------------------*/
   rc = yDLST_list_create (x_new->name, x_new);
   DEBUG_INPT   yLOG_value   ("yDLST"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('F', "GROUP å%sæ on line %d with %d fields, yDLST create failed (%d)", x_label, n, c, rc);
      group__free (&x_new);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('>', "GROUP å%sæ on line %d with %d fields, %s", x_label, n, c, x_desc);
   /*---(set group flag)-----------------*/
   my.f_group = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
after_handler           (int n, uchar *a_verb)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_label     [LEN_LABEL] = "";
   tGROUP     *x_group     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL || strlen (a_verb) <= 0) {
      yURG_err ('f', "after_handler called with null/empty verb");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   --rce;  if (strcmp (a_verb, "AFTER") != 0) {
      yURG_err ('f', "after_handler called with å%sæ verb", a_verb);
      DEBUG_INPT  yLOG_note    ("incorrect verb handler called");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(check group)--------------------*/
   DEBUG_INPT  yLOG_char    ("f_group"   , my.f_group);
   --rce;  if (my.f_group != 'y') {
      yURG_err ('f', "AFTER on line %d inside failed group, so failed", n);
      DEBUG_INPT  yLOG_note    ("not set to 'y'");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   yDLST_list_by_cursor (YDLST_DCURR, NULL, &x_group);
   DEBUG_INPT  yLOG_point   ("x_group"   , x_group);
   --rce;  if (x_group == NULL) {
      yURG_err ('f', "AFTER on line %d, no current group", n);
      DEBUG_INPT  yLOG_note    ("could not find parent group");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(parse fields)-------------------*/
   rc = yPARSE_scanf (a_verb, "L", x_label);
   DEBUG_INPT  yLOG_value   ("scanf"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "AFTER å%sæ on line %d, yPARSE_scanf failed (%d)", x_label, n, rc);
      x_group->note = 'A';
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(create list)--------------------*/
   rc = yDLST_seq_after (x_label);
   DEBUG_INPT   yLOG_value   ("after"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "AFTER å%sæ on line %d, yDLST create failed (%d)", x_label, n, rc);
      x_group->note = 'A';
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "AFTER å%sæ on line %d", x_label, n);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      execution helpers                       ----===*/
/*====================------------------------------------====================*/
static void      o___EXEC____________________o (void) {;}

char
group_mark_begin        (llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tGROUP     *x_group     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(update group)-------------------*/
   yDLST_list_by_cursor (YDLST_DCURR, NULL, &x_group);
   DEBUG_INPT  yLOG_point   ("x_group"   , x_group);
   --rce;  if (x_group == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_char    ("status"    , x_group->status);
   --rce;  if (x_group->status != GROUP_READY) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_group->status    = GROUP_RUNNING;
   DEBUG_INPT  yLOG_char    ("status"    , x_group->status);
   x_group->beg       = a_msec;
   EOS_VERBOSE  printf       ("%6lld group %s begin\n", a_msec, x_group->name);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
group_mark_done         (llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tGROUP     *x_group     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(update group)-------------------*/
   yDLST_list_by_cursor (YDLST_DCURR, NULL, &x_group);
   DEBUG_INPT  yLOG_point   ("x_group"   , x_group);
   --rce;  if (x_group == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_char    ("status"    , x_group->status);
   --rce;  if (x_group->status != GROUP_RUNNING) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_group->status    = GROUP_DONE;
   DEBUG_INPT  yLOG_char    ("status"    , x_group->status);
   x_group->end       = a_msec;
   x_group->dur       = x_group->end - x_group->beg;
   EOS_VERBOSE  printf       ("%6lld group %s end\n", a_msec, x_group->name);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
group_mark_clear        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tGROUP     *x_group     = NULL;
   /*---(update group)-------------------*/
   yDLST_list_by_cursor ('-', NULL, &x_group);
   --rce;  if (x_group == NULL)  return rce;
   /*---(update proc)--------------------*/
   x_group->status    = GROUP_READY;
   x_group->done      =    0;
   x_group->beg       =    0;
   x_group->end       =    0;
   x_group->dur       =    0;
   x_group->note      =  '-';
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
group__unit             (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        t           [LEN_RECD]  = "[ALPHA]";
   char        s           [LEN_RECD]  = "[OMEGA]";
   char        u           [LEN_RECD]  = "";
   int         c           =    0;
   tGROUP     *x_group     = NULL;
   void       *x_void      = NULL;
   tGROUP     *x_after     = NULL;
   char        x_ready     =  '-';
   int         x_fields    =    0;
   /*---(prepare)------------------------*/
   ystrlcpy  (unit_answer, "GROUP            : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   yDLST_backup ();
   if      (strcmp (a_question, "entry"   )        == 0) {
      yDLST_list_by_index (a_num, NULL, &x_group);
      if (x_group != NULL) {
         sprintf (t, "%2då%-.15sæ", strlen (x_group->name), x_group->name);
         sprintf (s, "%2då%-.40sæ", strlen (x_group->desc), x_group->desc);
         snprintf (unit_answer, LEN_RECD, "GROUP entry (%2d) : %-19.19s  %2d  %s", a_num, t, x_group->line, s);
      } else {
         snprintf (unit_answer, LEN_RECD, "GROUP entry (%2d) :  ·åæ                  ·   ·åæ", a_num);
      }
   }
   else if (strcmp (a_question, "exec"    )        == 0) {
      yDLST_list_by_index (a_num, NULL, &x_group);
      if (x_group != NULL) {
         sprintf (t, "%2då%-.15sæ", strlen (x_group->name), x_group->name);
         ystrlcpy (s, "", LEN_RECD);
         rc = yDLST_seq_by_cursor ('<', '[', &x_void, NULL, &x_after);
         while (rc >= 0 && x_void != NULL) {
            if (c > 0)  strlcat (s, ", ", LEN_RECD);
            if (x_after == NULL)  strlcat (s, "SEQ_ALPHA"    , LEN_RECD);
            else                  strlcat (s, x_after->name  , LEN_RECD);
            rc = yDLST_seq_by_cursor ('<', '>', &x_void, NULL, &x_after);
            ++c;
         }
         sprintf  (u, "%1då%.30sæ", c, s);
         snprintf (unit_answer, LEN_RECD, "GROUP exec  (%2d) : %-19.19s %2d %2d %c  %-33.33s  %4d %4d %4d %c", a_num, t, x_group->askd, x_group->done, x_group->status, u, x_group->beg, x_group->end, x_group->dur, x_group->note);
      } else {
         snprintf (unit_answer, LEN_RECD, "GROUP exec  (%2d) :  ·åæ                 ·  · ·  ·åæ                                   ·    ·    · ·", a_num);
      }
   }
   else if (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_RECD, "GROUP count      : %d", yDLST_list_count ());
   }
   else if (strcmp (a_question, "seq"     )        == 0) {
      /*> x_group = (tGROUP *) yDLST_seq_by_index ('*', a_num, NULL, NULL, &x_after);   <* 
       *> yDLST_list_by_index (a_num, NULL, &x_group);                                  <* 
       *> if (x_group != NULL)  sprintf (t, "[%s]", x_group->name);                     <* 
       *> if (x_after != NULL)  sprintf (s, "[%s]", x_after->name);                     <*/
      if (x_group == NULL && x_after == NULL) {
         snprintf (unit_answer, LEN_RECD, "GROUP seq   (%2d) : []                     []", a_num);
      } else {
         snprintf (unit_answer, LEN_RECD, "GROUP seq   (%2d) : %-20.20s   %s", a_num, t, s);
      }
   }
   yDLST_restore ();
   /*---(complete)-----------------------*/
   return unit_answer;
}




