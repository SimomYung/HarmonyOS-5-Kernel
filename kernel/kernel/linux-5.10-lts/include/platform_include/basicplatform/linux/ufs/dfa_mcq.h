/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * DFA driver
 */

#ifndef _DFA_MCQ_H
#define _DFA_MCQ_H

#include <linux/platform_device.h>
#include "scsi/ufs/ufshcd.h"
#include "ufs_mcq.h"
#include "dfa_inf.h"
#include "dfa_platform.h"

#ifdef CONFIG_SCSI_UFS_AQUILA
#include "dfa_aquila.h"
#endif

#ifdef CONFIG_SCSI_UFS_ORION
#include "dfa_orion.h"
#endif

#ifdef CONFIG_SCSI_UFS_PICTOR
#include "dfa_pictor.h"
#endif

#define DFA_MAX_ENTRIES            64
#define DFA_SQ_ENTRY_SIZE_IN_DWORD 16
#define DFA_CQ_ENTRY_SIZE_IN_DWORD 8
#define DFA_IRQ_NUMS               325
#define DFA_QUEUE_EN_OFFSET        31
#define DFA_QUEUE_ID_OFFSET        16
#define UFS_TYPE_OFFSET		   32
#define DFA_HW_QUEUES              5
#define DFA_MAX_QUEUE_NUM          80

#ifdef CONFIG_DFA_MCQ_OHOS
/* each cpu use 5 queues in UDK, only the next one queue is used in LDK */
#define DFA_LDK_QUEUE_INDEX        (DFA_HW_QUEUES * NR_CPUS)
#define DFA_ACTIVE_QUEUE_NUM       1
#define DFA_ACTIVE_QUEUE_START     DFA_LDK_QUEUE_INDEX
#else
/* each cpu use 5 queues */
#define DFA_ACTIVE_QUEUE_NUM       (DFA_HW_QUEUES * NR_CPUS)
#define DFA_ACTIVE_QUEUE_START      0
#endif

#define DFA_CQTEPS_MSI_START       160
#define DFA_ACTIVE_MSI_START       (DFA_CQTEPS_MSI_START + DFA_ACTIVE_QUEUE_START)
#define DFA_ACTIVE_MSI_END         (DFA_ACTIVE_MSI_START + DFA_ACTIVE_QUEUE_NUM - 1)

#define DFA_FLUSH_NR               DFA_HW_QUEUES * NR_CPUS
#define DFA_FLUSH_QUEUE            1
#define DFA_VCMD_TAG               (MCQ_TAG_NUM - 1)
#define DFA_SYNCC_CMD_TAG          (MCQ_TAG_NUM - 2)
#define PLL1_MID_FREQ              360000000
#define PLL0_4_0_FREQ              418000000
#define PLL0_FREQ                  239000000
#define PLL1_FREQ                  480000000
#define PLL2_FREQ                  964000000
#define PLL7_FREQ                  1290000000
#define DFA_LOG_ADDR               0x30C40000
#define DFA_LOG_SIZE               0x4400
#define DFA_LOG_MAX_SIZE           0x11000
#define DFA_LOG_BLOCK_MAX_SIZE     4

/* corresponds to DFA_META_INPUT_LEN in ufs_plat.c */
#define DFA_META_INPUT_LEN         11

/* CQISy - CQ y Interrupt Status Register  */
#define DFA_CQIS_TEPS         0x1
#define DFA_SQ_STOP_RUN_BIT   0x0
#define DFA_SQ_CQ_I_SIZE      0x1C
#define DFA_SQ_CQ_Y_SIZE      0x8
#define DFA_CQ_INTERRUPT_SIZE 0xC
#define DFA_SQ_RUN_TIME_SIZE  0x14
#define DFA_SQ_STOPPED_STATUS 0x1
#define DFA_SQ_RUNNING_STATUS 0x0
#define DFA_SQ_ENTRY_ABORT    (0xF << 28)
#define MASK_LOWER_ADDR       0xFFFFFF80

#define dfa_tag_idx(tag) ((tag) / BITS_PER_ITEM)
#define dfa_tag_off(tag) ((tag) % BITS_PER_ITEM)

