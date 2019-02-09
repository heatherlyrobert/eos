/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



/*====================------------------------------------====================*/
/*===----                      fundamental actions                     ----===*/
/*====================------------------------------------====================*/
static void  o___PRIMATIVE_______o () { return; }

char
proc__wipe          (tPROC *a_dst)
{
   /*---(master)-------------------------*/
   DEBUG_INPT   yLOG_snote   ("wipe master");
   a_dst->line        =    0;
   a_dst->name    [0] = '\0';
   a_dst->type        =  '-';
   a_dst->desc    [0] = '\0';
   a_dst->user    [0] = '\0';
   a_dst->uid         =   -1;
   a_dst->run     [0] = '\0';
   /*---(processing)---------------------*/
   DEBUG_INPT   yLOG_snote   ("wipe processing");
   a_dst->beg         =    0;
   a_dst->rpid        =   -1;
   a_dst->yexec       =  '-';
   a_dst->rc          =    0;
   a_dst->end         =    0;
   /*---(done)---------------------------*/
   return 0;
}

tPROC*
proc__new           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tPROC      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(create)-------------------------*/
   while (++x_tries < 10) {
      x_new = (tPROC *) malloc (sizeof (tPROC));
      if (x_new != NULL)     break;
   }
   DEBUG_INPT   yLOG_sint    (x_tries);
   DEBUG_INPT   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return NULL;
   }
   /*---(wipe)---------------------------*/
   proc__wipe (x_new);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return x_new;
}

