//PCB.C

voidf 
//DEFINENDO _PCB_CONSTRUCTOR
mainpoll_addproc,
mainpoll_delproc,
pcb_plus,
pcb_minus,
umproc_addproc,
umproc_delproc,

//DEFINENDO _PCB_DESTRUCTORS 
mainpoll_delproc,
pcb_minus,
umproc_delproc,
dummy;

static voidfun constructors[] = {
//DEFINENDO _PCB_CONSTRUCTOR 
mainpoll_addproc,
mainpoll_delproc,
pcb_plus,
pcb_minus,
umproc_addproc,
umproc_delproc,
};
#define NCONSTRUCTORS sizeof(constructors)/sizeof(voidfun)

static voidfun destructors[] = {
//DEFINENDO _PCB_DESTRUCTOR
mainpoll_delproc,
mainpoll_init,
pcb_minus,
umproc_delproc,
};
#define NDESTRUCTORS sizeof(destructors)/sizeof(voidfun)

static voidfun inits[] = {
//DEFINENDO _PCB_INITS 
mainpoll_addproc,
pcb_plus,
umproc_addproc,
};


PCB.H

/** struct pcb: */
struct pcb {
/* @{ */
/* Note: in this file there is not first argument. BTW, inside the capture_um
   file there is a pid_t first_child_pid */
//DEFINENDO _PCB_COMMON_FIELDS 
uint16_t flags;
long sysscno;              /* System call number */
unsigned long sysargs[6];
unsigned long erno;
int pollstatus;
mainpoll_addproc,
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
/*pcb_plus,
umproc_addproc,*/

void *stack; 	/**< NEW: process memory given */

//DEFINENDO _PCB_ONLY_FIELDS 
long umpid;
pid_t pid;		/**< Process Id of this entry */
int signum;
struct pcb *pp;		/**< Parent Process */
uint16_t behavior;
long retval;		/**< Return value */
long *saved_regs;	/**< Arguments for proceses' syscalls*/
/*mainpoll_addproc,*/
/* keep track of file system informations - look at clone 2
 *    * (CLONE_FS) */
struct pcb_fs *fdfs;
/*pcb_plus,*/
struct pcb_file *fds;	/**< File descriptors of this process */
/*umproc_addproc,*/
/* }@ */
};
  
struct npcb {
//DEFINENDO _PCB_COMMON_FIELDS 
uint16_t flags;
long sysscno;              /* System call number */
unsigned long sysargs[6];
unsigned long erno;
int pollstatus;
mainpoll_addproc,
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
/*pcb_plus,
umproc_addproc,
 
//DEFINENDO _NPCB_ONLY_FIELDS 
mainpoll_addproc,
pcb_plus,
umproc_addproc,*/
};
















