/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Contexthub 9030 driver.
 * Create: 2022-11-30
 */

#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/io.h>
#include <linux/of.h>
#include "inputhub_api/inputhub_api.h"
#include <platform_include/smart/linux/iomcu_status.h>
#include <platform_include/smart/linux/iomcu_ipc.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <securec.h>
#include <linux/arm-smccc.h>  /* just use in es */

#ifdef CONFIG_CONTEXTHUB_TZSP
#include <linux/arm_ffa.h>
#include <platform_include/see/ffa/ffa_msg_id.h>
#include <platform_include/see/ffa/ffa_plat_drv.h>
#endif

#include <platform_include/smart/drivers/io_die/int_parse.h>
#include <platform_include/smart/drivers/io_die/iodie_pcie.h>

#define MODULE_NAME                      "IO_DIE_PCIE"
#define IODIE_MAX_PAYLOAD                 (MAX_PKT_LENGTH - (int)sizeof(struct pkt_header))
#define NS_TO_MS                          1000000
#define IODIE_IPC_RETRY_TIMES             5
#define IODIE_IPC_RETRY_INTERVAL          200   // ms
#define IODIE_IPC_RETRY_THREHOLD          2000  // ms
#define IODIE_RECOVERY_SCENE_TIMEOUT      100   // ms
#define PCIE1_ASPM_DIS_OFFSET             0x4
#define PCIE1_ASPM_STAT_OFFSET            0xc
#define PCIE1_ASPM_ACPU_MASK              0x1
#define SCTRL_ADDR_OFFSET                 0xFFF
#define SCTRL_SEC_AGLIN                   0x800
#define SCTRL_PCIE1_ASPM_SIZE             0xF
#define IODIE_STATUS_MASK                 0x80000000
#define IODIE_STATUS_OFFSET               0x46C
#define IODIE_PCIE_RETRY_TIMES            3  // ms
#define IODIE_PCIE_RETRY_INTERVAL         5  // ms

#define IODIE_PCIE_MAX_RECOVERY_TIME      2
#define IODIE_PCIE_MAX_WAIT_TIME          13 // 13*100ms=1.3s, wait 1.3 seconds in worst case

struct iodie_vote{
	uint8_t vote_count[IODIE_USER_END];
	uint32_t total;
};

struct iodie_perf_stat{
	atomic64_t max_latency;
	enum iodie_cmd cmd;
};

struct iodie_status_msg{
	struct pkt_header hd;
	uint32_t status;
	uint32_t len;
	uint8_t *data;
};

struct iodie_gpio_msg{
	struct pkt_header hd;
	uint32_t gpio_id;
};

struct iodie_gpio_info {
	uint32_t gpio_id;
	iodie_gpio_intr_func func;
	void *data;
	uint8_t is_delay;
	uint8_t irq_type;
};

struct iodie_reg_op{
	uint32_t op;        /* 0 write 1 read */
	uint32_t slave_id;
	uint32_t reg_addr;
	uint32_t val;       /* write vlaue, if read, ignore it */
};

struct iodie_io_irq_info{
	uint32_t  gpio_id;
	uint32_t  irq_type;
};

static struct iodie_gpio_info g_iodie_gpio_info[IODIE_AO_GPIO_END - IODIE_AO_GPIO_BEGIN];
static struct iodie_perf_stat g_iodie_perf;
static iodie_user_func g_iodie_user_func_list[IODIE_USER_END];
static DEFINE_MUTEX(g_iodie_notify_lock);
static DEFINE_MUTEX(g_iodie_st_lock);
static enum iodie_sys_status g_sys_st = IODIE_ST_PRE_INITED;
static struct iodie_vote g_iodie_vote = {{0}, 0};
static uint8_t g_support_io_die = 1;
static uint32_t g_pcie1_aspm_addr_raw;
static void __iomem *g_pcie1_aspm_addr;
static struct work_struct gpio_delay_worker;
static struct work_struct g_iodie_err_handle_worker;
static struct completion g_iodie_recovery_scene;

int g_l0_flag;
u64 g_l0_jiffies;

typedef struct pcie_time{
	uint64_t gpio_cnt;
	uint64_t i2c_cnt;
    uint64_t l0;
} pcie_time;

