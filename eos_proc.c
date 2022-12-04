/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



static char s_print     [LEN_RECD]  = "";

#define    MAX_VERBS       30
typedef struct cVERB tVERB;
struct cVERB {
   char        abbr;
   char        terse       [LEN_TERSE];
   char        desc        [LEN_DESC];
   char        example     [LEN_HUND];
};
static const tVERB  s_verbs [MAX_VERBS] = {
   /*---(full execution)-------*/
   { 'G'             , "GROUP"     , "common start/finish times"         , "" },
   { 'A'             , "AFTER"     , "link groups in F>S sequences"      , "-example as might appear in job/conf file----------------------" },
   { ' '             , ""          , ""                                  , "" },
   /*---(full execution)-------*/
   { '/'             , "-terse---" , "-description---------------------" , "GROUP    § NORMAL     § all the common verbs             § ···>" },
   { EOS_TYPE_EXEC   , "exec"      , "execute and wait for a process"    , "  exec   § dhcpcd     § bring dhcp on-line               § ···>" },
   { EOS_TYPE_CONFIG , "config"    , "simple, fast configuration update" , "  config § hwclock    § set the clock from hardware      § ···>" },
   { EOS_TYPE_BOOT   , "once"      , "only during true boot/shutdown"    , "  once   § dmesg      § set the kernel logging level     § ···>" },
   { ' '             , ""          , ""                                  , "" },
   /*---(launch only)----------*/
   { '/'             , "-terse---" , "-description---------------------" , "GROUP    § BIG IRON   § more critial activities          § ···>" },
   { '/'             , "· · · · ·" , "· · · · · · · · · · · · · · · · ·" , "  AFTER  § NORMAL     § · · · · · · · · · · · · · · · ·  § ···>" },
   { EOS_TYPE_DAEMON , "daemon"    , "launch and confirm a daemon"       , "  daemon § hestia     § start the hestia getty daemon    § ···>" },
   { EOS_TYPE_MOUNT  , "mount"     , "mount and confirm a file system"   , "  mount  § sys_fs     § mount the sys filesystem         § ···>" },
   { EOS_TYPE_SERIAL , "serial"    , "launch the first in a series"      , "  serial § getty6     § place standard getty on tty6     § ···>" },
   { ' '             , ""          , ""                                  , "" },
   /*---(job control)----------*/
   { '/'             , "-terse---" , "-description---------------------" , "GROUP    § PROCESS    § common process control actions   § ···>" },
   { '/'             , "· · · · ·" , "· · · · · · · · · · · · · · · · ·" , "  AFTER  § NORMAL     § · · · · · · · · · · · · · · · ·  § ···>" },
   { EOS_TYPE_STOP   , "stop"      , "pause a running process"           , "  stop   § hestia     § pause while defragging the disk  § ···>" },
   { EOS_TYPE_CONT   , "cont"      , "restart a paused process"          , "  cont   § hestia     § restart after defrag             § ···>" },
   { EOS_TYPE_RESET  , "reset"     , "reload and refresh a daemon"       , "  reset  § khronos    § get the most recent config       § ···>" },
   { EOS_TYPE_PING   , "ping"      , "ping a daemon for health"          , "  ping   § artemis    § verify process cleanup active    § ···>" },
   { ' '             , ""          , ""                                  , "" },
   /*---(terminate)------------*/
   { '/'             , "-terse---" , "-description---------------------" , "GROUP    § TERMINATE  § verbs for shutting down          § ···>" },
   { '/'             , "· · · · ·" , "· · · · · · · · · · · · · · · · ·" , "  AFTER  § PROCESS    § · · · · · · · · · · · · · · · ·  § ···>" },
   { EOS_TYPE_KILL   , "kill"      , "violently kill a running process"  , "  kill   § dhcpcd     § take down dhcp                   § ···>" },
   { EOS_TYPE_WRAPUP , "wrapup"    , "gracefully terminate a process"    , "  wrapup § hestia     § stopping online access           § ···>" },
   { EOS_TYPE_UMOUNT , "umount"    , "unmount and confirm a file system" , "  umount § boot       § take boot filesystem off-line    § ···>" },
   /*---(done)-----------------*/
   { '-'             , "---"       , "---"                               , "" },
};