char
proc__populate          (tPROC *a_dst)
{
   a_dst->line   = yPARSE_recdno ();
   strlcpy (a_dst->name  , my.p_name  , LEN_NAME);
   a_dst->type   = my.p_type;
   strlcpy (a_dst->desc  , my.p_desc  , LEN_DESC);
   strlcpy (a_dst->user  , my.p_user  , LEN_NAME);
   a_dst->uid    = my.p_uid;
   strlcpy (a_dst->run   , my.p_run   , LEN_CMD);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        major actions                         ----===*/
/*====================------------------------------------====================*/
static void  o___ACTIONS_________o () { return; }

char
proc__parse             (char a_type)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_fields    =    0;
   char        t           [LEN_NAME];
   int         x_pos       =    0;
   tPASSWD    *x_pass      = NULL;
   tPROC      *x_proc      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_INPT  yLOG_note    ("clear out working area");
   my.p_ready = '-';
   strlcpy (my.p_name    , ""         , LEN_NAME);
   my.p_type  = '-';
   strlcpy (my.p_desc    , ""         , LEN_DESC);
   strlcpy (my.p_user    , ""         , LEN_NAME);
   my.p_uid   =  -1;
   strlcpy (my.p_run     , ""         , LEN_CMD);
   /*---(field count)--------------------*/
   rc = yPARSE_ready (&x_fields);
   DEBUG_INPT   yLOG_value   ("ready"     , rc);
   --rce;  if (rc != 'y') {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("fields"    , x_fields);
   --rce; if (x_fields != 5) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(type)---------------------------*/
   my.p_type = a_type;
   DEBUG_INPT   yLOG_char    ("p_type"    , my.p_type);
   /*---(name)---------------------------*/
   rc = yPARSE_popstr  (my.p_name);
   DEBUG_INPT   yLOG_value   ("name"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("p_name"    , my.p_name);
   x_proc = yDLST_line_find (my.p_name);
   DEBUG_INPT   yLOG_point   ("x_proc"    , x_proc);
   --rce;  if (x_proc != NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(desc)---------------------------*/
   rc = yPARSE_popstr  (my.p_desc);
   DEBUG_INPT   yLOG_value   ("desc"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("p_desc"    , my.p_desc);
   /*---(user)---------------------------*/
   rc = yPARSE_popstr  (my.p_user);
   DEBUG_INPT   yLOG_value   ("user"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("p_user"    , my.p_user);
   /*---(check uid)--------------------------*/
   x_pass = getpwnam (my.p_user);
   if (x_pass == NULL) {
      DEBUG_INPT   yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   my.p_uid = x_pass->pw_uid;
   DEBUG_INPT   yLOG_value ("p_uid"       , my.p_uid);
   /*---(run)----------------------------*/
   rc = yPARSE_popstr  (my.p_run);
   DEBUG_INPT   yLOG_value   ("run"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("p_run"     , my.p_run);
   /*---(set ready)----------------------*/
   my.p_ready = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
proc_create             (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tGROUP     *x_group     = NULL;
   tPROC      *x_proc      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(parse the record)---------------*/
   rc = proc__parse (a_type);
   DEBUG_INPT   yLOG_value   ("parse"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check its runable)--------------*/
   rc = yEXEC_runable (my.p_name, my.p_user, my.p_run, YEXEC_FULL);
   DEBUG_INPT   yLOG_value   ("runnable"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create data)--------------------*/
   x_proc = proc__new ();
   DEBUG_INPT   yLOG_point   ("x_proc"    , x_proc);
   --rce;  if (x_proc == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   rc = proc__populate (x_proc);
   DEBUG_INPT   yLOG_value   ("populate"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create list)--------------------*/
   rc = yDLST_line_create (x_proc->name, x_proc);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update list)--------------------*/
   x_group = (tGROUP *) yDLST_line_list ();
   DEBUG_INPT   yLOG_point   ("x_group"   , x_group);
   --rce;  if (x_group == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ++x_group->requested;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
proc__unit              (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_RECD]  = "[]";
   char        t           [LEN_RECD]  = "[]";
   int         c           =    0;
   void       *x_void      = NULL;
   tGROUP     *x_group     = NULL;
   tPROC      *x_proc      = NULL;
   char        x_ready     =  '-';
   int         x_fields    =    0;
   char        x_beg       =  '-';
   char        x_end       =  '-';
   char        x_focus     =  '-';
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "PROC             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_RECD, "PROC count       : %d", yDLST_line_allcount ());
   }
   else if (strcmp (a_question, "name"    )        == 0) {
      x_proc  = (tPROC  *) yDLST_line_entry (a_num, NULL);
      if (x_proc != NULL) {
         sprintf (t, "[%s]", x_proc->name);
         snprintf (unit_answer, LEN_RECD, "PROC name   (%2d) : %2d%-20.20s  %2d  %c", a_num, strlen (x_proc->name), t, x_proc->line, x_proc->type);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC name   (%2d) :  0[]                    -1  -", a_num);
      }
   }
   else if (strcmp (a_question, "user"    )        == 0) {
      x_proc  = (tPROC  *) yDLST_line_entry (a_num, NULL);
      if (x_proc != NULL) {
         sprintf (t, "[%s]", x_proc->user);
         snprintf (unit_answer, LEN_RECD, "PROC user   (%2d) : %2d%-20.20s  %d", a_num, strlen (x_proc->user), t, x_proc->uid);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC user   (%2d) :  0[]                    -1", a_num);
      }
   }
   else if (strcmp (a_question, "run"     )        == 0) {
      x_proc  = (tPROC  *) yDLST_line_entry (a_num, NULL);
      if (x_proc != NULL) {
         snprintf (unit_answer, LEN_RECD, "PROC run    (%2d) : %2d[%s]", a_num, strlen (x_proc->run), x_proc->run);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC run    (%2d) :  0[]", a_num);
      }
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      x_proc  = (tPROC  *) yDLST_line_entry (a_num, &x_void);
      x_group = (tGROUP *) x_void;
      if (x_proc != NULL) {
         sprintf (s, "[%s]", x_group->name);
         sprintf (t, "[%s]", x_proc->name);
         snprintf (unit_answer, LEN_RECD, "PROC entry  (%2d) : %-20.20s  %-20.20s  %2d", a_num, s, t, strlen (x_proc->run));
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC entry  (%2d) : []                    []                    -1", a_num);
      }
   }
   else if (strcmp (a_question, "exec"    )        == 0) {
      x_proc  = (tPROC  *) yDLST_line_entry (a_num, &x_void);
      x_group = (tGROUP *) x_void;
      if (x_proc != NULL) {
         sprintf (s, "[%-4.4s]", x_group->name);
         sprintf (t, "[%-4.4s]", x_proc->name);
         x_beg   = (x_proc->beg > 0) ? 'y' : '-';
         x_end   = (x_proc->end > 0) ? 'y' : '-';
         x_focus = (yDLST_focus_check (x_proc->name, NULL)) ? 'y' : '-';
         snprintf (unit_answer, LEN_RECD, "PROC exec   (%2d) : %-6.6s  %-6.6s  %c %c %6d %c %3d %c", a_num, s, t, x_focus, x_beg, x_proc->rpid, x_proc->yexec, x_proc->rc, x_end);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC exec   (%2d) : []      []      - -     -1 -   0 -", a_num);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


