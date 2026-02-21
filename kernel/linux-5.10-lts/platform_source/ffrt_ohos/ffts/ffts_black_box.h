#ifndef __FFTS_BLACK_BOX_H__
#define __FFTS_BLACK_BOX_H__

#include <linux/types.h>
#define FFTS_BUF_LEN_MAX 256

enum rdr_ffts_system_error_type {
	EXC_TYPE_FFTS_NO_ERROR = 0x0,
	/* DFX_BB_MOD_FFTS_START is 0xc0007000 */
	EXC_TYPE_BLOCK_PROC_EXCEPTION = 0xc0007001,
	EXC_TYPE_BLOCK_GPU_EXCEPTION = 0xc0007002,
	EXC_TYPE_BLOCK_NPU_EXCEPTION = 0xc0007003,
	EXC_TYPE_CMO_EXCEPTION = 0xc0007004,
	EXC_TYPE_RETRY_EXCEPTION = 0xc0007005,
	EXC_TYPE_ABNORMAL_EXCEPTION = 0xc0007006,
};

enum UDK_FFTS_CMD {
	CMD_FFTS_BLOCK_ENABLE = 0,
	CMD_FFTS_INSERT_EXCP_INFO = 1,
	CMD_FFTS_CHECK_GROBAL_BLOCK_STATE = 2,
	CMD_FFTS_BLOCK_DISABLE = 3,
	CMD_FFTS_CHECK_HW_BLOCK_STATE = 4,
	CMD_FFTS_DUMP_INFO = 5,
};

struct ffts_rdr_exception_info {
	u32 error_code;
	u16 limitation;
	u64 count;
};

struct ffts_rdr_dump_info {
	u32 modid;
	u32 etype;
	u64 coreid;
};

int ffts_mntn_copy_reg_to_bbox(const char *src_addr,
	unsigned int len);
void ffts_rdr_exception_report(uint32_t error_code);
int ffts_mntn_copy_udk_info_to_bbox(void *rd, size_t rd_size);
int ffts_udk_rdr_exception_report(void *rd, size_t rd_size);
int ffts_black_box_init(void);
int ffts_black_box_exit(void);

#endif /* __FFTS_BLACK_BOX_H__ */