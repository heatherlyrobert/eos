/*============================[[    beg-code    ]]============================*/
#include    "eos.h"



/*> char          g_silent      [LEN_LABEL] = "";                                     <* 
 *> char          g_confirm     [LEN_LABEL] = "";                                     <* 
 *> char          g_verbose     [LEN_LABEL] = "";                                     <*/

/*> char          g_verify      [LEN_SHORT] = "";                                     <* 
 *> char          g_install     [LEN_SHORT] = "";                                     <* 
 *> char          g_list        [LEN_SHORT] = "";                                     <* 
 *> char          g_check       [LEN_SHORT] = "";                                     <* 
 *> char          g_audit       [LEN_SHORT] = "";                                     <* 
 *> char          g_fix         [LEN_SHORT] = "";                                     <* 
 *> char          g_remove      [LEN_SHORT] = "";                                     <* 
 *> char          g_daemon      [LEN_SHORT] = "";                                     <* 
 *> char          g_normal      [LEN_SHORT] = "";                                     <*/

/*> static  char  s_print       [LEN_RECD]  = "";                                     <*/

/*> #define     MAX_OPTS    20                                                        <*/
/*> typedef struct cOPTS  tOPTS;                                                                        <* 
 *> struct cOPTS {                                                                                      <* 
 *>    char        option      [LEN_LABEL];                                                             <* 
 *>    char        levels      [LEN_SHORT];                                                             <* 
 *>    char        desc        [LEN_DESC];                                                              <* 
 *>    char        file        [LEN_TERSE];  /+ eos ast hyp han +/                                      <* 
 *>    char        run_as;                                                                              <* 
 *> };                                                                                                  <* 
 *> static const tOPTS   s_opts [MAX_OPTS] = {                                                          <* 
 *>    { "eos"      , "-··", "change to eos-mode"                           , "----", IAM_EOS      },   <* 
 *>    { "astraios" , "-··", "change to astraios-mode"                      , "----", IAM_ASTRAIOS },   <* 
 *>    { "hypnos"   , "-··", "change to hypnos-mode"                        , "----", IAM_HYPNOS   },   <* 
 *>    { "heracles" , "-··", "change to heracles-mode"                      , "----", IAM_HERACLES },   <* 
 *>    { "version"  , "1··", "present a short versioning string"            , "----", '·' },            <* 
 *>    { "help"     , "2··", "present a simple help message"                , "----", '·' },            <* 
 *>    { "verify"   , "vÿV", "verify local file for correctness"            , "FFFF", '·' },            <* 
 *>    { "install"  , "iðI", "verify local file, then install centrally "   , "FFFF", '·' },            <* 
 *>    { "count"    , "l··", "count all user-applicable central files"      , "··--", '·' },            <* 
 *>    { "list"     , "L··", "list all user-applicable central files"       , "··--", '·' },            <* 
 *>    { "check"    , "cýC", "check central file for correctness"           , "--FF", '·' },            <* 
 *>    { "audit"    , "aèA", "audit central environment and all its files"  , "----", '·' },            <* 
 *>    { "fix"      , "füF", "audit central environment and fix issues"     , "----", '·' },            <* 
 *>    { "remove"   , "røR", "remove file from central location"            , "··FF", '·' },            <* 
 *>    { "daemon"   , "dëD", "execute specific file in daemon-mode"         , "····", '·' },            <* 
 *>    { "prickly"  , "p÷P", "execute specific file in prickly daemon-mode" , "····", '·' },            <* 
 *>    { "normal"   , "nôN", "execute specific file in normal-mode"         , "----", '·' },            <* 
 *>    { ""         , "···", ""                                             , "····", '·' },            <* 
 *> };                                                                                                  <*/