/*====================------------------------------------====================*/
/*===----                    cleansing and cleaning                    ----===*/
/*====================------------------------------------====================*/
static void  o___CLEAN___________o () { return; }

char
proc__wipe          (tPROC *a_proc)
{
   DEBUG_INPT   yLOG_snote   ("master");
   a_proc->line        =    0;
   a_proc->name    [0] = '\0';
   a_proc->type        =  '-';
   a_proc->desc    [0] = '\0';
   DEBUG_INPT   yLOG_snote   ("command");
   a_proc->user    [0] = '\0';
   a_proc->uid         =   -1;
   a_proc->run     [0] = '\0';
   DEBUG_INPT   yLOG_snote   ("limits");
   a_proc->est         =    0;
   a_proc->minest      =    0;
   a_proc->maxest      =    0;
   DEBUG_INPT   yLOG_snote   ("flags");
   a_proc->value       =  '-';
   a_proc->track       =  '-';
   a_proc->rolling     =  '-';
   a_proc->strict      =  '-';
   a_proc->lower       =  '-';
   a_proc->upper       =  '-';
   a_proc->remedy      =  '-';
   DEBUG_INPT   yLOG_snote   ("results");
   a_proc->beg         =    0;
   a_proc->rpid        =   -1;
   a_proc->yexec       =  '-';
   a_proc->rc          =    0;
   a_proc->end         =    0;
   a_proc->dur         =    0;
   return 1;
}

char*
proc__memory            (tPROC *a_proc)
{
   int         i           =    0;
   /*---(prep)---------------------------*/
   strlcpy (s_print, "[____.___.___._______.______]", LEN_RECD);
   /*---(master)-------------------------*/
   ++i;  if (a_proc->line     != 0   )  s_print [i] = 'X';
   ++i;  if (a_proc->name [0] != 0   )  s_print [i] = 'X';
   ++i;  if (a_proc->type     != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->desc [0] != 0   )  s_print [i] = 'X';
   ++i;
   /*---(command)------------------------*/
   ++i;  if (a_proc->user [0] != 0   )  s_print [i] = 'X';
   ++i;  if (a_proc->uid      != -1  )  s_print [i] = 'X';
   ++i;  if (a_proc->run  [0] != 0   )  s_print [i] = 'X';
   ++i;
   /*---(limits)-------------------------*/
   ++i;  if (a_proc->est      != 0   )  s_print [i] = 'X';
   ++i;  if (a_proc->minest   != 0   )  s_print [i] = 'X';
   ++i;  if (a_proc->maxest   != 0   )  s_print [i] = 'X';
   ++i;
   /*---(flags)--------------------------*/
   ++i;  if (a_proc->value    != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->track    != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->rolling  != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->strict   != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->lower    != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->upper    != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->remedy   != '-' )  s_print [i] = 'X';
   ++i;
   /*---(results)------------------------*/
   ++i;  if (a_proc->beg      != 0   )  s_print [i] = 'X';
   ++i;  if (a_proc->rpid     != -1  )  s_print [i] = 'X';
   ++i;  if (a_proc->yexec    != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->rc       != 0   )  s_print [i] = 'X';
   ++i;  if (a_proc->end      != 0   )  s_print [i] = 'X';
   ++i;  if (a_proc->dur      != 0   )  s_print [i] = 'X';
   /*---(complete)-----------------------*/
   return s_print;
}



