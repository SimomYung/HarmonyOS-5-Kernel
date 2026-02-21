/*
  * Copyright (c) Huawei Technologies Co., Ltd. 2024-2027. All rights reserved.
  * Description: Contexthub IPC send api.
  * Create: 2024-08-08
  */

#ifndef __PMIC_IPC_H__
#define __PMIC_IPC_H__
#ifdef CONFIG_PMIC_56V200_PMU
#include <platform_include/smart/linux/iomcu_ipc.h>
#endif

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#ifdef CONFIG_PMIC_56V200_PMU
#define SOC_ACPU_SCTRL_BASE_ADDR PMIC_SOC_ACPU_SCTRL_BASE_ADDR
#define RECOVERY_MODE_FLAG_REG(addr) PMIC_SOC_SCTRL_SCBAKDATA0_ADDR(addr)
#define RECOVERY_MODE_FLAG BIT(31)
#else
#define SOC_ACPU_SCTRL_BASE_ADDR 0
#define RECOVERY_MODE_FLAG_REG(addr) 0
#define RECOVERY_MODE_FLAG BIT(31)
#endif

enum pmu_ipc_cmd {
	PMU_IPC_NOTIFY,
	PMU_IPC_NOTIFY_RESP,
};

enum pmu_ipc_id {
	IRQ_OTMP,
	IRQ_VBUS_INSERT,
	IRQ_VBUS_OUT,
	IRQ_ALARM_ON,
	IRQ_POWERON_6S,
	IRQ_POWERON_1S,
	IRQ_POWERKEY_UP,
	IRQ_POWERKEY_DOWN,
	IRQ_OCP,
	IRQ_COUL,
	IRQ_SIM1_UP,
	IRQ_SIM1_DOWN,
	IRQ_FUNCKEY_UP = 16,
	IRQ_FUNCKEY_DOWN,
	IRQ_XO32K_DIS,
};

#ifndef CONFIG_PMIC_56V200_PMU
#define TAG_PMU 0
struct pkt_header {
	unsigned char tag;
	unsigned char cmd;
	unsigned char resp:1;
	unsigned char hw_trans_mode:2; /* 0:IPC 1:SHMEM 2:64bitsIPC */
	unsigned char rsv:5; /* 5 bits */
	unsigned char partial_order;
	unsigned char tranid;
	unsigned char app_tag;
	unsigned short length;
};

__attribute__((weak)) int register_mcu_event_notifier(int tag, int cmd, int (*notify)(const struct pkt_header *head))
{
	return 0;
}

__attribute__((weak)) int unregister_mcu_event_notifier(int tag, int cmd, int (*notify)(const struct pkt_header *head))
{
	return 0;
}
#endif

struct pmu_ipc_msg {
	struct pkt_header hd;
	uint32_t irq_id;
};
#endif

