/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



static char s_print     [LEN_RECD]  = "";

#define    MAX_VERBS       20
typedef struct cVERB tVERB;
struct cVERB {
   char        abbr;
   char        terse       [LEN_TERSE];
   char        desc        [LEN_DESC];
};
static const tVERB  s_verbs [MAX_VERBS] = {
   /*---(full execution)-------*/
   { EOS_TYPE_BOOT   , "boot"      , "only perform during actual boot"   },
   { EOS_TYPE_CONFIG , "config"    , "simple configuration update"       },
   { EOS_TYPE_EXEC   , "exec"      , "execute and wait for a process"    },
   /*---(launch only)----------*/
   { EOS_TYPE_DAEMON , "daemon"    , "launch and confirm a daemon"       },
   { EOS_TYPE_MOUNT  , "mount"     , "mount a file system"               },
   { EOS_TYPE_SERIAL , "serial"    , "launch the first in a series"      },
   /*---(job control)----------*/
   { EOS_TYPE_STOP   , "stop"      , "pause a running process"           },
   { EOS_TYPE_CONT   , "cont"      , "restart a paused process"          },
   { EOS_TYPE_RESET  , "reset"     , "reload and refresh a daemon"       },
   { EOS_TYPE_PING   , "ping"      , "ping a daemon for health"          },
   /*---(terminate)------------*/
   { EOS_TYPE_KILL   , "kill"      , "kill a running process"            },
   { EOS_TYPE_WRAPUP , "wrapup"    , "gracefully terminate a process"    },
   { EOS_TYPE_UMOUNT , "umount"    , "unmount a file system"             },
   /*---(done)-----------------*/
   { '-'             , "---"       , "---"                               },
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
   a_proc->strict      =  '-';
   a_proc->lower       =  '-';
   a_proc->upper       =  '-';
   a_proc->remedy      =  '-';
   a_proc->handoff     =  '-';
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
   ++i;  if (a_proc->strict   != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->lower    != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->upper    != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->remedy   != '-' )  s_print [i] = 'X';
   ++i;  if (a_proc->handoff  != '-' )  s_print [i] = 'X';
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

int
proc__dur               (char *a_dur)
{
   char        rce         =  -10;
   char        t           [LEN_TERSE] = "";
   int         x_len       =    0;
   char        x_unit      =  '-';
   float       x_value     =  0.0;
   --rce;  if (a_dur == NULL)  return rce;
   strlcpy (t, a_dur, LEN_TERSE);
   x_len = strlen (t);
   if (x_len > 0) {
      x_unit = t [--x_len];
      if (x_unit != 0 && strchr ("smhd", x_unit) != NULL)  t [x_len] = '\0';
      else                                                 x_unit    = '-';
   }
   x_value = atof (t);
   if (x_value < 0)  return 0;
   switch (x_unit) {
   case 's' :  x_value *= 1;             break;
   case 'm' :  x_value *= 60;            break;
   case 'h' :  x_value *= 60 * 60;       break;
   case 'd' :  x_value *= 60 * 60 * 24;  break;
   default  :  x_value *= 1;             break;
   }
   return round (x_value);
}

char
proc__flags             (tPROC *a_new, uchar *a_flags)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   char        i           =    0;
   char        j           =    0;
   char        c           =  '-';
   char        x_flags     [LEN_LABEL] = "- -   - - - -   -";
   /*---(defense)------------------------*/
   --rce;  if (a_new   == NULL)  return rce;
   --rce;  if (a_flags != NULL) {
      x_len = strlen (a_flags);
      for (j = 0; j < x_len; ++j)   x_flags [j] = a_flags [j];
   }
   /*---(importance)---------------------*/
   --rce;  c = x_flags [i++];  if (strchr ("HML-"        , c) != NULL)   a_new->value   = c;  else return rce;
   /*---(tracking)-----------------------*/
   --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;
   --rce;  c = x_flags [i++];  if (strchr ("Yy-"         , c) != NULL)   a_new->track   = c;  else return rce;
   /*---(strictness)---------------------*/
   --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;
   --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;
   --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;
   --rce;  c = x_flags [i++];  if (strchr ("Ssa-·"       , c) != NULL)   a_new->strict  = c;  else return rce;
   --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;
   /*---(minimum estimate)---------------*/
   --rce;  c = x_flags [i++];  if (strchr ("=9876hqtz-·" , c) != NULL)   a_new->lower   = c;  else return rce;
   a_new->minest = a_new->est * 1000;
   switch (a_new->lower) {
   case '=' : a_new->minest *=  1.00;      break;
   case '9' : a_new->minest *=  0.90;      break;
   case '8' : a_new->minest *=  0.80;      break;  
   case '7' : a_new->minest *=  0.70;      break;  
   case '6' : a_new->minest *=  0.60;      break;  
   case 'h' : a_new->minest *=  0.50;      break;  
   case 'q' : a_new->minest *=  0.25;      break;  
   case 't' : a_new->minest *=  0.10;      break;  
   case '·' : break;
   default  : a_new->minest *=  0.00;      break;  
   }
   if (strchr (EOS_WAIT_LONG , a_new->type) != NULL && a_new->minest < 2000) {
      a_new->minest  = 2000;
   } else if (strchr (EOS_WAIT_SHORT, a_new->type) != NULL && a_new->minest < 1000) {
      a_new->minest  = 1000;
   }
   /*---(maximum estimate)---------------*/
   --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;
   --rce;  c = x_flags [i++];  if (strchr ("=1234HDTQXZ-·", c) != NULL)   a_new->upper   = c;  else return rce;
   a_new->maxest = a_new->est * 1000;
   switch (a_new->upper) {
   case '=' : a_new->maxest *=  1.00;      break;
   case '1' : a_new->maxest *=  1.10;      break;
   case '2' : a_new->maxest *=  1.20;      break;  
   case '3' : a_new->maxest *=  1.30;      break;  
   case '4' : a_new->maxest *=  1.40;      break;  
   case 'H' : a_new->maxest *=  1.50;      break;  
   case 'D' : a_new->maxest *=  2.00;      break;  
   case 'T' : a_new->maxest *=  3.00;      break;  
   case 'Q' : a_new->maxest *=  4.00;      break;  
   case 'X' : a_new->maxest *= 10.00;      break;  
   case '·' : break;
   default  : a_new->maxest  = 9999999;    break;  
   }
   /*---(recovery)-----------------------*/
   --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;
   --rce;  c = x_flags [i++];  if (strchr ("Fkr]-"      , c) != NULL)   a_new->remedy  = c;  else return rce;
   /*---(handoff)------------------------*/
   --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;
   --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;
   --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;
   --rce;  c = x_flags [i++];  if (strchr ("kh-"        , c) != NULL)   a_new->handoff = c;  else return rce;
   /*---(complete)-----------------------*/
   return 0;
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
   uchar       x_flags     [LEN_LABEL] = "- -   - - - -   -";
   char        x_run       [LEN_FULL]   = "";
   tPASSWD    *x_pass      = NULL;
   int         x_uid       =   -1;
   tPROC      *x_new       = NULL;
   tGROUP     *x_group     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   for (i = 0; i < MAX_VERBS; ++i) {
      if (s_verbs [i].abbr == '-')                 break;
      if (strcmp (s_verbs [i].terse, a_verb) != 0)  continue;
      x_type = s_verbs [i].abbr;
   }
   EOS_VERBOSE  printf       ("  step  : %s", a_verb);
   DEBUG_INPT   yLOG_char  ("x_type"      , x_type);
   --rce;  if (x_type == '-') {
      DEBUG_INPT  yLOG_note    ("incorrect verb handler called");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   EOS_VERBOSE  printf       (", %c", x_type);
   /*---(parse fields)-------------------*/
   rc = yPARSE_ready (&c);
   DEBUG_INPT  yLOG_value   ("fields"    , c);
   --rce;  if (c < 2) {
      DEBUG_INPT  yLOG_note    ("failed, only a verb");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   EOS_VERBOSE  printf       (", %d fields", c);
   if (c > 7)  c = 7;
   switch (c) {
   case 2 : rc = yPARSE_scanf (a_verb, "F"     , x_run);  break;
   case 3 : rc = yPARSE_scanf (a_verb, "UF"    , x_user , x_run);  break;
   case 4 : rc = yPARSE_scanf (a_verb, "LUF"   , x_label, x_user, x_run);  break;
   case 5 : rc = yPARSE_scanf (a_verb, "LUTF"  , x_label, x_user, x_dur, x_run);  break;
   case 6 : rc = yPARSE_scanf (a_verb, "LDUTF" , x_label, x_desc, x_user, x_dur, x_run);  break;
   case 7 : rc = yPARSE_scanf (a_verb, "LDUTLF", x_label, x_desc, x_user, x_dur, x_flags, x_run);  break;
   }
   EOS_VERBOSE  printf       (", %d scanf", rc);
   DEBUG_INPT  yLOG_value   ("scanf"     , rc);
   --rce;  if (rc < 0) {
      EOS_VERBOSE  printf       (", failed\n");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(label)------------------------------*/
   if (strcmp (x_label, "") == 0)   sprintf (x_label, "#%03d", n);
   /*---(check uid)--------------------------*/
   if (strcmp (x_user, "") == 0) {
      strlcpy (x_user, my.who, LEN_LABEL);
      x_uid = my.uid;
   } else {
      x_pass = getpwnam (x_user);
      DEBUG_INPT   yLOG_point   ("x_pass"    , x_pass);
      --rce;  if (x_pass == NULL) {
         EOS_VERBOSE  printf       (", failed\n");
         DEBUG_INPT   yLOG_exitr (__FUNCTION__, rce);
         return rce;
      }
      x_uid  = x_pass->pw_uid;
   }
   DEBUG_INPT   yLOG_value ("uid"         , x_uid);
   /*---(check its runable)--------------*/
   rc = yEXEC_runable (x_label, x_user, x_run, YEXEC_FULL);
   DEBUG_INPT   yLOG_value   ("runnable"  , rc);
   EOS_VERBOSE  printf       (", runable %d", rc);
   --rce;  if (rc < 0) {
      EOS_VERBOSE  printf       (", failed\n");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create data)--------------------*/
   rc = proc__new (&x_new);
   DEBUG_INPT   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      EOS_VERBOSE  printf       (", failed\n");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   EOS_VERBOSE  printf       (", new");
   /*---(populate)-----------------------*/
   x_new->line = n;
   EOS_VERBOSE  printf       (", line %d", n);
   strlcpy (x_new->name, x_label, LEN_LABEL);
   EOS_VERBOSE  printf       (", %s", x_label);
   x_new->type    = x_type;
   strlcpy (x_new->desc, x_desc , LEN_DESC);
   strlcpy (x_new->user, x_user , LEN_LABEL);
   x_new->uid     = x_uid;
   x_new->est     = proc__dur (x_dur);
   proc__flags (x_new, x_flags);
   strlcpy (x_new->run , x_run  , LEN_FULL);
   /*---(create line)--------------------*/
   rc = yDLST_line_create (x_label, x_new);
   EOS_VERBOSE  printf       (", ydlst %d", rc);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      EOS_VERBOSE  printf       (", failed\n");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update list)--------------------*/
   yDLST_line_list (NULL, &x_group);
   DEBUG_INPT   yLOG_point   ("x_group"   , x_group);
   --rce;  if (x_group == NULL) {
      EOS_VERBOSE  printf       (", failed\n");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ++x_group->askd;
   EOS_VERBOSE  printf       (", success\n");
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
   yDLST_line_by_cursor ('*', '-', NULL, &x_proc);
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
   yDLST_line_by_cursor ('*', '-', NULL, &x_proc);
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
   yDLST_line_by_cursor ('*', '-', NULL, &x_proc);
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
   yDLST_line_by_cursor ('*', '-', NULL, &x_proc);
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
      snprintf (unit_answer, LEN_RECD, "PROC count       : %d", yDLST_line_count ('*'));
   }
   else if (strcmp (a_question, "name"    )        == 0) {
      /*> x_proc  = (tPROC  *) yDLST_line_entry (a_num, NULL);                        <*/
      yDLST_line_by_index ('*', a_num, NULL, &x_proc);
      if (x_proc != NULL) {
         sprintf (t, "[%s]", x_proc->name);
         snprintf (unit_answer, LEN_RECD, "PROC name   (%2d) : %2d%-20.20s  %2d  %c", a_num, strlen (x_proc->name), t, x_proc->line, x_proc->type);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC name   (%2d) :  0[]                    -1  -", a_num);
      }
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      yDLST_line_by_index ('*', a_num, NULL, &x_proc);
      yDLST_line_list     (NULL, &x_group);
      if (x_proc != NULL) {
         sprintf (s, "%2d[%.15s]", strlen (x_group->name), x_group->name);
         sprintf (t, "%2d[%.15s]", strlen (x_proc->name) , x_proc->name);
         sprintf (u, "%2d[%.32s]", strlen (x_proc->run)  , x_proc->run);
         snprintf (unit_answer, LEN_RECD, "PROC entry  (%2d) : %-19.19s  %-19.19s  %c  %s", a_num, s, t, x_proc->type, u);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC entry  (%2d) :  -[]                  -[]                 -   -[]", a_num);
      }
   }
   else if (strcmp (a_question, "detail"  )        == 0) {
      yDLST_line_by_index ('*', a_num, NULL, &x_proc);
      if (x_proc != NULL) {
         sprintf (s, "[%.10s]", x_proc->user);
         x_beg   = (x_proc->beg > 0) ? 'y' : '-';
         x_end   = (x_proc->end > 0) ? 'y' : '-';
         snprintf (unit_answer, LEN_RECD, "PROC detail (%2d) : %c %c  %-12.12s  %4d  %4d  %4d %c%6d  %c  %4d  %4d %c  %4d   %c %c %c %c   %c",
               a_num, x_proc->value, x_proc->track, s, x_proc->uid,
               x_proc->est, x_proc->beg, x_beg,
               x_proc->rpid, x_proc->yexec, x_proc->rc,
               x_proc->end, x_end, x_proc->dur,
               x_proc->strict,  x_proc->lower, x_proc->upper, x_proc->remedy,
               x_proc->handoff);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC detail (%2d) : - -  []               -     -     - -     -  -     -     - -     -   - - - -   -", a_num);
      }
   }
   else if (strcmp (a_question, "limits"  )        == 0) {
      yDLST_line_by_index ('*', a_num, NULL, &x_proc);
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