/*====================------------------------------------====================*/
/*===----                     allocation and freeing                   ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
proc__new                   (tPROC **a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tPROC      *x_new       = NULL;
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
      x_new = (tPROC *) malloc (sizeof (tPROC));
      if (x_tries > 3)   break;
   }
   DEBUG_INPT   yLOG_sint    (x_tries);
   DEBUG_INPT   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   proc__wipe (x_new);
   /*---(save return)--------------------*/
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
proc__free              (tPROC **a_old)
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
proc__flags             (tPROC *a_new, uchar *a_flags, char *a_dur)
{
   char        rc          =    0;
   int         x_floor     =    0;
   char        x_terse     [LEN_HUND]  = "";
   char        x_fancy     [LEN_RECD]  = "";
   if (strchr (EOS_WAIT_SHORT, a_new->type) != NULL) x_floor = 1000;
   if (strchr (EOS_WAIT_LONG , a_new->type) != NULL) x_floor = 2000;
   DEBUG_INPT  yLOG_complex ("x_floor"   , "%c, %d", a_new->type, x_floor);
   rc = yEXEC_flags (a_new->est, x_floor, a_flags,
         &(a_new->value) , &(a_new->track) , &(a_new->rolling), &(a_new->strict),
         &(a_new->lower) , &(a_new->minest), 
         &(a_new->upper) , &(a_new->maxest), 
         &(a_new->remedy));
   DEBUG_INPT  yLOG_complex ("min/max"   , "%5d, %c %5d, %c %5d", a_new->est, a_new->lower, a_new->minest, a_new->upper, a_new->maxest);
   yURG_msg ('+', "duration %s, est %5ds, min %c %5dms, max %c %5dms", a_dur, a_new->est, a_new->lower, a_new->minest, a_new->upper, a_new->maxest);
   yEXEC_flags_feedback (x_terse, x_fancy);
   yURG_msg ('+', "flags %s", x_fancy);
   if (strchr (EOS_WAIT_SHORT, a_new->type) != NULL) {
      DEBUG_INPT  yLOG_note    ("short type");
      if (a_new->est == 0 && a_new->upper == '-')  a_new->maxest = a_new->minest;
   }
   if (strchr (EOS_WAIT_LONG , a_new->type) != NULL) {
      DEBUG_INPT  yLOG_note    ("long type");
      if (a_new->est == 0 && a_new->upper == '-')  a_new->maxest = a_new->minest;
   }
   DEBUG_INPT  yLOG_complex ("min/max"   , "%5d, %c %5d, %c %5d", a_new->est, a_new->lower, a_new->minest, a_new->upper, a_new->maxest);
   /*---(complete)-----------------------*/
   return rc;
}

