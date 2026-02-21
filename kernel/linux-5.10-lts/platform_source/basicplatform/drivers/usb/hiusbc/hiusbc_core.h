/*
 * hiusbc_core.h -- Driver for Hisilicon USB Controller.
 *
 * Copyright (c) 2019 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __HIUSBC_CORE_H
#define __HIUSBC_CORE_H

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/dmapool.h>
#include <linux/platform_device.h>
#include <linux/usb/gadget.h>
#include "hiusbc_reg.h"
#include <linux/platform_drivers/usb/chip_usb.h>
#include <linux/platform_drivers/usb/hiusb.h>

#define HIUSBC_AUTOSUSPEND_DELAY 5000
#define HIUSBC_EP_NUMS 32

#define DMA_ADDR_INVALID (~(dma_addr_t)0)

#define HIUSBC_DEV_REQ_RESPONSE_BUF_SIZE 512

#define HIUSBC_DEVICE_MODE_REG_BASE 0x3000

#define HIUSBC_TRPU_HOST_U2SEP_MBS 0x4
#define HIUSBC_GEN2_SPEED 0x1
#define HIUSBC_SCD_LOGIC0_SPACE 0x32C
#define HIUSBC_SCD_LOGIC1_SPACE 0x59D
#define HIUSBC_CSR_RXEQ_GEN 0x1
#define HIUSBC_CSR_REPEAT_MIN 0x521

#define HIUSBC_DOORBELL_INTERVAL 0x5
#define HIUSBC_PWM_MIN_GEN2 0x252

#define DEV_INT_MOD_INTERVAL 1

#define HIUSBC_CMD_FINISH 0
#define HIUSBC_CMD_PARM_ERROR 1
#define HIUSBC_CMD_ABORT 2
/* EP state is not STOP for StartXfer cmd */
#define HIUSBC_CMD_FAIL 3
#define HIUSBC_CMD_FRM_EXPIRED 0xe
#define HIUSBC_CMD_EVT_LOSS 0xf
#define hiusbc_get_cmd_done_info(x) \
	(((x) & MPI_APP_DEVICE_REG_DEV_CMD_DONE_INFO_MASK) >> \
	MPI_APP_DEVICE_REG_DEV_CMD_DONE_INFO_SHIFT)

#define hiusbc_ctl_besl_suspend(x) \
	(((x) << MPI_APP_DEVICE_REG_BESL_THRESHOLD_SUSPEND_SHIFT) & \
	MPI_APP_DEVICE_REG_BESL_THRESHOLD_SUSPEND_MASK)
#define hiusbc_ctl_besl_min(x) \
	(((x) << MPI_APP_DEVICE_REG_BESL_THRESHOLD_MIN_SHIFT) & \
	MPI_APP_DEVICE_REG_BESL_THRESHOLD_MIN_MASK)
#define hiusbc_ctl_besl_max(x) \
	(((x) << MPI_APP_DEVICE_REG_BESL_THRESHOLD_MAX_SHIFT) & \
	MPI_APP_DEVICE_REG_BESL_THRESHOLD_MAX_MASK)

/* u2portsc */
#define hiusbc_u2portsc_pls(x) \
	(((x) << MPI_APP_DEVICE_REG_DEV_U2_PLS_SHIFT) & \
	MPI_APP_DEVICE_REG_DEV_U2_PLS_MASK)
#define hiusbc_get_u2portsc_pls(x) \
	(((x) & MPI_APP_DEVICE_REG_DEV_U2_PLS_MASK) >> \
	MPI_APP_DEVICE_REG_DEV_U2_PLS_SHIFT)
#define hiusbc_get_u2portsc_speed(x) \
	(((x) & MPI_APP_DEVICE_REG_DEV_U2_PORT_SPEED_MASK) >> \
	MPI_APP_DEVICE_REG_DEV_U2_PORT_SPEED_SHIFT)

/* u3portsc */
#define hiusbc_u3portsc_pls(x) \
	(((x) << MPI_APP_DEVICE_REG_DEV_U3_PLS_SHIFT) & \
	MPI_APP_DEVICE_REG_DEV_U3_PLS_MASK)