static pcie_time g_pcie_time;

void* send_pcie_data_to_dubai(void)
{
	pr_info("gpio_cnt : %llu, i2c_cnt : %llu, l0 : %llu",
            g_pcie_time.gpio_cnt, g_pcie_time.i2c_cnt, g_pcie_time.l0);
	return (void*)&g_pcie_time;
}

void pcie_data_reset(void)
{
	g_pcie_time.i2c_cnt = 0;
	g_pcie_time.gpio_cnt = 0;
    g_pcie_time.l0 = 0;
}

static int io_die_ipc(const struct write_info *wr, struct read_info *rd)
{
	int ret;
	uint8_t times = 0;
	u64 entry_jiffies;
	u64 exit_jiffies;
	u64 diff_usecs;

	entry_jiffies = get_jiffies_64();
	ret = write_customize_cmd(wr, rd, true);
	while (ret < 0 && times < IODIE_IPC_RETRY_TIMES) {
		pr_err("%s: send err, times[%hhu] ret[%d]\n", __func__, times, ret);
		exit_jiffies = get_jiffies_64();
		diff_usecs = jiffies64_to_nsecs(exit_jiffies - entry_jiffies) / NS_TO_MS;
		if (diff_usecs > IODIE_IPC_RETRY_THREHOLD) {
			pr_err("%s: timeout, lantency[%llu]ms. \n", __func__, diff_usecs);
			break;
		}
		times++;
		msleep(IODIE_IPC_RETRY_INTERVAL);
		ret = write_customize_cmd(wr, rd, true);
	}

	return ret;
}

static int iodie_ipc_resp_handler(enum iodie_cmd cmd, struct read_info *rd, void *data, uint32_t len)
{
	pr_info("%s: resp len [%d]\n", __func__, rd->data_length);

	if (cmd == IODIE_CMD_REG_OP && rd->data_length == sizeof(u32)) {
		((struct iodie_reg_op *)data)->val = *(u32 *)rd->data;
		pr_info("%s: read res 0x%x\n", __func__, *(u32 *)rd->data);
	}

	if (cmd == IODIE_CMD_PCIE && rd->data_length == sizeof(u32)) {
		((struct iodie_pcie_cmd_op *)data)->x1 = *(u32 *)rd->data;
		pr_info("%s: cmd:0x%x, data:0x%x\n", __func__, ((struct iodie_pcie_cmd_op *)data)->cmd, *(u32 *)rd->data);
	}

	return EOK;
}

static int do_iodie_commu(enum iodie_cmd cmd, void *data, uint32_t len)
{
	int ret;
	struct write_info winfo;
	struct read_info rinfo;

	(void)memset_s(&winfo, sizeof(struct write_info), 0, sizeof(struct write_info));
	(void)memset_s(&rinfo, sizeof(struct read_info), 0, sizeof(struct read_info));

	winfo.wr_len = len;
	winfo.wr_buf = data;
	winfo.cmd = cmd;
	winfo.tag = TAG_IO_DIE;
	ret = io_die_ipc(&winfo, &rinfo);
	if (ret) {
		pr_err("%s: send err, ret is [%d]\n", __func__, ret);
		return ret;
	}

	if(rinfo.data_length > 0)
		iodie_ipc_resp_handler(cmd, &rinfo, data, len);

	if(rinfo.errno) {
		pr_err("%s: response err, ret is [%d]\n", __func__, rinfo.errno);
		return rinfo.errno;
	}

	return ret;
}

/* send iodie msg to sensorhub */
int iodie_commu(enum iodie_cmd cmd, void *data, uint32_t len)
{
	int ret;
	u64 entry_jiffies;
	u64 exit_jiffies;
	u64 diff_usecs;

	if (!g_support_io_die) {
		pr_info("%s: no io die. \n", __func__);
		return EOK;
	}

	entry_jiffies = get_jiffies_64();
	ret = do_iodie_commu(cmd, data, len);
	exit_jiffies = get_jiffies_64();
	diff_usecs = jiffies64_to_nsecs(exit_jiffies - entry_jiffies) / NS_TO_MS;
	if (unlikely(diff_usecs > (u64)atomic64_read(&g_iodie_perf.max_latency))) {
		atomic64_set(&g_iodie_perf.max_latency, (s64)diff_usecs);
		g_iodie_perf.cmd = cmd;
		pr_info("%s:max_latency, cmd[%u], lantency[%llu]ms\n", __func__, cmd, diff_usecs);
	}

	pr_info("%s: send complete, cmd[%u], lantency[%llu]ms.\n", __func__, cmd, diff_usecs);

	return ret;
}

