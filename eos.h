/*============================[[    beg-code    ]]============================*/

/*===[[ HEADER ]]=============================================================*/
/*                      ----------1---------2---------3---------4---------5---------6---------7*/
/*345678901 12345678901 -123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*--------- ----------- -----------------------------------------------------------------------*/
#define     P_FOCUS     "SA (system administration)"
#define     P_NICHE     "in (initializtation)"
#define     P_SUBJECT   "job execution framework"
#define     P_PURPOSE   "simple, reliable, and very transparent system initialization"
/*--------- ----------- -----------------------------------------------------------------------*/
#define     P_NAMESAKE  "eos-rhododactylos (rosy-fingered dawn)"
#define     P_PRONOUNCE "ee.ohs roh.doh.dahk.tee.ohs"
#define     P_HERITAGE  "titaness of daybreak who opens the gates of heaven for the sun"
#define     P_BRIEFLY   "she who opens the gates"
#define     P_IMAGERY   "radiant worman with white wings, golden arms, and rosy fingers"
#define     P_REASON    "fantastically poetic reference to system start-up"
/*--------- ----------- -----------------------------------------------------------------------*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*--------- ----------- -----------------------------------------------------------------------*/
#define     P_HOMEDIR   "/home/system/eos.goddess_of_dawn_and_startup"
#define     P_BASENAME  "eos"
#define     P_FULLPATH  "/sbin/eos"
#define     P_SUFFIX    "conf"
#define     P_CONTENT   "sequenced initialization"
/*--------- ----------- -----------------------------------------------------------------------*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_COMPILER  "gcc 5.3.0"
#define     P_CODESIZE  "small       (appoximately 1,000 slocl)"
#define     P_DEPENDS   "yDLST, yEXEC, ySEC, ySTR, yPARSE"
/*--------- ----------- -----------------------------------------------------------------------*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2010-10"
/*--------- ----------- -----------------------------------------------------------------------*/
#define     P_VERMAJOR  "2.--, rebuilding with better knowledge ;)"
#define     P_VERMINOR  "2.3-, repairing after major system update"
#define     P_VERNUM    "2.3b"
#define     P_VERTXT    "re-creating eos_prog unit tests, 1-6 are now passing"
/*--------- ----------- -----------------------------------------------------------------------*/
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"
/*--------- ----------- -----------------------------------------------------------------------*/
/*--------- 12345678901 -123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/

#define     P_HEADERS   \
   P_FOCUS, P_NICHE, P_SUBJECT, P_PURPOSE, \
   P_NAMESAKE, P_PRONOUNCE, P_HERITAGE, P_BRIEFLY, P_IMAGERY, P_REASON, \
   P_ONELINE, P_HOMEDIR, P_BASENAME, P_FULLPATH, \
   P_SUFFIX, P_CONTENT, \
   P_SYSTEM, P_LANGUAGE, P_COMPILER, P_CODESIZE, P_DEPENDS, \
   P_AUTHOR, P_CREATED, \
   P_VERMAJOR, P_VERMINOR, P_VERNUM, P_VERTXT

/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/



/*===[[ SUMMARY ]]============================================================*/
/* 
 *   eos is a clean, reliable, and light system initialization handler; which
 *   then grew into a job control system that also handles shutdown and ad-hoc
 *   job processing.
 *
 *
 *   eos is intended to be a clean, reliable, and light system initialization
 *   process to receive the hand off from the kernel and deliver a stable,
 *   secure, and well prepared system for use to kharon who will watch over
 *   the system while it is running.
 */