#define hiusbc_get_u3portsc_pls(x) \
	(((x) & MPI_APP_DEVICE_REG_DEV_U3_PLS_MASK) >> \
	MPI_APP_DEVICE_REG_DEV_U3_PLS_SHIFT)
#define hiusbc_get_u3portsc_speed(x) \
	(((x) & MPI_APP_DEVICE_REG_DEV_U3_PORT_SPEED_MASK) >> \
	MPI_APP_DEVICE_REG_DEV_U3_PORT_SPEED_SHIFT)

/* u2testmode */
#define hiusbc_u2_test_mode(x) \
	(((x) << MPI_APP_DEVICE_REG_DEV_TEST_MODE_SHIFT) & \
	MPI_APP_DEVICE_REG_DEV_TEST_MODE_MASK)
#define hiusbc_get_u2_test_mode(x) \
	(((x) & MPI_APP_DEVICE_REG_DEV_TEST_MODE_MASK) >> \
	MPI_APP_DEVICE_REG_DEV_TEST_MODE_SHIFT)

/* ebset */
#define hiusbc_dev_iman_offset(n) \
	(MPI_APP_DEVICE_REG_DEV_IMAN_0_OFFSET + ((n) * 0x50))
#define hiusbc_dev_imod_offset(n) \
	(MPI_APP_DEVICE_REG_DEV_IMOD_0_OFFSET + ((n) * 0x50))
#define hiusbc_dev_ebsz_offset(n) \
	(MPI_APP_DEVICE_REG_DEV_EBSZ_0_OFFSET + ((n) * 0x50))
#define hiusbc_dev_ebba0_offset(n) \
	(MPI_APP_DEVICE_REG_DEV_EBBA0_0_OFFSET + ((n) * 0x50))
#define hiusbc_dev_ebba1_offset(n) \
	(MPI_APP_DEVICE_REG_DEV_EBBA1_0_OFFSET + ((n) * 0x50))
#define hiusbc_dev_ebdp0_offset(n) \
	(MPI_APP_DEVICE_REG_DEV_EBDP0_0_OFFSET + ((n) * 0x50))
#define hiusbc_dev_ebdp1_offset(n) \
	(MPI_APP_DEVICE_REG_DEV_EBDP1_0_OFFSET + ((n) * 0x50))

#define HIUSBC_IMODC_MASK (0xffff << 16)
#define hiusbc_imodi(x) \
	(((x) << MPI_APP_DEVICE_REG_DEV_INT_MOD_INTERVAL_0_SHIFT) & \
	MPI_APP_DEVICE_REG_DEV_INT_MOD_INTERVAL_0_MASK)
#define HIUSBC_EHB_MASK (1 << 3)
#define HIUSBC_EBDP_LOW_MASK (0xfffffff << 4)
#define HIUSBC_EBDP_MASK ((u64) ~0xf)

/* drdmode */
#define HIUSBC_DRD_MODE_HOST 0
#define HIUSBC_DRD_MODE_DEVICE 1
#define hiusbc_drd_mode(x) \
	(((x) << MPI_APP_COM_REG_DRD_WORK_MODE_SHIFT) & \
	MPI_APP_COM_REG_DRD_WORK_MODE_MASK)
#define hiusbc_get_drd_mode(x) \
	(((x) & MPI_APP_COM_REG_DRD_WORK_MODE_MASK) >> \
	MPI_APP_COM_REG_DRD_WORK_MODE_SHIFT)

/* vbusselmode */
#define HIUSBC_VBUS_U2 0
#define HIUSBC_VBUS_U3 1
#define hiusbc_vbus_sel_mode(x) \
	(((x) << MPI_APP_COM_REG_VBUS_SEL_MODE_SHIFT) & \
	MPI_APP_COM_REG_VBUS_SEL_MODE_MASK)

/* u2vbussel */
#define HIUSBC_U2_VBUS_FROM_SOC 0
#define HIUSBC_U2_VBUS_FROM_ULPI 1
#define HIUSBC_U2_VBUS_FORCE 2
#define hiusbc_u2_vbus_sel(x) \
	(((x) << MPI_APP_COM_REG_U2_VBUS_SEL_SHIFT) & \
	MPI_APP_COM_REG_U2_VBUS_SEL_MASK)

