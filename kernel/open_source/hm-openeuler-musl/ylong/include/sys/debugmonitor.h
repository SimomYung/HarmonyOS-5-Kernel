

#ifndef _SYS_DEBUG_MONITOR_H
#define _SYS_DEBUG_MONITOR_H


#define DM_GDBCTRLC         0x99 /* reaction for ctrl+c of gdb */
#define DM_CHANGETHREAD     0x83 /* change thread for gdb - command `thread threadno` */
#define DM_GETNBTHREADS     0x76 /* get the number of threads of the process who owns debugged thread */
#define DM_GETLISTTHREADS   0x69 /* get the list of threads of the process who owns debugged thread */
#define DM_THREAD_ALIVE     0x70  /* return to gdb the info if a thread exists and is alive (not exited, not unused) */
#define DM_GESIGNAL_TTID    0x71 /* return the tid of the thread, which owns a signal(first one in task list) */
#define DM_SET_SW_BP        0x300 /* set a sw breakpoint */
#define DM_SET_HW_BP        0x301 /* set a hw breakpoint */
#define DM_SET_WP_W         0x302 /* set a hw read watchpoint */
#define DM_SET_WP_R         0x303 /* set a hw write watchpoint */
#define DM_SET_WP_A         0x304 /* set a hw acess watchpoint */
#define DM_DELETE_SW_BP     0x400 /* delete a sw breakpoint */
#define DM_DELETE_HW_BP     0x401 /* delete a hw breakpoint */
#define DM_DELETE_WP_W      0x402 /* delete a hw watchpoint */
#define DM_DELETE_WP_R      0x403 /* delete a hw watchpoint */
#define DM_DELETE_WP_A      0x404 /* delete a hw watchpoint */

/* System call */
long debugmonitor(int request, int pid, void *addr, void *data);

#endif /* _SYS_DEBUG_MONITOR_H */
