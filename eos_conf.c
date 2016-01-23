/*============================[[    beg-code    ]]============================*/
#include    "eos.h"


char        s_recd      [LEN_RECD];
int         s_nrecd;
char       *nada        = "(none)";

FILE       *f_conf      = NULL;







int         requested   = 0;
int         running     = 0;
int         complete    = 0;


/*====================------------------------------------====================*/
/*===----                      daemons and signals                     ----===*/
/*====================------------------------------------====================*/
static void      o___DAEMON__________________o (void) {;}

char               /* PURPOSE : daemonize the program ------------------------*/
CONF_daemon         (void)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   int       i    = 0;                       /* loop iterator                 */
   int       fd   = 0;                       /* file descriptor               */
   int       rc   = 0;
   /*---(check for foreground mode)-------------*/
   if (my.daemon != 'y') {
      yLOG_info  ("mode",   "user requested foreground mode running");
      yLOG_exit  (__FUNCTION__);
      printf ("     - debug is interactive\n");
      printf ("     - will not daemonize\n");
      return 0;
   }
   yLOG_info  ("mode",   "moving into daemon mode");
   /*> rc = yEXEC_daemon (my.logger, NULL);                                           <*/
   /*---(fix the umask)-------------------------*/
   umask(0077);
   yLOG_info  ("umask",  "tighten the default file permissions");
   /*---(close off all descriptors)-------------*/
   for (i = 0; i < 256; ++i) {
      if (i == my.logger) continue;
      close(i);
   }
   yLOG_info  ("fds",    "closed all inherited file descriptors");
   /*---(tie std fds to the bitbucket)----------*/
   fd = open("/dev/null", O_RDWR);
   if (fd < 0) {
      yLOG_info  ("fds",    "creation of safe fd FAILED");
   }
   dup2(fd, 0);
   dup2(fd, 1);
   dup2(fd, 2);
   yLOG_info  ("std fds",   "redirect stdin, stdout, and stderr to /dev/null");
   /*---(session id)----------------------------*/
   /*> setsid();                                                                      <*/
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char       /*----: setup signal handling -------------------------------------*/
CONF_signal          (void)
{
   /*---(design notes)-------------------*/
   /* options are...
    *    signal()        -- race conditions when they come fast (depricated)
    *    sigaction()     -- preferred method and standard in c
    *    signalfd()      -- linux specific that treats it like a file
    *
    */
   yLOG_enter (__FUNCTION__);
   /*---(set up structure)---------------*/
   struct sigaction sa;
   /*---(start with ignores)-------------*/
   yLOG_info  ("terminal", "ignore STOP, CONT, TIN, and TOUT");
   sa.sa_handler   = SIG_IGN;
   sa.sa_sigaction = NULL;
   sa.sa_flags     = SA_RESTART;
   /*> sigemptyset(&sa.sa_mask);                                                      <*/
   sigfillset(&sa.sa_mask);
   sa.sa_restorer  = NULL;
   /*> sigaction(SIGINT ,  &sa      , NULL);       /+ keyboard interrupt          +/   <* 
    *> sigaction(SIGQUIT,  &sa      , NULL);       /+ keyboard quit               +/   <* 
    *> sigaction(SIGTERM,  &sa      , NULL);       /+ terminate                   +/   <* 
    *> sigaction(SIGCONT,  &sa      , NULL);       /+ continue                    +/   <* 
    *> sigaction(SIGTSTP,  &sa      , NULL);       /+ keyboard stop               +/   <* 
    *> sigaction(SIGTTIN,  &sa      , NULL);       /+ tty input for background    +/   <* 
    *> sigaction(SIGTTOU,  &sa      , NULL);       /+ tty output for background   +/   <*/
   /*---(then to handles)----------------*/
   sa.sa_handler   = NULL;
   sa.sa_sigaction = CONF_comm;
   /*> sa.sa_flags     = SA_RESTART | SA_NOCLDSTOP | SA_NODEFER | SA_SIGINFO;         <*/
   /*> sa.sa_flags     = SA_RESTART | SA_NOCLDSTOP | SA_SIGINFO;                      <*/
   sa.sa_flags     = SA_RESTART | SA_SIGINFO;
   /*> sigemptyset(&sa.sa_mask);                                                      <*/
   sigfillset(&sa.sa_mask);
   sa.sa_restorer  = NULL;
   printf ("CHLD");
   yLOG_info  ("children", "pay attention to children");
   sigaction(SIGCHLD,  &sa      , NULL);       /* watch for children          */
   printf (", HUP");
   yLOG_info  ("program",  "look for HUP");
   sigaction(SIGHUP ,  &sa      , NULL);       /* hangup means refresh        */
   printf (", TERM/SEGV");
   yLOG_info  ("baddies",  "handle SEGV and TERM");
   sigaction(SIGTERM,  &sa      , NULL);       /* catch a kill                */
   sigaction(SIGSEGV,  &sa      , NULL);       /* catch a segfault            */

   printf (", keyboard");
   sigaction(SIGINT ,  &sa      , NULL);       /* keyboard interrupt          */
   sigaction(SIGQUIT,  &sa      , NULL);       /* keyboard quit               */
   sigaction(SIGTERM,  &sa      , NULL);       /* terminate                   */
   sigaction(SIGCONT,  &sa      , NULL);       /* continue                    */
   sigaction(SIGTSTP,  &sa      , NULL);       /* keyboard stop               */
   printf (", tty");
   sigaction(SIGTTIN,  &sa      , NULL);       /* tty input for background    */
   sigaction(SIGTTOU,  &sa      , NULL);       /* tty output for background   */
   /*---(complete)-----------------------*/
   yLOG_exit  (__FUNCTION__);
   printf (", done\n");
   return 0;
}

