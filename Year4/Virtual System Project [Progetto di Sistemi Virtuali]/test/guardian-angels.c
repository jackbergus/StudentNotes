/* Released under the GPL 2+ Dennis Olivetti-Renzo Davoli (c) 2011
   feat. Bergami Giacomo, Marelli Matteo
	 */
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/reg.h>
#include <bits/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <dlfcn.h>
#include <assert.h>
#include <sched.h>
#include <pthread.h>
#include <linux/futex.h>
#include <sys/time.h>

#define REGBYTES (__WORDSIZE/8)
#if REGBYTES == 8
#define REGSIZE long long int
#define REG_ORIG orig_rax
#define REG_RET  rax
#define REG_PAR0 rdi
#define REG_PAR1 rsi
#define REG_PAR2 rdx
#define REG_PAR3 r10
#define REG_PAR4 r8
#define REG_PAR5 r9
#define REG_IP   rip
#else
#define REGSIZE int
#define REG_ORIG orig_eax
#define REG_RET  eax
#define REG_PAR0 ebx
#define REG_PAR1 ecx
#define REG_PAR2 edx
#define REG_PAR3 esi
#define REG_PAR4 edi
#define REG_PAR5 ebp
#define REG_IP   eip
#endif



struct tdata{
	int pid;
	struct user_regs_struct *regs;
	int entering;
	int status;
	int wpid;
	int npid;
};

struct syscall{
	long number;
	REGSIZE params[6];
	long result;
	struct tdata ptrtd;
	void* memalloc;
};

#define MAXTENTA 200
int maxindex = 0;
pid_t mappos[MAXTENTA];
struct syscall arraysyscall[MAXTENTA];

/** initmap():
 *  Initializes all our data-structures
 */
void initmap(void) {
	int i;
	maxindex = 0;
	for (i=0; i<MAXTENTA; i++) { mappos[i]=-1; memset(arraysyscall+i,0,sizeof(struct syscall)); }
}
/** getpos():
 *  Given an added pid, it returns its position 
 */
int getpos(pid_t pid) {
	int i;
	for (i=0; i<maxindex; i++) 
		if (mappos[i]==pid) return i;
	return -1;
}
/** setpid():
 *  Sets a pid as currently traced
 */
int setpid(pid_t pid) {
	int pos = getpos(pid);
	/*printf("pos[sp]=%d\n", pos);*/
	if ((pos!=-1)&&(mappos[pos]!=-1)) return pos;
	else {
		mappos[maxindex++]=pid;;
		pos = maxindex-1;
		arraysyscall[pos].ptrtd.entering = 1;
		arraysyscall[pos].ptrtd.status = 0;
		arraysyscall[pos].ptrtd.wpid = arraysyscall[pos].ptrtd.pid = pid;
	}
	return pos;
}
/** removepid():
 *  Removes the traced pid from the data structure
 */
void removepid(pid_t pid) {
	int pos = getpos(pid);
	//printf("pos[rp]=%d\n", pos);
	if (pos==-1) return;
	if (mappos[pos]==-1) {
	
	
#define freeall(X) 			\
	if (X) {			\
			free(X);	\
			X = NULL;	\
	}
		freeall(arraysyscall[pos].memalloc);
		freeall(arraysyscall[pos].ptrtd.regs);
	}
	if (pos==maxindex-1) {
		mappos[pos--]=-1;
		while (mappos[pos]==-1) pos--;
		maxindex = pos+1;
		return;
	} else {
		mappos[pos] =  -1;
	}
	
}

#if 0

void testing_positions(void) {
	printf("testing_positions...\n\n");
	initmap();
	printf("testing pos: %d\n", getpos(20));
	int id1 = setpid(20);
	int id2 = setpid(20);
	printf("%d == %d\n", id1, id2);
	id2 = setpid(40);
	printf("%d != %d\n", id1, id2);
	int id3 = setpid(30);
	removepid(40);
	int id4 = setpid(80);
	printf("3: %d\t 4: %d\n", id3, id4);
	removepid(80);
	printf("maxpos: %d\t pid: %d\n", maxindex-1, mappos[maxindex-1]);
	removepid(20);
	printf("errore...mappos[id3=%d]==%d, maxpos=%d\n",id3,mappos[id3],maxindex);
	removepid(30);
	printf("maxpos: %d\t pid: %d\n", maxindex-1, mappos[maxindex-1]);
}

void main(void) {
	testing_positions();
}
#endif

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

volatile int lock=0;
#define pthread_mutex_lock(X) ({ \
		while (__sync_lock_test_and_set(&lock,1)) \
		sched_yield(); \
		})
#define pthread_mutex_unlock(X) __sync_lock_release(&lock)

void get_syscall(int pid, struct syscall *sc);
void put_syscall(int pid, struct syscall *sc);
void catch_write(struct syscall *sc, int *entering, pid_t wpid);

