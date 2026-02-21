/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: sec_region_inner.h.
 * Create: 2024-07-10
 */

#ifndef __SEC_REGION_INNER_H__
#define __SEC_REGION_INNER_H__

#include <linux/types.h>
#include <ddr_sec/ddr_sec_rdr_mntn.h>

/* define share_mem(bl31<->kernel) usage */
/* gic_id */
#define QICE_FIQ_GIC_NUMBER_ADDR            (ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_BASE_DDR_SEC)
#define QICE_FIQ_GIC_NUMBER_SIZE            0x8UL
/* trigger reboot depending on nv in himntn */
#define QICE_REBOOT_FLAG_ADDR               (QICE_FIQ_GIC_NUMBER_ADDR + QICE_FIQ_GIC_NUMBER_SIZE)
#define QICE_REBOOT_FLAG_SIZE               0x8UL
/* intr_info, including err_addr */
#define QICE_SEC_ERR_ADDR                   (QICE_REBOOT_FLAG_ADDR + QICE_REBOOT_FLAG_SIZE)
#define QICE_SEC_ERR_SIZE                   0x8UL
/* intr_info, including master_type, mid, err_reason, ns, access_type */
#define QICE_INTR_INFO_ADDR                 (QICE_SEC_ERR_ADDR + QICE_SEC_ERR_SIZE)
#define QICE_INTR_INFO_SIZE                 0x8UL
/* subreason_info, including master_reason, sub_reason */
#define QICE_SUBREASON_INFO_ADDR            (QICE_INTR_INFO_ADDR + QICE_INTR_INFO_SIZE)
#define QICE_SUBREASON_INFO_SIZE            0x4UL

struct qice_intr_mem {
	u32 mst_type : 8;
	u32 mid : 8;
	u32 err_reason : 8;
	u32 ns : 8;
	u32 access_type : 8;
	u32 init_flag : 8;
};

/* qice init type */
enum ddr_sec_opti_init_status {
	DDR_SEC_INIT_BEGIN = 0x0,
	DDR_SEC_INIT_END
};

/* qice err type */
enum ddr_sec_error_reason {
	ERR_REASON_SC = 0x0,
	ERR_REASON_MPU,
	ERR_REASON_QICE,
	ERR_REASON_MAX
};

enum abnml_sc_intr_opc_type {
	SC_READ_SHARED = 0x1,
	SC_READ_NO_SNP = 0x4,
	SC_READ_NO_SNP_SEP = 0x11,
	SC_WRITE_NO_SNP_PTL = 0x1c,
};

enum abnml_tbed_intr_opc_type {
	TBED_ACCESS_READ = 0x0,
	TBED_ACCESS_WRITE = 0x1,
	TBED_OPERATION_MAX
};

enum abnml_ibqe_intr_opc_type {
	IBQE_READ_OP0 = 0x0,
	IBQE_READ_OP1 = 0x1,
	IBQE_WRITE_OP0 = 0x4,
	IBQE_WRITE_OP1 = 0x5,
	IBQE_OPERATION_MAX
};

enum abnml_ibqe_intr_ns_type {
	IBQE_SECURE_OP0 = 0x0,
	IBQE_NON_SECURE_OP0,
	IBQE_PROTECT_OP0,
};

#endif
