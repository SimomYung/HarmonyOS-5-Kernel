/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi debug fs helpers
 *
 * This file is released under the GPLv2.
 */
#include <linux/debugfs.h>
#include <linux/delay.h>

struct file_and_data {
	const char *filename;
	umode_t mode;
	uint64_t *data;
};

static uint64_t sleep_seconds = 1;
static uint64_t use_kmalloc = 1;
static uint64_t log_reg;
static uint64_t log_i2c;
static uint64_t log_i2a;
static uint64_t csi_int;
static uint64_t sengen;
static uint64_t i2a_timeout = 500;
static uint64_t i2c_reg;
static uint64_t i2c_val;
static uint64_t color_bar_pat;
static uint64_t vc_num = 4;

static struct file_and_data i2a_files[] = {
	{ "sleep_seconds", 0600, &sleep_seconds },
	{ "use_kmalloc",   0600, &use_kmalloc },
	{ "log_reg",       0600, &log_reg },
	{ "log_i2c",       0600, &log_i2c },
	{ "log_i2a",       0600, &log_i2a },
	{ "color_bar_pat", 0600, &color_bar_pat },
	{ "i2a_timeout",   0600, &i2a_timeout },
	{ "i2c_reg",       0600, &i2c_reg },
	{ "csi_int",       0600, &csi_int },
	{ "sengen",        0600, &sengen },
	{ "vc_num",        0600, &vc_num },
};

#ifdef CONFIG_CAM_IDI2AXI_DEBUG

void sleep_here_wait(void)
{
	uint32_t sleep = sleep_seconds;

	while (sleep--) {
		pr_info("idi2axi sleeping, %u seconds left", sleep);
		ssleep(1);
		if (sleep > sleep_seconds)
			sleep = sleep_seconds;
	}
	pr_info("idi2axi sleeping done");
}

bool log_reg_rw(void)
{
	return log_reg != 0;
}

bool log_i2c_rw(void)
{
	return log_i2c != 0;
}

bool log_i2a_status(void)
{
	return log_i2a != 0;
}

bool csi_int_enable(void)
{
	return csi_int != 0;
}

bool sengen_enabled(void)
{
	return sengen != 0;
}

bool use_kmalloc_mem(void)
{
	return use_kmalloc != 0;
}

bool single_vc(void)
{
	return vc_num == 1;
}

uint16_t get_color_bar_pat(void)
{
	return color_bar_pat;
}

uint64_t get_i2a_timeout(void)
{
	return i2a_timeout;
}

int i2a_debugfs_init(void)
{
	uint32_t i;
	struct dentry *d = NULL;
	static bool debugfs_created;

	if (debugfs_created)
		return 0;

	d = debugfs_create_dir("i2a_debugfs", NULL);
	if (!d) {
		pr_err("failed to create i2a debugfs dir!\n");
		return -ENOMEM;
	}
	for (i = 0; i < ARRAY_SIZE(i2a_files); ++i)
		(void)debugfs_create_x64(i2a_files[i].filename,
				i2a_files[i].mode, d, i2a_files[i].data);

	debugfs_created = true;
	return 0;
}

#else /* !CONFIG_CAM_IDI2AXI_DEBUG */

void sleep_here_wait(void)
{
}

bool use_kmalloc_mem(void)
{
	return false;
}

bool log_reg_rw(void)
{
	return false;
}

bool log_i2c_rw(void)
{
	return false;
}

bool log_i2a_status(void)
{
	return false;
}

bool csi_int_enable(void)
{
	return false;
}

bool sengen_enabled(void)
{
	return false;
}

bool single_vc(void)
{
	return false;
}

uint16_t get_color_bar_pat(void)
{
	return 0;
}

uint64_t get_i2a_timeout(void)
{
	return 0;
}

int i2a_debugfs_init(void)
{
	return 0;
}

#endif /* !CONFIG_CAM_IDI2AXI_DEBUG */
