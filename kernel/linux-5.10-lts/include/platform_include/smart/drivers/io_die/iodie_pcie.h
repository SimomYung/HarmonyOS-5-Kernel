/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Contexthub 9030 driver.
 * Create: 2022-11-30
 */
#ifndef __IODIE_PCIE_H__
#define __IODIE_PCIE_H__

#define PCIE_STATUS_LEN 4

enum iodie_cmd{
	IODIE_CMD_START,
	IODIE_CMD_REQUEST_NORMAL = 0x01,
	IODIE_CMD_REQUEST_NORMAL_RESP = 0x02,
	IODIE_CMD_RELEASE_NORMAL = 0x03,
	IODIE_CMD_RELEASE_NORMAL_RESP = 0x04,
	IODIE_CMD_REQUEST_L0 = 0x5,
	IODIE_CMD_RELEASE_L0 = 0x7,
	IODIE_CMD_STATUS_NOTIFY = 0x09,
	IODIE_CMD_STATUS_NOTIFY_RESP = 0x0A,
	IODIE_CMD_GPIO_INTR_NOTIFY = 0x0B,
	IODIE_CMD_GPIO_INTR_NOTIFY_RESP = 0x0C,
	IODIE_CMD_GPIO_INTR_REGISTER = 0x0D,
	IODIE_CMD_GPIO_INTR_REGISTER_RESP = 0x0E,
	IODIE_CMD_GPIO_INTR_UNREGISTER = 0x0F,
	IODIE_CMD_GPIO_INTR_UNREGISTER_RESP = 0x10,
	IODIE_CMD_GPIO_READ = 0x11,
	IODIE_CMD_GPIO_READ_RESP = 0x12,
	IODIE_CMD_REG_OP = 0x13,
	IODIE_CMD_REG_OP_RESP = 0x14,
	IODIE_CMD_CLK_OUT_CFG = 0x15,
	IODIE_CMD_CLK_OUT_CFG_RESP = 0x16,
	IODIE_CMD_PCIE = 0x17,
	IODIE_CMD_PCIE_RESP = 0x18,
	IODIE_CMD_GPIO_ENABLE = 0x19,
	IODIE_CMD_GPIO_ENABLE_RESP = 0x1A,
	IODIE_CMD_SR_TEST = 0x1B,
	IODIE_CMD_SR_TEST_RESP = 0x1C,
	IODIE_CMD_I3C_STRESS_TEST = 0x1D,
	IODIE_CMD_I3C_STRESS_TEST_RESP = 0x1E,
	IODIE_CMD_RECOVERY = 0x1F,
	IODIE_CMD_RECOVERY_RESP = 0x20,
};

enum iodie_pcie_cmd {
	IODIE_PCIE_CMD_POWER,
	IODIE_PCIE_CMD_LINK_STATE_GET,
	IODIE_PCIE_CMD_LINK_SPEED_GET,
	IODIE_PCIE_CMD_RAM_TEST,
	IODIE_PCIE_CMD_LP,
	IODIE_PCIE_CMD_PHY_EYE_PATTERN,
	IODIE_PCIE_CMD_PHY_PARAM_DUMP,
	IODIE_PCIE_CMD_REG_DUMP,
};

enum iodie_pcie_eyefom_mode {
	IODIE_PCIE_COMMON_EYEFOM = 0,
	IODIE_PCIE_DEFAULT_EYEFOM,
	IODIE_PCIE_DIG_EYEFORM,
};

enum ioide_pcie_err_code{
	IODIE_ERR_END,
};

enum iodie_sys_status{
	IODIE_ST_BEGIN,
	IODIE_ST_ERROR = IODIE_ST_BEGIN,
	IODIE_ST_PRE_INITED,
	IODIE_ST_INITED,
	IODIE_ST_POWER_UP,
	IODIE_ST_POWER_DOWN,
	IODIE_ST_L0,
	IODIE_ST_L1,
	IODIE_ST_NON_EXIST,
	IODIE_ST_END,
};

enum iodie_pcie_user{
	IODIE_USER_BEGIN,
	IODIE_USER_GPIO = IODIE_USER_BEGIN,
	IODIE_USER_I2C,
	IODIE_USER_END,
};

typedef enum {
	IODIE_CLK_OUT_CMD_OPEN,
	IODIE_CLK_OUT_CMD_CLOSE,
	IODIE_CLK_OUT_CMD_END,
}iodie_clk_out_cfg_enum;

enum iodie_ao_gpio{
	IODIE_AO_GPIO_BEGIN = 2064,    /* keep pace with hi9030 ao gpio number in sensorhub  */
	IODIE_AO_GPIO0 = IODIE_AO_GPIO_BEGIN,
	IODIE_AO_GPIO1,
	IODIE_AO_GPIO2,
	IODIE_AO_GPIO3,
	IODIE_AO_GPIO4,
	IODIE_AO_GPIO5,
	IODIE_AO_GPIO6,
	IODIE_AO_GPIO7,
	IODIE_AO_GPIO_END,
};

