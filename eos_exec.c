/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



int         requested   = 0;
int         running     = 0;
int         complete    = 0;



/*====================------------------------------------====================*/
/*===----                           processes                          ----===*/
/*====================------------------------------------====================*/
static void      o___PROCESSES_______________o (void) {;}

char
EXEC_checkmount    (char *a_mount)
{
   yLOG_senter  (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;      /* return code for errors              */
   int         rc          = 0;        /* generic return code                 */
   FILE       *f           = NULL;     /* generic file pointer                */
   int         x_len       = 0;        /* length of mount name                */
   char        x_recd      [1000];     /* proc table entry                    */
   /*---(defenses)-----------------------*/
   yLOG_spoint  (a_mount);
   --rce;  if (a_mount == NULL) {
      yLOG_snote   ("pointer null, failed");
      yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(prepare)-----------------------*/
   yLOG_snote   (a_mount);
   x_len = strllen (a_mount, LEN_NAME);
   yLOG_svalue  ("len"       , x_len);
   --rce;  if (x_len <= 0) {
      yLOG_snote   ("name too short, failed");
      yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(check /proc/mounts)---------*/
   f = fopen ("/proc/mounts", "r");
   while (!feof (f)) {
      fgets (x_recd, 450, f);
      if (strncmp (x_recd, a_mount, x_len) == 0) {
         yLOG_snote   ("found in /proc/mounts");
         yLOG_sexit   (__FUNCTION__);
         return 1;
      }
   }
   fclose (f);
   /*---(check /proc/swaps)----------*/
   f = fopen ("/proc/swaps", "r");
   while (!feof (f)) {
      fgets (x_recd, 450, f);
      if (strncmp (x_recd, a_mount, x_len) == 0) {
         yLOG_snote   ("found in /proc/swaps");
         yLOG_sexit   (__FUNCTION__);
         return 2;
      }
   }
   fclose (f);
   /*---(done)-----------------------*/
   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
EXEC_launch        (void)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)-------*-----------------*/
   tPROC    *xdata     = NULL;
   tPROC    *xup       = NULL;
   char      ready     = 'y';
   int       i         = 0;
   /*---(find)---------------------------*/
   xdata = (tPROC*) yDLST_focus (HEAD);
   while (xdata != NULL) {
      /*---(precursors)------------------*/
      ready = 'y';
      xup   = yDLST_ups   (xdata->dlst, HEAD);
      while (xup   != NULL) {
         if  (xup->status != 'c') {
            ready = 'n';
            break;
         }
         xup   = yDLST_ups   (xdata->dlst, NEXT);
      }
      if (ready == 'y') {
         yLOG_info  ("ready"     , xdata->name);
         EXEC_run (xdata);
      } else {
         yLOG_info  ("waiting"   , xdata->name);
      }
      /*---(find next)-------------------*/
      ++i;
      xdata = (tPROC*) yDLST_focus (NEXT);
   }
   /*---(complete)-----------------------*/
   yLOG_exit  (__FUNCTION__);
   return -1;
}

char       /*-#<[: locate a process dlst based on pid ------------------------*/
EXEC_find          (int a_rpid)
{
   /*---(locals)-------*-----------------*/
   tPROC    *xdata     = NULL;
   /*---(find)---------------------------*/
   xdata = (tPROC*) yDLST_links (HEAD);
   while (xdata != NULL) {
      if (xdata->rpid == a_rpid)  return xdata->dlst;
      xdata = yDLST_links (NEXT);
   }
   /*---(complete)-----------------------*/
   return -1;
}

char       /*----: execute a particular process entry ------------------------*/
EXEC_run           (tPROC *a_proc)
{
   yLOG_enter  (__FUNCTION__);
   /*---(locals)--------------------------------*/
   int       rc        = 0;                       /* simple return code       */
   int       rpid      = 0;                       /* child pid for execution  */
   tPASSWD  *pass;
   FILE     *output = NULL;
   long      now;                                 /* present datetime         */
   tTIME    *curr_time = NULL;
   char      msg[200];
   char      envp[10][200];
   sigset_t  new_mask, old_mask;
   char      backup[LEN_CMD]    = "";          /* copy of the command for parsing */
   char     *p[20];                        /* pointers to cli arguments       */
   int       i         = 0;                /* loop iterator -- arguement      */
   char        name        [100];
   char        check       [200];
   char        run         [200];
   char        x_recd      [1000];
   int       len;
   tDIRENT    *den;                        /* daemon checking                 */
   DIR        *dir;                        /* daemon checking                 */                        /* daemon checking                 */
   char       *q;
   FILE       *f           = NULL;
   /*---(prepare)------------------------*/
   a_proc->rpid   = 0;
   yLOG_info   ("name"      , a_proc->name);
   yLOG_char   ("type"      , a_proc->type);
   strcpy (a_proc->result, "no check");
   /*---(check for milestones)-----------*/
   if (a_proc->type == '-') {
      a_proc->status = 'c';
      yDLST_focus_off (a_proc->dlst);
      yLOG_info   ("SUCCESS"   , "milestone completed");
      EXEC_children (a_proc->dlst);
      yLOG_exit   (__FUNCTION__);
      return 0;
   }
   /*---(check for boot only)------------*/
   if (a_proc->type == 'b' && my.test == 'y') {
      yLOG_info   ("special"   , "not boot, don't run");
      yDLST_focus_off  (a_proc->dlst);
      yDLST_active_on (a_proc->dlst);
      a_proc->rpid       = 0;
      a_proc->status     = 'r';
      ++running;
      yLOG_exit   (__FUNCTION__);
      return 1;
   }
   /*---(check for serial)---------------*/
   if (a_proc->type == 's' && getpid() !=  1 )  {
      yLOG_info   ("serial"    , "not boot, don't run");
      a_proc->status = 'c';
      yDLST_focus_off  (a_proc->dlst);
      yDLST_active_off (a_proc->dlst);
      ++complete;
      EXEC_children (a_proc->dlst);
      yLOG_exit   (__FUNCTION__);
      return 1;
   }
   /*---(check for daemons)--------------*/
   if (a_proc->type == 'd')  {
      strcpy (a_proc->result, "not found");
      sprintf (check, "%s", a_proc->check);
      len = strlen (check);
      dir = opendir("/proc");
      if (dir == NULL) return -1;
      while ((den = readdir(dir)) != NULL) {
         if (atoi(den->d_name) == 0)  continue;
         sprintf(name, "/proc/%s/stat", den->d_name);
         f = fopen (name, "r");
         if (f == NULL) continue;
         fgets (x_recd, 1000, f);
         q = strtok (x_recd, "(");
         q = strtok (NULL, ")");
         fclose (f);
         if (strncmp (q, check, len) == 0) {
            yLOG_info   ("check"    , "found in /proc");
            a_proc->status = 'c';
            yDLST_focus_off  (a_proc->dlst);
            yDLST_active_off (a_proc->dlst);
            ++complete;
            EXEC_children (a_proc->dlst);
            strcpy (a_proc->result, "running");
            yLOG_exit   (__FUNCTION__);
            closedir(dir);
            return 1;
         }
      }
      closedir(dir);
   }
   /*---(check if it needs to run)-------*/
   yLOG_info   ("user"      , a_proc->user);
   snprintf(msg, 200, "%-16.16s,%3d", a_proc->name, a_proc->seq);
   /*---(check for mounts)---------------*/
   if (a_proc->type == 'm') {
      /*---(prepare)--------------------*/
      strcpy (a_proc->result, "not found");
      sprintf (check, "%s ", a_proc->check);
      len = strlen (check);
      /*---(check /proc/mounts)---------*/
      f = fopen ("/proc/mounts", "r");
      while (!feof (f)) {
         fgets (x_recd, 450, f);
         if (strncmp (x_recd, check, len) == 0) {
            yLOG_info   ("check"     , "found in /proc/mounts");
            a_proc->status = 'c';
            yDLST_focus_off  (a_proc->dlst);
            yDLST_active_off (a_proc->dlst);
            ++complete;
            yLOG_info   ("complete"  , "mark done");
            EXEC_children (a_proc->dlst);
            strcpy (a_proc->result, "mounted");
            yLOG_exit   (__FUNCTION__);
            return 1;
         }
      }
      fclose (f);
      /*---(check /proc/swaps)----------*/
      f = fopen ("/proc/swaps", "r");
      while (!feof (f)) {
         fgets (x_recd, 450, f);
         if (strncmp (x_recd, check, strlen (check)) == 0) {
            yLOG_info   ("check"     , "found in /proc/swaps");
            a_proc->status = 'c';
            yDLST_focus_off  (a_proc->dlst);
            yDLST_active_off (a_proc->dlst);
            ++complete;
            yLOG_info   ("complete"  , "mark done");
            EXEC_children (a_proc->dlst);
            strcpy (a_proc->result, "mounted");
            yLOG_exit   (__FUNCTION__);
            return 1;
         }
      }
      fclose (f);
      /*---(done)-----------------------*/
   }
   /*---(check for mounts)---------------*/
   /*> if (a_proc->type != 'c') {                                                     <* 
    *>    if (strcmp (a_proc->check, nada) != 0) {                                    <* 
    *>       sprintf (check, "%s 2>&1 /dev/null", a_proc->check);                     <* 
    *>       rc = system (check);                                                     <* 
    *>       yLOG_value   ("rc"        , WEXITSTATUS (rc));                           <* 
    *>       if (WEXITSTATUS (rc) != 0) {                                             <* 
    *>          yLOG_info   ("check"     , "already run");                            <* 
    *>          a_proc->status = 'c';                                                 <* 
    *>          yDLST_focus_off  (a_proc->dlst);                                      <* 
    *>          yDLST_active_off (a_proc->dlst);                                      <* 
    *>          ++complete;                                                           <* 
    *>          EXEC_children (a_proc->dlst);                                          <* 
    *>          yLOG_exit   (__FUNCTION__);                                           <* 
    *>          return 1;                                                             <* 
    *>       } else {                                                                 <* 
    *>          yLOG_info   ("check"     , "must run");                               <* 
    *>       }                                                                        <* 
    *>    } else {                                                                    <* 
    *>       yLOG_info   ("check"     , "no check specified");                        <* 
    *>    }                                                                           <* 
    *> } else {                                                                       <* 
    *>    yLOG_info   ("check"     , "config item, not checked");                     <* 
    *> }                                                                              <*/
   /*---(fork off to execute)-------------------*/
   rpid = fork();
   if (rpid < 0) {                        /* error                            */
      yLOG_value  ("forked"    , rpid);
      yLOG_info   ("FAILURE"   , "negative pid");
      yLOG_exit   (__FUNCTION__);
      return -1;
   }
   if (rpid > 0) {
      yLOG_value  ("forked"    , rpid);
      a_proc->rpid       = rpid;
      yDLST_focus_off (a_proc->dlst);
      if (strchr("abcm", a_proc->type) != 0) {
         a_proc->status = 'r';
         yDLST_active_on (a_proc->dlst);
         ++running;
      } else {
         a_proc->status = 'c';
         yDLST_active_off (a_proc->dlst);
         ++complete;
      }
      /*---(log and exit)-----------------------*/
      a_proc->start  = yLOG_time();
      yLOG_info   ("SUCCESS"   , "all good");
      yLOG_exit   (__FUNCTION__);
      return 0;              /* parent moves on to next task     */
   }
   /*---(execute)-------------------------------*/
   output = fopen(STUFF, "a");
   fprintf(output, "\n");
   /*---(display header for debugging)----------*/
   fprintf(output, "=== execution logging ----------===\n");
   /*---(get the date)-----------------------*/
   now = time(NULL);
   curr_time = localtime(&now);
   strftime(msg, 50, "%Ss %Mm %Hh %dd %mm  %ww", curr_time);
   fprintf(output, "start     : %s\n",   msg);
   /*---(get user information)------------------*/
   pass = getpwnam(a_proc->user);
   if (pass == NULL) {
      exit (-2);
   }
   /*---(set execution environment)-------------*/
   rc = setenv("USER",                 pass->pw_name, 1);
   snprintf(envp[0], 200, "USER=%s",   pass->pw_name);
   fprintf(output, "USER set  : %s\n", pass->pw_name);
   if (rc <  0) {
      exit (-3);
   }
   rc = setenv("HOME",                 pass->pw_dir,  1);
   snprintf(envp[1], 200, "HOME=%s",   pass->pw_dir);
   fprintf(output, "HOME set  : %s\n", pass->pw_dir);
   if (rc <  0) {
      exit (-3);
   }
   rc = setenv("SHELL",                SHELL, 1);
   snprintf(envp[2], 200, "SHELL=%s",  SHELL);
   fprintf(output, "SHELL set : %s\n", SHELL);
   if (rc <  0) {
      exit (-3);
   }
   rc = setenv("PATH",                 PATH, 1);
   snprintf(envp[3], 200, "PATH=%s",   PATH);
   fprintf(output, "PATH set  : %s\n", PATH);
   if (rc <  0) {
      exit (-3);
   }
   /*---(set permissions)-----------------------*/
   rc = initgroups(a_proc->user, pass->pw_gid);
   if (rc <  0) {
      exit (-3);
   }
   rc = setregid(pass->pw_gid,         pass->pw_gid);
   snprintf(envp[4], 200, "GID=%d",    pass->pw_gid);
   fprintf(output, "GID set   : %d\n", pass->pw_gid);
   if (rc <  0) {
      return (-3);
   }
   rc = setreuid(pass->pw_uid, pass->pw_uid);
   snprintf(envp[5], 200, "UID=%d",    pass->pw_uid);
   fprintf(output, "UID set   : %d\n", pass->pw_uid);
   if (rc <  0) {
      return (-3);
   }
   /*---(set current dir)-----------------------*/
   rc = chdir(pass->pw_dir);
   fprintf(output, "chdir     : %s\n", pass->pw_dir);
   if (rc <  0) {
      return (-3);
   }
   /*---(try direct execution)------------------*/
   /*> sprintf (run, "%s > /dev/null", a_proc->run);                                  <*/
   sprintf (run, "%s", a_proc->run);
   fprintf(output, "execvp    : %.100s\n", run);
   fprintf(output, "===-----------------------------===\n");
   fflush (output);
   fclose (output);
   char    backup2[LEN_CMD];
   strncpy(backup2, run, LEN_CMD);
   /*> str_parse (backup2);                                                           <*/
   strlargs   (backup2, LEN_CMD, 20, &my.argc, my.argv);
   rc = execvp (*my.argv, my.argv);
   /*---(try shell execution)-------------------*/
   envp[6][0] = '\0';
   output = fopen(STUFF, "a");
   fprintf(output, "FAILED execvp, fallback...\n");
   fprintf(output, "execl     : %.100s\n", run);
   fprintf(output, "===-----------------------------===\n");
   fflush (output);
   fclose (output);
   rc = execl(SHELL, SHELL, "-c", run, NULL, NULL);
   /*---(this should never come back)-----------*/
   output = fopen(STUFF, "a");
   fprintf(output, "FAILED execl, just won't run\n");
   fprintf(output, "===-----------------------------===\n");
   fflush (output);
   fclose (output);
   _exit (-3);    /* must use _exit to get out properly                       */
}

char
EXEC_check         (void)
{
   /*> return 0;                                                                      <*/
   yLOG_enter (__FUNCTION__);
   /*---(locals)-------*-----------------*/
   tPROC    *xdata     = NULL;
   int       i         = 0;
   int       rc        = 0;
   tRUSAGE   r_use;
   int       status    = 0;
   int       errsave   = 0;
   /*---(find)---------------------------*/
   xdata = (tPROC*) yDLST_active (HEAD);
   while (xdata != NULL) {
      yLOG_info   ("review"    , xdata->name);
      if (xdata->type == 'b' && my.test == 'y')  rc = 1;
      else  rc = wait4 (xdata->rpid, &status, WNOHANG, &r_use);
      if (rc ==  0) {
         ++i;
         xdata = (tPROC*) yDLST_active (NEXT);
         continue;
      }
      if (rc < 0) {
         errsave = errno;
         yLOG_value ("zerrno"    , errsave);
         yLOG_info  ("zerrstr"   , strerror(errsave));
      } else {
         if (xdata->type == 'b' && my.test == 'y')  yLOG_senter ("bootskip");
         else                                         yLOG_senter ("zombie");
         yLOG_svalue ("pid"     , xdata->rpid);
         yLOG_snote  (xdata->name);
         yLOG_svalue ("wait_rc" , rc);
         yLOG_snote  ("done");
         if (xdata->type == 'b' && my.test == 'y')  yLOG_sexit  ("bootskip");
         else                                         yLOG_sexit  ("zombie");
         xdata->status = 'c';
         yDLST_active_off (xdata->dlst);
         xdata->end    = yLOG_time();
         xdata->swaps  = (int) (r_use.ru_nvcsw + r_use.ru_nivcsw);
         xdata->u_time = (int) ((r_use.ru_utime.tv_sec * 1000) + (r_use.ru_utime.tv_usec / 1000));
         xdata->s_time = (int) ((r_use.ru_stime.tv_sec * 1000) + (r_use.ru_stime.tv_usec / 1000));
         if (strchr("abcm", xdata->type) != 0)  ++complete;
         EXEC_children (xdata->dlst);
      }
      ++i;
      xdata = (tPROC*) yDLST_active (NEXT);
   }
   yLOG_value  ("requested" , requested);
   yLOG_value  ("complete"  , complete);
   /*---(complete)-----------------------*/
   yLOG_exit  (__FUNCTION__);
   return -1;
}

char       /* ---- : put all children into focus -----------------------------*/
EXEC_children       (int  a_index)
{
   yLOG_enter  (__FUNCTION__);
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- children           */
   tPROC    *xdata     = NULL;         /* data payload for child              */
   char      xmsg [100];
   /*---(identify parent)----------------*/
   xdata = yDLST_index (a_index);
   sprintf (xmsg, "(%3d) %s", a_index, xdata->name);
   yLOG_info   ("parent"    , xmsg);
   /*---(walk children)------------------*/
   xdata = yDLST_downs (a_index, i);
   while (xdata != NULL) {
      sprintf (xmsg, "(%3d) %s", xdata->dlst, xdata->name);
      yLOG_info   ("focus_on"  , xmsg);
      yDLST_focus_on  (xdata->dlst);
      xdata = yDLST_downs (a_index, ++i);
   }
   /*---(complete)-----------------------*/
   yLOG_value  ("processed" , i);
   yLOG_exit   (__FUNCTION__);
   return 0;
}




/*============================[[ end-of-code ]]===============================*/