/* u3vbussel */
#define HIUSBC_U3_VBUS_FROM_PIPE 0
#define HIUSBC_U3_VBUS_FORCE 1
#define hiusbc_u3_vbus_sel(x) \
	(((x) << MPI_APP_COM_REG_U3_VBUS_SEL_SHIFT) & \
	MPI_APP_COM_REG_U3_VBUS_SEL_MASK)

/* phydonerspmode */
#define HIUSBC_PHY_SET_U2_ONLY 0
#define HIUSBC_PHY_SET_U3_ONLY 1
#define HIUSBC_PHY_SET_BOTH 2
#define hiusbc_phy_set(x) \
	(((x) << MPI_APP_COM_REG_PHY_SEL_MODE_SHIFT) & \
	MPI_APP_COM_REG_PHY_SEL_MODE_MASK)

/* globaltimestamp */
#define hiusbc_get_bicnt(x) \
	(((x) & MPI_APP_COM_REG_BI_CNT_MASK) >> \
	MPI_APP_COM_REG_BI_CNT_SHIFT)

/* non-transfer events generate mode */
#define HIUSBC_NON_XFER_EVENT_MODE_MASK (1 << 6)
#define HIUSBC_NON_XFER_STATE_MASK (1 << 7)

static inline u32 hiusbc_readl(void __iomem *base, u32 offset)
{
	return readl(base + offset - HIUSBC_DEVICE_MODE_REG_BASE);
}

static inline void hiusbc_writel(u32 value, void __iomem *base, u32 offset)
{
	writel(value, base + offset - HIUSBC_DEVICE_MODE_REG_BASE);
}

struct hiusbc_generic_trb {
	u32 word[4];
};

/* For all the transfer ring trb. */
struct hiusbc_link_trb {
	u32	seg_ptrl;
	u32	seg_ptrh;
	u32	rsv;
	u32	ctrl;
};

struct hiusbc_xfer_trb {
	u32	buf_ptrl;
	u32	buf_ptrh;
	u32	length;
	u32	ctrl;
};
/* Ring Segment Pointer Lo, only LINK_TRB need this macro */
#define hiusbc_trb_seg_ptr_lo(x) ((x) & 0xfffffff0)

/* TRB Transfer Length */
#define HIUSBC_TRB_XFER_LENGTH_MASK 0x00ffffff
#define hiusbc_trb_xfer_length(x) ((x) & HIUSBC_TRB_XFER_LENGTH_MASK)
#define hiusbc_get_trb_xfer_length(x) ((x) & HIUSBC_TRB_XFER_LENGTH_MASK)

/* Control bits */
#define HIUSBC_TRB_CYCLE (1 << 0)
#define HIUSBC_TRB_TC (1 << 1)
#define HIUSBC_TRB_CH (1 << 4)
#define HIUSBC_TRB_IOC (1 << 5)
#define HIUSBC_TRB_IDT (1 << 6)
#define HIUSBC_TRB_BEI (1 << 9)
#define HIUSBC_TRB_TYPE_MASK (0x3f << 10)
#define hiusbc_trb_type(x) (((x) << 10) & HIUSBC_TRB_TYPE_MASK)
#define hiusbc_get_trb_type(x) (((x) & HIUSBC_TRB_TYPE_MASK) >> 10)
#define HIUSBC_TRB_RESPONSE_MASK (0x03 << 16)
#define hiusbc_trb_response(x) (((x) << 16) & HIUSBC_TRB_RESPONSE_MASK)
#define HIUSBC_TRB_DEV_ADDR_EN (1 << 23)
#define HIUSBC_TRB_DEV_ADDR_MASK (0x7f << 24)
#define hiusbc_trb_dev_addr(x) (((x) << 24) & HIUSBC_TRB_DEV_ADDR_MASK)

#define TRPU_HOST_U3PERF_HIGH_MODE (1 << 4)
#define TRPU_HOST_U2PERF_HIGH_MODE (1 << 3)

