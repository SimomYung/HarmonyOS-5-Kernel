/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Hisi fbex ca header files
 * Create : 2020/01/02
 */

#ifndef __HISI_FBEX_DEV_H_
#define __HISI_FBEX_DEV_H_

#include <linux/types.h>
#include <platform_include/see/fbe_ctrl.h>

#define FBEX3_IOC_MAGIC 'f'

struct fbex_req_v0 {
	u32 user;
	u32 file; /* D:0, C:1, A:2, B:3 */
	u32 len;
	u8 key[KEY_LEN];
	u8 flag;
};
/* v0 is renamed from old fbex_req, to keep cmdid not change */

#ifdef CONFIG_FILE_ENCRY_ACCESS_CONTROL
struct fbex_req_v1 {
	u32 user;
	u32 file; /* D:0, C:1, A:2, B:3 */
	u32 len;
	u8 key[KEY_LEN];
	u8 flag;
	u8 authtoken[AUTHTOKEN_MAXLEN];
	u32 authtoken_len;
};
/* v1 add authtoken from sd */
typedef struct fbex_req_v1 fbex_req;
/* only copy_to_user fbex_req_v0 */
#else
typedef struct fbex_req_v0 fbex_req;
#endif

#define HISI_FBEX_ADD_IV _IOWR(FBEX3_IOC_MAGIC, 1, struct fbex_req_v0)
#define HISI_FBEX_DEL_IV _IOW(FBEX3_IOC_MAGIC, 2, struct fbex_req_v0)
#define HISI_FBEX_LOCK_SCREEN _IOW(FBEX3_IOC_MAGIC, 3, struct fbex_req_v0)
#define HISI_FBEX_UNLOCK_SCREEN _IOWR(FBEX3_IOC_MAGIC, 4, struct fbex_req_v0)
#define HISI_FBEX_USER_LOGOUT _IOW(FBEX3_IOC_MAGIC, 8, struct fbex_req_v0)
#define HISI_FBEX_STATUS_REPORT _IOW(FBEX3_IOC_MAGIC, 12, struct fbex_req_v0)

struct cmd_table {
	u32 cmd;
	u32 id;
};
#ifdef CONFIG_FILE_ENCRY_ACCESS_CONTROL
u32 fbex_fill_authtoken(void **authtoken, u32 *authtoken_len);
#endif /* CONFIG_FILE_ENCRY_ACCESS_CONTROL */
#endif /* __HISI_FBEX_DEV_H_ */
