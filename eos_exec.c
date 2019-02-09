/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



/*> int         requested   = 0;                                                      <* 
 *> int         running     = 0;                                                      <* 
 *> int         complete    = 0;                                                      <*/


/*====================------------------------------------====================*/
/*===----                    specialty checks                          ----===*/
/*====================------------------------------------====================*/
static void      o___CHECKERS________________o (void) {;}

char
exec_check_daemon       (tPROC *a_proc, int *a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char        x_base      [LEN_NAME];
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_LOOP  yLOG_point   ("a_proc"    , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP  yLOG_info    ("->name"    , a_proc->name);
   /*---(parse name)---------------------*/
   DEBUG_LOOP  yLOG_info    ("->run"     , a_proc->run);
   p = strrchr (a_proc->run, '/');
   DEBUG_LOOP  yLOG_point   ("p"         , p);
   --rce;  if (p == NULL) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strcpy (x_base, p + 1);
   DEBUG_LOOP  yLOG_info    ("x_base"    , x_base);
   /*---(look for daemon)----------------*/
   rc = yEXEC_find (x_base, a_rpid);
   DEBUG_LOOP  yLOG_value   ("find"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
exec_check_mount        (tPROC *a_proc)
{
}

char
exec_check_serial       (tPROC *a_proc)
{
}


/*====================------------------------------------====================*/
/*===----                           processes                          ----===*/
/*====================------------------------------------====================*/
static void      o___PROCESSES_______________o (void) {;}

int
exec_check              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   tPROC      *x_proc      = NULL;
   int         x_return    =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(check count)--------------------*/
   DEBUG_LOOP   yLOG_value   ("actives"   , yDLST_active_count ());
   if (yDLST_active_count () <= 0) {
      DEBUG_LOOP  yLOG_note    ("no lines running/active, nothing to do");
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check all files)----------------*/
   for (x_proc = yDLST_active_seek (YDLST_HEAD); x_proc != NULL; x_proc = yDLST_active_seek (YDLST_NEXT)) {
      /*---(header)----------------------*/
      DEBUG_LOOP   yLOG_info    ("x_proc"    , x_proc->name);
      /*---(get file)--------------------*/
      x_group = (tGROUP *) yDLST_line_list ();
      DEBUG_LOOP   yLOG_point   ("x_group"    , x_group);
      if (x_group == NULL) {
         DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_LOOP   yLOG_info    ("->name"    , x_group->name);
      /*---(specialty checks)------------*/
      switch (x_proc->type) {
      case TYPE_LAUNCH :
         rc       = 'n';
         x_return = 0;
         break;
      case TYPE_DAEMON :
      case TYPE_SERIAL :
         DEBUG_LOOP   yLOG_note    ("daemon/serial existance checking");
         /*---(check normal)-------------*/
         rc  = exec_check_daemon (x_proc, &x_proc->rpid);
         DEBUG_LOOP   yLOG_value   ("check"      , rc);
         if (rc >  0) {
            DEBUG_LOOP   yLOG_note    ("already running");
            ++x_group->completed;
            x_proc->yexec = YEXEC_NORMAL;
            x_proc->rc    = 0;
            x_proc->end   = time (NULL);
            continue;
         }
         /*---(check if died)------------*/
         rc = yEXEC_check (x_proc->name, x_proc->rpid, &x_return);
         DEBUG_LOOP   yLOG_value   ("check"     , rc);
         break;
      case TYPE_MOUNT  :
         rc       = 'n';
         x_return = 0;
         break;
      case TYPE_CONFIG :
      case TYPE_BOOT   :
      default          :
         rc = yEXEC_check (x_proc->name, x_proc->rpid, &x_return);
         DEBUG_LOOP   yLOG_value   ("check"     , rc);
         if (rc == YEXEC_RUNNING) {
            DEBUG_LOOP   yLOG_note    ("still running, next");
            continue;
         }
         break;
      }
      /*---(update)----------------------*/
      ++x_group->completed;
      x_proc->yexec = rc;
      x_proc->rc    = x_return;
      x_proc->end   = time (NULL);
      /*---(reset lists)-----------------*/
      yDLST_active_off ();
      ++c;
      DEBUG_LOOP  yLOG_note    ("collected, next");
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return c;
}

char
exec_finish             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   int         c           =    0;
   int         x_done      =    0;
   int         x_count     =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(check all groups)---------------*/
   for (x_group = yDLST_list_seek (YDLST_HEAD); x_group != NULL; x_group = yDLST_list_seek (YDLST_NEXT)) {
      /*---(header)----------------------*/
      DEBUG_LOOP   yLOG_point   ("x_group"   , x_group);
      DEBUG_LOOP   yLOG_info    ("name"      , x_group->name);
      DEBUG_LOOP   yLOG_char    ("status"    , x_group->status);
      /*---(check for handled)-----------*/
      if (x_group->status == GROUP_READY) {
         DEBUG_LOOP   yLOG_note    ("not started, continue");
         continue;
      }
      if (x_group->status == GROUP_DONE) {
         DEBUG_LOOP   yLOG_note    ("done already, continue");
         ++x_done;
         continue;
      }
      /*---(show stats)------------------*/
      DEBUG_LOOP   yLOG_value   ("requested" , x_group->requested);
      DEBUG_LOOP   yLOG_value   ("completed" , x_group->completed);
      /*---(check for done)--------------*/
      if (x_group->requested == x_group->completed) {
         DEBUG_LOOP   yLOG_note  ("mark completed");
         x_group->status = GROUP_DONE;
         ++c;
         continue;
      }
      /*---(still running)---------------*/
      DEBUG_LOOP   yLOG_note  ("group still running");
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
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return c;
}

char
exec_start              (void)
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
   /*---(check all files)----------------*/
   for (x_group = yDLST_list_seek (YDLST_HEAD); x_group != NULL; x_group = yDLST_list_seek (YDLST_NEXT)) {
      /*---(header)----------------------*/
      DEBUG_LOOP   yLOG_point   ("x_group"   , x_group);
      DEBUG_LOOP   yLOG_info    ("name"      , x_group->name);
      DEBUG_LOOP   yLOG_char    ("status"    , x_group->status);
      /*---(check for handled)-----------*/
      if (x_group->status == GROUP_RUNNING) {
         DEBUG_LOOP   yLOG_note    ("already started, continue");
         continue;
      }
      if (x_group->status == GROUP_DONE) {
         DEBUG_LOOP   yLOG_note    ("done already, continue");
         continue;
      }
      /*---(check preds)-----------------*/
      x_ready = 'y';
      DEBUG_LOOP   yLOG_value   ("preds"     , yDLST_pred_count ());
      for (x_pred = yDLST_pred_seek (YDLST_HEAD); x_pred != NULL; x_pred = yDLST_pred_seek (YDLST_NEXT)) {
         DEBUG_LOOP   yLOG_point   ("x_pred"    , x_pred);
         DEBUG_LOOP   yLOG_info    ("name"      , x_pred->name);
         DEBUG_LOOP   yLOG_char    ("status"    , x_pred->status);
         if (x_pred->status != GROUP_DONE) {
            DEBUG_LOOP   yLOG_note  ("already handled");
            x_ready = '-';
            break;
         }
      }
      DEBUG_LOOP   yLOG_char    ("x_ready"   , x_ready);
      /*---(if not ready-----------------*/
      if (x_ready != 'y') {
         DEBUG_LOOP   yLOG_note  ("not ready to launch");
         continue;
      }
      /*---(ready)-----------------------*/
      x_group->status = GROUP_RUNNING;
      ++c;
      rc = yDLST_focus_list ();
      if (rc < 0) {
         DEBUG_LOOP   yLOG_note  ("focus failed");
         DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return c;
}

char
exec_dispatch           (int a_min)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tGROUP     *x_group     = NULL;
   tPROC      *x_proc      = NULL;
   int         x_rpid      =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP  yLOG_enter   (__FUNCTION__);
   /*---(check count)--------------------*/
   DEBUG_LOOP   yLOG_value   ("focused"   , yDLST_focus_count ());
   if (yDLST_focus_count () <= 0) {
      DEBUG_LOOP  yLOG_note    ("no lines focused, nothing to do");
      DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check all files)----------------*/
   for (x_proc = yDLST_focus_seek (YDLST_HEAD); x_proc != NULL; x_proc = yDLST_focus_seek (YDLST_NEXT)) {
      DEBUG_LOOP   yLOG_info    ("x_proc"    , x_proc->name);
      /*---(test)------------------------*/
      if (x_proc->end  > 0) {
         DEBUG_LOOP   yLOG_note  ("already competed");
         continue;
      }
      if (x_proc->beg  > 0) {
         DEBUG_LOOP   yLOG_note  ("already running");
         continue;
      }
      /*---(activate)--------------------*/
      yDLST_active_on ();
      yDLST_focus_off ();
      ++c;
      /*---(get group)-------------------*/
      x_group = (tGROUP *) yDLST_line_list ();
      DEBUG_LOOP   yLOG_point   ("x_group"    , x_group);
      if (x_group == NULL) {
         DEBUG_LOOP  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_LOOP   yLOG_info    ("->name"    , x_group->name);
      /*---(prerun checking)-------------*/
      switch (x_proc->type) {
      case TYPE_DAEMON :
      case TYPE_SERIAL :
         DEBUG_LOOP   yLOG_note    ("daemon/serial existance pre-checking");
         rc  = exec_check_daemon (x_proc, &x_proc->rpid);
         DEBUG_LOOP   yLOG_value   ("check"      , rc);
         if (rc > 0) {
            ++x_group->completed;
            x_proc->beg     = time (NULL);
            x_proc->yexec   = YEXEC_ALREADY;
            x_proc->rc      = 0;
            x_proc->end     = time (NULL);
            yDLST_active_off ();
            continue;
         }
         break;
      }
      /*---(run)-------------------------*/
      DEBUG_LOOP   yLOG_info    ("->run"     , x_proc->run);
      x_rpid = yEXEC_run (x_proc->name, x_proc->user, x_proc->run, YEXEC_DASH, YEXEC_FULL, YEXEC_FORK, my.name_exec);
      DEBUG_LOOP   yLOG_value   ("x_rpid"    , x_rpid);
      if (x_rpid <  0) {
         DEBUG_LOOP  yLOG_note    ("could not launch");
         ++x_group->completed;
         x_proc->beg     = time (NULL);
         x_proc->rpid    = -2;
         x_proc->yexec   = YEXEC_ERROR;
         x_proc->rc      = -2;
         x_proc->end     = time (NULL);
         yDLST_active_off ();
         continue;
      }
      /*---(update line)-----------------*/
      x_proc->beg     = time (NULL);
      x_proc->rpid    = x_rpid;
      DEBUG_LOOP  yLOG_note    ("launched, move to next");
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_LOOP  yLOG_exit    (__FUNCTION__);
   return c;
}



/*> char                                                                               <* 
 *> EXEC_checkmount    (char *a_mount)                                                 <* 
 *> {                                                                                  <* 
 *>    DEBUG_LOOP   yLOG_senter  (__FUNCTION__);                                       <* 
 *>    /+---(locals)-----------+-----------+-+/                                        <* 
 *>    char        rce         = -10;      /+ return code for errors              +/   <* 
 *>    int         rc          = 0;        /+ generic return code                 +/   <* 
 *>    FILE       *f           = NULL;     /+ generic file pointer                +/   <* 
 *>    int         x_len       = 0;        /+ length of mount name                +/   <* 
 *>    char        x_recd      [1000];     /+ proc table entry                    +/   <* 
 *>    /+---(defenses)-----------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_spoint  (a_mount);                                            <* 
 *>    --rce;  if (a_mount == NULL) {                                                  <* 
 *>       DEBUG_LOOP   yLOG_snote   ("pointer null, failed");                          <* 
 *>       DEBUG_LOOP   yLOG_sexit   (__FUNCTION__);                                    <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    /+---(prepare)-----------------------+/                                         <* 
 *>    DEBUG_LOOP   yLOG_snote   (a_mount);                                            <* 
 *>    x_len = strllen (a_mount, LEN_NAME);                                            <* 
 *>    DEBUG_LOOP   yLOG_svalue  ("len"       , x_len);                                <* 
 *>    --rce;  if (x_len <= 0) {                                                       <* 
 *>       DEBUG_LOOP   yLOG_snote   ("name too short, failed");                        <* 
 *>       DEBUG_LOOP   yLOG_sexit   (__FUNCTION__);                                    <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    /+---(check /proc/mounts)---------+/                                            <* 
 *>    f = fopen ("/proc/mounts", "r");                                                <* 
 *>    while (!feof (f)) {                                                             <* 
 *>       fgets (x_recd, 450, f);                                                      <* 
 *>       if (strncmp (x_recd, a_mount, x_len) == 0) {                                 <* 
 *>          DEBUG_LOOP   yLOG_snote   ("found in /proc/mounts");                      <* 
 *>          fclose (f);                                                               <* 
 *>          DEBUG_LOOP   yLOG_sexit   (__FUNCTION__);                                 <* 
 *>          return 1;                                                                 <* 
 *>       }                                                                            <* 
 *>    }                                                                               <* 
 *>    fclose (f);                                                                     <* 
 *>    /+---(check /proc/swaps)----------+/                                            <* 
 *>    f = fopen ("/proc/swaps", "r");                                                 <* 
 *>    while (!feof (f)) {                                                             <* 
 *>       fgets (x_recd, 450, f);                                                      <* 
 *>       if (strncmp (x_recd, a_mount, x_len) == 0) {                                 <* 
 *>          DEBUG_LOOP   yLOG_snote   ("found in /proc/swaps");                       <* 
 *>          fclose (f);                                                               <* 
 *>          DEBUG_LOOP   yLOG_sexit   (__FUNCTION__);                                 <* 
 *>          return 2;                                                                 <* 
 *>       }                                                                            <* 
 *>    }                                                                               <* 
 *>    fclose (f);                                                                     <* 
 *>    /+---(done)-----------------------+/                                            <* 
 *>    DEBUG_LOOP   yLOG_sexit   (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char                                                                               <* 
 *> EXEC_launch        (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-----------+-------------+/                                        <* 
 *>    tPROC      *x_proc      = NULL;          /+ current process                +/   <* 
 *>    tPROC      *x_pred      = NULL;          /+ predecessors                   +/   <* 
 *>    char        x_ready     = 'y';                                                  <* 
 *>    int         i           = 0;                                                    <* 
 *>    /+---(begin)--------------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    /+---(find)---------------------------+/                                        <* 
 *>    x_proc = (tPROC*) yDLST_focus (HEAD);                                           <* 
 *>    while (x_proc != NULL) {                                                        <* 
 *>       DEBUG_LOOP   yLOG_info    ("checking"  , x_proc->name);                      <* 
 *>       /+---(recursors)------------------+/                                         <* 
 *>       DEBUG_LOOPM  yLOG_enter   ("precursors");                                    <* 
 *>       x_ready = 'y';                                                               <* 
 *>       x_pred  = yDLST_ups   (x_proc->dlst, HEAD);                                  <* 
 *>       while (x_pred != NULL) {                                                     <* 
 *>          if  (x_pred->status != 'c') {                                             <* 
 *>             DEBUG_LOOPM  yLOG_info    ("waiting on", x_pred->name);                <* 
 *>             x_ready = 'n';                                                         <* 
 *>          } else {                                                                  <* 
 *>             DEBUG_LOOPM  yLOG_info    ("done"      , x_pred->name);                <* 
 *>          }                                                                         <* 
 *>          x_pred = yDLST_ups   (x_proc->dlst, NEXT);                                <* 
 *>       }                                                                            <* 
 *>       if (x_ready == 'y') {                                                        <* 
 *>          DEBUG_LOOPM  yLOG_note    ("ready to run");                               <* 
 *>       } else {                                                                     <* 
 *>          DEBUG_LOOPM  yLOG_note    ("must wait for precursors to finish");         <* 
 *>       }                                                                            <* 
 *>       DEBUG_LOOPM  yLOG_exit    ("precursors");                                    <* 
 *>       /+---(handle)----------------------+/                                        <* 
 *>       if (x_ready == 'y')   EXEC_run     (x_proc);                                 <* 
 *>       /+---(find next)-------------------+/                                        <* 
 *>       ++i;                                                                         <* 
 *>       x_proc = (tPROC*) yDLST_focus (NEXT);                                        <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return -1;                                                                      <* 
 *> }                                                                                  <*/

/*> char       /+-#<[: locate a process dlst based on pid ------------------------+/   <* 
 *> EXEC_find          (int a_rpid)                                                    <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-------*-----------------+/                                        <* 
 *>    tPROC    *xdata     = NULL;                                                     <* 
 *>    /+---(find)---------------------------+/                                        <* 
 *>    xdata = (tPROC*) yDLST_links (HEAD);                                            <* 
 *>    while (xdata != NULL) {                                                         <* 
 *>       if (xdata->rpid == a_rpid)  return xdata->dlst;                              <* 
 *>       xdata = yDLST_links (NEXT);                                                  <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return -1;                                                                      <* 
 *> }                                                                                  <*/

/*> char       /+----: execute a milestone ---------------------------------------+/   <* 
 *> EXEC_milesone      (tPROC *a_proc)                                                 <* 
 *> {                                                                                  <* 
 *>    /+---(defenses)-----------------------+/                                        <* 
 *>    if (a_proc       == NULL)  return -1;                                           <* 
 *>    if (a_proc->type != '-' )  return  0;                                           <* 
 *>    /+---(header)-------------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_senter  (__FUNCTION__);                                       <* 
 *>    DEBUG_LOOP   yLOG_snote   (a_proc->name);                                       <* 
 *>    /+---(mark complete)------------------+/                                        <* 
 *>    a_proc->status = 'c';                                                           <* 
 *>    yDLST_focus_off (a_proc->dlst);                                                 <* 
 *>    DEBUG_LOOP   yLOG_snote   ("completed");                                        <* 
 *>    DEBUG_LOOP   yLOG_sexit   (__FUNCTION__);                                       <* 
 *>    /+---(test children)------------------+/                                        <* 
 *>    EXEC_children (a_proc->dlst);                                                   <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 1;                                                                       <* 
 *> }                                                                                  <*/

/*> char       /+----: ckeck for boot only ---------------------------------------+/   <* 
 *> EXEC_test          (tPROC *a_proc)                                                 <* 
 *> {                                                                                  <* 
 *>    /+---(defenses)-----------------------+/                                        <* 
 *>    if (a_proc       == NULL)  return -1;                                           <* 
 *>    if (a_proc->type != 'b')   return  0;                                           <* 
 *>    if (my.test      != 'y')   return  0;                                           <* 
 *>    /+---(header)-------------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_senter  (__FUNCTION__);                                       <* 
 *>    DEBUG_LOOP   yLOG_snote   (a_proc->name);                                       <* 
 *>    /+---(mark complete)------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_snote   ("not boot, don't run");                              <* 
 *>    yDLST_focus_off  (a_proc->dlst);                                                <* 
 *>    yDLST_active_on  (a_proc->dlst);                                                <* 
 *>    a_proc->rpid       = 0;                                                         <* 
 *>    a_proc->status     = 'r';                                                       <* 
 *>    ++running;                                                                      <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_sexit   (__FUNCTION__);                                       <* 
 *>    return 1;                                                                       <* 
 *> }                                                                                  <*/

/*> char       /+----: ckeck for serial control ----------------------------------+/   <* 
 *> EXEC_serial        (tPROC *a_proc)                                                 <* 
 *> {                                                                                  <* 
 *>    /+---(defenses)-----------------------+/                                        <* 
 *>    if (a_proc       == NULL)  return -1;                                           <* 
 *>    if (a_proc->type != 's')   return  0;                                           <* 
 *>    if (my.pid       ==  1 )   return  0;                                           <* 
 *>    /+---(header)-------------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_senter  (__FUNCTION__);                                       <* 
 *>    DEBUG_LOOP   yLOG_snote   (a_proc->name);                                       <* 
 *>    /+---(mark complete)------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_info   ("serial"    , "not boot, don't run");                 <* 
 *>    a_proc->status = 'c';                                                           <* 
 *>    yDLST_focus_off  (a_proc->dlst);                                                <* 
 *>    yDLST_active_off (a_proc->dlst);                                                <* 
 *>    ++complete;                                                                     <* 
 *>    DEBUG_LOOP   yLOG_sexit   (__FUNCTION__);                                       <* 
 *>    /+---(test children)------------------+/                                        <* 
 *>    EXEC_children (a_proc->dlst);                                                   <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 1;                                                                       <* 
 *> }                                                                                  <*/

/*> char       /+----: ckeck for mounting ----------------------------------------+/   <* 
 *> EXEC_mount         (tPROC *a_proc)                                                 <* 
 *> {                                                                                  <* 
 *>    FILE       *f           = NULL;                                                 <* 
 *>    /+---(defenses)-----------------------+/                                        <* 
 *>    if (a_proc       == NULL)  return -1;                                           <* 
 *>    if (a_proc->type != 'm')   return  0;                                           <* 
 *>    /+---(prepare)--------------------+/                                            <* 
 *>    strcpy (a_proc->result, "not found");                                           <* 
 *>    sprintf (check, "%s ", a_proc->check);                                          <* 
 *>    len = strlen (check);                                                           <* 
 *>    /+---(check /proc/mounts)---------+/                                            <* 
 *>    f = fopen ("/proc/mounts", "r");                                                <* 
 *>    while (!feof (f)) {                                                             <* 
 *>       fgets (x_recd, 450, f);                                                      <* 
 *>       if (strncmp (x_recd, check, len) == 0) {                                     <* 
 *>          DEBUG_LOOP   yLOG_info   ("check"     , "found in /proc/mounts");         <* 
 *>          a_proc->status = 'c';                                                     <* 
 *>          yDLST_focus_off  (a_proc->dlst);                                          <* 
 *>          yDLST_active_off (a_proc->dlst);                                          <* 
 *>          ++complete;                                                               <* 
 *>          DEBUG_LOOP   yLOG_info   ("complete"  , "mark done");                     <* 
 *>          EXEC_children (a_proc->dlst);                                             <* 
 *>          strcpy (a_proc->result, "mounted");                                       <* 
 *>          DEBUG_LOOP   yLOG_exit   (__FUNCTION__);                                  <* 
 *>          return 1;                                                                 <* 
 *>       }                                                                            <* 
 *>    }                                                                               <* 
 *>    fclose (f);                                                                     <* 
 *>    /+---(check /proc/swaps)----------+/                                            <* 
 *>    f = fopen ("/proc/swaps", "r");                                                 <* 
 *>    while (!feof (f)) {                                                             <* 
 *>       fgets (x_recd, 450, f);                                                      <* 
 *>       if (strncmp (x_recd, check, strlen (check)) == 0) {                          <* 
 *>          DEBUG_LOOP   yLOG_info   ("check"     , "found in /proc/swaps");          <* 
 *>          a_proc->status = 'c';                                                     <* 
 *>          yDLST_focus_off  (a_proc->dlst);                                          <* 
 *>          yDLST_active_off (a_proc->dlst);                                          <* 
 *>          ++complete;                                                               <* 
 *>          DEBUG_LOOP   yLOG_info   ("complete"  , "mark done");                     <* 
 *>          EXEC_children (a_proc->dlst);                                             <* 
 *>          strcpy (a_proc->result, "mounted");                                       <* 
 *>          DEBUG_LOOP   yLOG_exit   (__FUNCTION__);                                  <* 
 *>          return 1;                                                                 <* 
 *>       }                                                                            <* 
 *>    }                                                                               <* 
 *>    fclose (f);                                                                     <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_sexit   (__FUNCTION__);                                       <* 
 *>    return 1;                                                                       <* 
 *> }                                                                                  <*/

/*> char       /+----: execute a particular process entry ------------------------+/                                                                <* 
 *> EXEC_run           (tPROC *a_proc)                                                                                                              <* 
 *> {                                                                                                                                               <* 
 *>    DEBUG_LOOP   yLOG_enter  (__FUNCTION__);                                                                                                     <* 
 *>    /+---(locals)--------------------------------+/                                                                                              <* 
 *>    int       rc        = 0;                       /+ simple return code       +/                                                                <* 
 *>    int       rpid      = 0;                       /+ child pid for execution  +/                                                                <* 
 *>    tPASSWD  *pass;                                                                                                                              <* 
 *>    FILE     *output = NULL;                                                                                                                     <* 
 *>    long      now;                                 /+ present datetime         +/                                                                <* 
 *>    tTIME    *curr_time = NULL;                                                                                                                  <* 
 *>    char      msg[200];                                                                                                                          <* 
 *>    char      envp[10][200];                                                                                                                     <* 
 *>    sigset_t  new_mask, old_mask;                                                                                                                <* 
 *>    char      backup[LEN_CMD]    = "";          /+ copy of the command for parsing +/                                                            <* 
 *>    char     *p[20];                        /+ pointers to cli arguments       +/                                                                <* 
 *>    int       i         = 0;                /+ loop iterator -- arguement      +/                                                                <* 
 *>    char        name        [100];                                                                                                               <* 
 *>    char        check       [200];                                                                                                               <* 
 *>    char        run         [200];                                                                                                               <* 
 *>    char        x_recd      [1000];                                                                                                              <* 
 *>    int       len;                                                                                                                               <* 
 *>    tDIRENT    *den;                        /+ daemon checking                 +/                                                                <* 
 *>    DIR        *dir;                        /+ daemon checking                 +/                        /+ daemon checking                 +/   <* 
 *>    char       *q;                                                                                                                               <* 
 *>    FILE       *f           = NULL;                                                                                                              <* 
 *>    /+---(prepare)------------------------+/                                                                                                     <* 
 *>    a_proc->rpid   = 0;                                                                                                                          <* 
 *>    DEBUG_LOOP   yLOG_info   ("name"      , a_proc->name);                                                                                       <* 
 *>    DEBUG_LOOP   yLOG_char   ("type"      , a_proc->type);                                                                                       <* 
 *>    strcpy (a_proc->result, "no check");                                                                                                         <* 
 *>    /+---(check for milestones)-----------+/                                                                                                     <* 
 *>    rc = EXEC_milesone (a_proc);                                                                                                                 <* 
 *>    if (rc > 0) {                                                                                                                                <* 
 *>       DEBUG_LOOP   yLOG_exit   (__FUNCTION__);                                                                                                  <* 
 *>       return 0;                                                                                                                                 <* 
 *>    }                                                                                                                                            <* 
 *>    /+---(check for boot only)------------+/                                                                                                     <* 
 *>    rc = EXEC_test     (a_proc);                                                                                                                 <* 
 *>    if (rc > 0) {                                                                                                                                <* 
 *>       DEBUG_LOOP   yLOG_exit   (__FUNCTION__);                                                                                                  <* 
 *>       return 1;                                                                                                                                 <* 
 *>    }                                                                                                                                            <* 
 *>    /+---(check for serial)---------------+/                                                                                                     <* 
 *>    rc = EXEC_serial   (a_proc);                                                                                                                 <* 
 *>    if (rc > 0) {                                                                                                                                <* 
 *>       DEBUG_LOOP   yLOG_exit   (__FUNCTION__);                                                                                                  <* 
 *>       return 1;                                                                                                                                 <* 
 *>    }                                                                                                                                            <* 
 *>    /+---(execute)------------------------+/                                                                                                     <* 
 *>    DEBUG_LOOP   yLOG_info   ("user"      , a_proc->user);                                                                                       <* 
 *>    DEBUG_LOOP   yLOG_info   ("command"   , a_proc->run);                                                                                        <* 
 *>    printf   ("%-8.8s : %-16.16s : %c :  %-70.70s\n", a_proc->user, a_proc->name, a_proc->type, a_proc->run);                                    <* 
 *>    snprintf (msg, 200, "%-16.16s", a_proc->name);                                                                                               <* 
 *>    rpid = yEXEC_run (my.name_exec, msg, a_proc->user, a_proc->run, SHELL, PATH, yEXEC_FORK);                                                    <* 
 *>    /+---(failed)-------------------------+/                                                                                                     <* 
 *>    if (rpid <  0) {                                                                                                                             <* 
 *>       a_proc->rpid       = 0;                                                                                                                   <* 
 *>       DEBUG_LOOP   yLOG_exit   (__FUNCTION__);                                                                                                  <* 
 *>       return -1;                                                                                                                                <* 
 *>    }                                                                                                                                            <* 
 *>    /+---(success)------------------------+/                                                                                                     <* 
 *>    DEBUG_LOOP   yLOG_value  ("forked"    , rpid);                                                                                               <* 
 *>    a_proc->rpid       = rpid;                                                                                                                   <* 
 *>    yDLST_focus_off (a_proc->dlst);                                                                                                              <* 
 *>    if (strchr("abcm", a_proc->type) != 0) {                                                                                                     <* 
 *>       DEBUG_LOOP   yLOG_note    ("add to running list");                                                                                        <* 
 *>       a_proc->status = 'r';                                                                                                                     <* 
 *>       yDLST_active_on (a_proc->dlst);                                                                                                           <* 
 *>       ++running;                                                                                                                                <* 
 *>    } else { /+ daemon and serial +/                                                                                                             <* 
*>       DEBUG_LOOP   yLOG_note    ("add to complete list");                                                                                       <* 
*>       a_proc->status = 'c';                                                                                                                     <* 
*>       yDLST_active_off (a_proc->dlst);                                                                                                          <* 
*>       ++complete;                                                                                                                               <* 
*>    }                                                                                                                                            <* 
*>    /+---(log and exit)-----------------------+/                                                                                                 <* 
*>    a_proc->start  = yLOG_time();                                                                                                                <* 
*>    DEBUG_LOOP   yLOG_info   ("SUCCESS"   , "all good");                                                                                         <* 
*>    DEBUG_LOOP   yLOG_exit   (__FUNCTION__);                                                                                                     <* 
*>    return 0;              /+ parent moves on to next task     +/                                                                                <* 
*> }                                                                                                                                               <*/

/*> char                                                                                               <* 
 *> EXEC_check         (void)                                                                          <* 
 *> {                                                                                                  <* 
 *>    /+---(locals)-----------+-----------+-+/                                                        <* 
 *>    tPROC      *x_data      = NULL;                                                                 <* 
 *>    int         i           = 0;                                                                    <* 
 *>    int         rc          = 0;                                                                    <* 
 *>    tRUSAGE     x_use;                                                                              <* 
 *>    int         status      = 0;                                                                    <* 
 *>    int         errsave     = 0;                                                                    <* 
 *>    char        msg         [200];                                                                  <* 
 *>    /+---(begin)--------------------------+/                                                        <* 
 *>    DEBUG_LOOP   yLOG_enter  (__FUNCTION__);                                                        <* 
 *>    /+---(find)---------------------------+/                                                        <* 
 *>    x_data = (tPROC*) yDLST_active (HEAD);                                                          <* 
 *>    while (x_data != NULL) {                                                                        <* 
 *>       /+---(header)----------------------+/                                                        <* 
 *>       DEBUG_LOOP   yLOG_info   ("review"    , x_data->name);                                       <* 
 *>       /+---(exit code)-------------------+/                                                        <* 
 *>       if (x_data->type == 'b' && my.test == 'y') {                                                 <* 
 *>          rc = 1;                                                                                   <* 
 *>       } else {                                                                                     <* 
 *>          /+> rc = wait4 (x_data->rpid, &status, WNOHANG, &x_use);                      <+/         <* 
 *>          snprintf (msg, 200, "%-15.15s", x_data->name);                                            <* 
 *>          rc = yEXEC_check (msg, x_data->rpid);                                                     <* 
 *>       }                                                                                            <* 
 *>       /+---(feedback)--------------------+/                                                        <* 
 *>       if (rc ==  1) {                                                                              <* 
 *>          ++i;                                                                                      <* 
 *>          x_data = (tPROC*) yDLST_active (NEXT);                                                    <* 
 *>          continue;                                                                                 <* 
 *>       }                                                                                            <* 
 *>       /+---(error)-----------------------+/                                                        <* 
 *>       if (rc < 0) {                                                                                <* 
 *>          errsave = errno;                                                                          <* 
 *>          DEBUG_LOOP   yLOG_value ("zerrno"    , errsave);                                          <* 
 *>          DEBUG_LOOP   yLOG_info  ("zerrstr"   , strerror(errsave));                                <* 
 *>          ++i;                                                                                      <* 
 *>          x_data = (tPROC*) yDLST_active (NEXT);                                                    <* 
 *>          continue;                                                                                 <* 
 *>       }                                                                                            <* 
 *>       /+---(normal)----------------------+/                                                        <* 
 *>       if (x_data->type == 'b' && my.test == 'y') {                                                 <* 
 *>          DEBUG_LOOP   yLOG_senter ("bootskip");                                                    <* 
 *>       } else {                                                                                     <* 
 *>          DEBUG_LOOP   yLOG_senter ("normal");                                                      <* 
 *>       }                                                                                            <* 
 *>       DEBUG_LOOP   yLOG_svalue ("pid"     , x_data->rpid);                                         <* 
 *>       DEBUG_LOOP   yLOG_snote  (x_data->name);                                                     <* 
 *>       DEBUG_LOOP   yLOG_svalue ("wait_rc" , rc);                                                   <* 
 *>       DEBUG_LOOP   yLOG_snote  ("done");                                                           <* 
 *>       if (x_data->type == 'b' && my.test == 'y') {                                                 <* 
 *>          DEBUG_LOOP   yLOG_sexit  ("bootskip");                                                    <* 
 *>       } else {                                                                                     <* 
 *>          DEBUG_LOOP   yLOG_sexit  ("normal");                                                      <* 
 *>       }                                                                                            <* 
 *>       x_data->status = 'c';                                                                        <* 
 *>       yDLST_active_off (x_data->dlst);                                                             <* 
 *>       x_data->end    = yLOG_time();                                                                <* 
 *>       wait4 (x_data->rpid, &status, WNOHANG, &x_use);                                              <* 
 *>       x_data->swaps  = (int) (x_use.ru_nvcsw + x_use.ru_nivcsw);                                   <* 
 *>       x_data->u_time = (int) ((x_use.ru_utime.tv_sec * 1000) + (x_use.ru_utime.tv_usec / 1000));   <* 
 *>       x_data->s_time = (int) ((x_use.ru_stime.tv_sec * 1000) + (x_use.ru_stime.tv_usec / 1000));   <* 
 *>       if (strchr("abcm", x_data->type) != 0)  ++complete;                                          <* 
 *>       EXEC_children (x_data->dlst);                                                                <* 
 *>       ++i;                                                                                         <* 
 *>       x_data = (tPROC*) yDLST_active (NEXT);                                                       <* 
 *>    }                                                                                               <* 
 *>    DEBUG_LOOP   yLOG_value  ("requested" , requested);                                             <* 
 *>    DEBUG_LOOP   yLOG_value  ("running"   , running);                                               <* 
 *>    DEBUG_LOOP   yLOG_value  ("complete"  , complete);                                              <* 
*>    /+---(complete)-----------------------+/                                                        <* 
*>    DEBUG_LOOP   yLOG_exit  (__FUNCTION__);                                                         <* 
*>    return -1;                                                                                      <* 
*> }                                                                                                  <*/

/*> char       /+ ---- : put all children into focus -----------------------------+/   <* 
 *> EXEC_children       (int  a_index)                                                 <* 
 *> {                                                                                  <* 
 *>    DEBUG_LOOP   yLOG_enter  (__FUNCTION__);                                        <* 
 *>    /+---(locals)-------*-----------------+/                                        <* 
 *>    int       i         = 0;            /+ loop iterator -- children           +/   <* 
 *>    tPROC    *x_data    = NULL;         /+ data payload for child              +/   <* 
 *>    char      x_msg     [100];                                                      <* 
 *>    /+---(identify parent)----------------+/                                        <* 
 *>    x_data = yDLST_index (a_index);                                                 <* 
 *>    sprintf (x_msg, "(%3d) %s", a_index, x_data->name);                             <* 
 *>    DEBUG_LOOP   yLOG_info   ("parent"    , x_msg);                                 <* 
 *>    /+---(walk children)------------------+/                                        <* 
 *>    x_data = yDLST_downs (a_index, i);                                              <* 
 *>    while (x_data != NULL) {                                                        <* 
 *>       sprintf (x_msg, "(%3d) %s", x_data->dlst, x_data->name);                     <* 
 *>       DEBUG_LOOP   yLOG_info   ("focus_on"  , x_msg);                              <* 
 *>       yDLST_focus_on  (x_data->dlst);                                              <* 
 *>       x_data = yDLST_downs (a_index, ++i);                                         <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_LOOP   yLOG_value  ("processed" , i);                                     <* 
 *>    DEBUG_LOOP   yLOG_exit   (__FUNCTION__);                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}





/*============================[[ end-of-code ]]===============================*/