/* get iodie pcie dieid */
#ifdef CONFIG_IO_DIE_PCIE_DEBUG
#define PCIE_DIEID_REG_START 0x25E1518
int iodie_pcie_dieid_read(uint32_t *dieid, uint32_t len)
{
	int ret;
	int i;
	struct iodie_reg_op iodie_dieid = {0};
	if (dieid == NULL) {
		pr_err("iodie pcie dieid read: dieid is null\n");
		return -EINVAL;
	}
	
	iodie_dieid.op = 1;
	iodie_dieid.slave_id = 0;
	for (i = 0; i < len; i++) {
		iodie_dieid.reg_addr = PCIE_DIEID_REG_START - 0x4 * i;
		ret = iodie_commu(IODIE_CMD_REG_OP, &iodie_dieid, sizeof(struct iodie_reg_op));
		if (ret != 0) {
			pr_err("iodie pcie dieid read failed, ret is 0x%x\n", ret);
			return -1;
		}
		dieid[i]=iodie_dieid.val;
	}

	return ret;
}
#endif

#ifdef CONFIG_DFX_DEBUG_FS
int iodie_reg_op(struct iodie_reg_op *data)
{
	if (data == NULL) {
		pr_err("iodie_reg_op: data is null\n");
		return -EINVAL;
	}

	pr_info("%s: op[%x], slave_id[%x], reg_addr[%x], val[%x]\n", __func__, data->op, data->slave_id,
		data->reg_addr, data->val);

	return iodie_commu(IODIE_CMD_REG_OP, data, sizeof(struct iodie_reg_op));
}

uint32_t iodie_reg_read(uint32_t reg)
{
	struct iodie_reg_op iodie_reg = {0};

	iodie_reg.op = 1;
	iodie_reg.slave_id = 0;
	iodie_reg.reg_addr = reg;

	iodie_reg_op(&iodie_reg);

	return iodie_reg.val;
}

void iodie_reg_write(uint32_t reg, uint32_t val)
{
	struct iodie_reg_op iodie_reg = {0};

	iodie_reg.op = 0;
	iodie_reg.slave_id = 0;
	iodie_reg.reg_addr = reg;
	iodie_reg.val = val;

	iodie_reg_op(&iodie_reg);
}
#endif

int iodie_notify_register(enum iodie_pcie_user user, iodie_user_func func)
{
	if (user >= IODIE_USER_END || func == NULL)
		return -EINVAL;

	mutex_lock(&g_iodie_notify_lock);
	g_iodie_user_func_list[user] = func;
	mutex_unlock(&g_iodie_notify_lock);

	return EOK;
}

static int iodie_set_status(enum iodie_sys_status st)
{
	if (st >= IODIE_ST_END)
		return -EINVAL;

	mutex_lock(&g_iodie_st_lock);
	g_sys_st = st;
	if (st == IODIE_ST_NON_EXIST)
		g_support_io_die = 0;

	mutex_unlock(&g_iodie_st_lock);

	return EOK;
}

enum iodie_sys_status iodie_get_status(void)
{
	enum iodie_sys_status st;

	mutex_lock(&g_iodie_st_lock);
	st = g_sys_st;
	mutex_unlock(&g_iodie_st_lock);

	return st;
}

/* sec read and write */
static int sec_config(bool is_rd, unsigned int addr, unsigned int val)
{
#ifdef CONFIG_CONTEXTHUB_TZSP
	int ret;
	struct ffa_send_direct_data args = {0};

	if (is_rd) {
		args.data0 = SECURE_ENG_READ;
	} else {
		args.data0 = SECURE_ENG_WRITE;
	}
	args.data1 = addr;
	args.data2 = val;

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0)
		pr_err("%s:ffa_platdrv_send_msg fail! ret:%d\n", __func__, ret);

	return args.data1;
