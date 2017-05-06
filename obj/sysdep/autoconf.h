/* obj/sysdep/autoconf.h.  Generated from autoconf.h.in by configure.  */
/*
 *	This file contains all system parameters automatically
 *	discovered by the configure script.
 */

/* System configuration file */
#define SYSCONF_INCLUDE "./sysdep/cf/linux.h"

/* Include debugging code */
/* #undef DEBUGGING */

/* 8-bit integer type */
#define INTEGER_8 char

/* 16-bit integer type */
#define INTEGER_16 short int

/* 32-bit integer type */
#define INTEGER_32 int

/* 64-bit integer type */
#define INTEGER_64 long int

/* CPU endianity */
#define CPU_LITTLE_ENDIAN 1
/* #undef CPU_BIG_ENDIAN */

/* Usual alignment for structures */
#define CPU_STRUCT_ALIGN 16

/* Characteristics of time_t */
#define TIME_T_IS_64BIT 1
#define TIME_T_IS_SIGNED 1

/* We have struct ip_mreqn in <netinet/in.h> */
#define HAVE_STRUCT_IP_MREQN 1

/* Protocols compiled in */
#define CONFIG_STATIC 1
#define CONFIG_RIP 1
/* #undef CONFIG_RADV */
#define CONFIG_BFD 1
#define CONFIG_BGP 1
#define CONFIG_OSPF 1
#define CONFIG_PIPE 1

/* We use multithreading */
#define USE_PTHREADS 1

/* We have <syslog.h> and syslog() */
#define HAVE_SYSLOG 1

/* We have <alloca.h> */
#define HAVE_ALLOCA_H 1

/* Are we using dmalloc? */
/* #undef HAVE_LIBDMALLOC */

/* Readline stuff */
#define HAVE_RL_CRLF 1
#define HAVE_RL_DING 1

/* struct sockaddr_in(6) */
/* #undef HAVE_SIN_LEN */

/* We have stdint.h */
#define HAVE_STDINT_H 1

#define CONFIG_PATH ?
