#ifndef OVERALL_SETTINGS_FILE
#define OVERALL_SETTINGS_FILE
#define OVERALL_SETTINGS_MOBSEL
#define OVERALL_SETTINGS_ETHCONT

#include <limits.h>
#include <string.h>
#include <errno.h>
#define USHRT_MIN 0

/* Typedefs */
typedef unsigned char byte;
typedef unsigned short coord_t;

/* Limits for types */

#define COORD_MAX USHRT_MAX
#define COORD_MIN USHRT_MIN

/* Flags of compile */
#define PRINT_LOGS
#define PRINT_ERRORS
#define PRINT_DEBUG
#define MAKE_GLOBAL_SXSY

#define SLEEP_TIME 10

/* Definitions for debug and logs */

#ifdef PRINT_DEBUG
#define SIZE_BUF_DEBUG 65535
#endif

//Definition LOG_CREATE and LOG_CLOSE
#if defined(PRINT_LOGS) || defined(PRINT_ERRORS)

#include <syslog.h>

#define LOG_OPT_DEFAULT LOG_NDELAY|LOG_PID
#define LOG_FAC_DEFAULT LOG_USER

#define LOG_CREATE() openlog("submarine.out", LOG_OPT_DEFAULT, LOG_FAC_DEFAULT);
#define LOG_CREATE_OPT(OPTIONS) openlog("submarine.out", OPTIONS, LOG_FAC_DEFAULT);
#define LOG_CREATE_FAC(FACILITY) openlog("submarine.out", LOG_OPT_DEFAULT, FACILITY);
#define LOG_CREATE_OPT_FAC(OPTIONS,FACILITY) openlog("submarine.out", OPTIONS, FACILITY);
#define LOG_CLOSE() closelog();

#else

#define LOG_CREATE() ;
#define LOG_CREATE_OPT(OPTIONS) ;
#define LOG_CREATE_FAC(FACILITY) ;
#define LOG_CREATE_OPT_FAC(OPTIONS,FACILITY) ;
#define LOG_CLOSE() ;

#endif


//Definition LOG_PRINT
#ifdef PRINT_LOGS

#include <syslog.h>

#define LOG_PRIOR_DEFAULT LOG_INFO

#define LOG_PRINT(STR) syslog(LOG_PRIOR_DEFAULT, STR);
#define LOG_PRINT1(STR, A) syslog(LOG_PRIOR_DEFAULT, STR, A);
#define LOG_PRINT2(STR, A, B) syslog(LOG_PRIOR_DEFAULT, STR, A, B);
#define LOG_PRINT3(STR, A, B, C) syslog(LOG_PRIOR_DEFAULT, STR, A, B, C);
#define LOG_PRINT4(STR, A, B, C, D) syslog(LOG_PRIOR_DEFAULT, STR, A, B, C, D);
#define LOG_PRINT5(STR, A, B, C, D, E) syslog(LOG_PRIOR_DEFAULT, STR, A, B, C, D, E);

#define LOG_PRINT_PR(PRIOR, STR) syslog(PRIOR, STR);
#define LOG_PRINT_PR1(PRIOR, STR, A) syslog(PRIOR STR, A);
#define LOG_PRINT_PR2(PRIOR, STR, A, B) syslog(PRIOR, STR, A, B);
#define LOG_PRINT_PR3(PRIOR, STR, A, B, C) syslog(PRIOR, STR, A, B, C);
#define LOG_PRINT_PR4(PRIOR, STR, A, B, C, D) syslog(PRIOR, STR, A, B, C, D);
#define LOG_PRINT_PR5(PRIOR, STR, A, B, C, D, E) syslog(PRIOR, STR, A, B, C, D, E);

#else

#define LOG_PRINT(STR) ;
#define LOG_PRINT1(STR, A) ;
#define LOG_PRINT2(STR, A, B) ;
#define LOG_PRINT3(STR, A, B, C) ;
#define LOG_PRINT4(STR, A, B, C, D) ;
#define LOG_PRINT5(STR, A, B, C, D, E) ;

#define LOG_PRINT_PR(PRIOR, STR) ;
#define LOG_PRINT_PR1(PRIOR, STR, A) ;
#define LOG_PRINT_PR2(PRIOR, STR, A, B) ;
#define LOG_PRINT_PR3(PRIOR, STR, A, B, C) ;
#define LOG_PRINT_PR4(PRIOR, STR, A, B, C, D) ;
#define LOG_PRINT_PR5(PRIOR, STR, A, B, C, D, E) ;

#endif

//Definition LOG_PRINT_ERR
#ifdef PRINT_ERRORS

#include <syslog.h>

#define LOG_PRINT_ERR(STR) LOG_PRINT(STR);LOG_PRINT1("%s", strerror(errno));
#define LOG_PRINT_ERR1(STR, A) LOG_PRINT1(STR, A);LOG_PRINT1("%s", strerror(errno));
#define LOG_PRINT_ERR2(STR, A, B) LOG_PRINT2(STR, A, B);LOG_PRINT1("%s", strerror(errno));
#define LOG_PRINT_ERR3(STR, A, B, C) LOG_PRINT3(STR, A, B, C);LOG_PRINT1("%s", strerror(errno));
#define LOG_PRINT_ERR4(STR, A, B, C, D) LOG_PRINT4(STR, A, B, C, D);LOG_PRINT1("%s", strerror(errno));
#define LOG_PRINT_ERR5(STR, A, B, C, D, E) LOG_PRINT5(STR, A, B, C, D, E);LOG_PRINT1("%s", strerror(errno));

#else

#define LOG_PRINT_ERR(STR) ;
#define LOG_PRINT_ERR1(STR, A) ;
#define LOG_PRINT_ERR2(STR, A, B) ;
#define LOG_PRINT_ERR3(STR, A, B, C) ;
#define LOG_PRINT_ERR4(STR, A, B, C, D) ;
#define LOG_PRINT_ERR5(STR, A, B, C, D, E) ;

#endif

/* Global sx and sy */
#ifdef MAKE_GLOBAL_SXSY
static coord_t sx = 1280;
static coord_t sy = 1024;
#endif



#endif