void       /*----: handle signals --------------------------------------------*/
CONF_comm           (int a_signal, siginfo_t *a_info, void *a_nada)
{
   /*---(locals)-------*-----------------*/
   int       status    = 0;
   int       xlink     = 0;
   tPROC    *xdata     = NULL;
   tRUSAGE   r_use;
   int       rc        = 0;
   int       errsave   = 0;
   switch (a_signal) {
   case  SIGCHLD:
      xlink  = EXEC_find (a_info->si_pid);
      if (xlink < 0) break;
      xdata = (tPROC*) yDLST_index (xlink);
      yLOG_senter("sigchld");
      yLOG_spoint((void *) a_info);
      yLOG_svalue("pid"     , (int) a_info->si_pid);
      yLOG_snote (xdata->name);
      xdata->rc = (int) a_info->si_status;
      yLOG_svalue("rc"      , (int) a_info->si_status);
      yLOG_snote ("wait");
      rc = wait4(a_info->si_pid, &status, WNOHANG, &r_use);
      if (rc <  0) errsave = errno;
      yLOG_svalue("wait_rc" , rc);
      yLOG_snote ("done");
      yLOG_sexit ("sigchld");
      xdata->status = 'c';
      yDLST_active_off (xdata->dlst);
      xdata->end    = yLOG_time();;
      if (rc >= 0) {
         if (strchr("abcm", xdata->type) != 0)  ++complete;
         xdata->swaps  = (int) (r_use.ru_nvcsw + r_use.ru_nivcsw);
         xdata->u_time = (int) ((r_use.ru_utime.tv_sec * 1000) + (r_use.ru_utime.tv_usec / 1000));
         xdata->s_time = (int) ((r_use.ru_stime.tv_sec * 1000) + (r_use.ru_stime.tv_usec / 1000));
      } else if (rc < 0) {
         yLOG_value ("cerrno"    , errsave);
         yLOG_info  ("cerrstr"   , strerror(errsave));
      }
      EXEC_children (xdata->dlst);
      break;
   case  SIGHUP:
      yLOG_info  ("SIGNAL", "SIGHUP MEANS REFRESH");
      break;
   case  SIGTERM:
      yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon (but eos can't)");
      break;
   case  SIGSEGV:
      yLOG_info  ("SIGNAL", "SIGSEGV means daemon blew up (but eos can't)");
      break;
   }
   /*---(complete)------------------------------*/
   return;
}


/*====================------------------------------------====================*/
/*===----                         configuration                        ----===*/
/*====================------------------------------------====================*/
static void      o___CONFIG__________________o (void) {;};


/*---(shared variables)------------------*/
int       s_group    = -1;             /* current group number                */
char      s_gname    [200] = "";       /* current group name                  */
int       s_beg      = -5;             /* current group beginning link        */
int       s_end      = -5;             /* current group ending link           */
int       s_before   = -5;             /* before group in a link              */

char               /* PURPOSE : open up the configuration file ---------------*/
CONF_open          (void)
{
   printf ("CONF_open     : ");
   yLOG_enter (__FUNCTION__);
   yLOG_info  ("file",    FILE_CONF);
   printf ("%s", FILE_CONF);
   /*---(open)---------------------------*/
   f_conf = fopen (FILE_CONF, "r");
   yLOG_point ("file*",   f_conf);
   printf (", *%p", f_conf);
   /*---(test)---------------------------*/
   if (f_conf == NULL) {
      printf(", not open, FATAL\n");
      yLOG_info  ("FAILED",  "can not open the initd.conf file");
      return -1;
   }
   /*---(status)-------------------------*/
   yLOG_info  ("status",  "file has been successfully opened");
   yLOG_exit  (__FUNCTION__);
   printf (", good to go\n");
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: find next valid record ----------------[ leaf   [ ------ ]-*/
CONF_read           (void)
{
   /*---(locals)-----------+-----------+-*/
   int         x_len       = 0;
   char       *rcs         = NULL;
   /*---(find next recd-----------------*/
   while (1) {
      /*---(read)------------------------*/
      rcs = fgets (s_recd, LEN_RECD, f_conf);
      if (rcs == NULL       )   return -1;
      if (feof(f_conf)           )   return -2;
      /*---(prepare)---------------------*/
      x_len = strlen (s_recd);
      if (x_len      <= 0   )   continue;
      s_recd [--x_len] = '\0';
      ++s_nrecd;
      /*---(filter)----------------------*/
      if (s_recd [0] == '#' )   continue;
      if (s_recd [0] == '\0')   continue;
      if (x_len      <  40  )   continue;
      /*---(report)----------------------*/
      /*> printf ("CONF_read (%2d): %3d:%-50.50s\n", s_nrecd, x_len, s_recd);          <*/
      break;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   return 0;
}

tPROC*     /*----: create a new proc opject ----------------------------------*/
CONF_new            (int a_num, char *a_name, char a_type)
{
   yLOG_break ();
   yLOG_enter (__FUNCTION__);
   /*---(locals)-------------------------*/
   tPROC      *xproc     = NULL;
   int         i         = 0;
   char       *p         = NULL;
   char       *q         = "\x1F";
   int         x_len     = 0;
   char       *x_desc    = NULL;
   char       *xuser     = NULL;
   char       *xcheck    = NULL;
   char       *xcmd      = NULL;
   tPASSWD    *pass;
   /*---(create)-------------------------*/
   for (i = 0; i < 3; ++i) {
      xproc = (tPROC *) malloc (sizeof(tPROC));
      if (xproc != NULL)     break;
   }
   if (xproc == NULL)        return NULL;
   /*---(record)-------------------------*/
   xproc->seq = a_num;
   yLOG_value ("->seq"   , xproc->seq);
   /*---(check name)-------------------------*/
   if (a_name == NULL) {
      yLOG_warn  ("name",  "can not be null");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   x_len = strlen (a_name);
   if (x_len < 1 || x_len >= LEN_NAME - 1) {
      yLOG_warn  ("name",  "not the proper length (1 - 20)");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   strncpy (xproc->name, a_name, LEN_NAME);
   yLOG_info  ("name"    , xproc->name);
   /*---(type)---------------------------*/
   if (strchr ("abcdmsGL", a_type) == 0) {
      yLOG_warn  ("type",  "not a legal value (acds)");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   xproc->type    = a_type;
   yLOG_char  ("type"    , xproc->type);
   /*---(check desc)-------------------------*/
   p = strtok (NULL, q);
   if (p == NULL) {
      yLOG_warn  ("desc",  "field not found");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   x_len = strltrim (p, ySTR_BOTH, LEN_DESC);
   if (x_len < 1 || x_len >= LEN_DESC - 1) {
      yLOG_warn  ("desc",  "not the proper length (1 - 50)");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   strlcpy (xproc->desc, p, LEN_DESC);
   yLOG_info  ("desc"    , xproc->desc);
   /*---(check user)-------------------------*/
   p = strtok (NULL, q);
   if (p == NULL) {
      yLOG_warn  ("user",  "field not found");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   x_len = strltrim (p, ySTR_BOTH, LEN_NAME);
   if (x_len < 1 || x_len >= LEN_NAME - 1) {
      yLOG_warn  ("user",  "not the proper length (1 - 20)");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   strlcpy (xproc->user, p, LEN_NAME);
   yLOG_info  ("user"    , xproc->user);
   /*---(check uid)--------------------------*/
   pass = getpwnam (xproc->user);                     /* get password entry       */
   if (pass == NULL) {
      yLOG_warn  ("user",  "user name not a valid account on the system");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   xproc->uid     = pass->pw_uid;
   yLOG_value ("uid"     , xproc->uid);
   /*---(check)--------------------------*/
   p = strtok (NULL, q);
   if (p == NULL) {
      yLOG_warn  ("check",  "field not found");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   x_len = strltrim (p, ySTR_BOTH, LEN_CMD);
   if (x_len < 0 || x_len >= LEN_CMD - 1) {
      yLOG_warn  ("check",  "not the proper length (1 - 200)");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   if (x_len  == 0)  strlcpy (xproc->check , nada  , LEN_CMD);
   else              strlcpy (xproc->check , p     , LEN_CMD);
   yLOG_info  ("check"    , xproc->check);
   /*---(run)----------------------------*/
   p = strtok (NULL, q);
   if (p == NULL) {
      yLOG_warn  ("run",  "field not found");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   x_len = strltrim (p, ySTR_BOTH, LEN_CMD);
   if (x_len < 1 || x_len >= LEN_CMD - 1) {
      yLOG_warn  ("run",  "not the proper length (1 - 200)");
      yLOG_exit  (__FUNCTION__);
      return NULL;
   }
   strlcpy (xproc->check , p     , LEN_CMD);
   yLOG_info  ("run"      , xproc->check);
   /*---(initialize)------------------*/
   xproc->rpid        = 0;
   xproc->status      = '-';
   xproc->start       = 0;
   xproc->end         = 0;
   xproc->u_time      = 0;
   xproc->s_time      = 0;
   xproc->swaps       = 0;
   xproc->rc          = 0;
   /*---(add to counts)---------------*/
   ++requested;
   /*---(complete)--------------------*/
   yLOG_exit  (__FUNCTION__);
   return xproc;
}

tPROC*     /*----: create a new milestone entry ------------------------------*/
CONF_milestone      (char *a_name)
{
   /*---(locals)-------------------------*/
   tPROC    *xproc     = NULL;
   int       i         = 0;
   /*---(create)-------------------------*/
   for (i = 0; i < 3; ++i) {
      xproc = (tPROC *) malloc(sizeof(tPROC));
      if (xproc != NULL)     break;
   }
   if (xproc == NULL)        return NULL;
   /*---(fill out)--------------------*/
   xproc->seq     = -1;
   strncpy (xproc->name, a_name, 50);
   xproc->type    = '-';
   strncpy (xproc->desc, a_name, 50);
   xproc->user[0] = '\0';
   xproc->uid     = -1;
   xproc->run[0]  = '\0';
   /*---(initialize)------------------*/
   xproc->rpid    = 0;
   xproc->status  = '-';
   xproc->start   = 0;
   xproc->end     = 0;
   xproc->u_time  = 0;
   xproc->s_time  = 0;
   xproc->swaps   = 0;
   xproc->rc      = 0;
   /*---(complete)--------------------*/
   return xproc;
}

char
CONF_parse          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        x_temp      [LEN_RECD];
   int         x_len       = 0;
   int         rc          = 0;
   char       *p           = NULL;
   char       *q           = "\x1F";
   char        x_name      [200];
   char        x_msg       [200] = "";
   int         x_link1     = -1;
   int         x_link2     = -1;
   tPROC      *x_proc      = NULL;
   int         x_item      = -1;
   /*---(backup)-------------------------*/
   strncpy (x_temp, s_recd, LEN_RECD);
   /*> printf ("CONF_parse    : %-50.50s\n", x_temp);                                  <*/
   if (s_group >= 0 && x_temp[0] == '\0' && strcmp (s_gname, "") != 0) {
      /*> sprintf (x_msg, "%s_end", s_gname);                                           <* 
       *> x_proc = CONF_milestone (x_msg);                                               <* 
       *> rc = yDLST_new    ( s_group, NULL, strdup(x_msg), x_proc);                     <*/
      strcpy (s_gname, "");
      return 0;
   }
   /*---(parse out name)-----------------*/
   p = strtok (x_temp, q);
   if (p == NULL) return 0;
   strltrim (p, ySTR_BOTH, 150);
   strlcpy (x_name, p, 150);
   /*---(parse out type)-----------------*/
   p = strtok (NULL, q);
   if (p == NULL) return 0;
   strltrim (p, ySTR_BOTH, 150);
   x_len = strlen (p);
   /*---(handle groups)------------------*/
   if (strncmp ("GROUP", p, 5) == 0) {
      /*---(get the desc)----------------*/
      strcpy (s_gname, x_name);
      p = strtok (NULL, q);
      if (p == NULL) return 0;
      strltrim (p, ySTR_BOTH, LEN_DESC);
      /*---(make the main node)----------*/
      yLOG_break  ();
      rc = yDLST_create (&s_group, x_name, strdup (x_name));
      yLOG_value  ("list id"   , s_group);
      yLOG_info   ("called"    , x_name);
      /*---(make the beg node)-----------*/
      sprintf (x_msg, "%s_beg", s_gname);
      x_proc = CONF_milestone (x_msg);
      rc = yDLST_new    ( s_group, &s_beg, strdup(x_msg), x_proc);
      x_proc->dlst = s_beg;
      yLOG_info   ("beg_name"  , x_msg);
      yLOG_value  ("beg_seq"   , s_beg);
      /*---(make the end node)-----------*/
      sprintf (x_msg, "%s_end", s_gname);
      x_proc = CONF_milestone (x_msg);
      rc = yDLST_new    ( s_group, &s_end, strdup(x_msg), x_proc);
      x_proc->dlst = s_end;
      yLOG_info   ("end_name"  , x_msg);
      yLOG_value  ("end_seq"   , s_end);
      /*---(prepare for details)---------*/
      yDLST_tie (s_beg, s_end, NULL);
      /*---(done)------------------------*/
   } else if (strncmp ("AFTER", x_name, 4) == 0) {
      yLOG_break  ();
      yLOG_note   ("creating an AFTER link between two groups");
      /*---(get the start)---------------*/
      sprintf (x_msg, "%s_end", p);
      x_link1 = yDLST_search (x_msg);
      yLOG_info   ("beg_name"  , x_msg);
      yLOG_value  ("beg_seq"   , x_link1);
      /*> printf ("tie (%4d) %s  ", x_link1, x_msg);                                     <*/
      /*---(get the current)-------------*/
      sprintf (x_msg, "%s_beg", s_gname);
      x_link2 = yDLST_search (x_msg);
      yLOG_info   ("end_name"  , x_msg);
      yLOG_value  ("end_seq"   , x_link2);
      /*> printf ("tie (%4d) %s  ", x_link1, x_msg);                                     <*/
      /*---(tie them)--------------------*/
      if (x_link1 >= 0 && x_link2 >= 0)  yDLST_tie (x_link1, x_link2, NULL);
      /*---(done)------------------------*/
   } else {
      x_proc = CONF_new (s_nrecd, x_name, p[0]);
      if (x_proc != NULL) {
         if (s_group >= 0) rc = yDLST_new    ( s_group, &x_item, strdup (x_name), x_proc);
         x_proc->dlst = x_item;
         yLOG_note   ("connecting new item");
         yDLST_tie (s_beg , x_item, NULL);
         yLOG_value  ("back to"   , s_beg);
         yDLST_tie (x_item, s_end , NULL);
         yLOG_value  ("fore to"   , s_end);
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
CONF_list           (void)
{
   yLOG_enter (__FUNCTION__);
   char     *xname     = NULL;
   tPROC    *xdata     = NULL;
   char     *xsave     = NULL;
   xname = yDLST_lists (HEAD);
   while (xname != NULL) {
      xsave = xname;
      yLOG_enter (xsave);
      xdata = (tPROC*) yDLST_list (CURR, HEAD);
      while (xdata != NULL) {
         yLOG_info  ("link",    xdata->name);
         xdata = yDLST_list (CURR, NEXT);
      }
      yLOG_exit  (xsave);
      xname = yDLST_lists (NEXT);
   }
   /*---(complete)-----------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char               /* PURPOSE : open up the configuration file ---------------*/
CONF_close          (void)
{
   int       rc        = 0;            /* generic return code                 */
   yLOG_enter (__FUNCTION__);
   yLOG_info  ("file",    FILE_CONF);
   /*> yLOG_value ("nprocs#"   , nprocs);                                             <*/
   /*---(open)---------------------------*/
   if (f_conf != NULL)  rc = fclose(f_conf);
   if (rc == 0) {
      yLOG_info  ("status",  "file has been successfully closed");
   } else {
      yLOG_info  ("FAILURE",  "file can't be closed");
   }
   f_conf = NULL;
   /*---(complete)-----------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char             /* [------] clean report of critical process feedback -------*/
CONF_report         (char a_mode)
{
   printf ("CONF_report   : ");
   yLOG_enter (__FUNCTION__);
   int         rc          = 0;
   int         i           = 0;                       /* loop iterator                 */
   int         status      = 0;
   tRUSAGE     r_use;
   FILE       *output      = NULL;
   char       *xname       = NULL;
   tPROC      *xdata       = NULL;
   char       *xsave       = NULL;
   char       *q           = "\x1F";
   long        now;                                 /* present datetime         */
   tTIME      *curr_time   = NULL;
   char        msg         [200];
   /*---(open output)--------------------*/
   yLOG_info  ("file"    , PERFORM);
   output = fopen (PERFORM, "w");
   /*> output = stdout;                                                               <*/
   yLOG_point ("file*"   , output);
   /*---(get the date)-----------------------*/
   now = time(NULL);
   curr_time = localtime(&now);
   /*> strftime(msg, 50, "%Ss %Mm %Hh %dd %mm  %ww", curr_time);                      <*/
   strftime (msg, 50, "%y.%m.%d.%H.%m.%S.%W", curr_time);
   /*---(write header)-----------------------*/
   fprintf (output, "# EOS (goddess of daybreak, rosy-fingered dawn)\n");
   fprintf (output, "# summary results reporting\n");
   fprintf (output, "# written %s\n",   msg);
   if (a_mode == 'a') {
      printf  ("after run ");
      fprintf (output, "# AFTER last run\n");
   } else {
      printf  ("before run");
      fprintf(output, "# before last run\n");
   }
   fprintf(output, "#\n");
   fprintf(output, "# ref  line  -----name---------  -  -------------------------desc------------------------  ---user---  --status--  -rpid-  --start---  ---end----  -rc-  u_time  s_time  swap  --dur- \n");
   yLOG_note  ("writing lines...");
   /*---(grab head)----------------------*/
   xname = yDLST_lists (HEAD);
   printf (", ");
   while (xname != NULL) {
      xsave = xname;
      yLOG_enter (xsave);
      xdata = (tPROC*) yDLST_list (CURR, HEAD);
      while (xdata != NULL) {
         yLOG_info  ("link",    xdata->name);
         ++i;
         /*---(references)---------------*/
         printf (".");
         fprintf(output, "  %3d ", i);
         if (xdata->seq  <= 0) fprintf(output, " %4s ", "    ");
         else                  fprintf(output, " %4d ", xdata->seq);
         if (xdata->type == '-') {
            fprintf(output, " %-15.15s     %c  %-50.50s    ", xdata->name   , ' ', xdata->desc  );
            fprintf(output, "                                                                                            \n");
         } else {
            fprintf(output, "    %-15.15s  %c     %-50.50s  %-10.10s  %-10.10s ",
                  xdata->name   , xdata->type   , xdata->desc  , xdata->user   , xdata->result);
            if (xdata->rpid == 0) {
               fprintf(output, "                                                                    \n");
            } else {
               fprintf(output, " %6d  %10lld ", xdata->rpid   , xdata->start);
               if (strchr("sd", xdata->type) != 0) {
                  fprintf(output, "                                                \n");
               } else {
                  fprintf(output, " %10lld  %4d  %6ld  %6ld  %4d  %6lld \n",
                        xdata->end  , xdata->rc     , xdata->u_time , xdata->s_time ,
                        xdata->swaps, xdata->end - xdata->start);
               }
            }
         }
         xdata = (tPROC*) yDLST_list (CURR, NEXT);
      }
      yLOG_exit  (xsave);
      xname = yDLST_lists (NEXT);
   }
   fprintf (output, "# ref  line  -----name---------  -  -------------------------desc------------------------  ---user---  --status--  -rpid-  --start---  ---end----  -rc-  u_time  s_time  swap  --dur- \n");
   fprintf (output, "# end of report\n");
   fflush (output);
   yLOG_info  ("flushing", "done");
   fclose (output);
   output = NULL;
   yLOG_info  ("closing" , "done");
   yLOG_exit  (__FUNCTION__);
   printf (", %d records, done\n", i);
   return  0;
}




/*============================[[ end-of-code ]]===============================*/
