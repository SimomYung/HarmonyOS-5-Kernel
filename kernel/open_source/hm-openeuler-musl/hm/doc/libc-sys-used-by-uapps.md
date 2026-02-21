| interface | devhost/devmgr | hguard | tppmgr | crypto | hmnetd |
| :---------: | :--------------: | :--------------: | :--------------: | :--------------: | :--------------: |
| __tl_lock | Y |        |        |        |      |
| pthread_create | Y | | Y | | |
| __wait | Y | | | | |
| vmlock | Y | | | | |
| clone | Y | | | | |
| __unmapself | Y | | | | |
| __tl_unlock | Y | | | | |
| pthread_atfork | Y | | | | |
| fesetround | Y | | | | |
| fegetround | Y | | | | |
| dprintf | Y | | | | |
| vdprintf | Y | | | | |
| longjmp | Y | | | | |
| open | Y | | | | |
| __block_app_sigs | Y | | | | |
| __restore_sigs | Y | | | | |
| __membarrier | Y | | | | |
| sem_post | Y | | | | |
| sem_init | Y | | | | |
| sem_wait | Y | | | | |
| sem_timedwait | Y | | | | |
| sem_trywait | Y | | | | |
| pthread_testcancel | Y | | | | |
| pthread_self | Y | | Y | | |
| pthread_kill | Y | | | | |
| pthread_cleanup_push | Y | | | | |
| __timedwait | Y | | | | |
| sem_destroy | Y | | | | |
| sigaction | Y | | | | |
| __restore | Y | | | | |
| mmap | Y | | | | |
| munmap | Y | | | | |
| read | Y | | | | |
| pread | Y | | | | |
| __syscall_cp | Y | | | | |
| mprotect | Y | | | | |
| pthread_mutex_lock | Y | | | | |
| pthread_mutex_timedlock | Y | | | | |
| pthread_mutex_trylock | Y | | | | |
| pthread_mutex_unlock | Y | | | | |
| pthread_cond_wait | Y | | | | |
| pthread_cond_timedwait | Y | | | | |
| pthread_cond_broadcast | Y | | | | |
| fstat | Y | | | | |
| fstatat | Y | | | | |
| procfdname | Y | | | | |
| close | Y | | | | Y |
| readlink | Y | | | | |
| fopen | Y | Y | | | |
| __fmodeflags | Y | | | | |
| __fdopen | Y | | | | |
| __ofl_add | Y | | | | |
| fclose | Y | Y | | | |
| fflush | Y | | | | |
| getdelim | Y | | | | |
| pthread_rwlock_wrlock | Y | | | | |
| pthread_rwlock_timedwrlock | Y | | | | |
| pthread_rwlock_trywrlock | Y | | | | |
| pthread_rwlock_rdlock | Y | | | | |
| pthread_rwlock_timedrdlock | Y | | | | |
| pthread_rwlock_unlock | Y | | | | |
| _exit | Y | | | | |
| __libc_version | Y | | | | |
| pthread_setcancelstate | Y | | | | |
| __inhibit_ptc | Y | | | | |
| __acquire_ptc | Y | | | | |
| __release_ptc | Y | | | | |
| setjmp | Y | | | | |
| raise | Y | | | | |
| abort | Y | | | Y | Y |
| getpid | Y | | | | |
| sigemptyset | Y | | | | |
| sigprocmask | Y | | | | |
| pthread_sigmask | Y | | | | |
| timer_create | Y | | | | |
| pthread_barrier_wait | Y | | | | |
| sigwaitinfo | Y | | | | |
| sigtimedwait | Y | | | | |
| __reset_tls | Y | | | | |
| pthread_once | Y | | | | |
| pthread_attr_setdetachstate | Y | | | | |
| pthread_barrier_init | Y | | | | |
| timer_settime | Y | | | | |
| access | Y | | | | Y |
| closedir | Y | Y | | | |
| opendir | Y | Y | | | |
| readdir | Y | Y | | | |
| getopt_long | Y | | | | |
| getopt | Y | | | | |
| getsubopt | Y | | | | |
| mblen | Y | | | | |
| fputs | Y | | | | |
| putc | Y | | | | |
| __overflow | Y | | | | |
| realpath | Y | | | | |
| stat | Y | | | | |
| waitpid | Y | | | | |
| timer_delete | Y | | | | |
| printf | Y | | | | |
| fork | Y | | | | |
| fgets | Y | Y | | | |
| wcstombs | Y | | | | |
| wcsrtombs | Y | | | | |
| ungetwc | Y | | | | |
| fwide | Y | | | | |
| __c_locale | Y | | | | |
| fgetwc | Y | | | | |
| mbrtowc | Y | | | | |
| getline | Y | | | | |
| sched_yield | Y | | | | |
| bsearch | Y | | | | |
| pthread_mutex_init | Y | | | | |
| posix_madvise | Y | | | | |
| nanosleep | Y | Y | | | |
| clock_nanosleep | Y | Y | | | |
| localtime_r | Y | | | | |
| __secs_to_zone |  | | | | |
| __map_file |  | | | | |
| kill | | Y | | | |
| prctl | | Y | | | |
| pthread_setchedparam | | Y | | | |
| pthread_join | | Y | | | |
| __assert_fail | Y |  | | | |
| sleep | | Y | Y | | |
| sync | | Y | | | |
| pthread_setcanceltype | | | Y | | |
| pthread_setname_np | | | Y | | |
| pthread_attr_setstacksize | | | Y | | |
| pthread_attr_setinheritsched | | | Y | | |
| sched_getscheduler | | | | Y | |
| dirname | | | | | |
| malloc | Y | Y | | Y | Y |
| __tls_get_addr | Y | Y | | Y | Y |
| write | Y | Y | | Y | Y |
| madvise | Y | Y | | Y | Y |
| fprintf | Y | Y | | Y | Y |
| pthread_attr_setschedpolicy | | | Y | | |
| pthread_attr_setschedparam | | | Y |  | |
| sched_get_priority_max | | | Y | | |
| sched_get_priority_min | | | Y | |  |
| htonl | | |  | | Y |
| htons | | |  | | Y |
| ntohl | | |  | | Y |
| ntohs | | |  | | Y |
| inet_aton | | |  | | Y |
| inet_ntop | | |  | | Y |
| regexec | | |  | | Y |
| tre_mem | | |  | | Y |
| regcomp | | |  | | Y |
| rand | | |  | | Y |
| random | | |  | | Y |
| gettimeofday | | |  | | Y |
| ioctl | | | Y | | |
| puts | | | | | Y |
| getauxval | Y | | | | |