#endif
	return -1;
}

#ifdef CONFIG_DFX_DEBUG_FS
int iodie_rdv_sec_config(bool is_rd, unsigned int addr, unsigned int val)
{
	return sec_config(is_rd, addr, val);
}
#endif

static int get_soc_pcie1_reg_addr(void)
{
	struct device_node *node;
	int ret;

	if (g_pcie1_aspm_addr == NULL) {
		node = of_find_compatible_node(NULL, NULL, "hisilicon,io-die");
		if (!node) {
			pr_err("%s, can not find iodie node\n", __func__);
			return -EFAULT;
		}

		ret = of_property_read_u32(node, "pcie1_reg", &g_pcie1_aspm_addr_raw);
		if (ret) {
			pr_err("%s failed to get reg err! ret=%d\n", __func__, ret);
			return -EFAULT;
		}

		g_pcie1_aspm_addr = ioremap(g_pcie1_aspm_addr_raw, SCTRL_PCIE1_ASPM_SIZE);
		if (g_pcie1_aspm_addr == NULL) {
			pr_err("%s remap failed\n", __func__);
			return -EFAULT;
		}
	}

	return EOK;
}

static void iodie_pcie_request_l0(void)
{
	get_soc_pcie1_reg_addr();

	if ((g_pcie1_aspm_addr_raw & SCTRL_ADDR_OFFSET) > SCTRL_SEC_AGLIN)
		(void)sec_config(false, g_pcie1_aspm_addr_raw, PCIE1_ASPM_ACPU_MASK);
	else
		writel(PCIE1_ASPM_ACPU_MASK, g_pcie1_aspm_addr);
}

static void iodie_pcie_release_l0(void)
{
	get_soc_pcie1_reg_addr();

	if ((g_pcie1_aspm_addr_raw & SCTRL_ADDR_OFFSET) > SCTRL_SEC_AGLIN)
		(void)sec_config(false, g_pcie1_aspm_addr_raw + PCIE1_ASPM_DIS_OFFSET, PCIE1_ASPM_ACPU_MASK);
	else
		writel(PCIE1_ASPM_ACPU_MASK, g_pcie1_aspm_addr + PCIE1_ASPM_DIS_OFFSET);
}

static uint32_t get_iodie_pcie_l0_stat(void)
{
	get_soc_pcie1_reg_addr();

	if ((g_pcie1_aspm_addr_raw & SCTRL_ADDR_OFFSET) > SCTRL_SEC_AGLIN)
		return sec_config(true, g_pcie1_aspm_addr_raw + PCIE1_ASPM_STAT_OFFSET, PCIE1_ASPM_ACPU_MASK);
	else
		return readl(g_pcie1_aspm_addr + PCIE1_ASPM_STAT_OFFSET);
}

static int iodie_link_recovery(void)
{
	uint8_t times = 0;

	while (check_iodie_link_status() != EOK && times < IODIE_PCIE_RETRY_TIMES) {
		pr_err("%s: times[%hhu]\n", __func__, times);
		times++;
		msleep(IODIE_PCIE_RETRY_INTERVAL);
	}

	return check_iodie_link_status();
}