enum {
	QUE_PROC_DISABLE = 1,
	QUE_PROC_ENABLE,
};

enum work_mode {
	MODE_BYPASS,
	MODE_DFA,
	MODE_STD,
	MODE_CMPT,
	WORK_MODE_MAX,
};

enum boot_mode {
	MODE_SYNC_BUF,
	MODE_DDR,
	MODE_RESET,
	MODE_OTHER,
	BOOT_MODE_MAX,
};

enum chip_mode {
	MODE_ASIC,
	MODE_FPGA,
	MODE_EMU,
	CHIP_MODE_MAX,
};

enum ufs_type {
	UFS_TYPE_950_ES,
	UFS_TYPE_950_CS,
	UFS_TYPE_NOT_950,
};

/* ACORE-DFA communication register index */
enum {
	DFA_WORK_MODE_IND = 0,
	DFA_BOOT_MODE_IND = 1,
	DFA_LOAD_ADDR_IND = 2,
	DFA_SWITCH_ADDR_IND = 3,
	DFA_DISABLE_QUEUE_IND = 4,
	DFA_UFS_LSU_IND = 5,
	DFA_UFS_PMC_IND = 6,
	DFA_SCEFUSEDATA2_IND = 7,
	DFA_SCEFUSEDATA8_IND = 8,
	DFA_RTC_TIME_IND = 9,
	DFA_UFS_INJECT_INTR_IND = 11,
	DFA_UFS_SRAM_CONFIG_IND = 12,
#if defined(CONFIG_SCSI_UFS_ORION) || defined(CONFIG_SCSI_UFS_PICTOR)
	DFA_WOP_WRITE_IND = 28,
	DFA_WRITE_IND = 29,
	DFA_READ_IND = 30,
#endif
	/* add new ind above */
	DFA_COMMU_BANK_IND = 31,
};

/* DFA-ACORE communication register index */
enum {
	DFA_INIT_DONE_REP = 0,
	DFA_WFI_READY_REP = 1,
	DFA_SUSPEND_INF_REP = 2,
	DFA_AUTO_WFI_REP = 3,
	DFA_PREPARE_RESET_REP = 4,
	DFA_UFS_PMC_REP = 6,
	DFA_UFS_DMD_REP = 7,
	DFA_UFS_DMD_UIC_PAL_REG = 8,
	DFA_UFS_DMD_UIC_DL_REG = 9,
	DFA_UFS_DMD_UIC_NL_REG = 10,
	DFA_UFS_DMD_UIC_TL_REG = 11,
	DFA_UFS_DMD_UIC_DME_REG = 12,
	DFA_UFS_DMD_UIC_COUNTER_OVERFLOW_REG = 13,
	DFA_UFS_DMD_UIC_COUNTER0_REG = 14,
	DFA_RESET_COMMIT_INFO = 15,
#if defined(CONFIG_SCSI_UFS_ORION) || defined(CONFIG_SCSI_UFS_PICTOR)
	DFA_READ_RES_IND = 30,
#endif
	/* add new rep above */
	DFA_COMMU_BANK_REP = 31,
};

/* UFSHCD UIC dsm flags */
enum {
	DFA_DSM_SYSBUS_ERR = (1 << 2),
	DFA_DSM_UIC_TRANS_ERR = (1 << 3),
	DFA_DSM_UFS_UIC_CMD_ERR = (1 << 4),
	DFA_DSM_CONTROLLER_ERR = (1 << 5),
	DFA_DSM_DEV_ERR = (1 << 6),
	DFA_DSM_UFS_TIMEOUT_ERR = (1 << 7),
	DFA_DSM_UFS_LINKUP_ERR = (1 << 11),
	DFA_DSM_UFS_ENTER_OR_EXIT_H8_ERR = (1 << 12),
	DFA_DSM_UFS_HARDWARE_RESET = (1 << 14),
	DFA_DSM_PA_INIT_ERR = (1 << 16),
	DFA_DSM_UFS_INLINE_CRYPTO_ERR = (1 << 17),
	DFA_DSM_UFS_LINE_RESET_ERR = (1 << 19),
	DFA_DSM_CODE_SEGMENT_ERR = 0xDEAD0001,
	DFA_DSM_ERR_MASK = 0xFFFFFFFF,
};