/*> char                                                                              <* 
 *> PROG__arg_init          (void)                                                    <* 
 *> {                                                                                 <* 
 *>    strlcpy (g_silent , "", LEN_LABEL);                                            <* 
 *>    strlcpy (g_confirm, "", LEN_LABEL);                                            <* 
 *>    strlcpy (g_verbose, "", LEN_LABEL);                                            <* 
 *>                                                                                   <* 
 *>    strlcpy (g_verify , "", LEN_SHORT);                                            <* 
 *>    strlcpy (g_install, "", LEN_SHORT);                                            <* 
 *>    strlcpy (g_list   , "", LEN_SHORT);                                            <* 
 *>    strlcpy (g_check  , "", LEN_SHORT);                                            <* 
 *>    strlcpy (g_audit  , "", LEN_SHORT);                                            <* 
 *>    strlcpy (g_fix    , "", LEN_SHORT);                                            <* 
 *>    strlcpy (g_remove , "", LEN_SHORT);                                            <* 
 *>    strlcpy (g_daemon , "", LEN_SHORT);                                            <* 
 *>    strlcpy (g_normal , "", LEN_SHORT);                                            <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> PROG__arg_single        (char *a_levels, char n)                                  <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        c           =  '·';                                                <* 
 *>    char        t           [LEN_SHORT] = "";                                      <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    --rce;  if (a_levels == NULL)              return rce;                         <* 
 *>    --rce;  if (strlen (a_levels) != 3)        return rce;                         <* 
 *>    --rce;  if (n        <  0)                 return rce;                         <* 
 *>    --rce;  if (n        >  2)                 return rce;                         <* 
 *>    /+---(get current)--------------------+/                                       <* 
 *>    c = a_levels [n];                                                              <* 
 *>    if (c == '·')                      return 0;                                   <* 
 *>    if (c == ' ')                      return 0;                                   <* 
 *>    if (c == '-')                      return 0;                                   <* 
 *>    /+---(can not be in any place)--------+/                                       <* 
 *>    --rce;  if (strchr (g_silent , c) != NULL) return rce;                         <* 
 *>    --rce;  if (strchr (g_confirm, c) != NULL) return rce;                         <* 
 *>    --rce;  if (strchr (g_verbose, c) != NULL) return rce;                         <* 
 *>    /+---(load correctly)-----------------+/                                       <* 
 *>    sprintf (t, "%c", c);                                                          <* 
 *>    switch (n) {                                                                   <* 
 *>    case  0  :   strlcat (g_silent , t, LEN_LABEL);   break;                       <* 
 *>    case  1  :   strlcat (g_confirm, t, LEN_LABEL);   break;                       <* 
 *>    case  2  :   strlcat (g_verbose, t, LEN_LABEL);   break;                       <* 
 *>    }                                                                              <* 
 *>    /+---(load by action)-----------------+/                                       <* 
 *>    switch (a_levels [0]) {                                                        <* 
 *>    case 'v' :   strlcat (g_verify , t, LEN_SHORT);   break;                       <* 
 *>    case 'i' :   strlcat (g_install, t, LEN_SHORT);   break;                       <* 
 *>    case 'l' :   strlcat (g_list   , t, LEN_SHORT);   break;                       <* 
 *>    case 'L' :   strlcat (g_list   , t, LEN_SHORT);   break;                       <* 
 *>    case 'c' :   strlcat (g_check  , t, LEN_SHORT);   break;                       <* 
 *>    case 'a' :   strlcat (g_audit  , t, LEN_SHORT);   break;                       <* 
 *>    case 'f' :   strlcat (g_fix    , t, LEN_SHORT);   break;                       <* 
 *>    case 'r' :   strlcat (g_remove , t, LEN_SHORT);   break;                       <* 
 *>    case 'd' :   strlcat (g_daemon , t, LEN_SHORT);   break;                       <* 
 *>    case 'n' :   strlcat (g_normal , t, LEN_SHORT);   break;                       <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> PROG__arg_load          (void)                                                    <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rc          =    0;                                                <* 
 *>    int         i           =    0;                                                <* 
 *>    char        c           =  '·';                                                <* 
 *>    char        t           [LEN_SHORT] = "";                                      <* 
 *>    char       *p           = NULL;                                                <* 
 *>    PROG__arg_init ();                                                             <* 
 *>    for (i = 0; i < MAX_OPTS; ++i) {                                               <* 
 *>       p = s_opts [i].levels;                                                      <* 
 *>       if (p [0] == '\0')  break;                                                  <* 
 *>       if (rc >= 0)  rc = PROG__arg_single (p, 0);                                 <* 
 *>       if (rc >= 0)  rc = PROG__arg_single (p, 1);                                 <* 
 *>       if (rc >= 0)  rc = PROG__arg_single (p, 2);                                 <* 
 *>       if (rc <  0)  break;                                                        <* 
 *>    }                                                                              <* 
 *>    return rc;                                                                     <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> PROG__arg_clearmode     (void)                                                    <* 
 *> {                                                                                 <* 
 *>    my.run_mode = '-';                                                             <* 
 *>    strlcpy (my.run_file , "", LEN_PATH);                                          <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                                               <* 
 *> PROG__arg_handle        (int *i, char *a_arg, char *a_next)                                        <* 
 *> {                                                                                                  <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                        <* 
 *>    char        rce         =  -10;                                                                 <* 
 *>    char        rc          =    0;                                                                 <* 
 *>    int         j           =    0;                                                                 <* 
 *>    char       *p           = NULL;                                                                 <* 
 *>    char        c           =  '·';                                                                 <* 
 *>    char        f           =  '·';                                                                 <* 
 *>    /+---(defense)------------------------+/                                                        <* 
 *>    --rce;  if (i     == NULL)              return rce;                                             <* 
 *>    --rce;  if (a_arg == NULL)              return rce;                                             <* 
 *>    --rce;  if (strlen (a_arg) <= 4) {                                                              <* 
 *>       yURG_err ('F', "option å%sæ too short, must be > 4 chars", a_arg);                           <* 
 *>       return rce;                                                                                  <* 
 *>    }                                                                                               <* 
 *>    --rce;  if (strncmp (a_arg, "--", 2) != 0) {                                                    <* 
 *>       yURG_err ('F', "option å%sæ must start with the prefix å--æ", a_arg);                        <* 
 *>       return rce;                                                                                  <* 
 *>    }                                                                                               <* 
 *>    --rce;  if (my.run_mode != '-') {                                                               <* 
 *>       yURG_err ('F', "run action already set (%c), can not update to å%sæ", my.run_mode, a_arg);   <* 
 *>       return rce;                                                                                  <* 
 *>    }                                                                                               <* 
 *>    /+---(walk options)-------------------+/                                                        <* 
 *>    for (j = 0; j < MAX_OPTS; ++j) {                                                                <* 
 *>       p = s_opts [j].option;                                                                       <* 
 *>       if (p [0] == '\0')  break;                                                                   <* 
 *>       if (strcmp (a_arg + 2, p) == 0) {                                                            <* 
 *>          c = s_opts [j].levels [0];                                                                <* 
 *>          break;                                                                                    <* 
 *>       }                                                                                            <* 
 *>       if (a_arg [2] == 'c' && strcmp (a_arg + 3, p) == 0) {                                        <* 
 *>          c = s_opts [j].levels [1];                                                                <* 
 *>          break;                                                                                    <* 
 *>       }                                                                                            <* 
 *>       if (a_arg [2] == 'v' && strcmp (a_arg + 3, p) == 0) {                                        <* 
 *>          c = s_opts [j].levels [2];                                                                <* 
 *>          break;                                                                                    <* 
 *>       }                                                                                            <* 
 *>    }                                                                                               <* 
 *>    /+---(handle failure)-----------------+/                                                        <* 
 *>    --rce;  if (c == '·') {                                                                         <* 
 *>       yURG_err ('F', "option å%sæ not found in database", a_arg);                                  <* 
 *>       return rce;                                                                                  <* 
 *>    }                                                                                               <* 
 *>    /+---(handle runas changes)-----------+/                                                        <* 
 *>    --rce;  if (c == '-') {                                                                         <* 
 *>       if (s_opts [j].run_as == '·') {                                                              <* 
 *>          yURG_err ('F', "option å%sæ used as a mode change, but not legal", a_arg);                <* 
 *>          return rce;                                                                               <* 
 *>       }                                                                                            <* 
 *>       my.run_as = s_opts [j].run_as;                                                               <* 
 *>       PROG__arg_clearmode ();                                                                      <* 
 *>       return 2;                                                                                    <* 
 *>    }                                                                                               <* 
 *>    /+---(handle run mode changes)--------+/                                                        <* 
 *>    my.run_mode = c;                                                                                <* 
 *>    /+---(get file flag)------------------+/                                                        <* 
 *>    switch (my.run_as) {                                                                            <* 
 *>    case IAM_EOS      : f = s_opts [j].file [0];        break;                                      <* 
 *>    case IAM_ASTRAIOS : f = s_opts [j].file [1];        break;                                      <* 
 *>    case IAM_HYPNOS   : f = s_opts [j].file [2];        break;                                      <* 
 *>    case IAM_HERACLES : f = s_opts [j].file [3];        break;                                      <* 
 *>    }                                                                                               <* 
 *>    /+---(handle option not allowed)------+/                                                        <* 
 *>    --rce;  if (f == '·') {                                                                         <* 
 *>       yURG_err ('F', "action å%sæ not allowed for run-as (%c)", a_arg, my.run_as);                 <* 
 *>       PROG__arg_clearmode ();                                                                      <* 
 *>       return rce;                                                                                  <* 
 *>    }                                                                                               <* 
 *>    /+---(handle simple option)-----------+/                                                        <* 
 *>    if (f == '-')    return 0;                                                                      <* 
 *>    /+---(handle file flag)---------------+/                                                        <* 
 *>    --rce;  if (f != 'F') {                                                                         <* 
 *>       yURG_err ('F', "action å%sæ not configured correctly", a_arg);                               <* 
 *>       PROG__arg_clearmode ();                                                                      <* 
 *>       return rce;                                                                                  <* 
 *>    }                                                                                               <* 
 *>    --rce;  if (a_next == NULL) {                                                                   <* 
 *>       yURG_err ('F', "action å%sæ requires a file name as an argument", a_arg);                    <* 
 *>       PROG__arg_clearmode ();                                                                      <* 
 *>       return rce;                                                                                  <* 
 *>    }                                                                                               <* 
 *>    strlcpy (my.run_file, a_next, LEN_PATH);                                                        <* 
 *>    ++(*i);                                                                                         <* 
 *>    /+---(complete)-----------------------+/                                                        <* 
 *>    return 1;                                                                                       <* 
 *> }                                                                                                  <*/



/*====================------------------------------------====================*/
/*===----                      supporting functions                    ----===*/
/*====================------------------------------------====================*/
static void      o___SUUPPORT________________o (void) {;}

char        verstring    [500];

char*        /*--: return versioning information ---------[ leaf-- [ ------ ]-*/
PROG_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strlcpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strlcpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   strlcpy (t, "[cbang      ]", 15);
#elif  __HEPH__  > 0
   strlcpy (t, "[hephaestus ]", 18);
#else
   strlcpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

char       /* PURPOSE : display usage help information -----------------------*/
PROG__usage             (void)
{
   printf ("see man pages for a better understanding of eos...\n");
   printf ("  man 1 eos    command-line initiation, use, and options\n");
   printf ("  man 5 eos    structure of config, files, and streams\n");
   printf ("  man 7 eos    decision rationale, scope, and objectives\n");
   exit   (0);
}

/*> char*                                                                             <* 
 *> PROG_iam           (char a_iam)                                                   <* 
 *> {                                                                                 <* 
 *>    switch (a_iam) {                                                               <* 
 *>    case IAM_EOS          :                                                        <* 
 *>       return "eos-rhododactylos (rosy-fingered dawn)";                            <* 
 *>       break;                                                                      <* 
 *>    case IAM_ASTRAIOS     :                                                        <* 
 *>       return "astraios-aeolus (sparkling wind father)";                           <* 
 *>       break;                                                                      <* 
 *>    case IAM_HYPNOS       :                                                        <* 
 *>       return "hypnos-epidotes (giver of sleep)";                                  <* 
 *>       break;                                                                      <* 
 *>    case IAM_HERACLES     :                                                        <* 
 *>       return "heracles-promachus (leader in battle)";                             <* 
 *>       break;                                                                      <* 
 *>    default               :                                                        <* 
 *>       return "unknown";                                                           <* 
 *>       break;                                                                      <* 
 *>    }                                                                              <* 
 *>    return "eh?";                                                                  <* 
 *> }                                                                                 <*/