int iodie_normal_request(enum iodie_pcie_user src)
{
	uint32_t pre_vote;
	int ret = EOK;

#if IODIE_PCIE_IDLE
	spinlock_t *resource_vote_lock = iodie_pcie_resource_vote_lock_get();
	unsigned long flags;
#endif
	if (src >= IODIE_USER_END)
		return -EINVAL;

	pr_info("%s: from [%u]. \n", __func__, (uint32_t)src);

	switch(src){
		case IODIE_USER_GPIO:
			g_pcie_time.gpio_cnt++;
			break;
		case IODIE_USER_I2C:
			g_pcie_time.i2c_cnt++;
			break;
		default:
			break;
	}

	if (check_iodie_link_status() != EOK) {
		if (iodie_link_recovery() != EOK)
		 	return -EFAULT;
	}

	mutex_lock(&g_iodie_st_lock);
	if (g_iodie_vote.vote_count[src] == 0xFF) {
		pr_err("%s: src [%u] upto max vote. \n", __func__, (uint32_t)src);
		mutex_unlock(&g_iodie_st_lock);
		return -EFAULT;
	}

	if (g_sys_st == IODIE_ST_ERROR) {
		pr_err("%s: status error %u. \n", __func__, g_sys_st);
		mutex_unlock(&g_iodie_st_lock);
		return -EFAULT;
	}

	pre_vote = g_iodie_vote.total;
	g_iodie_vote.vote_count[src]++;
	g_iodie_vote.total++;
	pr_info("%s total = %u \n", __func__, g_iodie_vote.total);

	if (pre_vote == 0 && g_iodie_vote.total != 0) {
#if IODIE_PCIE_IDLE
		if (iodie_pcie_hwspinlock_request()) {
			g_iodie_vote.vote_count[src]--;
			g_iodie_vote.total--;
			mutex_unlock(&g_iodie_st_lock);
			return -EFAULT;
		}
		spin_lock_irqsave(resource_vote_lock, flags);
		iodie_pcie_fnpll_request(); // only pcie1 use
#endif
		iodie_pcie_request_l0();
#if IODIE_PCIE_IDLE
		spin_unlock_irqrestore(resource_vote_lock, flags);
		iodie_pcie_hwspinlock_release();
#endif
		pr_info("iodie ap request normal stat 0x%x, form [%u]. \n", get_iodie_pcie_l0_stat(), (uint32_t)src);
	}
	g_l0_flag = 1;
	g_l0_jiffies = get_jiffies_64();
	mutex_unlock(&g_iodie_st_lock);

	return ret;
}

int iodie_normal_release(enum iodie_pcie_user src)
{
	int ret = EOK;
	u64 release_jiffies = 0;

#if IODIE_PCIE_IDLE
	spinlock_t *resource_vote_lock = iodie_pcie_resource_vote_lock_get();
	unsigned long flags;
#endif
	if (src >= IODIE_USER_END)
		return -EINVAL;

	pr_info("%s: from [%u]. \n", __func__, (uint32_t)src);

	mutex_lock(&g_iodie_st_lock);
	if (g_iodie_vote.vote_count[src] == 0) {
		pr_err("%s: src [%u] has no vote. \n", __func__, (uint32_t)src);
		mutex_unlock(&g_iodie_st_lock);
		return -EFAULT;
	}

	g_iodie_vote.vote_count[src]--;
	g_iodie_vote.total--;
	pr_info("%s total = %u \n", __func__, g_iodie_vote.total);
	if (g_iodie_vote.total == 0) {
#if IODIE_PCIE_IDLE
		if (iodie_pcie_hwspinlock_request()) {
			mutex_unlock(&g_iodie_st_lock);
			return -EFAULT;
		}
		spin_lock_irqsave(resource_vote_lock, flags);
#endif
		iodie_pcie_release_l0();
#if IODIE_PCIE_IDLE
		iodie_pcie_fnpll_release(); // only pcie1 use
		spin_unlock_irqrestore(resource_vote_lock, flags);
		iodie_pcie_hwspinlock_release();
#endif
		pr_info("iodie ap release normal stat 0x%x, form [%u] \n", get_iodie_pcie_l0_stat(), (uint32_t)src);
	}
	if (g_sys_st == IODIE_ST_ERROR && g_iodie_vote.total == 0) {
		pr_err("%s: status error %u. complete! \n", __func__, g_sys_st);
		complete(&g_iodie_recovery_scene);
	}
	if (g_l0_flag == 1) {
		g_l0_flag = 0;
		release_jiffies = get_jiffies_64();
		g_pcie_time.l0 += jiffies64_to_msecs(release_jiffies - g_l0_jiffies);
	}
	mutex_unlock(&g_iodie_st_lock);

	return ret;
}

static int iodie_wait_completion(void)
{
	bool wait = false;

	mutex_lock(&g_iodie_st_lock);
	pr_info("%s total = %u \n", __func__, g_iodie_vote.total);
	if (g_iodie_vote.total != 0 && g_sys_st == IODIE_ST_ERROR) {
		reinit_completion(&g_iodie_recovery_scene);
		wait = true;
	}
	mutex_unlock(&g_iodie_st_lock);

	if (wait) {
		if (wait_for_completion_timeout(&g_iodie_recovery_scene, msecs_to_jiffies(IODIE_RECOVERY_SCENE_TIMEOUT)) == 0) {
			pr_err("%s: wait for recovery scene timeout\n", __func__);
			return -EFAULT;
		}
	}
	return EOK;
}

