/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



/*====================------------------------------------====================*/
/*===----                      verify results                          ----===*/
/*====================------------------------------------====================*/
static void      o___VERIFY__________________o (void) {;}

char
exec__verify_mount      (uchar *a_run)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char        x_path      [LEN_LABEL];
   FILE       *f           = NULL;
   int         c           =    0;
   char        x_recd      [LEN_RECD];
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP  yLOG_point   ("a_run"     , a_run);
   --rce;  if (a_run == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP  yLOG_info    ("a_run"     , a_run);
   /*---(parse name)---------------------*/
   DEBUG_LOOP  yLOG_info    ("->run"     , a_run);
   p = strrchr (a_run, ' ');
   DEBUG_LOOP  yLOG_point   ("p"         , p);
   --rce;  if (p == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strcpy (x_path, p + 1);
   DEBUG_LOOP  yLOG_info    ("x_path"    , x_path);
   /*---(open /proc/mounts)----------*/
   DEBUG_LOOP  yLOG_info    ("open"      , "/proc/mounts");
   c = 0;
   f = fopen ("/proc/mounts", "r");
   DEBUG_LOOP  yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check /proc/mounts)---------*/
   while (!feof (f)) {
      fgets (x_recd, 450, f);
      ++c;
      if (strstr (x_recd, x_path) != NULL) {
         DEBUG_LOOP   yLOG_note    ("found in /proc/mounts");
         fclose (f);
         if (strstr (x_recd, "(rw,") == NULL) {
            DEBUG_LOOP   yLOG_note    ("concern, not marked as rw");
         }
         DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
         return 1;
      }
   }
   DEBUG_LOOP  yLOG_value   ("reviewed"  , c);
   DEBUG_LOOP  yLOG_note    ("not found in /proc/mounts");
   fclose (f);
   /*---(open /proc/swaps)---------------*/
   DEBUG_LOOP  yLOG_info    ("open"      , "/proc/swaps");
   c = 0;
   f = fopen ("/proc/swaps", "r");
   DEBUG_LOOP  yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP  yLOG_point   ("f"         , f);
   /*---(check /proc/swaps)--------------*/
   while (!feof (f)) {
      fgets (x_recd, 450, f);
      ++c;
      if (strstr (x_recd, x_path) != NULL) {
         DEBUG_LOOP   yLOG_note    ("found in /proc/swaps");
         fclose (f);
         DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
         return 2;
      }
   }
   DEBUG_LOOP  yLOG_value   ("reviewed"  , c);
   DEBUG_LOOP  yLOG_note    ("not found in /proc/swaps");
   fclose (f);
   /*---(done)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
exec__verify_daemon     (char *a_run, int *a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char        t           [LEN_RECD];
   char        x_base      [LEN_RECD];
   int         x_rpid      =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (a_rpid != NULL)  *a_rpid = 99999;
   /*---(defense)------------------------*/
   DEBUG_LOOP  yLOG_point   ("a_run"     , a_run);
   --rce;  if (a_run == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP  yLOG_info    ("a_run"     , a_run);
   /*---(cut at first space)-------------*/
   DEBUG_LOOP  yLOG_info    ("a_run"     , a_run);
   strlcpy (t, a_run, LEN_RECD);
   p = strchr (t, ' ');
   DEBUG_LOOP  yLOG_point   ("p"         , p);
   --rce;  if (p != NULL) {
      p [0] = '\0';
   }
   DEBUG_LOOP  yLOG_info    ("t"         , t);
   /*---(parse name)---------------------*/
   p = strrchr (t, '/');
   DEBUG_LOOP  yLOG_point   ("p"         , p);
   --rce;  if (p == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strcpy (x_base, p + 1);
   DEBUG_LOOP  yLOG_info    ("x_base"    , x_base);
   /*---(look for base name)-------------*/
   rc = yEXEC_find (x_base, &x_rpid);
   DEBUG_LOOP  yLOG_value   ("find"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(look for name with path)--------*/
   if (x_rpid == 99999)    rc += yEXEC_find (t, &x_rpid);
   else                    rc += yEXEC_find (t, NULL);
   DEBUG_LOOP  yLOG_value   ("find"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   if (a_rpid != NULL)  *a_rpid = x_rpid;
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                    check running procs                       ----===*/
/*====================------------------------------------====================*/
static void      o___CHECK___________________o (void) {;}

char
exec__check_launch      (tPROC *a_proc, llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_return    =    0;
   int         x_dur       =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_point   ("a_proc"     , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_value   ("end"        , a_proc->end);
   --rce;  if (a_proc->end > 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(time checking)---------------*/
   DEBUG_LOOP   yLOG_value   ("a_msec"     , a_msec);
   if (a_msec < 0)  a_msec = 0;
   if (a_msec < a_proc->beg)  a_msec = a_proc->beg;
   /*---(update)-------------------------*/
   DEBUG_LOOP   yLOG_note    ("normal process launch checking");
   rc = yEXEC_check (a_proc->name, a_proc->rpid, &x_return);
   DEBUG_LOOP   yLOG_value   ("check"      , rc);
   if (rc != YEXEC_RUNNING) {
      DEBUG_LOOP   yLOG_note    ("not running anymore");
      proc_mark_done  (a_msec, rc, x_return);
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(check limits)-------------------*/
   DEBUG_LOOP   yLOG_char    ("strict"     , a_proc->strict);
   --rce;  if (a_proc->strict == 'S') {
      x_dur = a_msec - a_proc->beg;
      DEBUG_LOOP   yLOG_complex ("durs"       , "min %c %7d, max %c %7d, %7d", a_proc->lower, a_proc->minest, a_proc->upper, a_proc->maxest, x_dur);
      if (x_dur > a_proc->maxest && a_proc->remedy == 'k') {
         kill (a_proc->rpid, SIGTERM);
         proc_mark_done  (a_msec, ']', -9);
         DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
         return 1;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
exec__check_mount       (tPROC *a_proc, llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_check     =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_point   ("a_proc"     , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_value   ("end"        , a_proc->end);
   --rce;  if (a_proc->end > 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(time checking)---------------*/
   DEBUG_LOOP   yLOG_value   ("a_msec"     , a_msec);
   if (a_msec < 0)  a_msec = 0;
   if (a_msec < a_proc->beg)  a_msec = a_proc->beg;
   /*---(wait period)-----------------*/
   x_check = a_proc->minest + a_proc->beg;
   DEBUG_LOOP   yLOG_value   ("x_check"    , x_check);
   if (a_msec < x_check) {
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(call launch)--------------------*/
   rc = exec__check_launch (a_proc, a_msec);
   DEBUG_LOOP   yLOG_value   ("launch"     , rc);
   --rce;  if (rc <= 0) {
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(verify results)-----------------*/
   rc  = exec__verify_mount (a_proc->run);
   DEBUG_LOOP   yLOG_value   ("check"      , rc);
   if (a_proc->type == EOS_TYPE_MOUNT  && rc >  0) {
      DEBUG_LOOP   yLOG_note    ("now mounted");
      proc_mark_done  (a_msec, YEXEC_NORMAL, 0);
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 1;
   }
   if (a_proc->type == EOS_TYPE_UMOUNT && rc == 0) {
      DEBUG_LOOP   yLOG_note    ("now unmounted");
      proc_mark_done  (a_msec, YEXEC_NORMAL, 0);
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(not mounted)--------------------*/
   proc_mark_done  (a_msec, YEXEC_ERROR , 0);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 1;
}

char
exec__check_daemon      (tPROC *a_proc, llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        c           =    0;
   int         x_check     =    0;
   int         x_rpid      =    0;
   int         x_return    =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_point   ("a_proc"     , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_info    ("name"       , a_proc->name);
   DEBUG_LOOP   yLOG_value   ("end"        , a_proc->end);
   --rce;  if (a_proc->end > 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(time checking)---------------*/
   DEBUG_LOOP   yLOG_value   ("a_msec"     , a_msec);
   if (a_msec < 0)  a_msec = 0;
   if (a_msec < a_proc->beg)  a_msec = a_proc->beg;
   /*---(wait period)-----------------*/
   x_check = a_proc->minest + a_proc->beg;
   DEBUG_LOOP   yLOG_value   ("x_check"    , x_check);
   if (a_msec < x_check) {
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(clear out extras)------------*/
   c = exec__verify_daemon (a_proc->run, &x_rpid);
   DEBUG_LOOP   yLOG_complex ("checking"   , "%1d, %5d", c, x_rpid);
   while (c > 1) {
      rc = yEXEC_check (a_proc->name, x_rpid, NULL);
      DEBUG_LOOP   yLOG_char    ("exit"       , rc);
      c  = exec__verify_daemon (a_proc->run, &x_rpid);
      DEBUG_LOOP   yLOG_complex ("checking"   , "%1d, %5d", c, x_rpid);
   }
   /*---(final daemon)----------------*/
   a_proc->rpid = x_rpid;
   rc = yEXEC_check (a_proc->name, a_proc->rpid, &x_return);
   switch (rc) {
   case  YEXEC_NOSUCH :  rc = YEXEC_RUNNING;  break;
   case  YEXEC_NORMAL :  rc = YEXEC_DIED;     break;
   }
   proc_mark_done  (a_msec, rc, x_return);
   /*---(complete)--------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 1;
}

char
exec__check_signal      (tPROC *a_proc, llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        c           =    0;
   int         x_check     =    0;
   int         x_rpid      =    0;
   int         x_return    =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_point   ("a_proc"     , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_info    ("name"       , a_proc->name);
   DEBUG_LOOP   yLOG_value   ("end"        , a_proc->end);
   --rce;  if (a_proc->end > 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(time checking)---------------*/
   DEBUG_LOOP   yLOG_value   ("a_msec"     , a_msec);
   if (a_msec < 0)  a_msec = 0;
   if (a_msec < a_proc->beg)  a_msec = a_proc->beg;
   /*---(wait period)-----------------*/
   x_check = a_proc->minest + a_proc->beg;
   DEBUG_LOOP   yLOG_value   ("x_check"    , x_check);
   if (a_msec < x_check) {
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(find existing)---------------*/
   c = exec__verify_daemon (a_proc->run, &x_rpid);
   DEBUG_LOOP   yLOG_complex ("checking"   , "%1d, %5d", c, x_rpid);
   /*---(check non-kill)--------------*/
   if (strchr (EOS_TYPE_NOSTOP, a_proc->type) != NULL) {
      if (c >= 1) {
         proc_mark_done  (a_msec, YEXEC_NORMAL, 0);
         DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
         return 1;
      } else {
         rc = yEXEC_check (a_proc->name, a_proc->rpid, &x_return);
         proc_mark_done  (a_msec, YEXEC_ERROR, x_return);
         DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
         return 1;
      }
   }
   /*---(check kills)-----------------*/
   else {
      if (c == 0) {
         proc_mark_done  (a_msec, YEXEC_NORMAL, 0);
         DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
         return 1;
      } else {
         proc_mark_done  (a_msec, YEXEC_ERROR, YEXEC_ERROR);
         DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
         return 1;
      }
   }
   /*---(complete)--------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 1;
}

int
exec_check              (llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPROC      *x_proc      = NULL;
   int         n           =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(check count)--------------------*/
   n = yDLST_active_count ();
   DEBUG_LOOP   yLOG_value   ("actives"   , n);
   if (n <= 0) {
      DEBUG_LOOP  yLOG_note    ("no lines running/active, nothing to do");
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(prepare)------------------------*/
   rc = yDLST_active_by_cursor ('[', NULL, &x_proc);
   DEBUG_LOOP   yLOG_complex ("head"      , "%3d, %p, %d", rc, x_proc, c);
   /*---(walk-through active)------------*/
   while (rc >= 0 && x_proc != NULL) {
      /*---(checkers)--------------------*/
      DEBUG_LOOP   yLOG_info    ("X_PROC"    , x_proc->name);
      switch (x_proc->type) {
      case EOS_TYPE_DAEMON : case EOS_TYPE_SERIAL :
         rc = exec__check_daemon (x_proc, a_msec);
         break;
      case EOS_TYPE_MOUNT  : case EOS_TYPE_UMOUNT :
         rc = exec__check_mount  (x_proc, a_msec);
         break;
      case EOS_TYPE_EXEC   : case EOS_TYPE_CONFIG :
      case EOS_TYPE_BOOT   :
         rc = exec__check_launch (x_proc, a_msec);
         break;
      case EOS_TYPE_KILL   : case EOS_TYPE_WRAPUP :
      case EOS_TYPE_RESET  : case EOS_TYPE_PING   :
         rc  = exec__check_signal (x_proc, a_msec);
         break;
      default          :
         rc = exec__check_launch (x_proc, a_msec);
         break;
      }
      DEBUG_LOOP   yLOG_value   ("checker"   , rc);
      /*---(next)------------------------*/
      if (rc == 1)  ++c;
      rc = yDLST_active_by_cursor ('>', NULL, &x_proc);
      DEBUG_LOOP   yLOG_complex ("next"      , "%3d, %p, %d", rc, x_proc, c);
      /*---(done)------------------------*/
   }
   /*---(summary)------------------------*/
   DEBUG_LOOP   yLOG_value   ("COMPLETED" , c);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return c;
}



/*====================------------------------------------====================*/
/*===----                    update group status                       ----===*/
/*====================------------------------------------====================*/
static void      o___GROUP___________________o (void) {;}

char
exec_finish             (llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   int         c           =    0;
   int         x_done      =    0;
   int         x_count     =    0;
   char        x_warn      =  '-';
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = yDLST_list_by_cursor ('[', NULL, &x_group);
   DEBUG_LOOP   yLOG_complex ("head"      , "%3d, %p, %d", rc, x_group, c);
   /*---(walk through groups)------------*/
   while (rc >= 0 && x_group != NULL) {
      DEBUG_LOOP   yLOG_complex ("GROUP"     , "%p, %-15.15s, %c, %2dr, %2dc", x_group, x_group->name, x_group->status, x_group->askd, x_group->done);
      /*---(filter not started)----------*/
      if (x_group->status == GROUP_READY) {
         DEBUG_LOOP   yLOG_note    ("not started, continue");
      }
      /*---(filter completed)------------*/
      else if (x_group->status == GROUP_DONE) {
         DEBUG_LOOP   yLOG_note    ("done already, continue");
         ++x_done;
      }
      /*---(check for done)--------------*/
      else  if (x_group->askd == x_group->done) {
         group_mark_done (a_msec);
         ++x_done;
         ++c;
      }
      /*---(still running)---------------*/
      else {
         DEBUG_LOOP   yLOG_note  ("group still running");
         rc = yDLST_list_by_cursor ('>', NULL, &x_group);
      }
      /*---(next)------------------------*/
      rc = yDLST_list_by_cursor ('>', NULL, &x_group);
      DEBUG_LOOP   yLOG_complex ("next"      , "%3d, %p, %d", rc, x_group, c);
      /*---(done)------------------------*/
   }
   /*---(check all done)-----------------*/
   DEBUG_LOOP   yLOG_value   ("x_done"    , x_done);
   x_count = yDLST_list_count ();
   DEBUG_LOOP   yLOG_value   ("x_count"   , x_count);
   if (x_done >= x_count) {
      DEBUG_LOOP   yLOG_note  ("all groups are complete");
      my.done_done = 'y';
   }
   /*---(summary)------------------------*/
   DEBUG_LOOP   yLOG_value   ("FINISHED"  , c);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return c;
}

char
exec_start              (llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   tGROUP     *x_pred      = NULL;
   char        x_ready     =  'y';
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = yDLST_list_by_cursor ('[', NULL, &x_group);
   DEBUG_LOOP   yLOG_complex ("head"      , "%3d, %p, %d", rc, x_group, c);
   /*---(walk through groups)------------*/
   while (rc >= 0 && x_group != NULL) {
      DEBUG_LOOP   yLOG_complex ("GROUP"     , "%p, %-15.15s, %c, %2dr, %2dc", x_group, x_group->name, x_group->status, x_group->askd, x_group->done);
      /*---(filter out running)----------*/
      if (x_group->status == GROUP_RUNNING) {
         DEBUG_LOOP   yLOG_note    ("already started, continue");
      }
      /*---(filter completed)------------*/
      else if (x_group->status == GROUP_DONE) {
         DEBUG_LOOP   yLOG_note    ("done already, continue");
      }
      /*---(review for ready)------------*/
      else {
         /*---(check predecessors)-------*/
         x_ready = 'y';
         DEBUG_LOOP   yLOG_value   ("preds"     , yDLST_seq_count ('<'));
         rc = yDLST_seq_by_cursor ('<', '[', NULL, NULL, &x_pred);
         DEBUG_LOOP   yLOG_complex ("head"      , "%3d, %p, %d", rc, x_pred, c);
         while (rc >= 0 && x_pred != NULL) {
            DEBUG_LOOP   yLOG_complex ("pred"      , "%p, %-15.15s, %c", x_pred, x_pred->name, x_pred->status);
            if (x_pred->status != GROUP_DONE) {
               DEBUG_LOOP   yLOG_note  ("group still running");
               x_ready = '-';
            }
            rc = yDLST_seq_by_cursor ('<', '>', NULL, NULL, &x_pred);
            DEBUG_LOOP   yLOG_complex ("next"      , "%3d, %p, %d", rc, x_pred, c);
         }
         /*---(ready)-----------------------*/
         DEBUG_LOOP   yLOG_char    ("x_ready"   , x_ready);
         if (x_ready == 'y') {
            group_mark_begin (a_msec);
            ++c;
            rc = yDLST_focus_list ();
            if (rc < 0) {
               DEBUG_LOOP   yLOG_note  ("focus failed");
               DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
               return rce;
            }
            DEBUG_LOOP   yLOG_info    ("STARTING"  , x_group->name);
         }
      }
      /*---(next)------------------------*/
      rc = yDLST_list_by_cursor ('>', NULL, &x_group);
      DEBUG_LOOP   yLOG_complex ("next"      , "%3d, %p, %d", rc, x_group, c);
      /*---(done)------------------------*/
   }
   /*---(summary)------------------------*/
   DEBUG_LOOP   yLOG_value   ("STARTED"   , c);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return c;
}



/*====================------------------------------------====================*/
/*===----                     dispatch new procs                       ----===*/
/*====================------------------------------------====================*/
static void      o___DISPATCH________________o (void) {;}

char
exec__dispatch_launch   (tPROC *a_proc, llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_rpid      =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_point   ("a_proc"     , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_value   ("beg"        , a_proc->beg);
   --rce;  if (a_proc->beg > 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(time checking)---------------*/
   DEBUG_LOOP   yLOG_value   ("a_msec"     , a_msec);
   if (a_msec < 0)  a_msec = 0;
   /*---(run)-------------------------*/
   x_rpid = yEXEC_run (a_proc->name, a_proc->user, a_proc->run, YEXEC_DASH, YEXEC_FULL, YEXEC_FORK, my.n_exec);
   DEBUG_LOOP   yLOG_value   ("x_rpid"    , x_rpid);
   /*---(handle bad launch)-----------*/
   if (x_rpid <  0) {
      DEBUG_LOOP  yLOG_note    ("could not launch");
      proc_mark_all_in_one (a_msec, -1, YEXEC_NOTREAL);
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(update line)-----------------*/
   proc_mark_begin (a_msec, x_rpid);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 1;
}

char
exec__dispatch_mount    (tPROC *a_proc, llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_point   ("a_proc"     , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_value   ("beg"        , a_proc->beg);
   --rce;  if (a_proc->beg > 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(time checking)---------------*/
   DEBUG_LOOP   yLOG_value   ("a_msec"     , a_msec);
   if (a_msec < 0)  a_msec = 0;
   /*---(check mount)--------------*/
   rc  = exec__verify_mount (a_proc->run);
   DEBUG_LOOP   yLOG_value   ("check"      , rc);
   if (a_proc->type == EOS_TYPE_MOUNT  && rc >  0) {
      DEBUG_LOOP   yLOG_note    ("already mounted");
      proc_mark_all_in_one (a_msec, a_proc->rpid, YEXEC_ALREADY);
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 1;
   }
   if (a_proc->type == EOS_TYPE_UMOUNT && rc == 0) {
      DEBUG_LOOP   yLOG_note    ("already unmounted");
      proc_mark_all_in_one (a_msec, a_proc->rpid, YEXEC_ALREADY);
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(launch)-------------------------*/
   rc = exec__dispatch_launch (a_proc, a_msec);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
exec__dispatch_daemon   (tPROC *a_proc, llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_rpid      =   -1;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_point   ("a_proc"     , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_value   ("beg"        , a_proc->beg);
   --rce;  if (a_proc->beg > 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(time checking)---------------*/
   DEBUG_LOOP   yLOG_value   ("a_msec"     , a_msec);
   if (a_msec < 0)  a_msec = 0;
   /*---(check daemon)-------------*/
   rc  = exec__verify_daemon (a_proc->run, &a_proc->rpid);
   DEBUG_LOOP   yLOG_value   ("check"      , rc);
   if (rc > 0) {
      DEBUG_LOOP   yLOG_note    ("already running");
      proc_mark_all_in_one (a_msec, a_proc->rpid, YEXEC_ALREADY);
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(launch)-------------------------*/
   rc = exec__dispatch_launch (a_proc, a_msec);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
exec__dispatch_signal   (tPROC *a_proc, llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        c           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP   yLOG_point   ("a_proc"     , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_value   ("beg"        , a_proc->beg);
   --rce;  if (a_proc->beg > 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(time checking)---------------*/
   DEBUG_LOOP   yLOG_value   ("a_msec"     , a_msec);
   if (a_msec < 0)  a_msec = 0;
   /*---(find it)---------------------*/
   c = exec__verify_daemon (a_proc->run, &a_proc->rpid);
   if (c <= 0) {
      proc_mark_all_in_one (a_msec, a_proc->rpid, YEXEC_NOSUCH);
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(send signal)-----------------*/
   --rce;  switch (a_proc->type) {
   case EOS_TYPE_KILL   :  kill (a_proc->rpid, SIGKILL);   break;
   case EOS_TYPE_WRAPUP :  kill (a_proc->rpid, SIGTERM);   break;
   case EOS_TYPE_RESET  :  kill (a_proc->rpid, SIGHUP );   break;
   case EOS_TYPE_PING   :  kill (a_proc->rpid, SIGUSR2);   break;
   case EOS_TYPE_STOP   :  kill (a_proc->rpid, SIGTSTP);   break;
   case EOS_TYPE_CONT   :  kill (a_proc->rpid, SIGCONT);   break;
   default :
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   proc_mark_begin (a_msec, a_proc->rpid);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return 1;
}

char
exec_dispatch           (llong a_msec)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   tPROC      *x_proc      = NULL;
   int         x_rpid      =    0;
   int         n           =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(check count)--------------------*/
   n = yDLST_focus_count ();
   DEBUG_LOOP   yLOG_value   ("focused"   , n);
   if (n <= 0) {
      DEBUG_LOOP  yLOG_note    ("no lines focused, nothing to do");
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(prepare)------------------------*/
   rc = yDLST_focus_by_cursor ('[', NULL, &x_proc);
   DEBUG_LOOP   yLOG_complex ("head"      , "%3d, %p, %d", rc, x_proc, c);
   /*---(walk-through focus)-------------*/
   while (rc >= 0 && x_proc != NULL) {
      DEBUG_LOOP   yLOG_complex ("PROC"      , "%p, %-15.15s, %6db, %6de", x_proc, x_proc->name, x_proc->beg, x_proc->end);
      /*---(filter completed)------------*/
      if (x_proc->end  > 0) {
         DEBUG_LOOP   yLOG_note  ("already competed");
      }
      /*---(filter running)--------------*/
      else if (x_proc->beg     > 0) {
         DEBUG_LOOP   yLOG_note  ("already running");
      }
      /*---(activate)--------------------*/
      else {
         /*---(update)-------------------*/
         yDLST_active_on ();
         yDLST_focus_off ();
         ++c;
         /*---(dispatch handlers)--------*/
         switch (x_proc->type) {
         case EOS_TYPE_DAEMON : case EOS_TYPE_SERIAL :
            rc  = exec__dispatch_daemon (x_proc, a_msec);
            break;
         case EOS_TYPE_MOUNT  : case EOS_TYPE_UMOUNT :
            rc  = exec__dispatch_mount  (x_proc, a_msec);
            break;
         case EOS_TYPE_BOOT   :
            if (my.run_mode == EOS_RUN_NORMAL)  rc  = exec__dispatch_launch (x_proc, a_msec);
            else                                 proc_mark_all_in_one (a_msec, 0, YEXEC_ALREADY);
            break;
         case EOS_TYPE_EXEC   : case EOS_TYPE_CONFIG : default :
            rc  = exec__dispatch_launch (x_proc, a_msec);
            break;
         case EOS_TYPE_KILL   : case EOS_TYPE_WRAPUP :
         case EOS_TYPE_RESET  : case EOS_TYPE_PING   :
            rc  = exec__dispatch_signal (x_proc, a_msec);
         }
      }
      /*---(next)------------------------*/
      rc = yDLST_focus_by_cursor ('>', NULL, &x_proc);
      DEBUG_LOOP   yLOG_complex ("next"      , "%3d, %p, %d", rc, x_proc, c);
      /*---(done)------------------------*/
   }
   /*---(summary)------------------------*/
   DEBUG_LOOP   yLOG_value   ("DISPATCHED", c);
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return c;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

int
exec__signal            (char *a_recd)
{
   char        t           [LEN_RECD]  = "";
   FILE       *f           = NULL;
   int         c           =    0;
   int         x_len       =    0;
   strcpy (a_recd, "");
   f = fopen ("/run/signal.txt", "rt");
   if (f == NULL)  return 0;
   while (1) {
      fgets (t, LEN_RECD, f);
      if (feof (f))  break;
      ++c;
   }
   x_len = strlen (a_recd);
   if (x_len > 0 && a_recd [x_len - 1] == '\n')  a_recd [--x_len] = '\0';
   fclose (f);
   return c;
}

char*        /*-> tbd --------------------------------[ light  [us.JC0.271.X1]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
exec__unit              (char *a_question)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        t           [LEN_RECD]  = "";
   int         c           =    0;
   /*---(defense)------------------------*/
   snprintf (unit_answer, LEN_RECD, "EXEC unit        : question unknown");
   /*---(simple)-------------------------*/
   if      (strcmp (a_question, "done"      )     == 0) {
      snprintf (unit_answer, LEN_RECD, "EXEC done        : %c", my.done_done);
   }
   else if (strcmp (a_question, "signal"    )     == 0) {
      c = exec__signal (t);
      snprintf (unit_answer, LEN_RECD, "EXEC done        : %2d [%s]", c, t);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



/*============================[[ end-of-code ]]===============================*/
