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
conf_read               (void)
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

/*> char               /+ PURPOSE : open up the configuration file ---------------+/   <* 
 *> CONF_open          (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(begin)--------------------------+/                                        <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    yLOG_enter (__FUNCTION__);                                                      <* 
 *>    yLOG_info  ("file"      , my.name_conf);                                        <* 
 *>    /+---(open)---------------------------+/                                        <* 
 *>    f_conf = fopen (my.name_conf, "r");                                             <* 
 *>    yLOG_point ("file*"     , f_conf);                                              <* 
 *>    /+---(test)---------------------------+/                                        <* 
 *>    if (f_conf == NULL) {                                                           <* 
 *>       yLOG_info  ("FAILED",  "can not open the initd.conf file");                  <* 
 *>       return -1;                                                                   <* 
 *>    }                                                                               <* 
 *>    /+---(status)-------------------------+/                                        <* 
 *>    yLOG_info  ("status",  "file has been successfully opened");                    <* 
 *>    yLOG_exit  (__FUNCTION__);                                                      <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char         /+--: find next valid record ----------------[ leaf   [ ------ ]-+/             <* 
 *> CONF_read           (void)                                                                   <* 
 *> {                                                                                            <* 
 *>    /+---(begin)--------------------------+/                                                  <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                                 <* 
 *>    /+---(locals)-----------+-----------+-+/                                                  <* 
 *>    int         x_len       = 0;                                                              <* 
 *>    char       *rcs         = NULL;                                                           <* 
 *>    /+---(find next recd-----------------+/                                                   <* 
 *>    while (1) {                                                                               <* 
 *>       /+---(read)------------------------+/                                                  <* 
 *>       rcs = fgets (s_recd, LEN_RECD, f_conf);                                                <* 
 *>       if (feof(f_conf)      ) {                                                              <* 
 *>          DEBUG_CONF   yLOG_note    ("end of file tound");                                    <* 
 *>          DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                           <* 
 *>          return 1;                                                                           <* 
 *>       }                                                                                      <* 
 *>       if (rcs == NULL       )  {                                                             <* 
 *>          DEBUG_CONF   yLOG_note    ("fgets read NULL");                                      <* 
 *>          DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                           <* 
 *>          return 2;                                                                           <* 
 *>       }                                                                                      <* 
 *>       /+---(prepare)---------------------+/                                                  <* 
 *>       x_len = strlen (s_recd);                                                               <* 
 *>       if (x_len      <= 0   )   continue;                                                    <* 
 *>       s_recd [--x_len] = '\0';                                                               <* 
 *>       ++s_nrecd;                                                                             <* 
 *>       /+---(filter)----------------------+/                                                  <* 
 *>       if (s_recd [0] == '#' )   continue;                                                    <* 
 *>       if (s_recd [0] == '\0')   continue;                                                    <* 
 *>       if (x_len      <  40  )   continue;                                                    <* 
 *>       /+---(report)----------------------+/                                                  <* 
 *>       /+> printf ("CONF_read (%2d): %3d:%-50.50s\n", s_nrecd, x_len, s_recd);          <+/   <* 
 *>       break;                                                                                 <* 
 *>       /+---(done)------------------------+/                                                  <* 
 *>    }                                                                                         <* 
 *>    /+---(complete)-----------------------+/                                                  <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                                 <* 
 *>    return 0;                                                                                 <* 
 *> }                                                                                            <*/

