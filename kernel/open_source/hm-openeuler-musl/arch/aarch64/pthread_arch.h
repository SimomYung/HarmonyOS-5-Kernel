static inline uintptr_t __get_tp(void)
{
	uintptr_t tp;
	__asm__ ("mrs %0,tpidr_el0" : "=r"(tp));
	return tp;
}

#define TLS_ABOVE_TP
#ifdef __ILP32__
#define GAP_ABOVE_TP 8
#else
#define GAP_ABOVE_TP 16
#endif

#define MC_PC pc