/* For the event trbs */
struct hiusbc_evt_trb {
	u32 trb_ptrl;
	u32 trb_ptrh;
	u32 status;
	u32 info;
};
/* Transfer Event TRB */
#define HIUSBC_TRB_CMPLT_CODE_MASK (0xff << 24)
#define hiusbc_get_trb_cmplt_code(x) (((x) & HIUSBC_TRB_CMPLT_CODE_MASK) >> 24)
#define HIUSBC_TRB_CTRL_NUM_MASK (0x0f << 28)
#define hiusbc_get_trb_ctrl_num(x) (((x) & HIUSBC_TRB_CTRL_NUM_MASK) >> 28)
#define HIUSBC_ISOC_NRDY_INFO_MASK (1 << 28)
#define HIUSBC_CTRL_NRDY_INFO_MASK (1 << 29)
#define hiusbc_get_trb_mf(x) \
		((((x) & 0xfe00000) >> 12) | (((x) & 0x03fe) >> 1))
#define HIUSBC_TRB_EPNUM_MASK (0x1f << 16)
#define hiusbc_get_trb_epnum(x) (((x) & HIUSBC_TRB_EPNUM_MASK) >> 16)
/* Command Completion Event TRB */
#define HIUSBC_TRB_CMD_TYPE_MASK 0x0f
#define hiusbc_get_trb_cmd_type(x) ((x) & HIUSBC_TRB_CMD_TYPE_MASK)
/* Device Event TRB */
#define HIUSBC_TRB_DEVT_TYPE_MASK (0x3ff << 16)
#define hiusbc_get_trb_devt_type(x) (((x) & HIUSBC_TRB_DEVT_TYPE_MASK) >> 16)
#define HIUSBC_TRB_DEVT_INFO_MASK (0x3f << 26)
#define hiusbc_get_trb_devt_info(x) (((x) & HIUSBC_TRB_DEVT_INFO_MASK) >> 26)

/* Vendor Device Test LMP Event TRB */
#define HIUSBC_TRB_TEST_FIELD_MASK (0xff << 24)
#define hiusbc_get_trb_vendor_test_field(x) \
	(((x) & HIUSBC_TRB_TEST_FIELD_MASK) >> 24)

union hiusbc_trb {
	struct hiusbc_generic_trb generic;
	struct hiusbc_link_trb link;
	struct hiusbc_xfer_trb transfer;
	struct hiusbc_evt_trb event;
};

/* Transfer TRB */
#define HIUSBC_TRB_TYPE_NORMAL 1
#define HIUSBC_TRB_TYPE_CTRL_DATA 2
#define HIUSBC_TRB_TYPE_CTRL_STATUS 3
#define HIUSBC_TRB_TYPE_LINK 4

/* for DEV_CMD_WORDx. */
struct hiusbc_cmd {
	u32	word0;
	u32	word1;
	u32	word2;
	u32	word3;
};
#define HIUSBC_EP_EVENT_INT_TARGET 0

#define HIUSBC_CMD_TYPE_SET_ADDR 0x00
#define HIUSBC_CMD_TYPE_ENABLE_EP 0x01
#define HIUSBC_CMD_TYPE_DISABLE_EP 0x02
#define HIUSBC_CMD_TYPE_START_TRANSFER 0x03
#define HIUSBC_CMD_TYPE_END_TRANSFER 0x04
#define HIUSBC_CMD_TYPE_SET_HALT 0x05
#define HIUSBC_CMD_TYPE_CLEAR_HALT 0x06
#define HIUSBC_CMD_TYPE_DEV_NOTE 0x07

enum hiusbc_dr_mode {
	HIUSBC_DR_MODE_UNKNOWN,
	HIUSBC_DR_MODE_HOST,
	HIUSBC_DR_MODE_DEVICE,
	HIUSBC_DR_MODE_BOTH,
};

