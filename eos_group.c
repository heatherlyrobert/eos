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
   a_group->requested   =    0;
   a_group->completed   =    0;
   /*---(timing)-------------------------*/
   a_group->beg         =    0;
   a_group->end         =    0;
   a_group->dur         =    0;
   a_group->warning     =  '-';
   /*---(done)---------------------------*/
   return 1;
}

char*
group__memory           (tGROUP *a_group)
{
   strlcpy (s_print, "[", LEN_RECD);
   if (a_group->line == 0)             strlcat (s_print, "_", LEN_RECD);
   else                                strlcat (s_print, "X", LEN_RECD);
   if (strlen (a_group->name) == 0)    strlcat (s_print, "_", LEN_RECD);
   else                                strlcat (s_print, "X", LEN_RECD);
   if (strlen (a_group->desc) == 0)    strlcat (s_print, "_", LEN_RECD);
   else                                strlcat (s_print, "X", LEN_RECD);
   strlcat (s_print, ".", LEN_RECD);
   if (a_group->status == GROUP_READY) strlcat (s_print, "_", LEN_RECD);
   else                                strlcat (s_print, "X", LEN_RECD);
   if (a_group->requested == 0)        strlcat (s_print, "_", LEN_RECD);
   else                                strlcat (s_print, "X", LEN_RECD);
   if (a_group->completed == 0)        strlcat (s_print, "_", LEN_RECD);
   else                                strlcat (s_print, "X", LEN_RECD);
   strlcat (s_print, ".", LEN_RECD);
   if (a_group->beg       == 0)        strlcat (s_print, "_", LEN_RECD);
   else                                strlcat (s_print, "X", LEN_RECD);
   if (a_group->end       == 0)        strlcat (s_print, "_", LEN_RECD);
   else                                strlcat (s_print, "X", LEN_RECD);
   if (a_group->warning   == '-')      strlcat (s_print, "_", LEN_RECD);
   else                                strlcat (s_print, "X", LEN_RECD);
   strlcat (s_print, "]", LEN_RECD);
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
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   --rce;  if (strcmp (a_verb, "GROUP") != 0) {
      DEBUG_INPT  yLOG_note    ("incorrect verb handler called");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(parse fields)-------------------*/
   rc = yPARSE_scanf (a_verb, "LD", x_label, x_desc);
   DEBUG_INPT  yLOG_value   ("scanf"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(create data)--------------------*/
   rc = group__new (&x_new);
   DEBUG_INPT   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   x_new->line = n;
   strlcpy (x_new->name, x_label, LEN_LABEL);
   strlcpy (x_new->desc, x_desc , LEN_DESC);
   /*---(create list)--------------------*/
   rc = yDLST_list_create (x_new->name, x_new);
   DEBUG_INPT   yLOG_value   ("yDLST"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   --rce;  if (strcmp (a_verb, "AFTER") != 0) {
      DEBUG_INPT  yLOG_note    ("incorrect verb handler called");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(parse fields)-------------------*/
   rc = yPARSE_scanf (a_verb, "L", x_label);
   DEBUG_INPT  yLOG_value   ("scanf"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(create list)--------------------*/
   rc = yDLST_seq_after (x_label);
   DEBUG_INPT   yLOG_value   ("after"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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
   /*---(update group)-------------------*/
   yDLST_list_by_cursor ('-', NULL, &x_group);
   --rce;  if (x_group == NULL)                  return rce;
   --rce;  if (x_group->status != GROUP_READY)   return rce;
   x_group->status    = GROUP_RUNNING;
   x_group->beg       = a_msec;
   /*---(complete)-----------------------*/
   return 0;
}

char
group_mark_done         (llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tGROUP     *x_group     = NULL;
   /*---(update group)-------------------*/
   yDLST_list_by_cursor ('-', NULL, &x_group);
   --rce;  if (x_group == NULL)                  return rce;
   --rce;  if (x_group->status != GROUP_RUNNING) return rce;
   x_group->status    = GROUP_DONE;
   x_group->end       = a_msec;
   x_group->dur       = x_group->end - x_group->beg;
   /*---(complete)-----------------------*/
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
   x_group->completed =    0;
   x_group->beg       =    0;
   x_group->end       =    0;
   x_group->dur       =    0;
   x_group->warning   =  '-';
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
   strlcpy  (unit_answer, "GROUP            : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   yDLST_backup ();
   if      (strcmp (a_question, "entry"   )        == 0) {
      yDLST_list_by_index (a_num, NULL, &x_group);
      if (x_group != NULL) {
         sprintf (t, "%2d[%-.15s]", strlen (x_group->name), x_group->name);
         sprintf (s, "%2d[%-.40s]", strlen (x_group->desc), x_group->desc);
         snprintf (unit_answer, LEN_RECD, "GROUP entry (%2d) : %-19.19s  %2d  %s", a_num, t, x_group->line, s);
      } else {
         snprintf (unit_answer, LEN_RECD, "GROUP entry (%2d) :  -[]                  -   -[]", a_num);
      }
   }
   else if (strcmp (a_question, "exec"    )        == 0) {
      yDLST_list_by_index (a_num, NULL, &x_group);
      if (x_group != NULL) {
         sprintf (t, "%2d[%-.15s]", strlen (x_group->name), x_group->name);
         strlcpy (s, "", LEN_RECD);
         rc = yDLST_seq_by_cursor ('<', '[', &x_void, NULL, &x_after);
         while (rc >= 0 && x_void != NULL) {
            if (c > 0)  strlcat (s, ", ", LEN_RECD);
            if (x_after == NULL)  strlcat (s, "SEQ_ALPHA"    , LEN_RECD);
            else                  strlcat (s, x_after->name  , LEN_RECD);
            rc = yDLST_seq_by_cursor ('<', '>', &x_void, NULL, &x_after);
            ++c;
         }
         sprintf  (u, "%1d[%.30s]", c, s);
         snprintf (unit_answer, LEN_RECD, "GROUP exec  (%2d) : %-19.19s %2d %2d %c  %-33.33s  %4d %4d %4d %c", a_num, t, x_group->requested, x_group->completed, x_group->status, u, x_group->beg, x_group->end, x_group->dur, x_group->warning);
      } else {
         snprintf (unit_answer, LEN_RECD, "GROUP exec  (%2d) :  -[]                 -  - -  -[]                                   -    -    - -", a_num);
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




