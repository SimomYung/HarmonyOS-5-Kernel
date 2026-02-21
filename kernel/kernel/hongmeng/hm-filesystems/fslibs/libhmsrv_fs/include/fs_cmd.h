/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of ubifs_cmd
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 28 20:50:43 2020
 */
#ifndef __FILESYSTEM_CMD_H__
#define __FILESYSTEM_CMD_H__

/* ubi cmd type */
enum ubi_dev_ctrl_cmd {
	UBI_GET_UBI_NUM,
	UBI_OPEN_VOL,
	UBI_GET_DEV_INFO,
	UBI_GET_VOL_INFO,
	UBI_CLOSE_VOL,
};

enum ubi_dev_acc_cmd {
	UBI_LEB_READ,
	UBI_LEB_WRITE,
	UBI_LEB_CHANGE,
	UBI_LEB_MAP,
	UBI_LEB_UNMAP,
	UBI_LEB_MAPPED,
	UBI_SYNC,
};

enum hm_ubi_cmd {
	UBI_DEV_CTRL,
	UBI_DEV_ACC,
};

struct ubi_dev_ctrl_ctx {
	enum ubi_dev_ctrl_cmd cmd;
	union {
		struct {
			union {
				int ubi_num;
				int major;
				int id;
			};
			union {
				struct {
					int vol_id;
					int mode;
				};
				struct {
					unsigned long vi;
					unsigned long name;
				};
				unsigned long di;
			};
		};
	};
};

struct ubi_dev_acc_ctx {
	enum ubi_dev_acc_cmd cmd;
	union {
		int id;
		int ubi_num;
	};
	unsigned long buf;
	unsigned int len;
	unsigned int lnum;
	unsigned int offs;
};

#endif