char
proc_handler            (int n, uchar *a_verb)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         i           =    0;
   int         c           =    0;
   char        x_type      =  '-';
   char        x_label     [LEN_LABEL] = "";
   char        x_desc      [LEN_DESC]  = "";
   char        x_user      [LEN_LABEL] = "";
   char        x_dur       [LEN_TERSE] = "0";
   uchar       x_flags     [LEN_TERSE] = "--·---·";
   char        x_run       [LEN_FULL]   = "";
   tPASSWD    *x_pass      = NULL;
   int         x_uid       =   -1;
   tPROC      *x_new       = NULL;
   tGROUP     *x_group     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(check group)--------------------*/
   --rce;  if (my.f_group != 'y') {
      yURG_err ('f', "PROC on line %d inside failed group, so failed", n);
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   rc = yDLST_list_by_cursor (YDLST_DCURR, NULL, &x_group);
   --rce;  if (x_group == NULL) {
      yURG_err ('f', "PROC on line %d, no current group", n);
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL || strlen (a_verb) <= 0) {
      yURG_err ('f', "proc_handler called with null/empty verb");
      x_group->note = 'P';
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   for (i = 0; i < MAX_VERBS; ++i) {
      if (s_verbs [i].abbr == '-')                 break;
      if (strcmp (s_verbs [i].terse, a_verb) != 0)  continue;
      x_type = s_verbs [i].abbr;
   }
   DEBUG_INPT   yLOG_char  ("x_type"      , x_type);
   --rce;  if (x_type == '-') {
      yURG_err ('f', "proc_handler called with å%sæ verb", a_verb);
      DEBUG_INPT  yLOG_note    ("incorrect verb handler called");
      x_group->note = 'P';
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse fields)-------------------*/
   rc = yPARSE_ready (&c);
   DEBUG_INPT  yLOG_value   ("fields"    , c);
   --rce;  if (c < 2) {
      yURG_err ('f', "step å%sæ on line %d with %d fields, too few", x_label, n, c);
      DEBUG_INPT  yLOG_note    ("failed, only a verb");
      x_group->note = 'P';
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (c > 7)  c = 7;
   switch (c) {
   case 2 : rc = yPARSE_scanf (a_verb, "F"     , x_run);  break;
   case 3 : rc = yPARSE_scanf (a_verb, "UF"    , x_user , x_run);  break;
   case 4 : rc = yPARSE_scanf (a_verb, "LUF"   , x_label, x_user, x_run);  break;
   case 5 : rc = yPARSE_scanf (a_verb, "LUTF"  , x_label, x_user, x_dur, x_run);  break;
   case 6 : rc = yPARSE_scanf (a_verb, "LDUTF" , x_label, x_desc, x_user, x_dur, x_run);  break;
   case 7 : rc = yPARSE_scanf (a_verb, "LDUTTF", x_label, x_desc, x_user, x_dur, x_flags, x_run);  break;
   }
   DEBUG_INPT  yLOG_value   ("scanf"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "step å%sæ on line %d with %d fields, yPARSE_scanf failed", x_label, n, c);
      x_group->note = 'P';
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(label)------------------------------*/
   if (strcmp (x_label, "") == 0)   sprintf (x_label, "#%03d", n);
   yURG_msg ('>', "  step å%sæ on line %d with %d fields, %s", x_label, n, c, x_desc);
   /*---(check user)-------------------------*/
   if (strcmp (x_user, "") == 0)   strlcpy (x_user, my.m_who, LEN_LABEL);
   rc = yEXEC_userdata (x_user, &x_uid, NULL, NULL, NULL);
   --rce;  if (rc < 0) {
      yURG_err ('f', "user requested failed å%sæ (%d)", my.m_who, rc);
      x_group->note = 'P';
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('+', "user requested successfully å%sæ (%d)", my.m_who, x_uid);
   /*---(check its runable)--------------*/
   rc = yEXEC_runable (x_label, x_user, x_run, YEXEC_FULL);
   DEBUG_INPT   yLOG_value   ("runnable"  , rc);
   --rce;  if (rc < 0) {
      yURG_err ('+', "command is NOT runable %2d å%sæ", strlen (x_run), x_run);
      x_group->note = 'P';
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('+', "command is runable %2d å%sæ", strlen (x_run), x_run);
   /*---(create data)--------------------*/
   rc = proc__new (&x_new);
   DEBUG_INPT   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      yURG_err ('f', "proc line could not be allocated (%d)", rc);
      x_group->note = 'P';
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   x_new->line = n;
   strlcpy (x_new->name, x_label, LEN_LABEL);
   x_new->type    = x_type;
   strlcpy (x_new->desc, x_desc , LEN_DESC);
   strlcpy (x_new->user, x_user , LEN_LABEL);
   x_new->uid     = x_uid;
   rc = yEXEC_dur_in_sec (x_dur, &(x_new->est));
   proc__flags (x_new, x_flags, x_dur);
   strlcpy (x_new->run , x_run  , LEN_FULL);
   /*---(create line)--------------------*/
   rc = yDLST_line_create (x_label, x_new);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "yDLST line could not be created (%d)", rc);
      x_group->note = 'P';
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update list)--------------------*/
   rc = yDLST_line_list (NULL, &x_group);
   DEBUG_INPT   yLOG_point   ("x_group"   , x_group);
   --rce;  if (x_group == NULL) {
      yURG_err ('f', "yDLST group for line not found (%d)", rc);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ++x_group->askd;
   yURG_msg ('+', "å%sæ step is successfully created", a_verb);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      execution helpers                       ----===*/
/*====================------------------------------------====================*/
static void      o___EXEC____________________o (void) {;}

char
proc_mark_begin         (llong a_msec, int a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPROC      *x_proc      = NULL;
   /*---(get current)--------------------*/
   yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_DCURR, NULL, &x_proc);
   /*---(defense)------------------------*/
   --rce;  if (x_proc      == NULL)  return rce;
   --rce;  if (x_proc->beg >  0)     return rce;
   /*---(update proc)--------------------*/
   x_proc->beg     = a_msec;
   x_proc->rpid    = a_rpid;
   EOS_VERBOSE  printf       ("%6lld    proc %s begin\n", a_msec, x_proc->name);
   /*---(complete)-----------------------*/
   return 0;
}

char
proc_mark_all_in_one    (llong a_msec, int a_rpid, char a_yexec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPROC      *x_proc      = NULL;
   tGROUP     *x_group     = NULL;
   /*---(get current)--------------------*/
   yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_DCURR, NULL, &x_proc);
   /*---(defense)------------------------*/
   --rce;  if (x_proc      == NULL)  return rce;
   --rce;  if (x_proc->beg >  0)     return rce;
   /*---(update proc)--------------------*/
   x_proc->beg     = a_msec;
   x_proc->rpid    = a_rpid;
   x_proc->yexec   = a_yexec;
   x_proc->rc      = 0;
   x_proc->end     = a_msec;
   x_proc->dur     = 0;
   /*---(update group)-------------------*/
   yDLST_line_list      (NULL, &x_group);
   --rce;  if (x_group == NULL)  return rce;
   ++x_group->done;
   /*---(deactivate)---------------------*/
   yDLST_active_off ();
   EOS_VERBOSE  printf       ("%6lld    proc %s begin/end (%c)\n", a_msec, x_proc->name, x_proc->yexec);
   /*---(complete)-----------------------*/
   return 0;
}

char
proc_mark_done          (llong a_msec, char a_yexec, int a_rc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPROC      *x_proc      = NULL;
   tGROUP     *x_group     = NULL;
   /*---(get current)--------------------*/
   yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_DCURR, NULL, &x_proc);
   /*---(defense)------------------------*/
   --rce;  if (x_proc      == NULL)  return rce;
   --rce;  if (x_proc->end >  0)     return rce;
   /*---(update proc)--------------------*/
   x_proc->yexec   = a_yexec;
   x_proc->rc      = a_rc;
   x_proc->end     = a_msec;
   x_proc->dur     = x_proc->end - x_proc->beg;
   if (x_proc->dur < 0)  x_proc->dur = 0;
   /*---(remark on limits)---------------*/
   if (strchr (EOS_TYPE_LIMITS, x_proc->type) != NULL && a_yexec == 'n') {
      if (x_proc->dur < x_proc->minest)  x_proc->yexec = '<';
      if (x_proc->dur > x_proc->maxest)  x_proc->yexec = '>';
   }
   /*---(update group)-------------------*/
   yDLST_line_list      (NULL, &x_group);
   --rce;  if (x_group == NULL)  return rce;
   ++x_group->done;
   switch (a_yexec) {
   case 'n' : case 'r' : case 'A' : case '#' :             break;
   default                        :  x_group->note = '#';  break;
   }
   /*---(deactivate)---------------------*/
   yDLST_active_off ();
   EOS_VERBOSE  printf       ("%6lld    proc %s end (%c)\n", a_msec, x_proc->name, x_proc->yexec);
   /*---(complete)-----------------------*/
   return 0;
}

char
proc_mark_clear         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPROC      *x_proc      = NULL;
   tGROUP     *x_group     = NULL;
   /*---(get current)--------------------*/
   yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_DCURR, NULL, &x_proc);
   /*---(defense)------------------------*/
   --rce;  if (x_proc      == NULL)  return rce;
   /*---(update group)-------------------*/
   yDLST_line_list      (NULL, &x_group);
   --rce;  if (x_group == NULL)  return rce;
   if (x_proc->end > 0)  --x_group->done;
   /*---(update proc)--------------------*/
   x_proc->beg         =    0;
   x_proc->rpid        =   -1;
   x_proc->yexec       =  '-';
   x_proc->rc          =    0;
   x_proc->end         =    0;
   x_proc->dur         =    0;
   /*---(complete)-----------------------*/
   return 0;
}