/*===[[ PURPOSE ]]=============================================================*

 *   init is a well-worn, familiar, time-tested, and posix-defined daemon that
 *   provides system initialization and shutdown services as well as acting as
 *   the parent of all processes on the system.  systems needs have changed a
 *   great deal since, but no common theory or clear replacements have emerged.
 *
 *   the original init was released in 1979 and was essentially directed by a
 *   single configuration file called inittab.  since then, init has diverged in
 *   at least four directions which are incompatible.  each of these directions
 *   requires a great deal of configuration, support files, and scripting which
 *   have makes them a brutally large, complex, unauditable monsters of their
 *   own.  i attempted to reengineer from gentoo's openrc, which is a popular
 *   one, but it was #!@%$ painful to read, modify, and understand.
 *
 *   there are a couple of desires that have driven these diveringing systems...
 *      - full installation automation for packages that effect boot  (NO, NO)
 *      - ability to handle a huge variety of systems out-of-the-box  (NO, NO)
 *      - avoid technical knowledge to sequence boot (depends, needs) (NO, NO)
 *      - faster boot times by delaying until needed -- hotplugging   (NO, NO)
 *      - faster boot times by running in parallel                    (YES)
 *   the result of all this has been a monster of interpreted shell scripts that
 *   and interpreted code that is wicked complex and not to be touched by
 *   anyone but the maintainer.  yuck.
 *
 *   the worst of the kitchen-sink thinking is to combine init, cron, and
 *   several other critical daemons into a single hulking, integrated, hot-
 *   plugging nightmare so people can atrophy into limbless gui dwellers.  give
 *   the people bread and circuses...
 *
 *   traditional init packages have many distinct missions and come bundled with
 *   quite a few tools to accomplish those.  we would like to unbundle these
 *   focuses into separate tools in order to increase clarity and cleanliness.
 *      1) startup/initialization           (eos, rosy fingered dawn)
 *      2) shutdown                         (nyx, goddess of the night)
 *      3) sleep/hybernation
 *      4) root of process tree             (kharon, ferryman of the dead)
 *      5) process reincarnation            (haides, god of the underworld)
 *      6) killing rouge processes          (artemes, huntress)
 *
 *   we will be implementing these as separate modules, each with their own
 *   very specific mission and testable in their own right.
 *
 *   general requirement across each of these will be...
 *      - strict glibc/ansi-c so it can be ported to and compilied on anything
 *      - fast and efficient because we want to enable tons of automation
 *      - solid logging and status updates so you never have to guess
 *      - clean, clean code so we can maintain it after long absences
 *      - tight, easy to read, and minimal configuration files
 *      - fullsome unit testing and regression testing suite
 *      - eliminate known and potential security gaps and hacking vectors
 *      - very specific to our system config to avoid security holes
 *
 *   "do one thing and do it well (securely) and focus on technical power users"
 *
 *   we don't want to just use the system, we want to *own* it; so that means
 *   we have to fundmentally understand the critical services which only tearing
 *   down and rebuilding can really teach -- even if its more basic in the end.
 *
 *   as luck would have it, minit (minimal init) is a wonderfully straight
 *   forward and clean implementation that we can build on.  it is licenced
 *   under the gnu gpl and so is perfect as our base.  so, we fork dcron...
 *
 *   so, as always, there are many, stable, accepted, existing programs and
 *   utilities written by better programmers which are likely superior in
 *   speed, size, capability, and reliability; BUT, i would not have learned
 *   nearly as much just using them, so i follow the adage..
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
*   at the end of the day, this is wicked critical and deserves the attention
*
*/
/*===[[ DECISION ]]============================================================*

 *   problem (pr)
 *      current system initialization has become wildly complex and flexible to
 *      the point of black magic.  while this is great for expanding linux usage
 *      to the masses, its a nightmare for technical power users.
 *
 *   objectives (o)
 *      - get back to a simple clean system initialization
 *      - learn the initialization process from start to finish
 *      - get rid of all the cruft that has built up for initialization
 *      - make this process rock-solid so that it is dependable
 *      - make it fast and lean so it can run on almost anything
 *      - make it secure and rugged so it doesn't get hacked and wacked
 *      - every program should do one thing well, not combine because it can
 *      - avoid hotplugging at all costs to keep the security nightmare minimal
 *
 *   alternatives (a)
 *      0) simple script file
 *         - this was the basis before sysvinit
 *         - still used by small distributions, tinygentoo, LFS, etc
 *         - everything is in one place and verifiable
 *         - but, difficult to add to simply and then test
 *         - this is quite uncommon any more
 *
 *      1) simple init (from 1969)
 *         - simple, plain text, human readable inpute file called inittab
 *         - is mainly just spawned gettys and ran the rc script once
 *         - very, very simple
 *
 *      2) bsd-style init (from early 70's)
 *         - rc script now reads /etc/rc.conf
 *         - the actual scripts are broken out into a few major groupings
 *         - there were no runlevel yet and very hard to automate changes
 *         - everyone switched to this style
 *         - only freebsd, netbsd, openbsd, and dragonflybsd still use it
 *
 *      3) system v init (sysvinit from 1983)
 *         - brought the concept of runlevels to make the system manageable
 *         - each run level in inittab calls the rc script with the new level
 *         - very, very well studied, debugged, tested, and documented
 *         - this is still them most commonly used
 *         - starts processes essentially one at a time
 *         - controlled by the telinit program to switch levels
 *
 *      4) launchd (apple in 2005)
 *         - try to replace init, rc, inetd, xinetd, atd, crond, watchdogd, ...
 *         - essentially consolidates launch services
 *         - significantly shortens boot on slow computers
 *
 *      5) upstart (ubuntu in 2006)
 *         - asynchonous, non-blocking
 *         - explicitly backwards compatible with sysvinit
 *         - currently used/maintained
 *
 *      6) systemd (fedora in 2009)
 *         - intended to handle dependencies better and more parallel
 *         - currently used/maintained
 *
 *      7) openrc (gentoo)
 *         - intended to be a cleaner, better set of rc scripts
 *         - scary set of complex rc scripts that are not meant to be changed
 *         - currently used/maintained
 *
 *      8) einit, initng, mudur, ruinit, etc.
 *         - go the highly parallel route and are essentially limited use/dead
 *
 *      9) minit
 *         - return to the world of simple inits
 *
 *      A) custom
*         - separate out startup, monitoring, shutdown, parent, etc
*         - really, really learn how it works
*         - stop with the all-things-to-all-people approach
*
*
*   decision
*      predictably, we will build our own system that provides the critical
*      services as separate, secure, and specialized programs. we will create
*      the following services...
*      - eos       : system initialization
*      - kharon    : parent to processes, monitor, collector of the dead
*      - nyx       : system shutdown
*      - haides    : daemon controller
*      - ???       : rouge process monitor
*      - khronos   : separate
*      - ???       : resource monitor
*      - resurect  : bring things back from the dead
*
*
*
*
*
*   a unix/linux system from grub to login and ready for action.  its goal is
*   to be consistent, steady, tight, and compact.
*
*   current methods of system startup have severely promoted flexibility,
   *   on-the-fly adaptation, automated update, and cross-system support.  all
   *   worthy goals, but the result is an almost unintelligible raft of scripts
   *   that are completely beyond a single administrator to grasp, require blind
   *   faith in software creators, and allow for automatic updates during software
   *   installation -- not bloody good for me.
   *
*   eos will allow for...
*      - a single configuration file (/etc/eos.conf)
*      - solid debugging code to allow noisy code tracing
*      - consistent and fullsome logging using yLOG
*      - dependency configuration using yDLST requirements/dependencies
*      - only multiuser command line mode (makes it harder for hackers)
*
*   eos will not allow for...
*      - different run levels (there are just better ways)
*      - multiple configuration files (makes it harder to verify configuration)
*      - alternate configuration file (potential hacker vector)
*      - init/telinit fifo at /dev/initctl (not a fullsome deal)
*      - inittab (we are going a diffenent way)
*
*/
/*===[[ DECISION ]]============================================================*
 *
 *   action field
 *      - sysinit    : immediately during system boot
 *      - boot       : run at start (after sysinit) no matter the runlevel
 *      - bootwait   : just like boot, but init will wait for them to complete
 *      - powerwait  : what to do when power is failing
 *      - ctrlaltdel : call when user hits this keystroke
 *      - once       : single time when runlevel is entered
 *      - wait       : like once, but must wait for it to complete
 *      - respawn    : restart whenever it terminates (esp for getty, sshd, etc)
 *      - off        : null action 
 *
 */