/*> char*                                                                             <* 
 *> PROG_runmode       (char a_run)                                                   <* 
 *> {                                                                                 <* 
 *>    switch (a_run) {                                                               <* 
 *>    case CASE_VERIFY  :                                                            <* 
 *>       strcpy (s_print, "verify");                                                 <* 
 *>       break;                                                                      <* 
 *>    case CASE_INSTALL :                                                            <* 
 *>       strcpy (s_print, "install");                                                <* 
 *>       break;                                                                      <* 
 *>    case CASE_LIST    :                                                            <* 
 *>       strcpy (s_print, "list");                                                   <* 
 *>       break;                                                                      <* 
 *>    case CASE_CHECK   :                                                            <* 
 *>       strcpy (s_print, "check");                                                  <* 
 *>       break;                                                                      <* 
 *>    case CASE_AUDIT   :                                                            <* 
 *>       strcpy (s_print, "audit");                                                  <* 
 *>       break;                                                                      <* 
 *>    case CASE_FIX     :                                                            <* 
 *>       strcpy (s_print, "fix");                                                    <* 
 *>       break;                                                                      <* 
 *>    case CASE_DAEMON  :                                                            <* 
 *>       strcpy (s_print, "daemon");                                                 <* 
 *>       break;                                                                      <* 
 *>    case CASE_NORMAL  :                                                            <* 
 *>       strcpy (s_print, "normal");                                                 <* 
 *>       break;                                                                      <* 
 *>    default           :                                                            <* 
 *>       strcpy (s_print, "unknown");                                                <* 
 *>       break;                                                                      <* 
 *>    }                                                                              <* 
 *>    IF_SILENT         strcat (s_print, " (silent)");                               <* 
 *>    else IF_CONFIRM   strcat (s_print, " (confirm)");                              <* 
 *>    else IF_VERBOSE   strcat (s_print, " (verbose)");                              <* 
 *>    else              strcat (s_print, " (unknown)");                              <* 
 *>    return s_print;                                                                <* 
 *> }                                                                                 <*/



/*====================------------------------------------====================*/
/*===----                     pre-startup functions                    ----===*/
/*====================------------------------------------====================*/
static void      o___PRESTART________________o (void) {;}