int thread_trace(void *p);

int volatile pcount = 1;

/** common_angel():
 *  This function describes the common loop for all the angels */
int common_angel(void* p) {
	int cond = 1;
	while (cond) {
		cond = thread_trace(p);
	}
	return 0;
}

/** sub_angel():
 *  This is every angel, exept the first */
int sub_angel(void *p) {
	struct tdata *td = (struct tdata*)p;
	int e;
	int status;
	//printf("attach %d\n",pid);
	
	/*sleep(3); Just to see the passage */
		/* Attach to the child process */
	if(ptrace(PTRACE_ATTACH,td->pid,0,0)) perror("attach");
	long data=PTRACE_O_TRACEEXIT|PTRACE_O_TRACEFORK|PTRACE_O_TRACEVFORK
		|PTRACE_O_TRACECLONE|PTRACE_O_TRACESYSGOOD|PTRACE_O_TRACEEXEC;
		
	waitpid(td->pid,NULL,0);
	
	if(ptrace(PTRACE_SETOPTIONS, td->pid, 0, data) == -1) perror("setoptions");
	/* Restore traced old status */
	td->regs->REG_IP -= 2;
	td->regs->REG_RET = td->regs->REG_ORIG;
	if(ptrace(PTRACE_SETREGS, td->pid, 0, td->regs)) perror("setregs");
	return common_angel(p);
}

/** main():
 *  The main program 
 */
int main(int argc, char *argv[]) {
	int i;
	int pid = fork();

	if ( !pid ) {
		ptrace( PTRACE_TRACEME, 0, 0, 0 );
		argv++;
		execvp(argv[0], argv);
	}
	else {
		printf("pid: %d\n",pid);
		/* Maybe to wait until the child hasn't executed */
		wait(NULL); 
		long data=PTRACE_O_TRACEEXIT|PTRACE_O_TRACEFORK|PTRACE_O_TRACEVFORK
			|PTRACE_O_TRACECLONE|PTRACE_O_TRACESYSGOOD|PTRACE_O_TRACEEXEC;

		ptrace(PTRACE_SETOPTIONS , pid , NULL , data);
		int pos = setpid(pid);
		common_angel(&arraysyscall[pos].ptrtd);
	}
}


/** thread_trace():
 *  This function is the common pattern for all the angels: traces the forks/clones
 *  occurred, and then generates a new Angel
 *  @param p: 	the tdata 
 *  @return:	it returns 0 if the angel has to stop, else it must return 1
 */
int thread_trace(void *p){
	struct tdata *td = (struct tdata*)p;
	
	/* If wpid is valid say it to continue with its execution until the next syscall is made */
	if(td->wpid != 0) if(ptrace(PTRACE_SYSCALL, td->wpid, 0, 0) != 0); //perror("ptrace_syscall");
		
	/* Wait from any children, wpid is the child whose has woken the tracer */
	td->wpid = wait4(-1, &td->status, __WALL|WUNTRACED|WCONTINUED, 0);
	
	/* If the child has exited or terminated by a signal sent by someone else */
	if (WIFEXITED(td->status) || WIFSIGNALED(td->status)){
		if(td->wpid == td->pid) {
			//printf("wifexited %d\n",wpid);
			pthread_mutex_lock(&mutex);
			removepid(td->pid);// ADDED: Removing from structure
			ptrace(PTRACE_CONT,td->wpid,NULL,WSTOPSIG(td->status)) ;
			pcount--;
			//printf("pcount-=%d\n",pcount);
			pthread_mutex_unlock(&mutex);
			/* Each tracer has just its own traced so it could be terminated */
				
		}else td->wpid = 0;
		return (pcount>0);
	}else if (WIFSTOPPED(td->status)){
		//printf("wifstopped %d\n",wpid);
		if (WSTOPSIG(td->status) == SIGTRAP){
			
		/* Enter here at the second fork call (exiting from it) */
		int event = (td->status >> 16) & 0xffff;
				
		ptrace(PTRACE_GETEVENTMSG, td->wpid, NULL, &td->npid);
				
			switch(event){
				case PTRACE_EVENT_FORK:
				case PTRACE_EVENT_VFORK:
				case PTRACE_EVENT_CLONE:
					pthread_mutex_lock(&mutex);
					//printf("pcount*=%d\n",pcount);
					pcount++;
					pthread_mutex_unlock(&mutex);
					//printf("fork %d\n",npid);
					return (pcount>0);
				case PTRACE_EVENT_EXEC:
				case PTRACE_EVENT_VFORK_DONE:	
				case PTRACE_EVENT_EXIT:
					pcount--;
					return (pcount>0);
				default:
					printf("unknown %d\n",td->npid);
						
			}
		} else {
				/* All other syscall entry point */
				/*printf("Signal (Signal Throwed) %d\n", WSTOPSIG(status));*/
		}
	} else
		printf("Status unknown: %d\n", td->status);

		if(td->wpid == 0 || !(td->status & 0x8000) ) return 1;
		
		get_syscall(td->wpid, arraysyscall + getpos(td->wpid));

		if(td->wpid != td->pid){
			/* Syscall of new process */
			int e,i,st;

			struct user_regs_struct *tregs = malloc(sizeof(struct user_regs_struct));
			/*struct tdata *t = malloc(sizeof(struct tdata));*/
			int pos = setpid(td->wpid);
			struct syscall *sc =  arraysyscall + pos;
			
			ptrace(PTRACE_GETREGS, td->wpid, 0, tregs );
			
			/* Replace the first syscall in a blocking poll */
			sc->number = __NR_poll;
			sc->params[0] = 0;
			sc->params[1] = 0;
			sc->params[2] = -1;
			put_syscall(td->wpid, sc);
			
			/* Detach this tracer from the new process (it's blocked so it can't escape) */ 
			if(ptrace(PTRACE_DETACH,td->wpid,NULL,0)) perror("ptrace detach");
			
			sc->ptrtd.pid = td->wpid;
			sc->ptrtd.regs = tregs;
			//printf("avvio thread\n");
			
			/* ADD: preparing new memory */
			sc->memalloc = (void*)malloc(8000000)+8000000;
			
			clone(sub_angel,
					(char*)sc->memalloc,
					CLONE_VM|CLONE_FS|CLONE_FILES|/*CLONE_SIGHAND|CLONE_THREAD|*/CLONE_SYSVSEM,
					(void*)&sc->ptrtd,NULL,NULL,NULL
					);
			/*pthread_create(NULL,NULL,thread_trace,t);*/
			td->wpid = 0;
			return 1;
		}
		
		catch_write(arraysyscall + getpos(td->wpid), &td->entering, td->wpid);
		return 1;
}