/*============================================================================*/

/*===========================================================================*/

/*   general query categories...
 *      who's currently logged in ?               (utmpx)     current
 *      who logged in over the last month ?       (wtmpx)     historical utmpx
 *      how long ago did a certain user log in ?  (lastlogx)
 *
 *
 *
 */
/*===========================================================================*/


/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YEOS 
#define YEOS  loaded



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/


/*===[[ PRIVATE HEADERS ]]====================================================*/
/*---(always)---------------*/
#include    <yLOG.h>              /* heatherly logger                         */
#include    <yURG.h>              /* heatherly debugging framework            */
#include    <yENV.h>              /* heatherly environmental support          */
/*---(common)---------------*/
#include    <ySTR.h>              /* heatherly safe string library            */
#include    <yPARSE.h>            /* heatherly file parsing                   */
/*---(optional)-------------*/
#include    <yDLST.h>             /* heatherly specialty 4:2 list w/deps      */
#include    <yEXEC.h>             /* heatherly process control                */
#include    <yJOBS.h>             /* heatherly job execution and control      */
#include    <ySEC.h>              /* heatherly security logging               */
/*---(done)-----------------*/



/*===[[ PUBLIC HEADERS ]]=====================================================*/
/*---(big standards)------------*/
#include    <stdio.h>             /* printf, scanf                            */
#include    <stdlib.h>            /* exit, system, ...                        */
#include    <sys/unistd.h>        /* fork, execl, getpid, getuid              */
#include    <string.h>            /* strcat, strcpy, strlen, ...              */

