#ifndef _SYS_PROCFS_H
#define _SYS_PROCFS_H

#include <sys/time.h>
#include <sys/types.h>
#include <sys/user.h>
#include <stdbool.h>

struct elf_siginfo {
	int si_signo;
	int si_code;
	int si_errno;
};

struct elf_prstatus {
	struct elf_siginfo pr_info;
	short int pr_cursig;
	unsigned long int pr_sigpend;
	unsigned long int pr_sighold;
	pid_t pr_pid;
	pid_t pr_ppid;
	pid_t pr_pgrp;
	pid_t pr_sid;
	struct {
		long tv_sec, tv_usec;
	} pr_utime, pr_stime, pr_cutime, pr_cstime;
	elf_gregset_t pr_reg;
	int pr_fpvalid;
};

#define ELF_PRARGSZ 80

struct elf_prpsinfo {
	char pr_state;
	char pr_sname;
	char pr_zomb;
	char pr_nice;
	unsigned long int pr_flag;
#if UINTPTR_MAX == 0xffffffff
	unsigned short int pr_uid;
	unsigned short int pr_gid;
#else
	unsigned int pr_uid;
	unsigned int pr_gid;
#endif
	int pr_pid, pr_ppid, pr_pgrp, pr_sid;
	char pr_fname[16];
	char pr_psargs[ELF_PRARGSZ];
};

typedef void *psaddr_t;
typedef elf_gregset_t prgregset_t;
typedef elf_fpregset_t prfpregset_t;
typedef pid_t lwpid_t;
typedef struct elf_prstatus prstatus_t;
typedef struct elf_prpsinfo prpsinfo_t;

typedef enum {
    HPE_PROC_CHECK_REG_PC,
    HPE_PROC_CHECK_REG_SP,
    HPK_PROC_CHECK_REG_BUTT
} hpe_proc_check_regs_t;

/**
 * @brief 寄存器PC/SP原子性检查
 *
 * @param regs 寄存器枚举, 取值范围{HPE_PROC_CHECK_REG_PC, HPE_PROC_CHECK_REG_SP}
 * @param addr 寄存器类型为HPE_PROC_CHECK_REG_PC时，addr表示风险区起始地址；
 *             寄存器类型为HPE_PROC_CHECK_REG_SP时，addr表示task白名单列表的起始地址。
 * @param len 寄存器类型为HPE_PROC_CHECK_REG_PC时，len表示风险区长度；
 *            寄存器类型为HPE_PROC_CHECK_REG_SP时，len表示task白名单长度。
 * @return int 检查OK返回 0；
 *             检查PC在风险区间/非白名单线程在信号处理，则返回1；
 *             其他错误返回-1 : 错误码存放在errno。
 */
int SysCheckRegs(hpe_proc_check_regs_t regs, void *addr, size_t len);


/**
 * @brief 寄存器PC/SP原子性检查
 *
 * @param regs 寄存器枚举, 取值范围{HPE_PROC_CHECK_REG_PC, HPE_PROC_CHECK_REG_SP}
 * @param addr 寄存器类型为HPE_PROC_CHECK_REG_PC时，addr表示风险区起始地址；
 *             寄存器类型为HPE_PROC_CHECK_REG_SP时，addr表示task白名单列表的起始地址。
 * @param len 寄存器类型为HPE_PROC_CHECK_REG_PC时，len表示风险区长度；
 *            寄存器类型为HPE_PROC_CHECK_REG_SP时，len表示task白名单长度。
 * @return int 检查OK返回 0；
 *             检查PC在风险区间/非白名单线程在信号处理，则返回1；
 *             其他错误返回-1 : 错误码存放在errno。
 */
int HpeProcCheckRegs(hpe_proc_check_regs_t regs, void *addr, size_t len);

#endif
