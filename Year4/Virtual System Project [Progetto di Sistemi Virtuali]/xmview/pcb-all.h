/* DO NOT EDIT. File Automagically generated */
#ifdef _PCB_DEFINITIONS

#	ifdef _VIEWOS_KM
#		include <kmview.h>
#	endif


#	define PCB_INUSE 0x1 
                      /* INUSE=0: unused element ready for allocation.  
												 never = 0 for running processes pcb, 
												 INUSE=0 is a flag for pcb managed outside capture_um (capture_nested) */
#	define PCB_ALLOCATED 0x2
                        /* Dynamically allocated pcb, to be freed. */
#	define PCB_SIGNALED 0x4
                        /* awake suspended process as it has been signaled */
#	ifdef _VIEWOS_KM
#	define PCB_KM_PRIVATEDIR 0x8 /* KM_CHROOT has already been sent (KMVIEW only) */
# endif

#	ifdef _VIEWOS_UM
#		define PCB_STARTING 0x8
                        /* the process/thread is starting */
#		define NOSC -1
#	endif

/* constants are compatible with PTRACE_SYS_VM definitions */
#	define SC_SKIP_CALL 0x5 /* SKIP_CALL */
#	define SC_SKIP_EXIT 0x2 /* SKIP_EXIT */
#	define SC_SAVEREGS 0x8
#	define SC_SUSPENDED 0x10
#	define SC_SUSPIN 0x10     /* SUSPENDED + IN  */
#	define SC_SUSPOUT 0x11    /* SUSPENDED + OUT */

#	define SC_VM_MASK 0x7 /*mask for SYS_VM */

#	define STD_BEHAVIOR SC_SKIP_EXIT  /* DO_SYSCALL SKIP_EXIT */
#	define SC_MODICALL (SC_SKIP_EXIT | SC_SAVEREGS) /* SKIP_EXIT and save regs */
#	define SC_FAKE (SC_SKIP_CALL | SC_SKIP_EXIT | SC_SAVEREGS) 
#	define SC_CALLONXIT (SC_SAVEREGS)
#	define SC_TRACEONLY ( 0 )

#	define IN 0
#	define OUT 1

#endif 

#ifdef _PCB_COMMON_FIELDS
	void *stack;		/**< The stackptr of the process */
	uint16_t flags;
	pid_t  its_angel;
	int  is_dead;        /* Indicates if the process is dead */
#	ifdef _VIEWOS_KM
#		ifdef __NR_socketcall
			struct kmview_event_socketcall event; 
#		else
			struct kmview_event_ioctl_syscall event; 
#		endif
		struct kmview_event_ioctl_sysreturn outevent; 
		long erno;
#	endif
#	ifdef _VIEWOS_UM
		long sysscno;              /**< System call number */
		unsigned long sysargs[6];
#	ifdef __NR_socketcall
		long sockaddr;
#	endif
		unsigned long erno;
#	endif
#endif

#ifdef _PCB_ONLY_FIELDS
	long umpid;
#	ifdef _VIEWOS_KM
	long kmpid;
#	endif
	pid_t pid;                /* Process Id of this entry */
	int signum;
#	ifdef _PROC_MEM_TEST
		int memfd; /* if !has_ptrace_multi, open /proc/PID/mem */
#	endif
	struct pcb *pp;         /* Parent Process */
	uint16_t behavior;
	long retval;
#	ifdef _VIEWOS_UM
		long *saved_regs;
#	endif
#endif
#ifdef _PCB_DEFINITIONS

/* STATUS DEFINITIONS */
void mainpoll_addproc(),mainpoll_delproc(),mainpoll_init();

#define READY 0
#define BLOCKED 1
#define WAKE_ME_UP 2
#define TERMINATED 3

#define bq_block(pc) ((pc)->pollstatus=BLOCKED)
#define bq_unblock(pc) ((pc)->pollstatus=READY)

#endif

#ifdef _PCB_COMMON_FIELDS
int pollstatus;
#endif

#ifdef _PCB_ONLY_FIELDS
#endif

#ifdef _NPCB_ONLY_FIELDS
#endif

#ifdef _PCB_CONSTRUCTOR
mainpoll_addproc,
#endif
#ifdef _PCB_DESTRUCTOR
mainpoll_delproc,
#endif
#ifdef _PCB_INITS
mainpoll_init,
#endif
#ifdef _PCB_DEFINITIONS
#define MAX_SOCKET_ARGS 6

struct supgroups {
	int count;
	size_t size;
	gid_t list[0];
};
struct supgroups *supgrp_create(size_t size);
struct supgroups *supgrp_get(struct supgroups *supgrp);
void supgrp_put(struct supgroups *supgrp);

void pcb_plus(),pcb_minus();
/* STATUS DEFINITIONS */
#define READY 0

#endif

#ifdef _PCB_COMMON_FIELDS
void *path;
struct stat64 pathstat;
struct ht_elem *hte;
void *mod_private_data;
struct timestamp tst;
epoch_t nestepoch;
/* path for tmp files that must be deleted over the next syscall */
/* see execve mgmt */
void *tmpfile2unlink_n_free;
uid_t ruid,euid,suid,fsuid;
gid_t rgid,egid,sgid,fsgid;
struct supgroups *grouplist;
uint16_t private_scno;
uint8_t needs_path_rewrite;
#endif

#ifdef _PCB_ONLY_FIELDS
/* keep track of file system informations - look at clone 2
 *    * (CLONE_FS) */
struct pcb_fs *fdfs;
#endif

#ifdef _NPCB_ONLY_FIELDS
#endif

#ifdef _PCB_CONSTRUCTOR
pcb_plus,
#endif
#ifdef _PCB_DESTRUCTOR
pcb_minus,
#endif
#ifdef _PCB_DEFINITIONS
struct pcb_file;
void umproc_addproc(),umproc_delproc();
#endif

#ifdef _PCB_COMMON_FIELDS
#endif

#ifdef _PCB_ONLY_FIELDS
/* file descriptors of this process */
struct pcb_file *fds;
#endif

#ifdef _NPCB_ONLY_FIELDS
#endif

#ifdef _PCB_CONSTRUCTOR
umproc_addproc,
#endif

#ifdef _PCB_DESTRUCTOR
umproc_delproc,
#endif
#ifdef _UM_MMAP

#ifdef _PCB_DEFINITIONS
struct pcb_mmap_entry;
void um_mmap_addproc(),um_mmap_delproc();
#endif

#ifdef _PCB_COMMON_FIELDS
#endif

#ifdef _PCB_ONLY_FIELDS
struct pcb_mmap_entry *um_mmap;
#endif

#ifdef _NPCB_ONLY_FIELDS
#endif

#ifdef _PCB_CONSTRUCTOR
um_mmap_addproc,
#endif

#ifdef _PCB_DESTRUCTOR
um_mmap_delproc,
#endif

#endif
#ifdef _UM_MMAP

#ifdef _PCB_DEFINITIONS
void um_select_addproc(),um_select_delproc();
#endif

#ifdef _PCB_COMMON_FIELDS
/* struct seldata* */
void *selset;
#endif

#ifdef _PCB_ONLY_FIELDS
#endif

#ifdef _NPCB_ONLY_FIELDS
#endif

#ifdef _PCB_CONSTRUCTOR
um_select_addproc,
#endif

#ifdef _PCB_DESTRUCTOR
um_select_delproc,
#endif

#endif