/*---(process)------------------*/
#include    <sys/wait.h>          /* sigaction, waitpid, wait4                */
#include    <sys/resource.h>      /* getrusage                                */
#include    <errno.h>             /* errno                                    */

/*---(timekeeping)--------------*/
#include    <time.h>              /* time, localtime, strftime, ...           */
#include    <sys/time.h>          /* gettimeofday                             */

/*---(filesystems)--------------*/
#include    <sys/mount.h>         /* mount                                    */
#include    <fcntl.h>             /* open                                     */
#include    <sys/stat.h>          /* fstat, umask                             */
#include    <dirent.h>

/*---(users)--------------------*/
#include    <pwd.h>               /* getpwuid, getpwnam                       */
#include    <grp.h>               /* initgroups                               */

/*---(user tracking)------------*/
#include    <utmp.h>              /* utmp, btmp, and wtmp tracking            */

/*---(other)--------------------*/
#include    <sys/utsname.h>
#include    <error.h>
#include    <sys/reboot.h>


#include    <termios.h>

#include    <yCOLOR_solo.h>


/*---(rational limits)------------------------------------------*/
#define     MAX_ARGV        20     /* max number of arguments   */

/*---(structures)-----------------------------------------------*/

typedef struct cACCESSOR tACCESSOR;
struct cACCESSOR
{
   /*---(files)----------------*/
   char        run_as;                      /* eos vs astraios vs unit        */
   char        run_mode;                    /* verify, daemon, normal         */
   char        run_file    [LEN_PATH];      /* file for verify, etc.          */
   char        halt;                        /* halt mode for nyx              */
   char        done_done;                   /* flag for all procs done        */
   llong       msec;                        /* current epoch in milliseconds  */
   int         tic;                         /* current loop                   */
   char        verbose;
   /*---(owner)----------------*/
   int         m_uid;                       /* uid of person who launched eos */
   char        m_who       [LEN_LABEL];     /* user name who launched eos     */
   int         pid;                         /* process id of eos              */
   int         ppid;                        /* parent process id of eos       */
   char        dev         [LEN_LABEL];     /* tty for output                 */
   /*---(commands)-------------*/
   char       *argv        [MAX_ARGV];      /* command in argv format         */
   int         argc;                        /* count of arguments             */
   /*---(flags)----------------*/
   char        status_proc;                 /* status /proc filesystem        */
   char        status_log;                  /* status /var/log/yLOG filesystem*/
   char        boot;                        /* is this an init run            */
   char        test;                        /* is this a test run             */
   /*---(files)-----------------*/
   char        n_conf      [LEN_FULL];      /* name of configuration file     */
   char        n_exec      [LEN_FULL];      /* name of execution detail file  */
   char        n_perf      [LEN_FULL];      /* name of execution speed file   */
   int         c_recdno;                    /* eos.conf record number         */
   int         c_verb      [LEN_LABEL];     /* eos.conf verb                  */
   /*---(current file)----------*/
   char        f_name      [LEN_HUND];      /* current file name              */
   char        f_user      [LEN_LABEL];     /* current file user name         */
   int         f_uid;                       /* current file user id           */
   char        f_desc      [LEN_DESC];      /* current file description       */
   char        f_dir       [LEN_PATH];      /* current file path              */
   char        f_full      [LEN_PATH];      /* current file full name/path    */
   int         f_lines;
   char        f_group;
   /*---(counts)----------------*/
   int         f_gall;
   int         f_gpass;
   int         f_gfail;
   int         f_aall;
   int         f_apass;
   int         f_afail;
   int         f_pall;
   int         f_ppass;
   int         f_pfail;
   char        f_note      [LEN_LABEL];
   /*---(current group)---------*/
   char        g_ready;                /* group record checks out             */
   char        g_name      [LEN_LABEL]; /* short name for reference            */
   char        g_desc      [LEN_DESC]; /* longer description                  */
   /*---(current proc)----------*/
   char        p_ready;                /* proc record checks out              */
   char        p_name      [LEN_LABEL]; /* short name for reference            */
   char        p_type;                 /* process type                        */
   char        p_desc      [LEN_DESC]; /* longer description                  */
   char        p_user      [LEN_LABEL]; /* user name                           */
   int         p_uid;                  /* user id to use to launch job        */
   char        p_check     [LEN_FULL];  /* command to check existance          */
   char        p_run       [LEN_FULL];  /* command to execute                  */
   /*---(arguments)-------------*/
   long        loop_msec;              /* wait time in milliseconds           */
   int         loop_max;               /* maximum loops allowed before quit   */
   /*---(done)------------------*/
};
extern tACCESSOR my;