static void iodie_err_handle(struct work_struct *work)
{
	int recovery_cnt = 0;
	int wait_cnt = 0;
	(void)work;

	for (; wait_cnt < IODIE_PCIE_MAX_WAIT_TIME; wait_cnt++) {
		if (iodie_wait_completion() == EOK) {
			break;
		}
		pr_err("%s: wait cnt %d\n", __func__, wait_cnt);
	}

	if (wait_cnt == IODIE_PCIE_MAX_WAIT_TIME) {
		pr_err("%s: wait fail.\n", __func__);
		return;
	}

	/* record error msg to RDR and trigger system error */
	pr_err("%s: io die pcie error, trigger rdr system\n", __func__);
	rdr_system_error(SENSORHUB_IODIE_MODID, 0, 0);

	for (; recovery_cnt < IODIE_PCIE_MAX_RECOVERY_TIME; recovery_cnt++) {
		if (iodie_commu(IODIE_CMD_RECOVERY, NULL, 0) == 0) {
			pr_info("%s: recovery success.\n ", __func__);
			return;
		}
		pr_err("%s: recovery fail %d.\n ", __func__, recovery_cnt);
		mdelay(10);
	}
}

static int iodie_status_notify_handler(const struct pkt_header *pkt)
{
	int i, ret;
	struct iodie_status_msg *data = (struct iodie_status_msg *)pkt;

	if (pkt == NULL || pkt->tag != TAG_IO_DIE || pkt->length < PCIE_STATUS_LEN) {
		pr_err("%s: invalid para\n", __func__);
		return -EINVAL;
	}

	pr_info("%s: tag:%x, cmd:%x, len:%x, recv status:%x\n", __func__, pkt->tag, pkt->cmd, pkt->length, data->status);
	ret = iodie_set_status(data->status);
	if (ret != EOK)
		return ret;

	pr_info("%s: curr status:%x\n", __func__, iodie_get_status());

	mutex_lock(&g_iodie_notify_lock);
	for (i = IODIE_USER_BEGIN; i < IODIE_USER_END; i++)
		if (g_iodie_user_func_list[i] != NULL)
			g_iodie_user_func_list[i](data->status);
	mutex_unlock(&g_iodie_notify_lock);

	if (data->status == IODIE_ST_INITED)
		queue_work(system_unbound_wq, &gpio_delay_worker);

	if (data->status == IODIE_ST_ERROR)
		queue_work(system_unbound_wq, &g_iodie_err_handle_worker);

	return EOK;
}

static void do_ao_gpio_late_register(struct work_struct *work)
{
	struct iodie_io_irq_info info;
	uint32_t i;
	(void)work;
	static uint8_t flag;

	if (flag) {
		pr_info("%s: completed before\n", __func__);
		return;
	}

	for (i = 0; i < (IODIE_AO_GPIO_END - IODIE_AO_GPIO_BEGIN); i++) {
		if (g_iodie_gpio_info[i].is_delay != 0) {
			info.gpio_id = g_iodie_gpio_info[i].gpio_id;
			info.irq_type = g_iodie_gpio_info[i].irq_type;
			if (iodie_commu(IODIE_CMD_GPIO_INTR_REGISTER, (void *)&info, sizeof(info)) != EOK)
				pr_err("%s: iodie commu err, gpio_id %u\n", __func__, info.gpio_id);
			pr_info("%s: gpio_id %u late register success\n", __func__, info.gpio_id);
		}
	}

	flag = 1;
}