#define _DFA_REQ_FLAG_SHIFT 24
#define _DFA_REQ_ID_MASK ((u32)((1UL << _DFA_REQ_FLAG_SHIFT) - 1))
#define _DFA_REQ_FLAG_MASK ((u32)(~_DFA_REQ_ID_MASK))
#define dfa_req_id_get(val) ((u32)(val) & _DFA_REQ_ID_MASK)
#define dfa_req_flag_get(val) ((u32)(((u32)(val) & _DFA_REQ_FLAG_MASK) >> _DFA_REQ_FLAG_SHIFT))

enum {
	DFA_REQ_FLAG_IGNORE_ID,
	DFA_REQ_FLAG_DELAY_RESET,
	DFA_REQ_FLAG_IMMEDIATELY_RESET,
};

#define dfa_req_flag(i) ((u32)(1UL << ((i) + _DFA_REQ_FLAG_SHIFT)))
#define dfa_req_make_bankc(flag, bank) ((u32)((u32)(flag) | ((u32)(bank) & _DFA_REQ_ID_MASK)))

#define is_id_flag_set(id, flag) (1UL & ((flag) >> (id)))
#define is_val_flag_set(id, flag) (1UL & ((flag) >> ((id) + _DFA_REQ_FLAG_SHIFT)))

enum {
	/* query, rpmb read, boot read/write, TM UPIU, read/write buffer */
	SQ0 = 0,
	/* CP+ */
	SQ1,
	/* CP */
	SQ2,
	/* sync read */
	SQ3,
	/* write, rpmb write, UNMAP, barrier, DataMove */
	SQ4,
};

#ifdef CONFIG_DFX_DEBUG_FS
#define dfa_hex_dump(prefix_str, buf, len) do {                       \
	size_t __len = (len);                                            \
	print_hex_dump(KERN_ERR, prefix_str,                             \
		       __len > 4 ? DUMP_PREFIX_OFFSET : DUMP_PREFIX_NONE,\
		       16, 4, buf, __len, false);                        \
} while (0)

enum {
	DFA_DEBUG_DIRECT_RESET = 1,
	DFA_DEBUG_TIMEOUT,
	DFA_DEBUG_DISORDER,
	DFA_DEBUG_INTER_CORE_TIMEOUT,
};
#endif

#define WRITE_BUFFER_DFA_SEND_DATA_BUFFER_ID 0x2b
#define WRITE_BUFFER_DFA_SEND_DATA_LENGTH_OFFSET 6
#define WRITE_BUFFER_DFA_SEND_DATA_TYPE_OFFSET 8

#define READ_BUFFER_SUB_OPCODE_OFFSET 3
#define READ_BUFFER_DFA_LP_POWER_TYPE_OFFSET 4
#define READ_BUFFER_DFA_LP_SWITCH_OFFSET 5
#define READ_BUFFER_DFA_LP_IDLE_TIMER_OFFSET 6
#define READ_BUFFER_DFA_LP_TIMER_SCALE_OFFSET 8

#define READ_BUFFER_DFA_LP_BUFFER_ID 0x16
#define READ_BUFFER_DFA_LP_SUB_OPCODE 0x8

/* dfa_autoh8_attr power type */
enum {
	/* autosleep */
	DFA_AUTO_SLEEP = 0,
	/* auto h8 */
	DFA_AUTO_H8 = 1
};

/* dfa_autoh8_attr timer scale */
enum {
	/* use default idle timer */
	DEFAULT_IDLE_TIMER = 0,
	/* timer scale: us */
	TIMER_SCALE_US = 1,
	/* timer scale: ms */
	TIMER_SCALE_MS = 2
};

/* dfa_autoh8_attr enable ctrl */
enum {
	ATTR_DISABLE = 0,
	ATTR_ENABTL = 1
};