/*> extern char g_silent   [LEN_LABEL];                                               <* 
 *> extern char g_confirm  [LEN_LABEL];                                               <* 
 *> extern char g_verbose  [LEN_LABEL];                                               <*/

/*> extern char g_verify   [LEN_SHORT];                                               <* 
 *> extern char g_install  [LEN_SHORT];                                               <* 
 *> extern char g_list     [LEN_SHORT];                                               <* 
 *> extern char g_check    [LEN_SHORT];                                               <* 
 *> extern char g_audit    [LEN_SHORT];                                               <* 
 *> extern char g_fix      [LEN_SHORT];                                               <* 
 *> extern char g_remove   [LEN_SHORT];                                               <* 
 *> extern char g_daemon   [LEN_SHORT];                                               <* 
 *> extern char g_normal   [LEN_SHORT];                                               <*/





#define     EOS_VERBOSE      if (my.verbose  == 'y') 

#define     EOS_RPTG_VERBS   'r'
#define     EOS_RPTG_CONTROL 'C'

/*---(directory names)--------------------------*/
#define     DIR_ETC          "/etc/"
#define     DIR_RUN          "/var/run/"
#define     DIR_YLOG         "/var/log/yLOG/"
#define     DIR_YHIST        "/var/log/yLOG.historical/"
#define     DIR_UNIT         "/tmp/eos_test/"

