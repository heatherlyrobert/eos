/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



/*====================------------------------------------====================*/
/*===----                      fundamental actions                     ----===*/
/*====================------------------------------------====================*/
static void  o___PRIMATIVE_______o () { return; }

char
group__wipe             (tGROUP *a_dst)
{
   /*---(master)-------------------------*/
   DEBUG_INPT   yLOG_snote   ("wipe master");
   a_dst->line        =    0;
   a_dst->name    [0] = '\0';
   a_dst->desc    [0] = '\0';
   /*---(processing)---------------------*/
   DEBUG_INPT   yLOG_snote   ("wipe processing");
   a_dst->status      = GROUP_READY;
   a_dst->requested   =    0;
   a_dst->completed   =    0;
   /*---(done)---------------------------*/
   return 0;
}

tGROUP*
group__new              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tGROUP     *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(create)-------------------------*/
   while (++x_tries < 10) {
      x_new = (tGROUP *) malloc (sizeof (tGROUP));
      if (x_new != NULL)     break;
   }
   DEBUG_INPT   yLOG_sint    (x_tries);
   DEBUG_INPT   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return NULL;
   }
   /*---(wipe)---------------------------*/
   group__wipe (x_new);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return x_new;
}

char
group__populate         (tGROUP *a_dst)
{
   a_dst->line = yPARSE_recdno ();
   strlcpy (a_dst->name  , my.g_name  , LEN_NAME);
   strlcpy (a_dst->desc  , my.g_desc  , LEN_DESC);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        major actions                         ----===*/
/*====================------------------------------------====================*/
static void  o___ACTIONS_________o () { return; }

char
group__parse            (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_fields    =    0;
   char        t           [LEN_NAME];
   tGROUP     *x_group     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_INPT  yLOG_note    ("clear out working area");
   my.g_ready = '-';
   strlcpy (my.g_name    , ""         , LEN_NAME);
   strlcpy (my.g_desc    , ""         , LEN_DESC);
   /*---(field count)--------------------*/
   rc = yPARSE_ready (&x_fields);
   DEBUG_INPT   yLOG_value   ("ready"     , rc);
   --rce;  if (rc != 'y') {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("fields"    , x_fields);
   --rce; if (x_fields != 8) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(name)---------------------------*/
   rc = yPARSE_popstr  (my.g_name);
   DEBUG_INPT   yLOG_value   ("name"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("g_name"    , my.g_name);
   x_group = yDLST_list_find (my.g_name);
   DEBUG_INPT   yLOG_point   ("x_group"   , x_group);
   --rce;  if (x_group != NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(desc)---------------------------*/
   rc = yPARSE_popstr  (my.g_desc);
   DEBUG_INPT   yLOG_value   ("desc"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("g_desc"    , my.g_desc);
   /*---(junk)---------------------------*/
   rc = yPARSE_popstr  (t);
   DEBUG_INPT   yLOG_value   ("junk"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set ready)----------------------*/
   my.g_ready = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
group__link             (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_before    [LEN_NAME];
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(run links)----------------------*/
   while (1) {
      /*---(name)---------------------------*/
      rc = yPARSE_popstr  (x_before);
      DEBUG_INPT   yLOG_value   ("popstr"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return c;
      }
      DEBUG_INPT   yLOG_info    ("x_before"  , x_before);
      DEBUG_INPT   yLOG_char    ("[0]"       , x_before [0]);
      --rce;  if (x_before [0] == '-') {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return c;
      }
      rc = yDLST_seq_after (x_before);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return c;
      }
      ++c;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
group_create            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tGROUP     *x_group     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(parse the record)---------------*/
   rc = group__parse ();
   DEBUG_INPT   yLOG_value   ("parse"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create data)--------------------*/
   x_group = group__new ();
   DEBUG_INPT   yLOG_point   ("x_group"   , x_group);
   --rce;  if (x_group == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   rc = group__populate (x_group);
   DEBUG_INPT   yLOG_value   ("populate"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create list)--------------------*/
   rc = yDLST_list_create (x_group->name, x_group);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(link)---------------------------*/
   rc = group__link ();
   DEBUG_INPT   yLOG_value   ("group"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
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
   char        t           [LEN_RECD]  = "[ALPHA]";
   char        s           [LEN_RECD]  = "[OMEGA]";
   int         c           =    0;
   tGROUP     *x_group     = NULL;
   tGROUP     *x_after     = NULL;
   char        x_ready     =  '-';
   int         x_fields    =    0;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "GROUP            : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "entry"   )        == 0) {
      x_group = (tGROUP *) yDLST_list_entry (a_num, &c);
      if (x_group != NULL) {
         sprintf (t, "[%s]", x_group->name);
         snprintf (unit_answer, LEN_RECD, "GROUP entry (%2d) : %2d%-30.30s    %2d  %2d", a_num, strlen (x_group->name), t, x_group->line, c);
      } else {
         snprintf (unit_answer, LEN_RECD, "GROUP entry (%2d) :  0[]                                 0   0", a_num);
      }
   }
   else if (strcmp (a_question, "exec"    )        == 0) {
      x_group = (tGROUP *) yDLST_list_entry (a_num, &c);
      if (x_group != NULL) {
         sprintf (t, "[%s]", x_group->name);
         snprintf (unit_answer, LEN_RECD, "GROUP exec  (%2d) : %2d%-30.30s    %2d  %2d  %c", a_num, strlen (x_group->name), t, x_group->requested, x_group->completed, x_group->status);
      } else {
         snprintf (unit_answer, LEN_RECD, "GROUP exec  (%2d) :  0[]                                 0   0  -", a_num);
      }
   }
   else if (strcmp (a_question, "yparse"  )        == 0) {
      x_ready = yPARSE_ready (&x_fields);
      snprintf (unit_answer, LEN_RECD, "GROUP yparse     : %2d  %c", x_fields, x_ready);
   }
   else if (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_RECD, "GROUP count      : %d", yDLST_list_count ());
   }
   else if (strcmp (a_question, "seq"     )        == 0) {
      x_group = (tGROUP *) yDLST_seq_entry (a_num, &x_after);
      if (x_group != NULL)  sprintf (t, "[%s]", x_group->name);
      if (x_after != NULL)  sprintf (s, "[%s]", x_after->name);
      if (x_group == NULL && x_after == NULL) {
         snprintf (unit_answer, LEN_RECD, "GROUP seq   (%2d) : []                     []", a_num);
      } else {
         snprintf (unit_answer, LEN_RECD, "GROUP seq   (%2d) : %-20.20s   %s", a_num, t, s);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}