char
proc_verblist           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        i           =    0;
   printf ("#!/sbin/eos --verblist\n");
   printf ("#  %s\n", P_ONELINE);
   printf ("#  report of verbs for use in job/conf files\n");
   printf ("\n\n");
   for (i = 0; i < MAX_VERBS; ++i) {
      if (s_verbs [i].abbr == '-')                  break;
      if (s_verbs [i].abbr == ' ') 
         printf ("\n");
      else 
         printf (" %c) %-12.12s  %-35s     %s\n", s_verbs [i].abbr,
               s_verbs [i].terse, s_verbs [i].desc, s_verbs [i].example);
   }
   printf ("\n\n");
   printf ("# end-of-file.  done, finito, completare, whimper [Ï´···\n");
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
   char        u           [LEN_RECD]  = "[]";
   int         c           =    0;
   void       *x_void      = NULL;
   tGROUP     *x_group     = NULL;
   tPROC      *x_proc      = NULL;
   char        x_ready     =  '-';
   int         x_fields    =    0;
   char        x_beg       =  '-';
   char        x_end       =  '-';
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "PROC             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_RECD, "PROC count       : %d", yDLST_line_count (YDLST_GLOBAL));
   }
   else if (strcmp (a_question, "name"    )        == 0) {
      /*> x_proc  = (tPROC  *) yDLST_line_entry (a_num, NULL);                        <*/
      yDLST_line_by_index (YDLST_GLOBAL, a_num, NULL, &x_proc);
      if (x_proc != NULL) {
         sprintf (t, "å%sæ", x_proc->name);
         snprintf (unit_answer, LEN_RECD, "PROC name   (%2d) : %2d%-20.20s  %2d  %c", a_num, strlen (x_proc->name), t, x_proc->line, x_proc->type);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC name   (%2d) :  ·åæ                    -1  ·", a_num);
      }
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      yDLST_line_by_index (YDLST_GLOBAL, a_num, NULL, &x_proc);
      yDLST_line_list     (NULL, &x_group);
      if (x_proc != NULL) {
         sprintf (s, "%2då%.15sæ", strlen (x_group->name), x_group->name);
         sprintf (t, "%2då%.15sæ", strlen (x_proc->name) , x_proc->name);
         sprintf (u, "%2då%.32sæ", strlen (x_proc->run)  , x_proc->run);
         snprintf (unit_answer, LEN_RECD, "PROC entry  (%2d) : %-19.19s  %-19.19s  %c  %s", a_num, s, t, x_proc->type, u);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC entry  (%2d) :  ·åæ                  ·åæ                 ·   ·åæ", a_num);
      }
   }
   else if (strcmp (a_question, "detail"  )        == 0) {
      yDLST_line_by_index (YDLST_GLOBAL, a_num, NULL, &x_proc);
      if (x_proc != NULL) {
         sprintf (s, "å%.10sæ", x_proc->user);
         x_beg   = (x_proc->beg > 0) ? 'y' : '-';
         x_end   = (x_proc->end > 0) ? 'y' : '-';
         snprintf (unit_answer, LEN_RECD, "PROC detail (%2d) : %c %c %c  %-12.12s  %4d  %4d  %4d %c%6d  %c  %4d  %4d %c  %4d   %c %c %c %c",
               a_num, x_proc->value, x_proc->track, x_proc->rolling, s, x_proc->uid,
               x_proc->est, x_proc->beg, x_beg,
               x_proc->rpid, x_proc->yexec, x_proc->rc,
               x_proc->end, x_end, x_proc->dur,
               x_proc->strict,  x_proc->lower, x_proc->upper, x_proc->remedy);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC detail (%2d) : · · ·  åæ               ·     ·     · ·     ·  ·     ·     · ·     ·   · · · ·", a_num);
      }
   }
   else if (strcmp (a_question, "limits"  )        == 0) {
      yDLST_line_by_index (YDLST_GLOBAL, a_num, NULL, &x_proc);
      if (x_proc != NULL) {
         snprintf (unit_answer, LEN_RECD, "PROC limits (%2d) : est %4d   %c   lower %c %7d   upper %c %7d   %c", 
               a_num, x_proc->est, x_proc->strict,
               x_proc->lower, x_proc->minest,
               x_proc->upper, x_proc->maxest,
               x_proc->remedy);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC limits (%2d) : est 1234   -   lower - 1234567   upper - 1234567   -", a_num);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