struct dfa_lp_ctrl_attr {
	u8 power_type;
	u8 enable;
	/* 2 Bytes, Big Edian */
	u16 idle_timer;
	u8 timer_scale;
};

typedef enum {
    SOC_HTS_ID_HTS,
    SOC_HTS_ID_FFTS,
    SOC_HTS_ID_GPU,
    SOC_HTS_ID_NPU_HWTS,
    SOC_HTS_ID_NPU_TSCPU,
    SOC_HTS_ID_ISP,
    SOC_HTS_ID_DSS,
    SOC_HTS_ID_VENC,
    SOC_HTS_ID_VDEC,
    SOC_HTS_ID_AUDIO,
    SOC_HTS_ID_IOMCU,
    SOC_HTS_ID_IPP,
    SOC_HTS_ID_AAE,
    SOC_HTS_ID_DPA,
    SOC_HTS_ID_DFA,
    SOC_HTS_ID_ACPU
} soc_hts_id_type_t;

/* dfa_mcq.c */
void dfa_set_sdev(struct scsi_device *dev);
int dfa_get_base_addr(struct ufs_hba *hba, struct device_node *np);
int dfa_link_startup(struct ufs_hba *hba);
bool dfa_get_dev_cmd_tag(struct ufs_hba *hba, int *tag_out);
void dfa_put_dev_cmd_tag(struct ufs_hba *hba);
void dfa_mcq_transfer_req_compl_all(struct ufs_hba *hba);
void dfa_enable_ooo_mode(struct ufs_hba *hba, bool enable);
void dfa_mcq_make_queues_operational(struct ufs_hba *hba);
void dfa_inflt_req_clear(struct ufs_hba *hba, int tag);
void dfa_mcq_stop_run_sq(struct dfa_hw_queue *dhq, bool stop);
void dfa_get_intr_core_irq(struct platform_device *pdev, struct ufs_hba *hba);
void dfa_dhq_slot_clear(struct ufs_hba *hba);
int dfa_config_mcq(struct ufs_hba *hba, bool async);
void dfa_prepare_req_desc_hdr(struct ufshcd_lrb *lrbp,
	u8 *upiu_flags, enum dma_data_direction cmd_dir);
int dfa_memory_alloc(struct ufs_hba *hba);
int dfa_lrbp_configure(struct ufs_hba *hba, unsigned int tag, const char *func);
void dfa_host_memory_configure(struct ufs_hba *hba);
void dfa_send_command(struct ufs_hba *hba, unsigned int tag);
int dfa_queuecommand(struct Scsi_Host *host, struct scsi_cmnd *cmd);
void dfa_reset_dev_cmd_complete(
	struct ufs_hba *hba, unsigned int tag, struct ufshcd_lrb *lrbp);
int dfa_queuecommand_directly(struct ufs_hba *hba,
	struct scsi_cmnd *cmd, unsigned int timeout, bool eh_handle);
int dfa_exec_spec_cmd(struct ufs_hba *hba, struct scsi_cmnd *cmd, unsigned int timeout);
int dfa_exec_dev_cmd(struct ufs_hba *hba, enum dev_cmd_type cmd_type, int timeout);
int dfa_tt_do_scsi_cmd(struct ufs_hba *hba,
	struct scsi_cmnd *cmd, unsigned int timeout, ktime_t start_time);
void dfa_clk_init(struct ufs_hba *hba);
int dfa_suspend(struct ufs_hba *hba, enum ufs_pm_op pm_op);
int dfa_resume(struct ufs_hba *hba, enum ufs_pm_op pm_op);
void dfa_mcq_params_init(struct ufs_hba *hba);
void dfa_dump_lrbp_status(struct ufs_hba *hba);
void dfa_dump_status(struct ufs_hba *hba, enum blk_dump_scene dump_type);
bool dfa_is_need_report_bit_error(struct ufs_hba *hba);
void dfa_dmd_handle(struct ufs_hba *hba);
int dfa_direct_flush(struct scsi_device *sdev, struct ufs_hba *hba, unsigned char *buffer);