enum hiusbc_ep_xfer_state {
	/*
	 * Havn't sent StartXfer cmd,
	 * or have received CmdCmplt Event for EndXfer cmd.
	 * When goes into IDLE, we must make sure
	 * that the ring->queue is empty and queued_request equal to 0.
	 * because we send StartXfer cmd INIT = 1
	 * with the first TRB's dma addr in ring->dequeue.
	 */
	HIUSBC_XFER_IDLE = 0,
	/* Have sent StartXfer cmd. */
	HIUSBC_XFER_INPROGRESS,
	/*
	 * Have sent StartXfer cmd,
	 * and have received all the XferCmplt Events.
	 */
	HIUSBC_XFER_WAITING,
	/* Have sent EndXfer cmd, and havn't receive the CmdCmplt Event. */
	HIUSBC_XFER_END,
};

enum hiusbc_ctrl_stage {
	HIUSBC_SETUP_STAGE = 0,
	HIUSBC_DATA_STAGE,
	HIUSBC_STATUS_STAGE,
};

enum hiusbc_link_state {
	HIUSBC_LINK_STATE_U0 = 0,
	HIUSBC_LINK_STATE_U1,
	HIUSBC_LINK_STATE_U2,
	HIUSBC_LINK_STATE_U3,
	HIUSBC_LINK_STATE_ESS_DIS,
	HIUSBC_LINK_STATE_RX_DET,
	HIUSBC_LINK_STATE_ESS_INAC,
	HIUSBC_LINK_STATE_POLLING,
	HIUSBC_LINK_STATE_RECOVERY,
	HIUSBC_LINK_STATE_HOT_RESET,
	HIUSBC_LINK_STATE_COMPLIANCE,
	HIUSBC_LINK_STATE_LOOPBACK,
	HIUSBC_LINK_STATE_UNKNOWN,

	HIUSBC_LINK_STATE_L3 = 0,
	HIUSBC_LINK_STATE_RESET,
	HIUSBC_LINK_STATE_L1,
	HIUSBC_LINK_STATE_L2,
	HIUSBC_LINK_STATE_L0,
	HIUSBC_LINK_STATE_TEST,
	HIUSBC_LINK_STATE_L1_RESUME,
	HIUSBC_LINK_STATE_L2_RESUME,
};

enum hiusbc_link_speed {
	HIUSBC_SPEED_FULL = 0,
	HIUSBC_SPEED_HIGH,
	HIUSBC_SPEED_SUPER,
	HIUSBC_SPEED_SUPER_PLUS,
};

enum hiusbc_link_speed_portsc {
	HIUSBC_PORTSC_SPEED_INVALID = 0,
	HIUSBC_PORTSC_SPEED_FULL,
	HIUSBC_PORTSC_SPEED_LOW,
	HIUSBC_PORTSC_SPEED_HIGH,
	HIUSBC_PORTSC_SPEED_SUPER,
	HIUSBC_PORTSC_SPEED_SUPER_PLUS,
};

struct hiusbc_ep {
	char name[20];
	struct usb_ep ep;
	struct list_head queue;
	struct hiusbc *hiusbc;

	u8 epnum;
	u32 si;

	struct hiusbc_xfer_ring *ring;

	wait_queue_head_t wait_end_xfer_cmd;
	wait_queue_head_t wait_disable_ep_cmd;

	enum hiusbc_ep_xfer_state xfer_state;
	u32 nrdy_mf;
	unsigned nrdy_received:1;
	unsigned enabled:1;
	unsigned stalled:1;
	unsigned wedged:1;
	unsigned end_xfer_pending:1;
	unsigned disable_pending:1;
	unsigned set_halt_pending:1;
	/* prevent startxfer and lpm request conflict. */
	unsigned flow_control:1;

	unsigned is_in:1;

	/* Debug Flags */
	u32 succ_num;
	u32 short_num;
	u32 nrdy_num;
	u32 babble_num;
	u32 fifo_err_num;
	u32 isoc_overrun_num;
	u32 trans_err_num;
	u32 miss_service_num;
	u32 ctrl_short_num;
};

struct hiusbc_req {
	struct usb_request req;
	struct list_head queue;
	struct hiusbc_ep *hep;

	u8 epnum;

	union hiusbc_trb *first_trb;
	struct hiusbc_xfer_seg *start_seg;
	dma_addr_t trb_dma;

	union hiusbc_trb *last_trb;
	struct hiusbc_xfer_seg *last_seg;

	unsigned dma_mapped:1;
	unsigned dcs:1;
	unsigned queued_to_ring:1;
	unsigned zlp:1;