/*---(file names)-------------------------------*/
#define     FILE_CONF        ".conf"
#define     FILE_EXEC        "_execs.execution_feedback"
#define     FILE_PERF        "_speed.execution_tracking"


#define     UTMP             "/var/run/utmp"
#define     WTMP             "/var/log/wtmp"
#define     BTMP             "/var/log/btmp"
#define     SULOG            "/var/log/sulog"
#define     LASTLOG          "/var/log/lastlog"
#define     PASSWD           "/etc/passwd"
#define     SHADOW           "/etc/shadow"
#define     GROUP            "/etc/group"
#define     GSHADOW          "/etc/gshadow"
#define     TTY              "/etc/securetty"
#define     HESTIA           "/var/log/hestia.logging"

#define     LOGIN            "/etc/login.defs"
#define     NOLOGIN          "/etc/nologin"
#define     LIMITS           "/etc/limits"
#define     USERTTY          "/etc/usertty"

#define     LOGGER           if (my.logger >= 1)



/*---(full execution)-------*/
#define     EOS_TYPE_BOOT        'b'
#define     EOS_TYPE_CONFIG      'c'
#define     EOS_TYPE_EXEC        'e'
/*---(launch only)----------*/
#define     EOS_TYPE_DAEMON      'd'
#define     EOS_TYPE_MOUNT       'm'
#define     EOS_TYPE_SERIAL      's'
/*---(job control)----------*/
#define     EOS_TYPE_STOP        'S'
#define     EOS_TYPE_CONT        'C'
#define     EOS_TYPE_RESET       'R'
#define     EOS_TYPE_PING        'P'
/*---(terminate)------------*/
#define     EOS_TYPE_KILL        'k'
#define     EOS_TYPE_UMOUNT      'u'
#define     EOS_TYPE_WRAPUP      'w'
/*---(groups)---------------*/
#define     EOS_WAIT_LONG        "dswR"
#define     EOS_WAIT_SHORT       "mukP"
/*---(special)--------------*/
#define     EOS_TYPE_LIMITS      "bce"
#define     EOS_TYPE_NOSTOP      "SCRP"
/*---(helper)---------------*/
#define     EOS_TYPE_DIV         '-'



#define     GROUP_READY      '-'
#define     GROUP_RUNNING    'R'
#define     GROUP_DONE       'd'


typedef struct cGROUP tGROUP;
struct cGROUP {
   /*---(master)-------------------------*/
   int         line;                        /* line in eos.conf               */
   char        name        [LEN_LABEL];      /* short name for reference       */
   char        desc        [LEN_DESC];      /* longer description             */
   /*---(processing)---------------------*/
   char        status;                      /* ready, focused, done           */
   int         askd;                        /* count of procs in group        */
   int         done;                        /* count of completed procs       */
   /*---(timing)-------------------------*/
   llong       beg;                         /* when group started             */
   llong       end;                         /* when group finished            */
   int         dur;                         /* duration open                  */
   char        note;                        /* trouble flag                   */
   /*---(reporting)----------------------*/
   uchar       col;                         /* reporting col/horz             */
   uchar       row;                         /* reporting row/vert             */
   short       x, y;                        /* report position (unit test)    */
   /*---(done)---------------------------*/
};

