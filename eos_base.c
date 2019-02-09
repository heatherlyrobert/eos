/*============================[[    beg-code    ]]============================*/
#include    "eos.h"


char        s_recd      [LEN_RECD];
int         s_nrecd;
static char       *nada        = "(none)";

FILE       *f_conf      = NULL;


char        unit_answer [LEN_RECD];





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
   /*---(locals)--------------------------------*/
   int       i    = 0;                       /* loop iterator                 */
   int       fd   = 0;                       /* file descriptor               */
   int       rc   = 0;
   /*---(begin)--------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
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
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}


char
base_config             (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_type      =  '-';
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(open file)----------------------*/
   DEBUG_INPT  yLOG_info    ("name_conf"  , my.name_conf);
   rc = yPARSE_open_in (my.name_conf);
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(lines)--------------------------*/
   while (rc >= 0) {
      /*---(read)------------------------*/
      rc = yPARSE_read (&my.c_recdno, NULL);
      DEBUG_INPT   yLOG_value   ("yparse"    , rc);
      if (rc == 0) {
         DEBUG_INPT  yLOG_note    ("end-of-file");
         break;
      }
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(verb)------------------------*/
      rc = yPARSE_popstr  (&my.c_verb);
      DEBUG_INPT   yLOG_value   ("popstr"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("c_verb"    , my.c_verb);
      x_type = my.c_verb [0];
      DEBUG_INPT   yLOG_char    ("x_type"    , x_type);
      /*---(handle)----------------------*/
      if (x_type == 'G')     rc = group_create ();
      else                   rc = proc_create  (x_type);
      DEBUG_INPT   yLOG_value   ("create"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(done)------------------------*/
   }
   /*---(close file)---------------------*/
   rc = yPARSE_close_in ();
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char             /+ [------] clean report of critical process feedback -------+/                                                                                                                                                  <* 
 *> CONF_report         (char a_mode)                                                                                                                                                                                                 <* 
 *> {                                                                                                                                                                                                                                 <* 
 *>    /+---(begin)--------------------------+/                                                                                                                                                                                       <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                                                                                                                                                                      <* 
 *>    int         rc          = 0;                                                                                                                                                                                                   <* 
 *>    int         i           = 0;                       /+ loop iterator                 +/                                                                                                                                         <* 
 *>    int         status      = 0;                                                                                                                                                                                                   <* 
 *>    tRUSAGE     r_use;                                                                                                                                                                                                             <* 
 *>    FILE       *output      = NULL;                                                                                                                                                                                                <* 
 *>    char       *xname       = NULL;                                                                                                                                                                                                <* 
 *>    tPROC      *xdata       = NULL;                                                                                                                                                                                                <* 
 *>    char       *xsave       = NULL;                                                                                                                                                                                                <* 
 *>    char       *q           = "\x1F";                                                                                                                                                                                              <* 
 *>    long        now;                                 /+ present datetime         +/                                                                                                                                                <* 
 *>    tTIME      *curr_time   = NULL;                                                                                                                                                                                                <* 
 *>    char        msg         [200];                                                                                                                                                                                                 <* 
 *>    /+---(open output)--------------------+/                                                                                                                                                                                       <* 
 *>    DEBUG_CONF   yLOG_info  ("file"    , my.name_perf);                                                                                                                                                                            <* 
 *>    output = fopen (my.name_perf, "w");                                                                                                                                                                                            <* 
 *>    /+> output = stdout;                                                               <+/                                                                                                                                         <* 
 *>    DEBUG_CONF   yLOG_point ("file*"   , output);                                                                                                                                                                                  <* 
 *>    /+---(get the date)-----------------------+/                                                                                                                                                                                   <* 
 *>    now = time(NULL);                                                                                                                                                                                                              <* 
 *>    curr_time = localtime(&now);                                                                                                                                                                                                   <* 
 *>    /+> strftime(msg, 50, "%Ss %Mm %Hh %dd %mm  %ww", curr_time);                      <+/                                                                                                                                         <* 
 *>    strftime (msg, 50, "%y.%m.%d.%H.%m.%S.%W", curr_time);                                                                                                                                                                         <* 
 *>    /+---(write header)-----------------------+/                                                                                                                                                                                   <* 
 *>    fprintf (output, "# EOS (goddess of daybreak, rosy-fingered dawn)\n");                                                                                                                                                         <* 
 *>    fprintf (output, "# summary results reporting\n");                                                                                                                                                                             <* 
 *>    fprintf (output, "# written %s\n",   msg);                                                                                                                                                                                     <* 
 *>    if (a_mode == 'a') {                                                                                                                                                                                                           <* 
 *>       fprintf (output, "# AFTER last run\n");                                                                                                                                                                                     <* 
 *>    } else {                                                                                                                                                                                                                       <* 
 *>       fprintf(output, "# before last run\n");                                                                                                                                                                                     <* 
 *>    }                                                                                                                                                                                                                              <* 
 *>    fprintf(output, "#\n");                                                                                                                                                                                                        <* 
 *>    fprintf(output, "# ref  line  -----name---------  -  -------------------------desc------------------------  ---user---  --status--  -rpid-  --start---  ---end----  -rc-  u_time  s_time  swap  --dur- \n");    <* 
 *>    DEBUG_CONF   yLOG_note  ("writing lines...");                                                                                                                                                                                  <* 
 *>    /+---(grab head)----------------------+/                                                                                                                                                                                       <* 
 *>    xname = yDLST_lists (HEAD);                                                                                                                                                                                                    <* 
 *>    while (xname != NULL) {                                                                                                                                                                                                        <* 
 *>       xsave = xname;                                                                                                                                                                                                              <* 
 *>       DEBUG_CONF   yLOG_enter (xsave);                                                                                                                                                                                            <* 
 *>       xdata = (tPROC*) yDLST_list (CURR, HEAD);                                                                                                                                                                                   <* 
 *>       while (xdata != NULL) {                                                                                                                                                                                                     <* 
 *>          DEBUG_CONF   yLOG_info  ("link",    xdata->name);                                                                                                                                                                        <* 
 *>          ++i;                                                                                                                                                                                                                     <* 
 *>          /+---(references)---------------+/                                                                                                                                                                                       <* 
 *>          fprintf(output, "  %3d ", i);                                                                                                                                                                                           <* 
 *>          if (xdata->seq  <= 0) fprintf(output, " %4s ", "    ");                                                                                                                                                                 <* 
 *>          else                  fprintf(output, " %4d ", xdata->seq);                                                                                                                                                             <* 
 *>          if (xdata->type == '-') {                                                                                                                                                                                                <* 
 *>             fprintf(output, " %-15.15s     %c  %-50.50s    ", xdata->name   , ' ', xdata->desc  );                                                                                                                             <* 
 *>             fprintf(output, "                                                                                            \n");                                                                                          <* 
 *>          } else {                                                                                                                                                                                                                 <* 
 *>             fprintf(output, "    %-15.15s  %c     %-50.50s  %-10.10s  %-10.10s ",                                                                                                                                            <* 
 *>                   xdata->name   , xdata->type   , xdata->desc  , xdata->user   , xdata->result);                                                                                                                                  <* 
 *>             if (xdata->rpid == 0) {                                                                                                                                                                                               <* 
 *>                fprintf(output, "                                                                    \n");                                                                                                                 <* 
 *>             } else {                                                                                                                                                                                                              <* 
 *>                fprintf(output, " %6d  %10lld ", xdata->rpid   , xdata->start);                                                                                                                                                  <* 
 *>                if (strchr("sd", xdata->type) != 0) {                                                                                                                                                                              <* 
 *>                   fprintf(output, "                                                \n");                                                                                                                                    <* 
 *>                } else {                                                                                                                                                                                                           <* 
 *>                   fprintf(output, " %10lld  %4d  %6ld  %6ld  %4d  %6lld \n",                                                                                                                                                <* 
 *>                         xdata->end  , xdata->rc     , xdata->u_time , xdata->s_time ,                                                                                                                                             <* 
 *>                         xdata->swaps, xdata->end - xdata->start);                                                                                                                                                                 <* 
 *>                }                                                                                                                                                                                                                  <* 
 *>             }                                                                                                                                                                                                                     <* 
 *>          }                                                                                                                                                                                                                        <* 
*>          xdata = (tPROC*) yDLST_list (CURR, NEXT);                                                                                                                                                                                <* 
*>       }                                                                                                                                                                                                                           <* 
*>       DEBUG_CONF   yLOG_exit  (xsave);                                                                                                                                                                                            <* 
*>       xname = yDLST_lists (NEXT);                                                                                                                                                                                                 <* 
*>    }                                                                                                                                                                                                                              <* 
*>    fprintf (output, "# ref  line  -----name---------  -  -------------------------desc------------------------  ---user---  --status--  -rpid-  --start---  ---end----  -rc-  u_time  s_time  swap  --dur- \n");   <* 
*>    fprintf (output, "# end of report\n");                                                                                                                                                                                         <* 
*>    fflush (output);                                                                                                                                                                                                               <* 
*>    DEBUG_CONF   yLOG_info  ("flushing", "done");                                                                                                                                                                                  <* 
*>    fclose (output);                                                                                                                                                                                                               <* 
*>    output = NULL;                                                                                                                                                                                                                 <* 
*>    DEBUG_CONF   yLOG_info  ("closing" , "done");                                                                                                                                                                                  <* 
*>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                                                                                                                                                                      <* 
*>    return  0;                                                                                                                                                                                                                     <* 
*> }                                                                                                                                                                                                                                 <*/




/*============================[[ end-of-code ]]===============================*/