char
PROG__verbose           (int a_argc, char *a_argv[], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        x_loud      =  '-';
   /*---(default)------------------------*/
   my.verbose = '-';
   yURG_msg_std ();  yURG_msg_mute ();
   yURG_err_std ();  yURG_err_mute ();
   /*---(handle unit test)---------------*/
   if (a_unit == 'y' || a_unit == 'k') {
      yURG_msg_tmp ();
      yURG_err_tmp ();
   }
   /*---(check for pre-loud)-------------*/
   for (i = 1; i < a_argc; ++i) {
      /*> printf (", %s", a_argv [i]);                                                <*/
      if (a_argv [i][0] != '-')       continue;
      if (a_argv [i][1] != '-')       continue;
      if (strcmp (a_argv [i], "--vdaemon" ) == 0)   x_loud = 'y';
   }
   /*---(set loud)-----------------------*/
   if (x_loud == 'y' && getpid () == 1) {
      yURG_msg_live ();
      my.verbose = 'y';
   }
   yURG_err_live ();
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--> detirmine behavior --------------------[ leaf-- [ ------ ]-*/
PROG__runas             (int a_argc, char *a_argv[])
{
   char        s           [LEN_HUND]  = "";
   /*---(defaults)-----------------------*/
   my.run_as         = '-';
   my.run_mode       = '-';
   /*---(specific)-----------------------*/
   yURG_msg ('>', "%s %s", P_NAMESAKE, P_SUBJECT);
   /* must also trap _debug versions */
   if      (strncmp (a_argv [0], "eos"           ,  3) == 0)    my.run_as = IAM_EOS;
   else if (strncmp (a_argv [0], "/sbin/eos"     ,  9) == 0)    my.run_as = IAM_EOS;
   else if (strcmp  (a_argv [0], "init"              ) == 0)    my.run_as = IAM_EOS;
   else if (strncmp (a_argv [0], "astraios"      ,  8) == 0)    my.run_as = IAM_ASTRAIOS;
   else if (strncmp (a_argv [0], "/sbin/astraios", 14) == 0)    my.run_as = IAM_ASTRAIOS;
   else if (strcmp  (a_argv [0], "shutdown"          ) == 0)    my.run_as = IAM_ASTRAIOS;
   else if (strcmp  (a_argv [0], "halt"              ) == 0)    my.run_as = IAM_ASTRAIOS;
   else if (strcmp  (a_argv [0], "restart"           ) == 0)    my.run_as = IAM_ASTRAIOS;
   else if (strncmp (a_argv [0], "hypnos"        ,  6) == 0)    my.run_as = IAM_HYPNOS;
   else if (strncmp (a_argv [0], "/sbin/hypnos"  , 12) == 0)    my.run_as = IAM_HYPNOS;
   else if (strncmp (a_argv [0], "heracles"      ,  8) == 0)    my.run_as = IAM_HERACLES;
   else if (strncmp (a_argv [0], "/sbin/heracles", 14) == 0)    my.run_as = IAM_HERACLES;
   else if (strcmp  (a_argv [0], "H"                 ) == 0)    my.run_as = IAM_HERACLES;
   else if (strcmp  (a_argv [0], "/sbin/H"           ) == 0)    my.run_as = IAM_HERACLES;
   else {
      return -1;
   }
   yJOBS_iam (my.run_as, s);
   yURG_msg ('-', "run as (%c) %s", my.run_as, s);
   if (strcmp (my.n_conf, "") == 0) {
      switch (my.run_as) {
      case IAM_EOS       : snprintf (my.n_conf   , 200, "%seos%s"     , DIR_ETC , FILE_CONF); break;
      case IAM_UEOS      : snprintf (my.n_conf   , 200, "%seos%s"     , DIR_UNIT, FILE_CONF); break;
      case IAM_ASTRAIOS  : snprintf (my.n_conf   , 200, "%sastraios%s", DIR_ETC , FILE_CONF); break;
      case IAM_UASTRAIOS : snprintf (my.n_conf   , 200, "%sastraios%s", DIR_UNIT, FILE_CONF); break;
      case IAM_HYPNOS    : snprintf (my.n_conf   , 200, "%shypnos%s"  , DIR_ETC , FILE_CONF); break;
      case IAM_UHYPNOS   : snprintf (my.n_conf   , 200, "%shypnos%s"  , DIR_UNIT, FILE_CONF); break;
      }
   }
   yURG_msg ('-', "conf å%sæ", my.n_conf);
   if (strcmp (my.n_exec, "") == 0) {
      switch (my.run_as) {
      case IAM_EOS       :
      case IAM_UEOS      : snprintf (my.n_exec   , 200, "%seos%s"     , DIR_YLOG, FILE_EXEC); break;
      case IAM_ASTRAIOS  :
      case IAM_UASTRAIOS : snprintf (my.n_exec   , 200, "%sastraios%s", DIR_YLOG, FILE_EXEC); break;
      case IAM_HYPNOS    :
      case IAM_UHYPNOS   : snprintf (my.n_exec   , 200, "%shypnos%s"  , DIR_YLOG, FILE_EXEC); break;
      case IAM_HERACLES  :
      case IAM_UHERACLES : snprintf (my.n_exec   , 200, "%sheracles%s", DIR_YLOG, FILE_EXEC); break;
      }
   }
   yURG_msg ('-', "exec å%sæ", my.n_exec);
   if (strcmp (my.n_perf, "") == 0) {
      switch (my.run_as) {
      case IAM_EOS       :
      case IAM_UEOS      : snprintf (my.n_perf   , 200, "%seos%s"     , DIR_YLOG, FILE_PERF); break;
      case IAM_ASTRAIOS  :
      case IAM_UASTRAIOS : snprintf (my.n_perf   , 200, "%sastraios%s", DIR_YLOG, FILE_PERF); break;
      case IAM_HYPNOS    :
      case IAM_UHYPNOS   : snprintf (my.n_perf   , 200, "%shypnos%s"  , DIR_YLOG, FILE_PERF); break;
      case IAM_HERACLES  :
      case IAM_UHERACLES : snprintf (my.n_perf   , 200, "%sheracles%s", DIR_YLOG, FILE_PERF); break;
      }
   }
   yURG_msg ('-', "perf å%sæ", my.n_perf);
   yURG_msg (' ', "");
   DEBUG_TOPS   yLOG_info    ("conf"      , my.n_conf);
   DEBUG_TOPS   yLOG_info    ("exec"      , my.n_exec);
   DEBUG_TOPS   yLOG_info    ("perf"      , my.n_perf);
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--> before even logger --------------------[ leaf-- [ ------ ]-*/
PROG__boot              (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         i           =    0;
   char       *a           = NULL;          /* current argument               */
   FILE       *f           = NULL;          /* generic file pointer           */
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   int         c           =    0;
   int         x_tries     =    0;
   char        x_ylog      =  '-';
   /*---(quick out)----------------------*/
   for (i = 1; i < a_argc; ++i) {
      a = a_argv[i];
      if (a == NULL) return rce;
      if (a[0] == '@')       continue;
      if (strcmp (a, "--eos") == 0)  my.run_as    = IAM_EOS;
   }
   if (my.run_as != IAM_EOS) return 0;
   if (getpid () != 1)       return 0;
   /*---(remount /)----------------------*/
   yURG_msg ('>', "eos boot preparation...");
   rc = mount ("/dev/sda4", "/", "ext4", MS_REMOUNT | MS_NOATIME, NULL);
   yURG_msg ('-', "remounted /dev/sda4 (%d)", rc);
   /*---(check on /proc)-----------------*/
   /*> mkdir ("/proc", 0755);                                                         <*/
   while (1) {
      f = fopen ("/proc/mounts", "rt");
      if (f != NULL)    break;
      if (x_tries > 3)  break;
      rc = mount ("proc"  , "/proc", "proc", 0, NULL);
      ++x_tries;
   }
   --rce;  if (f == NULL) {
      yURG_err ('f', "/proc could not be mounted in 3 tries");
      return rce;
   }
   if (x_tries == 0)  yURG_msg ('-', "/proc already successfully mounted");
   else               yURG_msg ('-', "/proc successfully mounted after (%d) tries", x_tries);
   /*---(show /proc/mounts)--------------*/
   while (1) {
      fgets (x_recd, 450, f);
      if (feof (f))  break;
      x_len = strlen (x_recd);
      if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
      /*> printf ("%2d [%s]\n", c, x_recd);                                           <*/
      ++c;
      if (strstr (x_recd, "yLOG") != NULL) {
         yURG_msg ('-', "/var/log/yLOG already successfully mounted");
         x_ylog = 'y';
      }
   }
   fclose (f);
   if (x_ylog == '-')  yURG_msg ('-', "/var/log/yLOG not yet mounted");
   /*---(check on yLOG)------------------*/
   if (x_ylog != 'y') {
      rc = mount ("varlog", "/var/log/yLOG", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME, "size=500m");
      yURG_msg ('-', "/var/log/yLOG mounted successfully (%d)", rc);
   }
   yURG_msg ('-', "");
   /*---(close)--------------------------*/
   return 0;
}

char
PROG_prestart           (int a_argc, char *a_argv[], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(startup)------------------------*/
   if (rc >= 0)  rc = PROG__verbose   (a_argc, a_argv, a_unit);
   if (rc >= 0)  rc = PROG__runas     (a_argc, a_argv);
   if (rc >= 0)  rc = PROG__boot      (a_argc, a_argv);
   DEBUG_TOPS  yLOG_value   ("prestart"  , rc);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                    debugging startup                         ----===*/
/*====================------------------------------------====================*/
static void      o___DEBUGGING_______________o (void) {;}

char
PROG_debugging          (int a_argc, char *a_argv[], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(startup)------------------------*/
   if (rc >= 0)  rc = yURG_logger  (a_argc, a_argv);
   if (rc >= 0)  rc = yURG_urgs    (a_argc, a_argv);
   DEBUG_TOPS  yLOG_value   ("debugging" , rc);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      startup functions                       ----===*/
/*====================------------------------------------====================*/
static void      o___STARTUP_________________o (void) {;}

char         /*--: pre-argument initialization -----------[ leaf-- [ ------ ]-*/
PROG__init              (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   FILE       *f           = NULL;          /* generic file pointer           */
   int         x_pos       =   -1;
   /*---(log header)---------------------*/
   DEBUG_TOPS   yLOG_info    ("namesake", P_NAMESAKE);
   DEBUG_TOPS   yLOG_info    ("heritage", P_HERITAGE);
   DEBUG_TOPS   yLOG_info    ("imagery" , P_IMAGERY);
   DEBUG_TOPS   yLOG_info    ("purpose" , P_PURPOSE);
   DEBUG_TOPS   yLOG_note    ("BASE SOFTWARE");
   DEBUG_TOPS   yLOG_info    ("eos"     , PROG_version    ());
   DEBUG_TOPS   yLOG_note    ("EVERYWHERE LIBRARIES");
   DEBUG_TOPS   yLOG_info    ("yLOG"    , yLOGS_version   ());
   DEBUG_TOPS   yLOG_info    ("yURG"    , yURG_version    ());
   DEBUG_TOPS   yLOG_note    ("COMMON LIBRARIES");
   DEBUG_TOPS   yLOG_info    ("ySTR"    , ySTR_version    ());
   DEBUG_TOPS   yLOG_info    ("yPARSE"  , yPARSE_version  ());
   DEBUG_TOPS   yLOG_note    ("SPECIFIC LIBRARIES");
   DEBUG_TOPS   yLOG_info    ("yDLST"   , yDLST_version   ());
   DEBUG_TOPS   yLOG_info    ("yEXEC"   , yEXEC_version   ());
   DEBUG_TOPS   yLOG_info    ("yJOBS"   , yJOBS_version   ());
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "program initialization...");
   /*---(set globals)--------------------*/
   yURG_msg ('-', "set defaults for major globals");
   my.done_done      = '-';
   my.test           = '-';
   my.loop_msec      = 100;
   my.loop_max       = my.loop_msec * 10 * 240; /* four minutes */
   strlcpy (my.dev, "/dev/tty1", LEN_LABEL);
   /*> PROG__arg_load ();                                                             <*/
   /*---(call whoami)--------------------*/
   rc = yEXEC_whoami (&my.pid, &my.ppid, &my.m_uid, NULL, &my.m_who, 'n');
   DEBUG_TOPS   yLOG_value   ("whoami"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "yEXEC_whoami failed (%d)", rc);
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "yEXEC_whoami returned %d pid, %d uid, %s user", my.pid, my.m_uid, my.m_who);
   DEBUG_TOPS   yLOG_value   ("pid"       , my.pid);
   DEBUG_TOPS   yLOG_value   ("ppid"      , my.ppid);
   DEBUG_TOPS   yLOG_value   ("uid"       , my.m_uid);
   DEBUG_TOPS   yLOG_info    ("who"       , my.m_who);
   yURG_msg ('-', "");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG__setaction         (cchar a_act, cchar *a_file)
{
   if (my.run_mode != '-')  {
      yURG_err ('F', "can not set multiple actions.  already %c, requested %c", my.run_mode, a_act);
      return -98;
   }
   my.run_mode = a_act;
   strlcpy (my.run_file, a_file, LEN_PATH);
   /*> printf ("s_act = %c, s_file = %s\n", s_act, s_file);                           <*/
   return 0;
}

#define    TWOARG         if (x_two == 1)
#define    ELSEONE(r)     else { yURG_err ('F', "action å%sæ requires a file name as an argument", a);  rc = r; }

/*> char         /+--: evaluate command line arguments -------[ leaf   [ ------ ]-+/                                                 <* 
 *> PROG__args_OLD          (int a_argc, char *a_argv[])                                                                             <* 
 *> {                                                                                                                                <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                                      <* 
 *>    char        rce         =  -10;                                                                                               <* 
 *>    char        rc          =    0;                                                                                               <* 
 *>    int         i           =    0;                                                                                               <* 
 *>    char       *a           = NULL;          /+ current argument               +/                                                 <* 
 *>    int         x_len       =    0;          /+ argument length                +/                                                 <* 
 *>    int         x_total     =    0;          /+ total argument count           +/                                                 <* 
 *>    int         x_args      =    0;          /+ argument count                 +/                                                 <* 
 *>    int         x_num       =    0;          /+ numeric argument holder        +/                                                 <* 
 *>    char        x_two       =    0;                                                                                               <* 
 *>    /+---(process)------------------------+/                                                                                      <* 
 *>    DEBUG_TOPS   yLOG_enter   (__FUNCTION__);                                                                                     <* 
 *>    yURG_msg ('>', "command line arguments handling...");                                                                         <* 
 *>    yURG_msg ('-', "total of %d arguments, including name", a_argc);                                                              <* 
 *>    /+---(defaults)------------------------------+/                                                                               <* 
 *>    my.run_mode = '-';                                                                                                            <* 
 *>    strlcpy (my.run_file, "", LEN_PATH);                                                                                          <* 
 *>    /+---(walk args)-----------------------------+/                                                                               <* 
 *>    for (i = 1; i < a_argc; ++i) {                                                                                                <* 
 *>       /+---(prepare)---------------------+/                                                                                      <* 
 *>       a = a_argv[i];                                                                                                             <* 
 *>       if (a == NULL) {                                                                                                           <* 
 *>          yURG_err ('f', "arg %d is NULL", i);                                                                                    <* 
 *>          DEBUG_TOPS   yLOG_note    ("FATAL, found a null argument, really bad news");                                            <* 
 *>          DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);                                                                          <* 
 *>          return rce;                                                                                                             <* 
 *>       }                                                                                                                          <* 
 *>       /+---(debugging--------------------+/                                                                                      <* 
 *>       if (a[0] == '@')       continue;                                                                                           <* 
 *>       /+---(two arg check)---------------+/                                                                                      <* 
 *>       ++x_args;                                                                                                                  <* 
 *>       DEBUG_ARGS  yLOG_info     ("argument"  , a);                                                                               <* 
 *>       if (i < a_argc - 1) x_two = 1; else x_two = 0;                                                                             <* 
 *>       if (x_two == 0)  yURG_msg ('-', "%d = %s, single arg only", i, a);                                                         <* 
 *>       else             yURG_msg ('-', "%d = %s, possible two arg", i, a);                                                        <* 
 *>       /+---(run as)----------------------+/                                                                                      <* 
 *>       if      (strcmp (a, "--eos"       ) == 0)  my.run_as    = IAM_EOS;                                                         <* 
 *>       else if (strcmp (a, "--astraios"  ) == 0)  my.run_as    = IAM_ASTRAIOS;                                                    <* 
 *>       else if (strcmp (a, "--hypnos"    ) == 0)  my.run_as    = IAM_HYPNOS;                                                      <* 
 *>       else if (strcmp (a, "--heracles"  ) == 0)  my.run_as    = IAM_HERACLES;                                                    <* 
 *>       /+---(verify local)----------------+/                                                                                      <* 
 *>       else if (strcmp (a, "--verify"    ) == 0)  { TWOARG  rc = PROG__setaction (ACT_VERIFY  , a_argv [++i]); ELSEONE (-1);  }   <* 
 *>       else if (strcmp (a, "--cverify"   ) == 0)  { TWOARG  rc = PROG__setaction (ACT_CVERIFY , a_argv [++i]); ELSEONE (-2);  }   <* 
 *>       else if (strcmp (a, "--vverify"   ) == 0)  { TWOARG  rc = PROG__setaction (ACT_VVERIFY , a_argv [++i]); ELSEONE (-3);  }   <* 
 *>       /+---(incomming)-------------------+/                                                                                      <* 
 *>       else if (strcmp (a, "--install"   ) == 0)  { TWOARG  rc = PROG__setaction (ACT_INSTALL , a_argv [++i]); ELSEONE (-4);  }   <* 
 *>       else if (strcmp (a, "--cinstall"  ) == 0)  { TWOARG  rc = PROG__setaction (ACT_CINSTALL, a_argv [++i]); ELSEONE (-5);  }   <* 
 *>       else if (strcmp (a, "--vinstall"  ) == 0)  { TWOARG  rc = PROG__setaction (ACT_VINSTALL, a_argv [++i]); ELSEONE (-6);  }   <* 
 *>       /+---(outgoing)--------------------+/                                                                                      <* 
 *>       else if (strcmp (a, "--remove"    ) == 0)  { TWOARG  rc = PROG__setaction (ACT_REMOVE  , a_argv [++i]); ELSEONE (-7);  }   <* 
 *>       else if (strcmp (a, "--cremove"   ) == 0)  { TWOARG  rc = PROG__setaction (ACT_CREMOVE , a_argv [++i]); ELSEONE (-8);  }   <* 
 *>       else if (strcmp (a, "--vremove"   ) == 0)  { TWOARG  rc = PROG__setaction (ACT_VREMOVE , a_argv [++i]); ELSEONE (-9);  }   <* 
 *>       /+---(check installed)-------------+/                                                                                      <* 
 *>       else if (strcmp (a, "--count"    ) == 0)   { PROG__setaction (ACT_COUNT   , ""); }                                         <* 
 *>       else if (strcmp (a, "--list"     ) == 0)   { PROG__setaction (ACT_LIST    , ""); }                                         <* 
 *>       else if (strcmp (a, "--check"     ) == 0)  { TWOARG  rc = PROG__setaction (ACT_CHECK   , a_argv [++i]); ELSEONE (-10); }   <* 
 *>       else if (strcmp (a, "--ccheck"    ) == 0)  { TWOARG  rc = PROG__setaction (ACT_CCHECK  , a_argv [++i]); ELSEONE (-11); }   <* 
 *>       else if (strcmp (a, "--vcheck"    ) == 0)  { TWOARG  rc = PROG__setaction (ACT_VCHECK  , a_argv [++i]); ELSEONE (-12); }   <* 
 *>       /+---(check security)--------------+/                                                                                      <* 
 *>       else if (strcmp (a, "--audit"     ) == 0)  { PROG__setaction (ACT_AUDIT   , ""); }                                         <* 
 *>       else if (strcmp (a, "--caudit"    ) == 0)  { PROG__setaction (ACT_CAUDIT  , ""); }                                         <* 
 *>       else if (strcmp (a, "--vaudit"    ) == 0)  { PROG__setaction (ACT_VAUDIT  , ""); }                                         <* 
 *>       /+---(fix implementation-----------+/                                                                                      <* 
 *>       else if (strcmp (a, "--fix"       ) == 0)  { PROG__setaction (ACT_FIX     , ""); }                                         <* 
 *>       else if (strcmp (a, "--cfix"      ) == 0)  { PROG__setaction (ACT_CFIX    , ""); }                                         <* 
 *>       else if (strcmp (a, "--vfix"      ) == 0)  { PROG__setaction (ACT_VFIX    , ""); }                                         <* 
 *>       /+---(daemon)----------------------+/                                                                                      <* 
 *>       else if (strcmp (a, "--daemon"    ) == 0)  { PROG__setaction (ACT_DAEMON  , ""); }                                         <* 
 *>       else if (strcmp (a, "--cdaemon"   ) == 0)  { PROG__setaction (ACT_CDAEMON , ""); }                                         <* 
 *>       else if (strcmp (a, "--vdaemon"   ) == 0)  { PROG__setaction (ACT_VDAEMON , ""); }                                         <* 
 *>       /+---(normal)----------------------+/                                                                                      <* 
 *>       else if (strcmp (a, "--normal"    ) == 0)  { PROG__setaction (ACT_NORMAL  , ""); }                                         <* 
 *>       else if (strcmp (a, "--cnormal"   ) == 0)  { PROG__setaction (ACT_CNORMAL , ""); }                                         <* 
 *>       else if (strcmp (a, "--vnormal"   ) == 0)  { PROG__setaction (ACT_VNORMAL , ""); }                                         <* 
 *>       /+---(reporting)-------------------+/                                                                                      <* 
 *>       else if (strcmp (a, "--show_verb" ) == 0)  my.run_mode  = EOS_RPTG_VERBS;                                                  <* 
 *>       else if (strcmp (a, "--show_cntl" ) == 0)  my.run_mode  = EOS_RPTG_CONTROL;                                                <* 
 *>       /+---(files)-----------------------+/                                                                                      <* 
 *>       /+> else if (strcmp (a, "--conf"      ) == 0)  { TWOARG rc = BASE_file_cli ("conf", a_argv [i]);  } <*                     <* 
 *>        *> else if (strcmp (a, "--exec"      ) == 0)  { TWOARG rc = BASE_file_cli ("exec", a_argv [i]);  } <*                     <* 
 *>        *> else if (strcmp (a, "--perf"      ) == 0)  { TWOARG rc = BASE_file_cli ("perf", a_argv [i]);  } <+/                    <* 
 *>       /+---(other)-----------------------+/                                                                                      <* 
 *>       else if (strcmp (a, "--help"      ) == 0)  PROG__usage ();                                                                 <* 
 *>       /+---(speeds)----------------------+/                                                                                      <* 
 *>       else if (strcmp(a, "--normal"      ) == 0) {                                                                               <* 
 *>          my.loop_msec  = 10;                                                                                                     <* 
 *>          my.loop_max   = 1000;                                                                                                   <* 
 *>       }                                                                                                                          <* 
 *>       else if (strcmp(a, "--fast"        ) == 0) {                                                                               <* 
 *>          my.loop_msec  = 1;                                                                                                      <* 
 *>          my.loop_max   = 10000;                                                                                                  <* 
 *>       }                                                                                                                          <* 
 *>       else if (strcmp(a, "--slow"        ) == 0) {                                                                               <* 
 *>          my.loop_msec  = 100;                                                                                                    <* 
 *>          my.loop_max   = 100;                                                                                                    <* 
 *>       }                                                                                                                          <* 
 *>       else if (strcmp(a, "--short"       ) == 0) {                                                                               <* 
 *>          my.loop_msec  = 100;                                                                                                    <* 
 *>          my.loop_max   = 20;                                                                                                     <* 
 *>       }                                                                                                                          <* 
 *>       /+---(unknown)---------------------+/                                                                                      <* 
 *>       else  {                                                                                                                    <* 
 *>          yURG_err ('F', "requested action å%sæ not understood or incomplete", a);                                                <* 
 *>          rc = -99;                                                                                                               <* 
 *>       }                                                                                                                          <* 
 *>       if (rc < 0)  break;                                                                                                        <* 
 *>    }                                                                                                                             <* 
 *>    /+---(verify)-------------------------+/                                                                                      <* 
 *>    yURG_msg ('-', "run as (%c) %s", my.run_as, PROG_iam (my.run_as));                                                            <* 
 *>    yURG_msg ('-', "mode   (%c) %s", my.run_mode, PROG_runmode (my.run_mode));                                                    <* 
 *>    yURG_msg ('-', "msec   %d", my.loop_msec);                                                                                    <* 
 *>    yURG_msg ('-', "max    %d", my.loop_max);                                                                                     <* 
 *>    /+---(display urgents)----------------+/                                                                                      <* 
 *>    DEBUG_ARGS   yLOG_note    ("summarization of argument processing");                                                           <* 
 *>    DEBUG_ARGS   yLOG_value   ("entries"   , x_total);                                                                            <* 
 *>    DEBUG_ARGS   yLOG_value   ("arguments" , x_args);                                                                             <* 
 *>    DEBUG_ARGS   yLOG_char    ("test"      , my.test);                                                                            <* 
 *>    DEBUG_ARGS   yLOG_value   ("loop_msec" , my.loop_msec);                                                                       <* 
 *>    DEBUG_ARGS   yLOG_value   ("loop_max"  , my.loop_max);                                                                        <* 
 *>    yURG_msg ('-', "");                                                                                                           <* 
 *>    /+---(complete)-----------------------+/                                                                                      <* 
 *>    DEBUG_TOPS   yLOG_exit    (__FUNCTION__);                                                                                     <* 
 *>    return rc;                                                                                                                    <* 
 *> }                                                                                                                                <*/


char         /*--: evaluate command line arguments -------[ leaf   [ ------ ]-*/
PROG__args              (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char       *a           = NULL;          /* current argument               */
   char       *b           = NULL;          /* next argument                  */
   int         x_len       =    0;          /* argument length                */
   int         x_total     =    0;          /* total argument count           */
   int         x_args      =    0;          /* argument count                 */
   int         x_num       =    0;          /* numeric argument holder        */
   char        x_two       =    0;
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_HUND]  = "";
   /*---(process)------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "command line arguments handling...");
   yURG_msg ('-', "total of %d arguments, including name", a_argc);
   /*---(defaults)------------------------------*/
   /*> PROG__arg_clearmode ();                                                        <*/
   /*---(walk args)-----------------------------*/
   for (i = 1; i < a_argc; ++i) {
      /*---(prepare)---------------------*/
      a = a_argv [i];
      if (a == NULL) {
         yURG_err ('f', "arg %d is NULL", i);
         DEBUG_TOPS   yLOG_note    ("FATAL, found a null argument, really bad news");
         DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (i < a_argc - 1)  b = a_argv [i + 1];
      else                 b = NULL;
      /*---(debugging--------------------*/
      if (a [0] == '@')       continue;
      /*---(two arg check)---------------*/
      ++x_args;
      DEBUG_ARGS  yLOG_info     ("argument"  , a);
      /*> rc = PROG__arg_handle (&i, a, b);                                           <*/
      rc = yJOBS_args_handle (&(my.run_as), &(my.run_mode), my.run_file, &i, a, b);
      if (rc < 0)  break;
   }
   /*---(verify)-------------------------*/
   yJOBS_iam  (my.run_as  , s);
   yJOBS_mode (my.run_mode, t);
   yURG_msg ('-', "run as (%c) %s", my.run_as, s);
   yURG_msg ('-', "mode   (%c) %s", my.run_mode, t);
   yURG_msg ('-', "msec   %d", my.loop_msec);
   yURG_msg ('-', "max    %d", my.loop_max);
   /*---(display urgents)----------------*/
   DEBUG_ARGS   yLOG_note    ("summarization of argument processing");
   DEBUG_ARGS   yLOG_value   ("entries"   , x_total);
   DEBUG_ARGS   yLOG_value   ("arguments" , x_args);
   DEBUG_ARGS   yLOG_char    ("test"      , my.test);
   DEBUG_ARGS   yLOG_value   ("loop_msec" , my.loop_msec);
   DEBUG_ARGS   yLOG_value   ("loop_max"  , my.loop_max);
   yURG_msg ('-', "");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*--: final preparation for execution -------[ leaf   [ ------ ]-*/
PROG__begin             (void)
{
   /*---(locals)-----------+-------------*/
   char        rce         =  -10;
   char        rc          =    0;
   char        s           [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "prepare the program for run-time...");
   /*---(check if must be root)----------*/
   --rce;  if (my.m_uid != 0 && my.run_as != IAM_HERACLES) {
      yJOBS_iam  (my.run_as  , s);
      yURG_err ('f', "%s can only run as root", s);
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(daemonize)-----------------------------*/
   IF_DAEMON {
      rc = yEXEC_daemon (yLOGS_lognum (), &my.pid);
      DEBUG_TOPS   yLOG_value   ("daemonize" , rc);
      --rce;  if (rc < 0) {
         yURG_err ('f', "could not enter semi-daemon mode (pid 0)");
         DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_msg ('-', "successfully operating in semi-daemon mode (pid 0)");
      DEBUG_TOPS   yLOG_note    ("operating as a semi-daemon (process one)");
      /*---(update console)---------------------*/
      rc = ySEC_open (my.dev, NULL, YEXEC_STDOUT, YEXEC_NO, YEXEC_NO);
      EOS_VERBOSE  printf       (", tty_open (%d)", rc);
      DEBUG_TOPS   yLOG_value   ("console"   , rc);
      --rce;  if (rc < 0) {
         yURG_err ('f', "could not open %s (%d)", my.dev, rc);
         DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_msg ('-', "openned %s as primary console successfully", my.dev);
   } else {
      yURG_msg ('-', "not requested to operate as semi-daemon");
      DEBUG_TOPS   yLOG_note    ("will not use semi-daemon mode");
   }
   /*---(setup signals)-------------------------*/
   IF_DAEMON {
      DEBUG_TOPS   yLOG_note    ("signals set to bullet-proof (dangerous)");
      /*> rc = yEXEC_signal (YEXEC_HARD, YEXEC_NO, YEXEC_WAIT, NULL, NULL);           <*/
      yURG_msg ('-', "requesting signals to bullet-proof mode");
      rc = yEXEC_signal (YEXEC_SOFT, YEXEC_NO, YEXEC_YES, NULL, NULL);
   } else {
      DEBUG_TOPS   yLOG_note    ("signals set to soft for normal working");
      yURG_msg ('-', "requesting signals to normal mode");
      rc = yEXEC_signal (YEXEC_SOFT, YEXEC_NO, YEXEC_WAIT, NULL, NULL);
   }
   DEBUG_TOPS   yLOG_value   ("signals"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could set signals properly (%d)", rc);
      DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(startup dlst library)------------------*/
   yURG_msg ('-', "setting up and initializing yDLST");
   DEBUG_ARGS   yLOG_info    ("yDLST"    ,"initializing");
   rc = yDLST_init ();
   yURG_msg ('-', "setting up and initializing yPARSE");
   DEBUG_ARGS   yLOG_info    ("yPARSE"   ,"initializing");
   rc = yPARSE_init  ('-', NULL, '-');
   rc = yPARSE_delimiters  ("§");
   /*---(set file names)-----------------*/
   DEBUG_ARGS   yLOG_note    ("setting file names");
   /*---(complete)------------------------------*/
   yURG_msg ('-', "");
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG__final             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_char    ("run_mode"  , my.run_mode);
   /*---(set output routing)-------------*/
   yJOBS_final (my.m_uid);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG_startup            (int a_argc, char *a_argv[], char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(startup)------------------------*/
   if (rc >= 0)  rc = PROG__init   (a_argc, a_argv);
   if (rc >= 0)  rc = PROG__args   (a_argc, a_argv);
   if (rc >= 0)  rc = PROG__begin  ();
   if (rc >= 0)  rc = PROG__final  ();
   DEBUG_TOPS  yLOG_value   ("startup"   , rc);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      shutdown functions                      ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVER__________________o (void) {;}

char
PROG_driver             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(route action)-------------------*/
   IF_VERSION {
   }
   else IF_HELP   {
   }
   /*---(local/incomming)----------------*/
   else IF_VERIFY {
   }
   else IF_INSTALL {
   }
   /*---(central)------------------------*/
   else IF_LIST    {
   }
   else IF_CHECK   {
   }
   else IF_AUDIT   {
   }
   else IF_FIX     {
   }
   /*---(outgoing)-----------------------*/
   else IF_REMOVE  {
   }
   /*---(run)----------------------------*/
   else IF_DAEMON  {
   }
   else IF_NORMAL  {
   }
   else {
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      shutdown functions                      ----===*/
/*====================------------------------------------====================*/
static void      o___SHUTDOWN________________o (void) {;}

char         /*--: wrapup program execution --------------[ leaf   [ ------ ]-*/
PROG_end           (void)
{
   /*---(create utmp boot)----------------------*/
   yLOG_enter (__FUNCTION__);
   /*> rptg_dump  ();                                                                 <*/
   yLOG_info  ("logger",   "shutting down logger");
   yLOG_exit  (__FUNCTION__);
   yLOGS_end   ();
   return 0;
}

char
PROG_shutdown           (void)
{
   printf("nyx -- system shutdown\n");
   /*---(locals)-------------------------*/
   FILE     *f         = NULL;
   int       rc        = 0;
   int       pid       = 0;
   /*---(mark shutdown)------------------*/
   ySEC_shutdown ();
   /*---(clean up the drives)------------*/
   printf ("   - sync drives three times\n");
   sync   ();
   sync   ();
   sync   ();
   sleep  (1);
   /*---(lock up this program)-----------*/
   signal (SIGTERM, SIG_IGN);
   signal (SIGSTOP, SIG_IGN);
   signal (SIGKILL, SIG_IGN);
   /*---(shutdown processes)-------------*/
   /*> printf("   - send TERM message\n");                                            <* 
    *> kill(-1, SIGTERM);                                                             <* 
    *> sleep(1);                                                                      <*/
   /*> printf("   - send KILL message\n");                                            <* 
    *> kill(-1, SIGKILL);                                                             <* 
    *> sleep(1);                                                                      <*/
   /*---(kill slim)----------------------*/
   /*> f = fopen ("/var/run/slim.pid", "r");                                          <* 
    *> if (f != NULL) {                                                               <* 
    *>    rc = fscanf (f, "%d", &pid);                                                <* 
    *>    if (rc == 1) {                                                              <* 
    *>       kill (pid, SIGTERM);                                                     <* 
    *>    }                                                                           <* 
    *>    fclose (f);                                                                 <* 
    *> }                                                                              <*/
   /*---(call the shutdown)--------------*/
   if  (my.halt  == 'H') {
      printf("   - halt\n");
      rc = reboot (RB_POWER_OFF);
   }
   if  (my.halt  == 'R') {
      printf("   - reboot\n");
      rc = reboot (RB_AUTOBOOT);
   }
   /*---(these calls shouldn't come back)*/
   return rc;
}


/*====================------------------------------------====================*/
/*===----                     signal handling                          ----===*/
/*====================------------------------------------====================*/
static void      o___SIGNALS_________________o (void) {;}

/*> void      PROG_signal       (int a_signal, siginfo_t *a_info, void *a_nada) {return;};   <*/

/*> void       /+----: handle signals --------------------------------------------+/                     <* 
 *> PROG_signal         (int a_signal, siginfo_t *a_info, void *a_nada)                                  <* 
 *> {                                                                                                    <* 
 *>    /+---(locals)-------*-----------------+/                                                          <* 
 *>    int       status    = 0;                                                                          <* 
 *>    int       xlink     = 0;                                                                          <* 
 *>    tPROC    *xdata     = NULL;                                                                       <* 
 *>    tRUSAGE   r_use;                                                                                  <* 
 *>    int       rc        = 0;                                                                          <* 
 *>    int       errsave   = 0;                                                                          <* 
 *>    DEBUG_SIGN   yLOG_note   ("CONF_comm called");                                                    <* 
 *>    DEBUG_SIGN   yLOG_value  ("a_signal"  , a_signal);                                                <* 
 *>    DEBUG_SIGN   yLOG_point  ("a_info"    , a_info);                                                  <* 
 *>    DEBUG_SIGN   yLOG_point  ("a_nada"    , a_nada);                                                  <* 
 *>    switch (a_signal) {                                                                               <* 
 *>    case  SIGCHLD:                                                                                    <* 
 *>       DEBUG_SIGN   yLOG_info    ("SIGNAL", "SIGCHLD means a child has terminated");                  <* 
 *>       xlink  = EXEC_find (a_info->si_pid);                                                           <* 
 *>       if (xlink < 0) break;                                                                          <* 
 *>       xdata = (tPROC*) yDLST_index (xlink);                                                          <* 
 *>       DEBUG_SIGN   yLOG_senter  ("sigchld");                                                         <* 
 *>       DEBUG_SIGN   yLOG_spoint  ((void *) a_info);                                                   <* 
 *>       DEBUG_SIGN   yLOG_svalue  ("pid"     , (int) a_info->si_pid);                                  <* 
 *>       DEBUG_SIGN   yLOG_snote   (xdata->name);                                                       <* 
 *>       xdata->rc = (int) a_info->si_status;                                                           <* 
 *>       DEBUG_SIGN   yLOG_svalue  ("rc"      , (int) a_info->si_status);                               <* 
 *>       DEBUG_SIGN   yLOG_snote   ("wait");                                                            <* 
 *>       rc = wait4(a_info->si_pid, &status, WNOHANG, &r_use);                                          <* 
 *>       if (rc <  0) errsave = errno;                                                                  <* 
 *>       DEBUG_SIGN   yLOG_svalue  ("wait_rc" , rc);                                                    <* 
 *>       DEBUG_SIGN   yLOG_snote   ("done");                                                            <* 
 *>       DEBUG_SIGN   yLOG_sexit   ("sigchld");                                                         <* 
 *>       xdata->status = 'c';                                                                           <* 
 *>       yDLST_active_off (xdata->dlst);                                                                <* 
 *>       xdata->end    = yLOG_time();;                                                                  <* 
 *>       if (rc >= 0) {                                                                                 <* 
 *>          if (strchr("abcm", xdata->type) != 0)  ++complete;                                          <* 
 *>          xdata->swaps  = (int) (r_use.ru_nvcsw + r_use.ru_nivcsw);                                   <* 
 *>          xdata->u_time = (int) ((r_use.ru_utime.tv_sec * 1000) + (r_use.ru_utime.tv_usec / 1000));   <* 
 *>          xdata->s_time = (int) ((r_use.ru_stime.tv_sec * 1000) + (r_use.ru_stime.tv_usec / 1000));   <* 
 *>       } else if (rc < 0) {                                                                           <* 
 *>          DEBUG_SIGN   yLOG_value   ("cerrno"    , errsave);                                          <* 
 *>          DEBUG_SIGN   yLOG_info    ("cerrstr"   , strerror(errsave));                                <* 
 *>       }                                                                                              <* 
 *>       EXEC_children (xdata->dlst);                                                                   <* 
 *>       break;                                                                                         <* 
 *>    case  SIGHUP:                                                                                     <* 
 *>       DEBUG_SIGN   yLOG_info  ("SIGNAL", "SIGHUP MEANS REFRESH");                                    <* 
 *>       break;                                                                                         <* 
 *>    case  SIGTERM:                                                                                    <* 
 *>       DEBUG_SIGN   yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon (but eos can't)");          <* 
 *>       break;                                                                                         <* 
 *>    case  SIGSEGV:                                                                                    <* 
 *>       DEBUG_SIGN   yLOG_info  ("SIGNAL", "SIGSEGV means daemon blew up (but eos can't)");            <* 
 *>       break;                                                                                         <* 
 *>    }                                                                                                 <* 
 *>    /+---(complete)------------------------------+/                                                   <* 
 *>    return;                                                                                           <* 
 *> }                                                                                                    <*/

/*> char       /+----: setup signal handling -------------------------------------+/   <* 
 *> CONF_signal          (void)                                                        <* 
 *> {                                                                                  <* 
 *>    /+---(design notes)-------------------+/                                        <* 
 *>    /+ options are...                                                               <* 
 *>     *    signal()        -- race conditions when they come fast (depricated)       <* 
 *>     *    sigaction()     -- preferred method and standard in c                     <* 
 *>     *    signalfd()      -- linux specific that treats it like a file              <* 
 *>     *                                                                              <* 
 *>     +/                                                                             <* 
 *>    /+---(set up structure)---------------+/                                        <* 
 *>    struct sigaction sa;                                                            <* 
 *>    /+---(begin)--------------------------+/                                        <* 
 *>    DEBUG_TOPS   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    /+---(set actions)--------------------+/                                        <* 
 *>    sa.sa_handler   = NULL;                                                         <* 
 *>    sa.sa_sigaction = CONF_comm;                                                    <* 
 *>    sa.sa_flags     = SA_RESTART | SA_SIGINFO;                                      <* 
 *>    sigfillset (&sa.sa_mask);                                                       <* 
 *>    sa.sa_restorer  = NULL;                                                         <* 
 *>    DEBUG_SIGN   yLOG_info  ("children", "pay attention to children");              <* 
 *>    sigaction (SIGCHLD,  &sa      , NULL);       /+ watch for children         +/   <* 
 *>    DEBUG_SIGN   yLOG_info  ("program",  "look for HUP");                           <* 
 *>    sigaction (SIGHUP ,  &sa      , NULL);       /+ hangup means refresh       +/   <* 
 *>    DEBUG_SIGN   yLOG_info  ("baddies",  "handle SEGV and TERM");                   <* 
 *>    sigaction (SIGTERM,  &sa      , NULL);       /+ catch a kill               +/   <* 
 *>    sigaction (SIGSEGV,  &sa      , NULL);       /+ catch a segfault           +/   <* 
 *>    DEBUG_SIGN   yLOG_info  ("keyboard", "stop keyboard kills and pauses");         <* 
 *>    sigaction (SIGINT ,  &sa      , NULL);       /+ keyboard interrupt         +/   <* 
 *>    sigaction (SIGQUIT,  &sa      , NULL);       /+ keyboard quit              +/   <* 
 *>    sigaction (SIGTERM,  &sa      , NULL);       /+ terminate                  +/   <* 
 *>    sigaction (SIGCONT,  &sa      , NULL);       /+ continue                   +/   <* 
 *>    sigaction (SIGTSTP,  &sa      , NULL);       /+ keyboard stop              +/   <* 
 *>    DEBUG_SIGN   yLOG_info  ("terminal", "ignore stadin and stdout");               <* 
 *>    sigaction (SIGTTIN,  &sa      , NULL);       /+ tty input for background   +/   <* 
 *>    sigaction (SIGTTOU,  &sa      , NULL);       /+ tty output for background  +/   <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_TOPS   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/



/*====================------------------------------------====================*/
/*===----                   helpers for unit testing                   ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}


char       /*----: set up program test file locations ------------------------*/
prog__unit_files   (void)
{
   char        x_cmd       [LEN_RECD];
   chdir    ("/tmp");
   sprintf  (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT);
   system   (x_cmd);
   rmdir    (DIR_UNIT);
   sprintf  (x_cmd, "mkdir %s   > /dev/null", DIR_UNIT);
   system   (x_cmd);
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_quiet   (void)
{
   int         x_argc      = 1;
   char       *x_argv [1]  = { "eos" };
   PROG_prestart   (x_argc, x_argv, 'y');
   PROG_debugging  (x_argc, x_argv, 'y');
   PROG_startup    (x_argc, x_argv, 'y');
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_loud    (void)
{
   int         x_argc      = 5;
   char       *x_argv [5]  = { "eos_unit", "@@kitchen", "@@yparse", "@@ydlst", "@@yexec"  };
   PROG_prestart   (x_argc, x_argv, 'y');
   PROG_debugging  (x_argc, x_argv, 'y');
   PROG_startup    (x_argc, x_argv, 'y');
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
prog__unit_end     (void)
{
   char        x_cmd       [LEN_RECD];
   chdir    ("/tmp");
   sprintf  (x_cmd, "rm -fr %s* > /dev/null", DIR_UNIT);
   system   (x_cmd);
   rmdir    (DIR_UNIT);
   PROG_end       ();
   return 0;
}

char*
prog__unit              (char *a_question)
{
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_HUND]  = "";
   char        u           [LEN_LABEL];
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "RPTG             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "mode"    )        == 0) {
      yJOBS_iam  (my.run_as  , s);
      yJOBS_mode (my.run_mode, t);
      snprintf (unit_answer, LEN_RECD, "PROG mode        : iam (%c) %-18.18s, run (%c) %-18.18s, å%sæ", my.run_as, s, my.run_mode, t, my.run_file);
   }
   else if (strcmp (a_question, "action"        )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG action      : %c  %2då%sæ", my.run_mode, strlen (my.run_file), my.run_file);
   }
   else if (strcmp (a_question, "args"          )  == 0) {
      sprintf (s, "%2då%.15sæ", strlen (g_silent) , g_silent);
      sprintf (t, "%2då%.15sæ", strlen (g_confirm), g_confirm);
      sprintf (u, "%2då%.15sæ", strlen (g_verbose), g_verbose);
      snprintf (unit_answer, LEN_HUND, "PROG args        : %-20.20s  %-20.20s  %s", s, t, u);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}




/*============================[[ end-of-code ]]===============================*/
