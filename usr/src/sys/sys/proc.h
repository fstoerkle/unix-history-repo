/*	proc.h	3.2	%H%	*/

/*
 * One structure allocated per active
 * process. It contains all data needed
 * about the process while the
 * process may be swapped out.
 * Other per process data (user.h)
 * is swapped with the process.
 *
 * NB: OFFSETS HERE ARE ALSO DEFINED IN proc.m
 */
struct	proc
{
	struct	proc *p_link;	/* linked list of running processes */
	struct	proc *p_rlink;
	struct	pte *p_addr;	/* u-area kernel map address */
	char	p_usrpri;	/* user-priority based on p_cpu and p_nice */
	char	p_pri;		/* priority, negative is high */
	char	p_cpu;		/* cpu usage for scheduling */
	char	p_stat;
	char	p_time;		/* resident time for scheduling */
	char	p_nice;		/* nice for cpu usage */
	char	p_slptime;	/* time since last block */
	int	p_flag;
	int	p_sig;		/* signals pending to this process */
	int	p_ignsig;	/* ignored signals */
	short	p_uid;		/* user id, used to direct tty signals */
	short	p_pgrp;		/* name of process group leader */
	short	p_pid;		/* unique process id */
	short	p_ppid;		/* process id of parent */
	short	p_poip;		/* count of page outs in progress */
	short	p_szpt;		/* copy of page table size */
	size_t	p_tsize;	/* size of text (clicks) */
	size_t	p_dsize;	/* size of data space (clicks) */
	size_t	p_ssize;	/* copy of stack size (clicks) */
	size_t 	p_rssize; 	/* current resident set size in clicks */
	size_t	p_swrss;	/* resident set size before last swap */
	swblk_t	p_swaddr;	/* disk address of u area when swapped */
	caddr_t p_wchan;	/* event process is awaiting */
	struct	text *p_textp;	/* pointer to text structure */
	int	p_clktim;	/* time to alarm clock signal */
	struct	pte *p_p0br;	/* page table base P0BR */
	struct	proc *p_xlink;	/* linked list of procs sharing same text */
	short	p_faults;	/* faults in last second */
	short	p_aveflt;	/* average of p_faults into past */
	short	p_ndx;		/* proc index for memall (because of vfork) */
	short	p_idhash;	/* hashed based on p_pid for kill+exit+... */
};

#define	PIDHSZ		63
#define	PIDHASH(pid)	((pid) % PIDHSZ)

#ifdef KERNEL
short	pidhash[PIDHSZ];

struct	proc *pfind();
#endif

#ifdef	KERNEL
extern	struct proc proc[];	/* the proc table itself */

#ifdef FASTVAX
#define	NQS	32		/* 32 run queues */
struct	prochd {
	struct	proc *ph_link;	/* linked list of running processes */
	struct	proc *ph_rlink;
} qs[NQS];
int	whichqs;		/* bit mask summarizing non-empty qs's */
#else
struct	proc *runq;
#endif
#endif

/* stat codes */
#define	SSLEEP	1		/* awaiting an event */
#define	SWAIT	2		/* (abandoned state) */
#define	SRUN	3		/* running */
#define	SIDL	4		/* intermediate state in process creation */
#define	SZOMB	5		/* intermediate state in process termination */
#define	SSTOP	6		/* process being traced */

/* flag codes */
#define	SLOAD	0x00001		/* in core */
#define	SSYS	0x00002		/* swapper or pager process */
#define	SLOCK	0x00004		/* process being swapped out */
#define	SSWAP	0x00008		/* save area flag */
#define	STRC	0x00010		/* process is being traced */
#define	SWTED	0x00020		/* another tracing flag */
#define	SULOCK	0x00040		/* user settable lock in core */
#define	SPAGE	0x00080		/* process in page wait state */
#define	SKEEP	0x00100		/* another flag to prevent swap out */
#define	SDLYU	0x00200		/* delayed unlock of pages */
#define	SWEXIT	0x00400		/* working on exiting */
#define	SPHYSIO	0x00800		/* doing physical i/o (bio.c) */
#define	SVFORK	0x01000		/* process resulted from vfork() */
#define	SVFDONE	0x02000		/* another vfork flag */
#define	SNOVM	0x04000		/* no vm, parent in a vfork() */
#define	SPAGI	0x08000		/* init data space on demand, from inode */
#define	SANOM	0x10000		/* system detected anomalous vm behavior */
#define	SUANOM	0x20000		/* user warned of anomalous vm behavior */
#define	STIMO	0x40000		/* timing out during sleep */

/*
 * parallel proc structure
 * to replace part with times
 * to be passed to parent process
 * in ZOMBIE state.
 *
 * THIS SHOULD BE DONE WITH A union() CONSTRUCTION
 */
struct	xproc
{
	struct	proc *xp_link;
	struct	proc *xp_rlink;
	struct	pte *xp_addr;
	char	xp_usrpri;
	char	xp_pri;		/* priority, negative is high */
	char	xp_cpu;		/* cpu usage for scheduling */
	char	xp_stat;
	char	xp_time;	/* resident time for scheduling */
	char	xp_nice;	/* nice for cpu usage */
	char	xp_slptime;
	int	xp_flag;
	int	xp_sig;		/* signals pending to this process */
	int	xp_ignsig;
	short	xp_uid;		/* user id, used to direct tty signals */
	short	xp_pgrp;	/* name of process group leader */
	short	xp_pid;		/* unique process id */
	short	xp_ppid;	/* process id of parent */
	short	xp_xstat;	/* Exit status for wait */
	struct	vtimes xp_vm;
};