/*> tPROC*     /+----: create a new milestone entry ------------------------------+/   <* 
 *> CONF_milestone      (char *a_name)                                                 <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-------------------------+/                                        <* 
 *>    tPROC    *xproc     = NULL;                                                     <* 
 *>    int       i         = 0;                                                        <* 
 *>    /+---(create)-------------------------+/                                        <* 
 *>    for (i = 0; i < 3; ++i) {                                                       <* 
 *>       xproc = (tPROC *) malloc(sizeof(tPROC));                                     <* 
 *>       if (xproc != NULL)     break;                                                <* 
 *>    }                                                                               <* 
 *>    if (xproc == NULL)        return NULL;                                          <* 
 *>    /+---(fill out)--------------------+/                                           <* 
 *>    xproc->seq     = -1;                                                            <* 
 *>    strncpy (xproc->name, a_name, 50);                                              <* 
 *>    xproc->type    = '-';                                                           <* 
 *>    strncpy (xproc->desc, a_name, 50);                                              <* 
 *>    xproc->user[0] = '\0';                                                          <* 
 *>    xproc->uid     = -1;                                                            <* 
 *>    xproc->run[0]  = '\0';                                                          <* 
 *>    /+---(initialize)------------------+/                                           <* 
 *>    xproc->rpid    = 0;                                                             <* 
 *>    xproc->status  = '-';                                                           <* 
 *>    xproc->start   = 0;                                                             <* 
 *>    xproc->end     = 0;                                                             <* 
 *>    xproc->u_time  = 0;                                                             <* 
 *>    xproc->s_time  = 0;                                                             <* 
 *>    xproc->swaps   = 0;                                                             <* 
 *>    xproc->rc      = 0;                                                             <* 
 *>    /+---(complete)--------------------+/                                           <* 
 *>    return xproc;                                                                   <* 
 *> }                                                                                  <*/