/*---(structures)-----------------------------------------------*/
typedef struct  cPROC tPROC;
struct cPROC {
   /*---(master)-------------------------*/
   int         line;                        /* line in eos.conf               */
   char        name        [LEN_LABEL];     /* short name for reference       */
   char        type;                        /* process type                   */
   char        desc        [LEN_DESC];      /* longer description             */
   /*---(command)------------------------*/
   char        user        [LEN_LABEL];     /* user name                      */
   int         uid;                         /* user id to use to launch job   */
   char        run         [LEN_FULL];      /* command to execute             */
   char        altname     [LEN_TITLE];     /* name if changes (daemons, ...) */
   /*---(estimates)----------------------*/
   int         est;                         /* expected duration in seconds   */
   int         minest;                      /* lower limit estimate in msecs  */
   int         maxest;                      /* upper limit estimate in msecs  */
   /*---(flags)--------------------------*/
   char        value;                       /* on a H-M-L scale               */
   char        track;                       /* detailed tracking              */
   char        rolling;                     /* history rolling                */
   char        strict;                      /* adherence to limits            */
   char        lower;                       /* lower limit on duration        */
   char        upper;                       /* upper limit on duration        */
   char        remedy;                      /* limit violation remedy         */
   /*---(results)------------------------*/
   llong       beg;                         /* start msec                     */
   int         rpid;                        /* process id of crond            */
   char        yexec;                       /* end code from yEXEC            */
   int         rc;                          /* return code                    */
   llong       end;                         /* end msec                       */
   int         dur;                         /* duration in seconds            */
   /*---(done)---------------------------*/
};


/*---(convienence typedefs)-------------------------------------*/
typedef long   long      llong;
typedef const  int       cint;
typedef const  long      clong;
typedef const  char      cchar;

typedef struct stat      tSTAT;
typedef struct passwd    tPASSWD;
typedef struct rusage    tRUSAGE;
typedef struct tm        tTIME;
typedef struct dirent    tDIRENT;
typedef struct timespec  tTSPEC;
typedef struct termios   tTERMIOS;


extern      char      verstring    [500];

extern      int         complete;


extern      char        unit_answer [LEN_RECD];

/*===[[ EOS_MAIN.C ]]=========================================================*/
int         main               (int a_argc, char *a_argv[]);

/*===[[ EOS_PROG.C ]]=========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*> char        PROG__arg_init          (void);                                       <*/
/*> char        PROG__arg_single        (char *a_levels, char n);                     <*/
/*> char        PROG__arg_load          (void);                                       <*/
/*> char        PROG__arg_clearmode     (void);                                       <*/
/*> char        PROG__arg_handle        (int *i, char *a_arg, char *a_next);          <*/
/*---(program)--------------*/
char*       PROG_version            (void);
char*       PROG_usage              (void);
char        PROG_reset              (void);
/*---(prestart)-------------*/
char        PROG__verbose           (int a_argc, char *a_argv[], char a_unit, int a_rpid);
char        PROG__runas             (char *a_name);
char        PROG__boot              (int a_argc, char *a_argv[], int a_rpid);
char        PROG_prestart           (int a_argc, char *a_argv[], char a_unit);
/*---(debugging)------------*/
char        PROG_debugging          (int a_argc, char *a_argv[], char a_unit);
/*---(startup)--------------*/
char        PROG__init              (void);
char        PROG__args              (int a_argc, char *a_argv[]);
char        PROG__begin             (void);
char        PROG__final             (void);
char        PROG_startup            (int a_argc, char *a_argv[], char a_unit);
/*---(shutdown)-------------*/
char        PROG_end                (void);
char        PROG_shutdown           (void);
/*---(signals)-------------*/
/*> void        PROG_signal        (int a_signal, siginfo_t *a_info, void *a_nada);   <*/
/*---(unittest)------------*/
char        PROG_testfiles          (void);
char        PROG_testquiet          (void);
char        PROG_testloud           (void);
char*       prog__unit              (char *a_question);

