/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2021. All rights reserved.
 * Description: Contexthub share memory driver
 * Create: 2016-04-01
 */
#ifndef __IOMCU_DUMP_H__
#define __IOMCU_DUMP_H__
#include <linux/types.h>
#include <platform_include/smart/linux/base/ap/protocol.h>
#include <platform_include/basicplatform/linux/ipc_rproc.h>
#include <platform_include/smart/linux/iomcu_config.h>
#include <mntn_public_interface.h>

#ifdef __cplusplus
extern "C" {
#endif

// moved from iomcu_ipc.h
#define IOM3_ST_NORMAL     0
#define IOM3_ST_RECOVERY   1
#define IOM3_ST_REPEAT     2

#define IOM3_RECOVERY_UNINIT 0
#define IOM3_RECOVERY_IDLE (IOM3_RECOVERY_UNINIT + 1)
#define IOM3_RECOVERY_START (IOM3_RECOVERY_IDLE + 1)
#define IOM3_RECOVERY_MINISYS (IOM3_RECOVERY_START + 1)
#define IOM3_RECOVERY_DOING (IOM3_RECOVERY_MINISYS + 1)
#define IOM3_RECOVERY_3RD_DOING (IOM3_RECOVERY_DOING + 1)
#define IOM3_RECOVERY_FAILED (IOM3_RECOVERY_3RD_DOING + 1)
#define IOM3_RECOVERY_BOXREADY (IOM3_RECOVERY_FAILED + 1)

#define SENSORHUB_MODID DFX_BB_MOD_IOM_START
#define SENSORHUB_USER_MODID (DFX_BB_MOD_IOM_START + 1)
#define SENSORHUB_FDUL_MODID (DFX_BB_MOD_IOM_START + 2)
#define SENSORHUB_TINY_MODID (DFX_BB_MOD_IOM_START + 3)
#ifdef CONFIG_CONTEXTHUB_DFX_UNI_NOC
#define SENSORHUB_IOMCU_DMA_MODID (DFX_BB_MOD_IOM_START + 4)
#endif
#define SENSORHUB_IODIE_MODID (DFX_BB_MOD_IOM_START + 5)
#define PATH_MAXLEN 128
#define HISTORY_LOG_SIZE 256
#define HISTORY_LOG_MAX 0x80000 /* 512k */
#define ROOT_UID 0
#define SYSTEM_GID 1000
#define DIR_LIMIT 0770
#define FILE_LIMIT 0660
#ifdef CONFIG_CONTEXTHUB_LOG_PATH_VAR
#define SH_DMP_DIR "/var/log/hisi/sensorhub-log/"
#define SH_DMP_FS "/var/log"
#else
#ifdef CONFIG_INPUTHUB_AS
#define SH_DMP_DIR  "/data/log/sensorhub-log/"
#else
#define SH_DMP_DIR  "/data/log/sensorhub-log/"
#endif
#define SH_DMP_FS  "/data/lost+found"
#endif
#define SH_DMP_HISTORY_FILE "history.log"

#define DATATIME_MAXLEN 24 /* 14+8 +2, 2: '-'+'\0' */

enum exp_source {
	SH_FAULT_HARDFAULT = 0,
	SH_FAULT_BUSFAULT,
	SH_FAULT_USAGEFAULT,
	SH_FAULT_MEMFAULT,
	SH_FAULT_NMIFAULT,
	SH_FAULT_ASSERT,
	SH_FAULT_INTERNELFAULT = 16,
	SH_FAULT_IPC_RX_TIMEOUT,
	SH_FAULT_IPC_TX_TIMEOUT,
	SH_FAULT_RESET,
	SH_FAULT_USER_DUMP,
	SH_FAULT_RESUME,
	SH_FAULT_REDETECT,
	SH_FAULT_PANIC,
	SH_FAULT_NOC,
	SH_FAULT_REACT,
	SH_FAULT_IO_DIE = 26,
	SH_FAULT_ULSR_RESUME,
	BOX_RECOVERY,
	LPMCU_TIMEOUT,
	SH_FAULT_EXP_BOTTOM,
};

enum rec_user_e {
	REC_USR_LOG = 0,
	REC_USR_DUMP,
	REC_USR_AOD,
	REC_USR_FP,
	REC_USR_FP_UD,
	REC_USR_KB,
	REC_USR_MLPS,
	REC_USR_MOTION,
	REC_USR_SENSOR,
	REC_USR_THP,
	REC_USR_IPC,
	REC_USR_BLE,
	REC_USR_CHRE,
	REC_USR_UDI,
	REC_USR_AR,
	REC_USR_FLP,
	REC_USR_IGS,
	REC_USR_IODIE,
	REC_USR_PEDOMETER,
	REC_USR_BLPWM,
	REC_USR_BASTET,
	REC_USR_DSS_TRISTATEID,
	REC_USR_MAX,
};

union dump_info {
	unsigned int value;
	struct {
		unsigned int tag : 8; /* bit[0-7]:  ipc tag */
		unsigned int cmd : 8; /* bit[8-15]: ipc cmd */
		unsigned int reserved : 16; /* bit[16-31]: reserved */
	}reg;
	unsigned int modid;
};

/**
 * @brief save dump info to history.
 *
 * @param info : dump info, refer to "union dump_info".
 * @return NA
 */
void save_dump_info_to_history(union dump_info info);

/**
 * @brief notify iomcu to do recovery.
 *
 * @param modid  : module id.
 * @param f      : exception source.
 * @return int   : SUCCESS or FAIL.
 */
int iom3_need_recovery(int modid, enum exp_source f);

/**
 * @brief register iomcu recovery notifier.
 *
 * @param nb     : notifier.
 * @return int   : SUCCESS or FAIL.
 */
int register_iom3_recovery_notifier(enum rec_user_e usr, struct notifier_block *nb);

/**
 * @brief unregister iomcu recovery notifier.
 *
 * @param nb     : notifier.
 * @return int   : SUCCESS or FAIL.
 */
int unregister_iom3_recovery_notifier(enum rec_user_e usr, struct notifier_block *nb);

/**
 * @brief get iomcu state.
 *
 * @return int   : iomcu state, refer to IOM3_ST_XXX.
 */
int get_iom3_state(void);

/**
 * @brief check iomcu dump status.
 *
 * @return int   : SUCCESS or FAIL.
 */
int iomcu_check_dump_status(bool is_lock);

/**
 * @brief get iomcu recovery state.
 *
 * @return int : iomcu recovery state, refer to IOM3_RECOVERY_XXX
 */
int get_iom3_rec_state(void);

/**
 * @brief set iomcu recovery state.
 *
 * @param state : iomcu recovery state.
 * @return NA
 */
void set_iom3_rec_state(int state);

/**
 * @brief clr iomcu dump miniready state.
 *
 * @param state : iomcu dump miniready state.
 * @return NA
 */
void clr_iom3_dump_miniready_state(void);
u8 get_mcu_recovery_stat(void);
#ifdef __cplusplus
}
#endif
#endif /* __IOMCU_DUMP_H__ */
