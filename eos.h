/*============================[[    beg-code    ]]============================*/

/*===[[ HEADER ]]=============================================================*/
/*
 *   focus         : (SA) system administration
 *   niche         : (in) initialization
 *   purpose       : simple, reliable, and transparent system initialization
 *
 *   namesake      : eos-rhododactylos (rosy-fingered)
 *   heritage      : titaness of daybreak, opens the gates of heaven for the sun
 *   imagery       : radiant winged woman with golden arms and rosy fingers
 *
 *   base_system   : gnu/linux   (powerful, ubiquitous, technical, and hackable)
 *   lang_name     : ansi-c      (wicked, limitless, universal, and everlasting)
 *   dependencies  : yLOG, yDLST, dash
 * 
 *   author        : heatherly
 *   created       : 2010-10
 *   size          : small       (approximately 1,000 slocL)
 *
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 * 
 *   simplicity is prerequisite for reliability and security; but logging,
 *   tracing, and unit testing are signs of true wisdom
 *
 */

/*===[[ SUMMARY ]]============================================================*/
/* 
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
#ifndef YINIT
#define YINIT loaded



/*===[[ VERSIONING ]]=========================================================*/
/* rapidly evolving version number to aid with visual change confirmation     */
#define     VER_NUM   "2.0e"
#define     VER_TXT   "checking for mounts is now ready and unit tested"



/*===[[ PRIVATE HEADERS ]]====================================================*/
#include    <yDLST.h>             /* heatherly specialty 4:2 list w/deps      */
#include    <ySEC.h>              /* heatherly security logging               */
#include    <yEXEC.h>             /* heatherly process control                */
#include    <ySTR.h>              /* heatherly safe string library            */
#include    <yLOG.h>              /* heatherly logger                         */
#include    <yURG.h>              /* heatherly debugging framework            */
#include    <yPARSE.h>            /* heatherly file parsing                   */

/*===[[ PUBLIC HEADERS ]]=====================================================*/
/*---(big standards)------------*/
#include    <stdio.h>             /* printf, scanf                            */
#include    <stdlib.h>            /* exit, system, ...                        */
#include    <sys/unistd.h>        /* fork, execl, getpid, getuid              */
#include    <string.h>            /* strcat, strcpy, strlen, ...              */

/*---(process)------------------*/
#include    <sys/wait.h>          /* sigaction, waitpid, wait4                */
#include    <sys/time.h>          /* getrusage                                */
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



/*---(error codes)----------------------------------------------*/
#define     ERR_NOT_ROOT          -1
#define     ERR_NOT_PID_ONE       -2
#define     ERR_NO_TMPFS          -3
#define     ERR_NO_LOGGER         -4



/*---(rational limits)------------------------------------------*/
#define     LEN_NAME        20     /* max name field            */
#define     LEN_DESC        50     /* max desc field            */
#define     LEN_CMD        200     /* max command len           */
#define     LEN_RECD      1000     /* max record len            */
#define     MAX_ARGV        20     /* max number of arguments   */

/*---(structures)-----------------------------------------------*/

struct cACCESSOR
{
   /*---(files)----------------*/
   char        quiet;                  /* bool : 0=normal, 1=quiet            */
   char        updates;                /* bool : 0=normal, 1=quiet            */
   int         logger;                 /* log file so that we don't close it  */
   int         locker;                 /* lock file in /var/run               */
   char        done_done;              /* flag for all procs done             */
   /*---(owner)----------------*/
   int         uid;                    /* uid of person who launched eos      */
   char        who         [LEN_NAME]; /* user name who launched eos          */
   int         pid;                    /* process id of eos                   */
   int         ppid;                   /* parent process id of eos            */
   /*---(commands)-------------*/
   char       *argv        [MAX_ARGV]; /* command in argv format              */
   int         argc;                   /* count of arguments                  */
   /*---(flags)----------------*/
   char        status_proc;            /* status of /proc filesystem          */
   char        status_log;             /* status of /var/log/yLOG filesystem  */
   char        daemon;                 /* daemon mode                         */
   char        init;                   /* is this an init run                 */
   char        test;                   /* is this a test run                  */
   /*---(files)-----------------*/
   char        name_conf   [LEN_CMD];  /* name of configuration file          */
   char        name_exec   [LEN_CMD];  /* name of execution detail file       */
   char        name_perf   [LEN_CMD];  /* name of execution speed file        */
   int         c_recdno;               /* eos.conf record number              */
   int         c_verb      [LEN_NAME]; /* eos.conf verb                       */
   /*---(current group)---------*/
   char        g_ready;                /* group record checks out             */
   char        g_name      [LEN_NAME]; /* short name for reference            */
   char        g_desc      [LEN_DESC]; /* longer description                  */
   /*---(current proc)----------*/
   char        p_ready;                /* proc record checks out              */
   char        p_name      [LEN_NAME]; /* short name for reference            */
   char        p_type;                 /* process type                        */
   char        p_desc      [LEN_DESC]; /* longer description                  */
   char        p_user      [LEN_NAME]; /* user name                           */
   int         p_uid;                  /* user id to use to launch job        */
   char        p_check     [LEN_CMD];  /* command to check existance          */
   char        p_run       [LEN_CMD];  /* command to execute                  */
   /*---(arguments)-------------*/
   long        loop_msec;              /* wait time in milliseconds           */
   int         loop_max;               /* maximum loops allowed before quit   */
   /*---(done)------------------*/
} my;