/** catch_write(): 
 * Checks if the caught syscall is a write and make a toupper() to all its characters */
void catch_write(struct syscall *sc, int *entering, pid_t wpid){
	if(sc->number == SYS_write && sc->params[0] == 1){
		if (*entering){
			*entering = 0;
			int nword = sc->params[2]/REGBYTES + (sc->params[2]%REGBYTES?1:0);
			char *buf = malloc(nword*REGBYTES);
			int i;
			for(i = 0; i < nword; i++){
				*(REGSIZE*)(buf+REGBYTES*i) = ptrace(PTRACE_PEEKDATA, wpid, (char*)sc->params[1]+REGBYTES*i, 0);
			}
			for( i = 0; i < (REGBYTES * nword); i++){
				buf[i] = toupper(buf[i]);
			}
			for( i = 0; i < nword; i++){
				ptrace(PTRACE_POKEDATA, wpid, (char*)sc->params[1]+REGBYTES*i, *(REGSIZE*)(buf+REGBYTES*i));
			}
			free(buf);
			
		} else {
			*entering = 1;
		}
	}
}

/** get_syscall():
 * Get the registers of the syscall invoked by pid and 
 * store them in the sc syscall struct */
void get_syscall(int pid, struct syscall *sc){
	struct user_regs_struct regs;
	memset(&regs,0,sizeof(regs));
	if( ptrace( PTRACE_GETREGS, pid, 0, &regs ) != 0 )perror("getregs");
	sc->number = regs.REG_ORIG;
	sc->result = regs.REG_RET;
	//The kernel interface uses %rdi, %rsi, %rdx, %r10, %r8 and %r9.
	sc->params[0] = regs.REG_PAR0;
	sc->params[1] = regs.REG_PAR1;
	sc->params[2] = regs.REG_PAR2;
	sc->params[3] = regs.REG_PAR3;
	sc->params[4] = regs.REG_PAR4;
	sc->params[5] = regs.REG_PAR5;
	//printf("%d: get_syscall-> %d\n",getpid(),sc->number); 
}

/** put_syscall():
 * Get the registers of the syscall invoked by pid and 
 * replace them with the params stored in the sc sycall struct */
void put_syscall(int pid, struct syscall *sc){
	struct user_regs_struct regs;
	if(ptrace( PTRACE_GETREGS, pid, 0, &regs ) != 0)perror("getregs2");
	regs.REG_ORIG = sc->number;
	regs.REG_RET = sc->result;
	regs.REG_PAR0 = sc->params[0];
	regs.REG_PAR1 = sc->params[1];
	regs.REG_PAR2 = sc->params[2];
	regs.REG_PAR3 = sc->params[3];
	regs.REG_PAR4 = sc->params[4];
	regs.REG_PAR5 = sc->params[5];
	//printf("%d: put_syscall-> %d\n",getpid(),sc->number);
	if(ptrace( PTRACE_SETREGS, pid, 0, &regs ) != 0)perror("setregs");
}