/* dfa_error.c */
enum blk_eh_timer_return dfa_eh_timed_out(struct scsi_cmnd *scmd);
void dfa_print_host_state(struct ufs_hba *hba);
int dfa_try_to_abort_task(struct ufs_hba *hba, int tag);
int dfa_abort(struct scsi_cmnd *cmd);
int dfa_prepare_reset(struct ufs_hba *hba);
void dfa_reset_directly(struct ufs_hba *hba);
void dfa_error_handler(struct kthread_work *work);
bool is_dfa_enabled_backdata(struct ufs_hba *hba);
void dfa_dump_fw_log(struct dfa_hba_info *dhba);

#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
void dfa_uie_utrd_prepare(struct ufs_hba *hba, struct ufshcd_lrb *lrbp);
#endif
#ifdef CONFIG_DFX_DEBUG_FS
void dfa_print_trs(struct ufs_hba *hba, bool pr_prdt);
unsigned int dfa_get_debug_index(void);
void dfa_reset_debug_index(void);
#else
static inline void dfa_print_trs(struct ufs_hba *hba, bool pr_prdt)
{
	return;
}
#endif

/* ufs-plat.c */
void hufs_crypto_set_keyindex(struct ufs_hba *hba, struct ufshcd_lrb *lrbp,
	u32 *hash_res, u32 *crypto_cci, unsigned long *flags);
void hufs_get_meta_data_factor(struct ufshcd_lrb *lrbp, struct ufs_hba *hba,
	u32 *dword_8, u32 *dword_9, u32 *dword_10, u32 *dword_11);
#if defined(CONFIG_SCSI_UFS_ENHANCED_INLINE_CRYPTO) && defined(CONFIG_DFX_DEBUG_FS)
void hufs_inline_crypto_debug(struct ufs_hba *hba, u32 *hash_res, u32 *crypto_enable, u64 dun);
#endif

/* ufshcd.c */
void ufshcd_check_disable_dev_tmt_cnt(struct ufs_hba *hba, struct scsi_cmnd *cmd);
int ufshcd_set_dev_pwr_mode(struct ufs_hba *hba, enum ufs_dev_pwr_mode pwr_mode);
int ufshcd_scsi_cmd_status(struct ufshcd_lrb *lrbp, int scsi_status);

static inline struct cq_entry *dfa_mcq_cur_cqe(struct dfa_hw_queue *q)
{
	struct cq_entry *cqe = q->cqe_base_addr;

	return cqe + q->cq_hp_slot;
}

static inline bool dfa_mcq_is_sq_full(struct dfa_hw_queue *q)
{
	return ((q->sq_tp_slot + 1) % q->max_entries) == q->sq_hp_slot;
}

static inline bool dfa_mcq_is_cq_empty(struct dfa_hw_queue *q)
{
	return q->cq_hp_slot == q->cq_tp_slot;
}

static inline void dfa_mcq_update_sq_hp(struct dfa_hw_queue *q)
{
	u32 val;

	val = readl(que_proc_sq_i_sqh(q->que_proc_base, q->id));
	q->sq_hp_slot = val / sizeof(struct dfa_utp_transfer_req_desc);
}

static inline void dfa_mcq_update_sq_tp(struct dfa_hw_queue *q)
{
	q->sq_tp_slot = (q->sq_tp_slot + 1) % q->max_entries;
	writel(q->sq_tp_slot * sizeof(struct dfa_utp_transfer_req_desc),
		que_proc_sq_i_sqt(q->que_proc_base, q->id));
}

static inline void dfa_mcq_update_cq_hp(struct dfa_hw_queue *q)
{
	q->cq_hp_slot = (q->cq_hp_slot + 1) % q->max_entries;
	writel(q->cq_hp_slot * sizeof(struct cq_entry),
		que_proc_cq_i_cqh(q->que_proc_base, q->id));
}

static inline void dfa_mcq_update_cq_tp(struct dfa_hw_queue *q)
{
	u32 val;

	val = readl(que_proc_cq_i_cqt(q->que_proc_base, q->id));
	q->cq_tp_slot = val / sizeof(struct cq_entry);
}