/*---(directory names)--------------------------*/
#define     RUN_EOS          'e'
#define     RUN_ASTRAIOS     'a'

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


#define     TYPE_BOOT        'b'
#define     TYPE_CONFIG      'c'
#define     TYPE_DAEMON      'd'
#define     TYPE_LAUNCH      'l'
#define     TYPE_MOUNT       'm'
#define     TYPE_SERIAL      's'
#define     TYPE_ALL         "bcdlms"

#define     GROUP_READY      '-'
#define     GROUP_RUNNING    'R'
#define     GROUP_DONE       'd'


typedef struct cGROUP tGROUP;
struct cGROUP {
   /*---(master)-------------------------*/
   int         line;                        /* line in eos.conf               */
   char        name        [LEN_NAME];      /* short name for reference       */
   char        desc        [LEN_DESC];      /* longer description             */
   /*---(processing)---------------------*/
   int         requested;                   /* count of procs in group        */
   int         completed;                   /* count of completed procs       */
   char        status;                      /* ready, focused, done           */
   /*---(done)---------------------------*/
};

/*---(structures)-----------------------------------------------*/
typedef struct  cPROC tPROC;
struct cPROC {
   /*---(master)-------------------------*/
   int         line;                        /* line in eos.conf               */
   char        name        [LEN_NAME];      /* short name for reference       */
   char        type;                        /* process type                   */
   char        desc        [LEN_DESC];      /* longer description             */
   char        user        [LEN_NAME];      /* user name                      */
   int         uid;                         /* user id to use to launch job   */
   char        run         [LEN_CMD];       /* command to execute             */
   /*---(processing)---------------------*/
   llong       beg;                         /* start time                     */
   int         rpid;                        /* process id of crond            */
   char        yexec;                       /* end code from yEXEC            */
   int         rc;                          /* return code                    */
   llong       end;                         /* end time                       */
   /*---(done)---------------------------*/
};


/*---(convienence typedefs)-------------------------------------*/
typedef long   long      llong;
typedef const  int       cint;
typedef const  long      clong;
typedef const  char      cchar;

typedef struct FILE      tFILE;
typedef struct stat      tSTAT;
typedef struct passwd    tPASSWD;
typedef struct rusage    tRUSAGE;
typedef struct tm        tTIME;
typedef struct dirent    tDIRENT;
typedef struct timespec  tTSPEC;


extern      char      verstring    [500];

extern      int         requested;
extern      int         running;
extern      int         complete;


extern      char        unit_answer [LEN_RECD];

/*===[[ EOS_MAIN.C ]]=========================================================*/
int         main               (int a_argc, char *a_argv[]);

/*===[[ EOS_PROG.C ]]=========================================================*/
/*---(program)--------------*/
char*       PROG_version       (void);
/*> char        PROG_urgview       (int  a_argc, char *a_argv[]);                     <*/
/*> char        PROG_mountproc     (void);                                            <*/
/*> char        PROG_logtest       (void);                                            <*/
char        PROG_init          (char a_which);
char        PROG_args          (int  a_argc, char *a_argv[]);
char        PROG_begin         (void);
char        PROG_end           (void);
/*---(signals)-------------*/
/*> void        PROG_signal        (int a_signal, siginfo_t *a_info, void *a_nada);   <*/
/*---(unittest)------------*/
char        PROG_testfiles     (void);
char        PROG_testquiet     (void);
char        PROG_testloud      (void);

/*===[[ EOS_CONF.C ]]=========================================================*/
/*---(daemon)---------------*/
char        CONF_daemon        (void);
/*> void        CONF_comm          (int a_sig, siginfo_t *a_info, void *a_nada);      <*/
/*---(config)---------------*/
char        CONF_open          (void);
char        CONF_read          (void);
char        CONF_parse         (void);
char        CONF_list          (void);
char        CONF_close         (void);
char        CONF_report        (char);
char        CONF_tableview     (void);

/*===[[ EOS_EXEC.C ]]=========================================================*/
/*---(processes)------------*/
char        EXEC_checkmount    (char*);
char        EXEC_launch        (void);
char        EXEC_find          (int);
char        EXEC_run           (tPROC*);
char        EXEC_check         (void);
char        EXEC_children      (int);


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        base_config             (void);

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        group_create            (void);
char*       group__unit             (char *a_question, int a_num);

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        proc_create             (char a_type);
char*       proc__unit              (char *a_question, int a_num);

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        rptg_performance        (void);


#endif
/*============================[[ end-of-code ]]===============================*/