int iodie_gpio_intr_register(enum iodie_ao_gpio gpio_id, enum irq_type type, iodie_gpio_intr_func func, void *data)
{
	struct iodie_io_irq_info info;
	uint32_t id;
	int ret;

	pr_info("%s: enter, gpio_id %u\n", __func__, gpio_id);
	if (gpio_id >= IODIE_AO_GPIO_END || type >= HI9030_IRQ_TYPE_UNKNOWN) {
		pr_err("%s:invalid gpio id %u, type %u\n", __func__, gpio_id, type);
		return -EINVAL;
	}

	if (func == NULL) {
		pr_err("%s: func is null, gpio_id %u\n", __func__, gpio_id);
		return -EINVAL;
	}

	id = gpio_id - IODIE_AO_GPIO_BEGIN;
	if (g_sys_st == IODIE_ST_PRE_INITED) {
		pr_info("%s: iodie not ready, delay process\n", __func__);
		g_iodie_gpio_info[id].is_delay = 1;
		ret = EOK;
		goto finish;
	}

	info.gpio_id = gpio_id;
	info.irq_type = type;
	ret = iodie_commu(IODIE_CMD_GPIO_INTR_REGISTER, (void *)&info, sizeof(info));
	if (ret != EOK) {
		pr_err("%s: iodie commu err, gpio_id %u\n", __func__, gpio_id);
		return ret;
	}

finish:
	g_iodie_gpio_info[id].gpio_id = gpio_id;
	g_iodie_gpio_info[id].irq_type = type;
	g_iodie_gpio_info[id].func = func;
	g_iodie_gpio_info[id].data = data;

	return ret;
}

int iodie_gpio_intr_unregister(enum iodie_ao_gpio gpio_id)
{
	uint32_t id;
	int ret;

	pr_info("%s: enter\n", __func__);
	if (gpio_id >= IODIE_AO_GPIO_END) {
		pr_err("%s:invalid gpio id %x\n", __func__, (uint32_t)gpio_id);
		return -EINVAL;
	}

	id = gpio_id;
	ret = iodie_commu(IODIE_CMD_GPIO_INTR_UNREGISTER, (void *)&id, sizeof(id));
	if (ret != EOK)
		return ret;

	id = gpio_id - IODIE_AO_GPIO_BEGIN;
	g_iodie_gpio_info[id].gpio_id = 0;
	g_iodie_gpio_info[id].func = NULL;
	g_iodie_gpio_info[id].data = NULL;

	return 0;
}

int iodie_gpio_enable(enum iodie_ao_gpio gpio_id)
{
	uint32_t id;

	if (gpio_id >= IODIE_AO_GPIO_END) {
		pr_err("%s:invalid gpio id %x\n", __func__, (uint32_t)gpio_id);
		return -EINVAL;
	}
	id = gpio_id;

	return iodie_commu(IODIE_CMD_GPIO_ENABLE, (void *)&id, sizeof(id));
}

static int iodie_ao_gpio_handler(const struct pkt_header *pkt)
{
	uint32_t id;
	struct iodie_gpio_msg *data = (struct iodie_gpio_msg *)pkt;

	if (pkt == NULL || pkt->tag != TAG_IO_DIE || pkt->length < PCIE_STATUS_LEN) {
		pr_err("%s: invalid para\n", __func__);
		return -EINVAL;
	}

	pr_info("%s: tag:%u, cmd:%u, len:%u, recv gpio:%u\n", __func__, pkt->tag, pkt->cmd, pkt->length, data->gpio_id);
	if (data->gpio_id >= IODIE_AO_GPIO_END) {
		pr_err("%s: invalid gpio id:%u\n", __func__, data->gpio_id);
		return -EINVAL;
	}

	id = data->gpio_id - IODIE_AO_GPIO_BEGIN;
	if (g_iodie_gpio_info[id].func != NULL)
		g_iodie_gpio_info[id].func((void *)(&id));

	return EOK;
}

int iodie_clk_out_sys(iodie_clk_out_cfg_enum op)
{
	uint32_t data;

	if (op >= IODIE_CLK_OUT_CMD_END) {
		pr_err("iodie_clk_out_sys: invalid para %u\n", op);
		return -EINVAL;
	}
	data = op;

	return iodie_commu(IODIE_CMD_CLK_OUT_CFG, (void *)&data, sizeof(uint32_t));
}