	struct timespec64 queue_time;
	struct timespec64 doorbell_time;
	struct timespec64 event_time;
	struct timespec64 giveback_time;
};

struct hiusbc_evt_ring {
	union hiusbc_trb *trbs;
	unsigned int size;
	dma_addr_t dma;
	union hiusbc_trb *dequeue;
	unsigned int ccs:1;

	struct hiusbc *hiusbc;
	unsigned int int_idx;
};

#define HIUSBC_TRBS_PER_EVT_RING 512

struct hiusbc_xfer_seg {
	union hiusbc_trb *trbs;
	struct hiusbc_xfer_seg *next;
	dma_addr_t dma;
};

struct hiusbc_xfer_ring {
	struct hiusbc_xfer_seg *first_seg;
	struct hiusbc_xfer_seg *last_seg;
	union hiusbc_trb *enqueue;
	struct hiusbc_xfer_seg *enqueue_seg;
	union hiusbc_trb *dequeue;
	struct hiusbc_xfer_seg *dequeue_seg;
	u32 pcs;
	unsigned int num_segs;
	unsigned int num_trbs_free;

	/* Usb requests that have queued to this ring. */
	struct list_head queue;
	unsigned int num_queued_req;
};

#define HIUSBC_TRBS_PER_XFER_SEG 256
#define HIUSBC_XFER_SEG_SIZE (HIUSBC_TRBS_PER_XFER_SEG * 16)
#define HIUSBC_DATA_BUF_SIZE_MAX ((1 << 24) - 1)

/* Schedule TRB for SI in the future at least HIUSBC_DEV_IST mfs. */
#define HIUSBC_DEV_IST 32
#define HIUSBC_TRB_NUM_IN_ISOC_TD_MAX 12
#define HIUSBC_TRB_NUM_FOR_ONE_PACKET_MAX 12

#define HIUSBC_U3PHY_MODE 0x1C
#define HIUSBC_U3PHY_FFE_V48_LANE0 0x7E000
#define HIUSBC_U3PHY_FFE_V48_LANE1 0x1F000
#define HIUSBC_U3PHY_FFE_V36_LANE0 0x5E000
#define HIUSBC_U3PHY_FFE_V36_LANE1 0xF000
#define HIUSBC_U3PHY_FFE_V48 0xB003400
#define HIUSBC_U3PHY_FFE_V36 0x8002700
#define u3_phy_ffe_sig_reg(base, n) ((base) + 0x2544 + ((n) * 0x800))
#define u3_phy_txdeemphctrl(base, n) ((base) + 0x898 + ((n) * 0x200))

struct hiusbc_statistics {
	u32 dev_event_total_num;
	u32 disconnect_num;
	u32 connect_num;
	u32 rst_num;
	u32 plc_num;
	u32 suspend_num;
	u32 resume_num;
	u32 l1_suspend_num;
	u32 l1_resume_num;
	u32 sof_num;
	u32 phy_err_num;
	u32 combo_num;
	u32 eb_full_num;
	u32 unsuccess_num;
	u32 event_after_discon_num;
};

struct hiusbc {
	spinlock_t lock;
	struct usb_gadget gadget;
	struct usb_gadget_driver *gadget_driver;
	struct device *dev;
	struct device *sysdev;
	struct platform_device *xhci;

	void __iomem *host_regs;
	void __iomem *dev_regs;
	void __iomem *com_regs;
	void __iomem *hub_regs;
	void __iomem *lmi_regs;
	void __iomem *u2_pl_regs;
	void __iomem *u2_piu_regs;
	void __iomem *u3_pl_regs;
	void __iomem *u3_link_regs;
	void __iomem *combophy_regs;

	void __iomem *regs;
	size_t regs_size;

	struct hiusbc_ep *eps[HIUSBC_EP_NUMS];

	u32 gen2_txdeemph_reg;
	u32 gen2_txdeemph_v48_cp13;
	u32 gen2_txdeemph_v48_cp14;
	u32 gen2_txdeemph_v48_cp15;
	u32 gen2_txdeemph_v48_cp16;