enum irq_type {
	HI9030_IRQ_TYPE_NONE = 0,
	HI9030_IRQ_TYPE_EDGE_RISING,
	HI9030_IRQ_TYPE_EDGE_FALLING,
	HI9030_IRQ_TYPE_EDGE_BOTH,
	HI9030_IRQ_TYPE_LEVEL_HIGH,
	HI9030_IRQ_TYPE_LEVEL_LOW,
	HI9030_IRQ_TYPE_UNKNOWN,
};

enum {
	HI_PCIE_VOTE_COMMON_RST_PCIE0 = 0x6,
	HI_PCIE_VOTE_COMMON_RST_PCIE1,
	HI_PCIE_VOTE_COMMON_CLK_PCIE0,
	HI_PCIE_VOTE_COMMON_CLK_PCIE1,
	HI_PCIE_VOTE_MEM_SD_PCIE0,
	HI_PCIE_VOTE_MEM_SD_PCIE1,
	HI_PCIE_VOTE_FNPLL_PCIE0,
	HI_PCIE_VOTE_FNPLL_PCIE1_ACPU,
	HI_PCIE_VOTE_FNPLL_PCIE1_IOMCU,
	HI_PCIE_VOTE_MAX,
};

struct iodie_i3c_stress_test {
	uint32_t op;        /* 0 write 1 read */
	uint32_t slave_id;
	uint32_t write_val;
	uint32_t read_val;
	uint32_t times;
};

struct iodie_pcie_cmd_op {
	uint32_t host_id;
	uint32_t port_id;
	uint32_t cmd;
	uint32_t x1;
	uint32_t x2;
	uint32_t x3;
	uint32_t x4;
};

#define BG_CTRL_REG_0						0x0AC8
#define PLL_LOCK_DET_RO						0x0B70
#define PCS_STATUS							0x1400
#define PLL_BANDCTRLR1						0x0B30
#define PLL_BANDCTRLR2						0x0B38
#define TERMCTRL							0x154C
#define PMACTRLSTATUS						0x0A00
#define PLL_KBAND_CPLT						0x0B74
#define REG_PV_DEBUG_CFG					0x09B0
#define PMA_PCS_IO_NEW_ADD					0x0E00
#define PMA_INTF_COM_SIG_CTRL_STS_REG_2		0x0F88
#define PMA_DBG_STS_REG						0x1594
#define PMA_DBG_INFO_SEL_REG				0x158C
#define PIPE_INTF_SIG_CTRL_STS_REG_1		0x1B84

#define SCTRL_PCIE_RESOURCE_VOTE_REG			0x4D0

#define SCTRL_PCIE_RESOURCE_VOTE_RST_MASK		0xC0
#define SCTRL_PCIE_RESOURCE_VOTE_CLK_MASK		0x300
#define SCTRL_PCIE_RESOURCE_VOTE_MEM_SD_MASK	0xC00
#define SCTRL_PCIE_RESOURCE_VOTE_FNPLL_MASK		0x7000

#define SCTRL_SCPERCTRL17_REG					0x344

/* kphy eye param */
#define QUADRANT1			0x21
#define QUADRANT2			0x2
#define QUADRANT3			0x14
#define QUADRANT4			0x38
#define GRAPH_LEN			32
#define QUA_LEN				16
#define IMAGE_WIDTH			66

typedef void (*iodie_gpio_intr_func)(void *data);
typedef void (*iodie_user_func)(enum iodie_sys_status status);

int iodie_gpio_intr_register(enum iodie_ao_gpio gpio_id, enum irq_type type, iodie_gpio_intr_func func, void *data);
int iodie_gpio_intr_unregister(enum iodie_ao_gpio gpio_id);
int iodie_gpio_enable(enum iodie_ao_gpio gpio_id);

enum iodie_sys_status iodie_get_status(void);
int check_iodie_link_status(void);
int iodie_notify_register(enum iodie_pcie_user user, iodie_user_func func);

int iodie_normal_request(enum iodie_pcie_user src);
int iodie_normal_release(enum iodie_pcie_user src);

int iodie_clk_out_sys(iodie_clk_out_cfg_enum op);

void* send_pcie_data_to_dubai(void);
void pcie_data_reset(void);

#ifdef CONFIG_IO_DIE_PCIE_DEBUG
int iodie_pcie_dieid_read(uint32_t *dieid, uint32_t len);
#endif

#ifndef IODIE_PCIE_IDLE
#define IODIE_PCIE_IDLE 1
#endif

int iodie_pcie_hwspinlock_request(void);
void iodie_pcie_hwspinlock_release(void);
#if IODIE_PCIE_IDLE
void iodie_pcie_fnpll_request(void);
void iodie_pcie_fnpll_release(void);
#endif
spinlock_t *iodie_pcie_resource_vote_lock_get(void);
u32 iodie_pcie_asic_tyep_get(void);

int iodie_sr_test(int threshold);
int iodie_i3c_stress_test(struct iodie_i3c_stress_test *data);
int iodie_commu(enum iodie_cmd cmd, void *data, uint32_t len);
void iodie_pcie_regs_dump(void);

u32 iodie_pcie_sctrl_reg_readl(u32 reg);
void iodie_pcie_sctrl_reg_writel(u32 val, u32 reg);

#endif