/*===[[ EOS_EXEC.C ]]=========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(processes)------------*/
char        EXEC_checkmount         (char*);
char        EXEC_launch             (void);
char        EXEC_find               (int);
char        EXEC_run                (tPROC*);
char        EXEC_check              (void);
char        EXEC_children           (int);


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(content)--------------*/
char        BASE_handler            (int n, uchar *a_verb, char a_exist, void *a_handler);
char        BASE__prepare           (cchar a_loc, cchar *a_full, cchar *a_fname, cchar *a_fuser, int a_fuid);
char        BASE_pull_detail        (cchar a_loc, cchar *a_full, cchar *a_fname, cchar *a_fuser, int a_fuid);
char        BASE_pull               (cchar *a_fname);
/*---(execution)------------*/
llong       BASE_msec               (void);
char        BASE_console            (void);
char        BASE_execute            (void);
char        BASE_kharon             (void);
/*---(done)-----------------*/



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(cleansing)------------*/
char        group__wipe             (tGROUP *a_group);
char*       group__memory           (tGROUP *a_group);
/*---(memory)---------------*/
char        group__new              (tGROUP **a_new);
char        group__free             (tGROUP **a_old);
/*---(existance)------------*/
char        group_handler           (int n, uchar *a_verb);
char        after_handler           (int n, uchar *a_verb);
/*---(exec)-----------------*/
char        group_mark_begin        (llong a_msec);
char        group_mark_done         (llong a_msec);
char        group_mark_clear        (void);
/*---(unittest)-------------*/
char*       group__unit             (char *a_question, int a_num);



/*---(cleansing)------------*/
char        proc__wipe              (tPROC *a_proc);
char*       proc__memory            (tPROC *a_proc);
/*---(memory)---------------*/
char        proc__new               (tPROC **a_new);
char        proc__free              (tPROC **a_old);
/*---(existance)------------*/
char        proc__flags             (tPROC *a_new, uchar *a_flags, char *a_dur);
char        proc_handler            (int n, uchar *a_verb);
/*---(exec)-----------------*/
char        proc_mark_begin         (llong a_msec, int a_rpid);
char        proc_mark_all_in_one    (llong a_msec, int a_rpid, char a_yexec);
char        proc_mark_done          (llong a_msec, char a_yexec, int a_rc);
char        proc_mark_clear         (void);
/*---(reporting)------------*/
char        proc_verblist           (void);
/*---(unittest)-------------*/
char*       proc__unit              (char *a_question, int a_num);


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        rptg__pert_clear        (void);
char        rptg__pert_col          (void);
char        rptg__pert_row          (void);
char        rptg__pert_groups       (void);
char        rptg_pert               (void);
char        rptg_gantt              (void);
char        rptg_dump               (void);
char*       rptg__unit              (char *a_question, int a_num);


/*---(verify)---------------*/
char        exec__verify_mount      (char a_run [LEN_FULL]);
char        exec__verify_daemon     (char *a_run, int *a_rpid);
/*---(check)----------------*/
char        exec__check_launch      (tPROC *a_proc, llong a_msec);
char        exec__check_mount       (tPROC *a_proc, llong a_msec);
char        exec__check_daemon      (tPROC *a_proc, llong a_msec);
char        exec__check_signal      (tPROC *a_proc, llong a_msec);
int         exec_check              (llong a_msec);
char        exec_finish             (llong a_msec);
char        exec_start              (llong a_msec);
/*---(dispatch)-------------*/
char        exec__dispatch_launch   (tPROC *a_proc, llong a_msec);
char        exec__dispatch_mount    (tPROC *a_proc, llong a_msec);
char        exec__dispatch_daemon   (tPROC *a_proc, llong a_msec);
char        exec__dispatch_signal   (tPROC *a_proc, llong a_msec);
char        exec_dispatch           (llong a_msec);
/*---(unit test)------------*/
char*       exec__unit              (char *a_question);
/*---(done)-----------------*/


char        eos_yjobs               (cchar a_req, cchar *a_data);

#endif
/*============================[[ end-of-code ]]===============================*/