	u32 gen2_txdeemph_v36_cp13;
	u32 gen2_txdeemph_v36_cp14;
	u32 gen2_txdeemph_v36_cp15;
	u32 gen2_txdeemph_v36_cp16;

	u32 irq;
	u32 sys_err_irq;
	/* Debug */
	u32 total_irq_count;
	u32 xfer_irq_count;

	struct hiusbc_evt_ring *event_ring;
	struct dma_pool *xfer_seg_pool;
	void *bounce_buf;
	dma_addr_t bounce_dma;

	struct tasklet_struct	bh;

	enum hiusbc_dr_mode support_dr_mode;
	enum hiusbc_dr_mode current_dr_mode;
	enum hiusbc_dr_mode desired_dr_mode;
	struct extcon_dev *edev;
	struct notifier_block edev_nb;

	u8 *dev_req_response_buf;
	struct hiusbc_req dev_req;
	struct usb_ctrlrequest setup_packet;
	enum hiusbc_ctrl_stage cur_stage;
	enum hiusbc_ctrl_stage next_stage;
	u8 ctrl_num;

	struct usb_endpoint_descriptor *ep0_desc;

	u16 imodi;
	u8 test_mode;

	enum usb_device_speed max_speed;

	u8 hird_suspend;
	u8 hird_min;
	u8 hird_max;

	u8 u1_sel;
	u8 u1_pel;
	u16 u2_sel;
	u16 u2_pel;

	struct usb_phy *usb_phy_u2;
	struct usb_phy *usb_phy_u3;

	struct phy *phy_u2;
	struct phy *phy_u3;

	struct dentry *root;
	struct debugfs_regset32 *regset;
	u8 cp_test;

	/* clk delta cfg */
	unsigned clk_delta_pn;
	unsigned clk_delta_cn;
	unsigned sg_supported;

	struct hiusbc_statistics stat;
	unsigned need_data_stage:1;
	unsigned status_stage_dir:1;
	unsigned data_stage_dir:1;
	unsigned delayed_status:1;

	unsigned connected:1;
	unsigned remote_wakeup:1;
	unsigned u1_enable:1;
	unsigned u2_enable:1;
	unsigned ep0_stall_pending:1;
	unsigned usb2_host_lpm_capable:1;
	unsigned usb3_host_lpm_capable:1;
	unsigned usb2_dev_lpm_capable:1;
	unsigned usb3_dev_lpm_capable:1;
	unsigned usb3_dev_lpm_u1_accept:1;
	unsigned usb3_dev_lpm_u2_accept:1;
	unsigned usb3_dev_lpm_u1_initiate:1;
	unsigned usb3_dev_lpm_u2_initiate:1;
	unsigned usb3_dev_lpm_ux_exit:1;
	unsigned ep0_changed_by_gadget_api:1;
	unsigned set_vbus_quirk:1;
	unsigned events_pending:1;
	unsigned host_abort_cmd_quirk:1;
	unsigned host_ca_timeout_quirk:1;
	unsigned running:1;
	unsigned is_fpga:1;
	unsigned dis_gadget_u2_lpm_bulk_always_idle:1;
	unsigned disconnect_in_suspend:1;
	unsigned ltssm_dump:1;
	unsigned last_is_connect_event:1;
	unsigned babble_expected:1;
	unsigned host_u3_low_perf_mode:1;

	/* Debug */
	unsigned db_on_received_event:1;
	unsigned trpu_reset_switch:1;
	unsigned stop_before_drd_switch:1;
	unsigned reset_before_drd_switch:1;

#ifdef CONFIG_HIUSBC_EDA_TEST_CASE
	int force_maxp;
	int force_interval;
#endif
	struct timespec64 doorbell_time;
	struct timespec64 irq_time;
	struct timespec64 handle_event_time;
};

void hiusbc_set_mode(struct hiusbc *hiusbc,
	enum hiusbc_dr_mode desired_mode);
int hiusbc_get_irq(struct hiusbc *hiusbc);
int hiusbc_reset_controller(struct hiusbc *hiusbc);
void hiusbc_set_speed(struct hiusbc *hiusbc,
	enum usb_device_speed speed);

#endif /* __HIUSBC_CORE_H */
