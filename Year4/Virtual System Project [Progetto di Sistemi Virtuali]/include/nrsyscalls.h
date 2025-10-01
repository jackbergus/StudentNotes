#ifndef _NRSYSCALLS_H
#define _NRSYSCALLS_H
#include <asm/unistd.h>

#define _UM_NR_syscalls 309
#ifdef __NR_socketcall
#define _UM_NR_sockcalls 21
#endif

#endif