/*> char                                                                                           <* 
 *> CONF_parse          (void)                                                                     <* 
 *> {                                                                                              <* 
 *>    /+---(locals)-----------+-----------+-+/                                                    <* 
 *>    char        x_temp      [LEN_RECD];                                                         <* 
 *>    int         x_len       = 0;                                                                <* 
 *>    int         rc          = 0;                                                                <* 
 *>    char       *p           = NULL;                                                             <* 
 *>    char       *q           = "\x1F";                                                           <* 
 *>    char        x_name      [200];                                                              <* 
 *>    char        x_msg       [200] = "";                                                         <* 
 *>    int         x_link1     = -1;                                                               <* 
 *>    int         x_link2     = -1;                                                               <* 
 *>    tPROC      *x_proc      = NULL;                                                             <* 
 *>    int         x_item      = -1;                                                               <* 
 *>    /+---(begin)--------------------------+/                                                    <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                                   <* 
 *>    /+---(backup)-------------------------+/                                                    <* 
 *>    strncpy (x_temp, s_recd, LEN_RECD);                                                         <* 
 *>    /+> printf ("CONF_parse    : %-50.50s\n", x_temp);                                  <+/     <* 
 *>    if (s_group >= 0 && x_temp[0] == '\0' && strcmp (s_gname, "") != 0) {                       <* 
 *>       /+> sprintf (x_msg, "%s_end", s_gname);                                           <*     <* 
 *>        *> x_proc = CONF_milestone (x_msg);                                               <*    <* 
 *>        *> rc = yDLST_new    ( s_group, NULL, strdup(x_msg), x_proc);                     <+/   <* 
 *>       strcpy (s_gname, "");                                                                    <* 
 *>       return 0;                                                                                <* 
 *>    }                                                                                           <* 
 *>    /+---(parse out name)-----------------+/                                                    <* 
 *>    p = strtok (x_temp, q);                                                                     <* 
 *>    if (p == NULL) return 0;                                                                    <* 
 *>    strltrim (p, ySTR_BOTH, 150);                                                               <* 
 *>    strlcpy (x_name, p, 150);                                                                   <* 
 *>    yLOG_info   ("name"      , x_name);                                                         <* 
 *>    /+---(parse out type)-----------------+/                                                    <* 
 *>    p = strtok (NULL, q);                                                                       <* 
 *>    if (p == NULL) return 0;                                                                    <* 
 *>    strltrim (p, ySTR_BOTH, 150);                                                               <* 
 *>    x_len = strlen (p);                                                                         <* 
 *>    yLOG_info   ("type"      , p);                                                              <* 
 *>    /+---(handle groups)------------------+/                                                    <* 
 *>    if (strncmp ("GROUP", p, 5) == 0) {                                                         <* 
 *>       /+---(get the desc)----------------+/                                                    <* 
 *>       strcpy (s_gname, x_name);                                                                <* 
 *>       p = strtok (NULL, q);                                                                    <* 
 *>       if (p == NULL) return 0;                                                                 <* 
 *>       strltrim (p, ySTR_BOTH, LEN_DESC);                                                       <* 
 *>       /+---(make the main node)----------+/                                                    <* 
 *>       rc = yDLST_create (&s_group, x_name, strdup (x_name));                                   <* 
 *>       yLOG_value  ("list id"   , s_group);                                                     <* 
 *>       yLOG_info   ("called"    , x_name);                                                      <* 
 *>       /+---(make the beg node)-----------+/                                                    <* 
 *>       sprintf (x_msg, "%s_beg", s_gname);                                                      <* 
 *>       x_proc = CONF_milestone (x_msg);                                                         <* 
 *>       rc = yDLST_new    ( s_group, &s_beg, strdup(x_msg), x_proc);                             <* 
 *>       x_proc->dlst = s_beg;                                                                    <* 
 *>       yLOG_info   ("beg_name"  , x_msg);                                                       <* 
 *>       yLOG_value  ("beg_seq"   , s_beg);                                                       <* 
 *>       /+---(make the end node)-----------+/                                                    <* 
 *>       sprintf (x_msg, "%s_end", s_gname);                                                      <* 
 *>       x_proc = CONF_milestone (x_msg);                                                         <* 
 *>       rc = yDLST_new    ( s_group, &s_end, strdup(x_msg), x_proc);                             <* 
 *>       x_proc->dlst = s_end;                                                                    <* 
 *>       yLOG_info   ("end_name"  , x_msg);                                                       <* 
 *>       yLOG_value  ("end_seq"   , s_end);                                                       <* 
 *>       /+---(prepare for details)---------+/                                                    <* 
 *>       yDLST_tie (s_beg, s_end, NULL);                                                          <* 
 *>       /+---(done)------------------------+/                                                    <* 
 *>    } else if (strncmp ("AFTER", x_name, 4) == 0) {                                             <* 
 *>       yLOG_note   ("creating an AFTER link between two groups");                               <* 
 *>       /+---(get the start)---------------+/                                                    <* 
 *>       sprintf (x_msg, "%s_end", p);                                                            <* 
*>       x_link1 = yDLST_search (x_msg);                                                          <* 
*>       yLOG_info   ("beg_name"  , x_msg);                                                       <* 
*>       yLOG_value  ("beg_seq"   , x_link1);                                                     <* 
*>       /+> printf ("tie (%4d) %s  ", x_link1, x_msg);                                     <+/   <* 
*>       /+---(get the current)-------------+/                                                    <* 
*>       sprintf (x_msg, "%s_beg", s_gname);                                                      <* 
*>       x_link2 = yDLST_search (x_msg);                                                          <* 
*>       yLOG_info   ("end_name"  , x_msg);                                                       <* 
*>       yLOG_value  ("end_seq"   , x_link2);                                                     <* 
*>       /+> printf ("tie (%4d) %s  ", x_link1, x_msg);                                     <+/   <* 
*>       /+---(tie them)--------------------+/                                                    <* 
*>       if (x_link1 >= 0 && x_link2 >= 0)  yDLST_tie (x_link1, x_link2, NULL);                   <* 
*>       /+---(done)------------------------+/                                                    <* 
*>    } else {                                                                                    <* 
   *>       x_proc = CONF_new (s_nrecd, x_name, p[0]);                                               <* 
      *>       if (x_proc != NULL) {                                                                    <* 
         *>          if (s_group >= 0) rc = yDLST_new    ( s_group, &x_item, strdup (x_name), x_proc);     <* 
            *>          x_proc->dlst = x_item;                                                                <* 
            *>          yLOG_note   ("connecting new item");                                                  <* 
            *>          yDLST_tie (s_beg , x_item, NULL);                                                     <* 
            *>          yLOG_value  ("back to"   , s_beg);                                                    <* 
            *>          yDLST_tie (x_item, s_end , NULL);                                                     <* 
            *>          yLOG_value  ("fore to"   , s_end);                                                    <* 
            *>       }                                                                                        <* 
            *>    }                                                                                           <* 
            *>    /+---(complete)-----------------------+/                                                    <* 
            *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                                   <* 
            *>    return 0;                                                                                   <* 
            *> }                                                                                              <*/

            /*> char                                                                              <* 
             *> CONF_tableview      (void)                                                        <* 
             *> {                                                                                 <* 
             *>    /+---(locals)-----------+-----------+-+/                                       <* 
             *>    FILE       *f           = NULL;                                                <* 
             *>    void       *x_list      = NULL;                                                <* 
             *>    void       *x_link      = NULL;                                                <* 
             *>    char        x_header    = 'y';                                                 <* 
             *>    /+---(prepare)------------------------+/                                       <* 
             *>    f = fopen (my.file_debug, "w");                                                <* 
             *>    if (f == NULL) return -1;                                                      <* 
             *>    fprintf (f, "## eos debugging file\n");                                        <* 
             *>    fprintf (f, "##    displays linked list format of configuration file\n");      <* 
             *>    fprintf (f, "\n");                                                             <* 
             *>    /+---(list lists)---------------------+/                                       <* 
             *>    x_list = yDLST_lists (HEAD);                                                   <* 
             *>    while (x_list != NULL) {                                                       <* 
             *>       yDLST_showlist (x_header, f);                                               <* 
             *>       x_header = 'n';                                                             <* 
             *>       x_list = yDLST_lists (NEXT);                                                <* 
             *>    }                                                                              <* 
             *>    /+---(list with links)----------------+/                                       <* 
             *>    x_list = yDLST_lists (HEAD);                                                   <* 
             *>    while (x_list != NULL) {                                                       <* 
             *>       yDLST_showlist ('y', f);                                                    <* 
             *>       x_link = yDLST_list  (CURR, HEAD);                                          <* 
             *>       while (x_link != NULL) {                                                    <* 
             *>          yDLST_showlink (f);                                                      <* 
             *>          x_link = yDLST_list  (CURR, NEXT);                                       <* 
             *>       }                                                                           <* 
             *>       x_list = yDLST_lists (NEXT);                                                <* 
             *>    }                                                                              <* 
             *>    /+---(shutdown)-----------------------+/                                       <* 
             *>    fclose (f);                                                                    <* 
             *>    /+---(complete)-----------------------+/                                       <* 
             *>    return 0;                                                                      <* 
             *> }                                                                                 <* 
             *>                                                                                   <* 
             *> char                                                                              <* 
             *> CONF_list           (void)                                                        <* 
             *> {                                                                                 <* 
             *>    /+---(begin)--------------------------+/                                       <* 
             *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                      <* 
             *>    char     *xname     = NULL;                                                    <* 
             *>    tPROC    *xdata     = NULL;                                                    <* 
             *>    char     *xsave     = NULL;                                                    <* 
             *>    xname = yDLST_lists (HEAD);                                                    <* 
             *>    while (xname != NULL) {                                                        <* 
             *>       xsave = xname;                                                              <* 
             *>       DEBUG_CONF   yLOG_enter (xsave);                                            <* 
             *>       xdata = (tPROC*) yDLST_list (CURR, HEAD);                                   <* 
             *>       while (xdata != NULL) {                                                     <* 
             *>          yLOG_info  ("link",    xdata->name);                                     <* 
             *>          xdata = yDLST_list (CURR, NEXT);                                         <* 
             *>       }                                                                           <* 
             *>       DEBUG_CONF   yLOG_exit  (xsave);                                            <* 
             *>       xname = yDLST_lists (NEXT);                                                 <* 
             *>    }                                                                              <* 
             *>    /+---(complete)-----------------------+/                                       <* 
             *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                      <* 
             *>    return 0;                                                                      <* 
             *> }                                                                                 <*/

            /*> char               /+ PURPOSE : open up the configuration file ---------------+/            <* 
             *> CONF_close          (void)                                                                  <* 
             *> {                                                                                           <* 
             *>    /+---(begin)--------------------------+/                                                 <* 
             *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                                <* 
             *>    int       rc        = 0;            /+ generic return code                 +/            <* 
             *>    DEBUG_CONF   yLOG_info  ("file"      , my.name_conf);                                    <* 
             *>    /+> yLOG_value ("nprocs#"   , nprocs);                                             <+/   <* 
             *>    /+---(open)---------------------------+/                                                 <* 
             *>    if (f_conf != NULL)  rc = fclose(f_conf);                                                <* 
             *>    if (rc == 0) {                                                                           <* 
             *>       DEBUG_CONF   yLOG_info  ("status",  "file has been successfully closed");             <* 
             *>    } else {                                                                                 <* 
             *>       DEBUG_CONF   yLOG_info  ("FAILURE",  "file can't be closed");                         <* 
             *>    }                                                                                        <* 
             *>    f_conf = NULL;                                                                           <* 
             *>    /+---(complete)-----------------------+/                                                 <* 
             *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                                <* 
             *>    return 0;                                                                                <* 
             *> }                                                                                           <*/

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