static inline u32 dfa_mcq_read_cqis(struct ufs_hba *hba, int i)
{
	return readl(que_proc_cq_i_cqis(hba->dhba.que_proc, i));
}

static inline void dfa_mcq_write_cqis(struct ufs_hba *hba, u32 val, int i)
{
	writel(val, que_proc_cq_i_cqis(hba->dhba.que_proc, i));
}

static inline void dfa_mcq_enable_esi(struct ufs_hba *hba)
{
	writel(0x20000, que_proc_msi_device_id(hba->dhba.que_proc));
}

static inline bool is_dfa_lrbp(struct ufshcd_lrb *lrbp)
{
	return lrbp->dlrb.is_dfa_utrd;
}

static inline bool is_dfa_ftl(struct dfa_hba_info *dhba)
{
	return dhba->workmode == MODE_DFA ? true : false;
}

static inline bool is_dfa_std(struct dfa_hba_info *dhba)
{
	return dhba->workmode == MODE_STD ? true : false;
}

/**
 * dfa_get_tr_ocs - Get the DFA UTRD Overall Command Status
 * @lrbp: pointer to local command reference block
 *
 * This function is used to get the OCS field from UTRD
 * Returns the OCS field in the UTRD
 */
static inline int dfa_get_tr_ocs(struct ufshcd_lrb *lrbp)
{
	return (int)lrbp->dlrb.ocs;
}

static inline struct dfa_hw_queue *dfa_mcq_find_hwq(
	struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
#ifdef CONFIG_DFA_MCQ_OHOS
	return &hba->dhba.dhq[0];
#else
	if (lrbp->task_tag == DFA_SYNCC_CMD_TAG)
		return &hba->dhba.dhq[DFA_FLUSH_NR];
	return &hba->dhba.dhq[lrbp->dlrb.cpu * DFA_HW_QUEUES + lrbp->dlrb.sq];
#endif
}

static inline unsigned long *dfa_tag_bitmap(struct ufs_hba *hba, int tag)
{
	return &hba->dhba.dfa_inflt_reqs[dfa_tag_idx(tag)];
}

static inline void dfa_irq_sche_lock(struct ufs_hba *hba, unsigned long flag)
{
#ifdef CONFIG_DFA_MCQ_OHOS
	spin_lock_irqsave(&hba->dhba.dfa_spinlock, flag);
#else
	local_irq_disable();
	preempt_disable();
#endif
}

static inline void dfa_irq_sche_unlock(struct ufs_hba *hba, unsigned long flag)
{
#ifdef CONFIG_DFA_MCQ_OHOS
	spin_unlock_irqrestore(&hba->dhba.dfa_spinlock, flag);
#else
	preempt_enable();
	local_irq_enable();
#endif
}

#ifdef CONFIG_SCSI_UFS_LOW_POWER_VOTE
void dfa_low_power_vote(struct ufs_hba *hba, bool idle);
#endif
#ifdef CONFIG_UFS_LOW_POWER_VOTE_STATUS_SYNC
void dfa_lowpower_vote_status_sync(struct ufs_hba *hba, bool idle);
void dfa_cpu_clk_vote(struct ufs_hba *hba, bool idle);
int dfa_sram_deep_sleep_vote(struct ufs_hba *hba, bool idle);
#endif
#ifdef CONFIG_LIBLINUX_UDK_UFS
int sd_proxy_block_cmd(bool is_blocked);
#endif

void dfa_set_timestamp_attr(struct ufs_hba *hba);

void dfa_auto_h8_switch(bool enable);
void dfa_lp_ctrl(struct dfa_lp_ctrl_attr *attr);

#ifdef CONFIG_AI_IN_FLASH
int dfa_vcmd_send(struct dfa_vcmd_vars *vars);
#endif

#ifndef CONFIG_DFA_MCQ_OHOS
void dfa_irq_cpuhp_remove(struct ufs_hba *hba);
#else
static inline void dfa_irq_cpuhp_remove(struct ufs_hba *hba)
{
	return;
}
#endif
#endif /* End of Header */