int check_iodie_link_status(void)
{
	struct device_node *node = NULL;
	static void __iomem *status_addr;
	uint32_t status;
	uint32_t reg_addr;
	uint32_t reg_size;
	int ret;

	if (status_addr == NULL) {
		node = of_find_compatible_node(NULL, NULL, "hisilicon,io-die");
		if (node == NULL) {
			pr_err("can not find hisilicon,io-die node !\n");
			return -EFAULT;
		}

		ret = of_property_read_u32_index(node, "iodie_status_reg", 0, &reg_addr);
		if (ret) {
			pr_err("%s failed to get reg addr! ret=%d\n", __func__, ret);
			return -EFAULT;
		}

		ret = of_property_read_u32_index(node, "iodie_status_reg", 1, &reg_size);
		if (ret) {
			pr_err("%s failed to get reg size! ret=%d\n", __func__, ret);
			return -EFAULT;
		}

		status_addr = ioremap(reg_addr, reg_size);
		if (status_addr ==  NULL) {
			pr_err("%s remap failed\n", __func__);
			return -EFAULT;
		}
	}

	status = readl(status_addr + IODIE_STATUS_OFFSET);
	if ((status & IODIE_STATUS_MASK) == 0) {
		pr_err("%s iodie link err, status 0x%x\n", __func__, status);
		return -EFAULT;
	}

	return EOK;
}

int iodie_sr_test(int threshold)
{
	return iodie_commu(IODIE_CMD_SR_TEST, (void *)&threshold, sizeof(int));
}

int iodie_i3c_stress_test(int threshold)
{
	return iodie_commu(IODIE_CMD_I3C_STRESS_TEST, (void *)&threshold, sizeof(int));
}


static void iodie_recovery_iom3_handler(void)
{
	pr_info("%s: iodie status [%d]", __func__, iodie_get_status());
}

static int iodie_recovery_notifier(struct notifier_block *nb,
	unsigned long foo, void *bar)
{
	pr_info("%s %lu +\n", __func__, foo);
	switch (foo) {
	case IOM3_RECOVERY_IDLE:
	case IOM3_RECOVERY_START:
	case IOM3_RECOVERY_MINISYS:
	case IOM3_RECOVERY_3RD_DOING:
	case IOM3_RECOVERY_FAILED:
		break;
	case IOM3_RECOVERY_DOING:
		iodie_recovery_iom3_handler();
		break;
	default:
		pr_err("%s -unknow state %lu\n", __func__, foo);
		break;
	}
	pr_info("%s -\n", __func__);
	return 0;
}

static struct notifier_block iodie_recovery_notify = {
	.notifier_call = iodie_recovery_notifier,
	.priority = -1,
};

static int __init iodie_init(void)
{
	int ret;

	ret = register_mcu_event_notifier(TAG_IO_DIE, IODIE_CMD_STATUS_NOTIFY, iodie_status_notify_handler);
	if (ret != 0) {
		pr_err( "register mcu event failed =%d\n", ret);
		return ret;
	}

	ret = register_mcu_event_notifier(TAG_IO_DIE, IODIE_CMD_GPIO_INTR_NOTIFY, iodie_ao_gpio_handler);
	if (ret != 0) {
		pr_err( "register mcu event failed =%d\n", ret);
		return ret;
	}

	register_iom3_recovery_notifier(REC_USR_IODIE, &iodie_recovery_notify);
	INIT_WORK(&gpio_delay_worker, do_ao_gpio_late_register);
	INIT_WORK(&g_iodie_err_handle_worker, iodie_err_handle);

	ret = get_soc_pcie1_reg_addr();
	if (ret != 0) {
		pr_err( "get pcie1 reg addr failed =%d\n", ret);
		return ret;
	}
	init_completion(&g_iodie_recovery_scene);

	pr_info("%s ok\n", __func__);
	g_l0_flag = -1;
	g_l0_jiffies = get_jiffies_64();
	pcie_data_reset();

	return 0;
}

static void __exit iodie_exit(void)
{
	unregister_mcu_event_notifier(TAG_IO_DIE, IODIE_CMD_STATUS_NOTIFY, iodie_status_notify_handler);
	unregister_mcu_event_notifier(TAG_IO_DIE, IODIE_CMD_GPIO_INTR_NOTIFY, iodie_ao_gpio_handler);
	pr_info("exit %s\n", __func__);
}

late_initcall_sync(iodie_init);
module_exit(iodie_exit);

