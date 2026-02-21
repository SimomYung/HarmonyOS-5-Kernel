/*
 * tsens.c
 *
 * tsensor module
 *
 * Copyright (c) 2017-2021 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/thermal.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <platform_include/basicplatform/linux/ipc_rproc.h>

#include <linux/compiler.h>
#include <platform_include/cee/linux/lpm_thermal.h>
#include <securec.h>
#include <thermal_core.h>
#include <trace/events/power.h>
#ifdef CONFIG_THERMAL_SUPPORT_FFA
#include <platform_include/see/ffa/ffa_plat_drv.h>
#endif
#if (defined CONFIG_TSENS_TEMP_SAPPHIRE) && (defined CONFIG_VENDOR_EFUSE)
#include <platform_include/see/efuse_driver.h>
#endif

#ifdef CONFIG_THERMAL_MNTN_DMD
#include <platform_include/cee/linux/thermal_dmd.h>
#endif

#define TSENS_DRIVER_NAME		"ithermal-tsens"
#define READTEMP_SVC_REG_RD		0xc5009900UL
/*
 * CAPACITY_OF_TSENSOR_ARRAY should not be less than "ithermal,sensors" in dtsi,
 * which is stored in g_tmdev->tsens_num_sensor.
 */
#define CAPACITY_OF_TSENSOR_ARRAY	10
#define MAX_SENSOR_NUM			40
#define CELSIUS_SCALING			1000
static struct tsens_tm_device *g_tmdev;
const char *g_tsensor_name[CAPACITY_OF_TSENSOR_ARRAY];
u32 g_tsensor_mode[CAPACITY_OF_TSENSOR_ARRAY];

/* 
 * level      interval     show
 * COLD     (-inf,    0)    -5
 * LOW      [   0,   40)    30
 * NORMAL   [  40,   70]    60
 * HIGH     [  70,  105]    80
 * CRITICAL [ 105, +inf]   125
 */
#define THERMAL_LEVEL_COLD_TEMP_SHOW		(-5)
#define THERMAL_LEVEL_COLD_LOW_THRESHOLD	0
#define THERMAL_LEVEL_LOW_TEMP_SHOW		30
#define THERMAL_LEVEL_LOW_NORMAL_THRESHOLD	40
#define THERMAL_LEVEL_NORMAL_TEMP_SHOW		60
#define THERMAL_LEVEL_NORMAL_HIGH_THRESHOLD	70
#define THERMAL_LEVEL_HIGH_TEMP_SHOW		80
#define THERMAL_LEVEL_HIGH_CRITICAL_THRESHOLD	105
#define THERMAL_LEVEL_CRITICAL_TEMP_SHOW	125

enum thermal_level {
	THERMAL_LEVEL_COLD,
	THERMAL_LEVEL_LOW,
	THERMAL_LEVEL_NORMAL,
	THERMAL_LEVEL_HIGH,
	THERMAL_LEVEL_CRITICAL,
	THERMAL_LEVEL_MAX = THERMAL_LEVEL_CRITICAL,
};

static int g_temp_level_table[THERMAL_LEVEL_MAX + 1] = {
	THERMAL_LEVEL_COLD_TEMP_SHOW,
	THERMAL_LEVEL_LOW_TEMP_SHOW,
	THERMAL_LEVEL_NORMAL_TEMP_SHOW,
	THERMAL_LEVEL_HIGH_TEMP_SHOW,
	THERMAL_LEVEL_CRITICAL_TEMP_SHOW,
};

static int g_temp_level_threshold_table[THERMAL_LEVEL_MAX] = {
	THERMAL_LEVEL_COLD_LOW_THRESHOLD,
	THERMAL_LEVEL_LOW_NORMAL_THRESHOLD,
	THERMAL_LEVEL_NORMAL_HIGH_THRESHOLD,
	THERMAL_LEVEL_HIGH_CRITICAL_THRESHOLD,
};

/* the tz in this list is a copy of other tz */
static const char *const g_sysfs_no_show_list[] = {
	"shell_thermal",
	"npu_thermal",
};
#ifdef trace_perf
/* the list is available in eng version */
static const char *const g_trace_ip_print_list[] = {
	"cluster0",
	"cluster1",
	"cluster2",
	"gpu",
	"npu",
};

/* shell trace is available in any case */
static const char *const g_trace_shell_print_list[] = {
	"shell_front",
	"shell_frame",
	"shell_back",
};
#endif

/* Trips: warm and cool */
enum tsens_trip_type {
	TSENS_TRIP_ORIGNUM = 0,
#ifdef CONFIG_THERMAL_TRIP
	TSENS_TRIP_THROTTLING = TSENS_TRIP_ORIGNUM,
	TSENS_TRIP_SHUTDOWN,
	TSENS_TRIP_BELOW_VR_MIN,
	TSENS_TRIP_NUM,
#endif
};

#ifndef CONFIG_THERMAL_TSENSOR_GET_TEMP
enum sensor_type {
	TYPE_TSENSOR = 0,
	TYPE_PVTSENSOR,
};
#endif

struct tsens_tm_device_sensor {
	struct thermal_zone_device *tz_dev;
	enum thermal_device_mode mode;
	int reg_no;
	unsigned int sensor_num;
	int sensor_type;
#ifdef CONFIG_THERMAL_TRIP
	s32 temp_throttling;
	s32 temp_shutdown;
	s32 temp_below_vr_min;
#endif
};

struct tsens_tm_device {
	struct platform_device *pdev;
	int tsens_num_sensor;
	bool tsens_level;
#ifndef CONFIG_THERMAL_TSENSOR_GET_TEMP
	u32 adc_start_value;
	u32 adc_end_value;
	u32 adc_delta_value;
	u32 pvtsensor_adc_start_value;
	u32 pvtsensor_adc_end_value;
	u32 adc_sup_start_value;
	u32 adc_sup_end_value;
	u32 adc_sup_delta_value;
	u32 adc_sup_start_value_v2;
	u32 adc_sup_end_value_v2;
	u32 adc_sup_delta_value_v2;
#endif
	struct tsens_tm_device_sensor sensor[0];
};

struct tsens_inf {
#ifndef CONFIG_THERMAL_TSENSOR_GET_TEMP
	unsigned int register_info;
#endif
	int tsens_num_sensors;
};

#ifndef CONFIG_THERMAL_TSENSOR_GET_TEMP
#define TSENSOR_8BIT			8
#define TSENS_THRESHOLD_MIN_CODE	0x0
#define TSENS_TEMP_START_VALUE		(-40) /* -40 deg C */
#define TSENS_TEMP_END_VALUE		125
#define TSENS_TEMP_START_VALUE_1	(-20) /* -40 deg C */
#define TSENS_TEMP_END_VALUE_1		105
#else
#define TSENS_TEMP_ERR		(-40)
#endif

int g_tsensor_debug = 0;

#if (defined CONFIG_TSENS_TEMP_SAPPHIRE) && (defined CONFIG_VENDOR_EFUSE)
#define        EFUSE_CHIP_VERSION_START     151
#define        EFUSE_CHIP_VERSION_SIZE      3
#define        CHIP_VERSION_MASK            0x7

static bool is_chip_ver2(void)
{
	u32 value = 0;
	u32 ret;
	struct efuse_desc efuse_desc;

	efuse_desc.buf = &value;
	efuse_desc.buf_size = 1;
	efuse_desc.start_bit = EFUSE_CHIP_VERSION_START;
	efuse_desc.bit_cnt = EFUSE_CHIP_VERSION_SIZE;
	efuse_desc.item_vid = EFUSE_KERNEL_DIEID_VALUE;

	ret = efuse_read_value_t(&efuse_desc);
	if (ret != 0)
		return false;

	value |= (value & CHIP_VERSION_MASK);
	if (value == 2)
		return true;
	return false;
}

#define VERSION_V120_VALUE_1            1
#define VERSION_V120_VALUE_2            2
enum TSENSOR_VERSION {
	TSENSOR_VERSION_0,
	TSENSOR_VERSION_1,
	TSENSOR_VERSION_2,
	TSENSOR_VERSION_MAX,
};

#define        EFUSE_TSENSOR_VER_START     159 // 1618 - EFUSE_REMOTE_TSENSOR_START
#define        EFUSE_TSENSOR_VER_SIZE      3

static enum TSENSOR_VERSION get_tsensor_version(void)
{
	enum TSENSOR_VERSION version;
	u32 value = 0;
	u32 ret;
	struct efuse_desc efuse_desc;

	efuse_desc.buf = &value;
	efuse_desc.buf_size = 1;
	efuse_desc.start_bit = EFUSE_TSENSOR_VER_START;
	efuse_desc.bit_cnt = EFUSE_TSENSOR_VER_SIZE;
	efuse_desc.item_vid = EFUSE_KERNEL_REMOTE_TSENSOR;

	ret = efuse_read_value_t(&efuse_desc);
	if (ret != EFUSE_OK)
		return TSENSOR_VERSION_2;

	value |= (value & CHIP_VERSION_MASK);
	switch (value) {
	case VERSION_V120_VALUE_1:
		version = TSENSOR_VERSION_1;
		break;
	case VERSION_V120_VALUE_2:
		version = TSENSOR_VERSION_2;
		break;
	default:
		version = TSENSOR_VERSION_2;
		break;
	}

	return version;
}
#endif

static bool check_tz_type(const char **whitelist,
			  unsigned int array_size,
			  char *type, int *indx)
{
	unsigned int i, str_len;

	for (i = 0; i < array_size; i++) {
		str_len = strlen(type);
		if ((str_len == strlen(whitelist[i])) &&
		    (strncmp(type, whitelist[i], str_len) == 0)) {
			if (indx != NULL)
				*indx = i;
			return true;
		}
	}
	if (indx != NULL)
		*indx = -1;

	return false;
}

static bool check_temperature_hidden(struct thermal_zone_device *tz)
{
	bool ret = false;

	if (!g_tmdev) {
		pr_err("%s: g_tmdev is not initialized!\n", __func__);
		return false;
	}
	if (check_tz_type((const char **)g_tsensor_name,
			  g_tmdev->tsens_num_sensor,
			  tz->type, NULL))
		ret = true;
	else if (check_tz_type((const char **)g_sysfs_no_show_list,
			       ARRAY_SIZE(g_sysfs_no_show_list),
			       tz->type, NULL))
		ret = true;

	return ret;
}

static bool check_temperature_no_show(struct thermal_zone_device *tz)
{
	bool ret = false;

	if (check_tz_type((const char **)g_sysfs_no_show_list,
			  ARRAY_SIZE(g_sysfs_no_show_list),
			  tz->type, NULL))
		ret = true;

	return ret;
}

static enum thermal_level get_temperature_level(struct thermal_zone_device *tz,
						int temperature)
{
	enum thermal_level level = THERMAL_LEVEL_NORMAL;

	if (tz->emul_temperature != 0)
		return level;
	for (level = THERMAL_LEVEL_COLD; level < THERMAL_LEVEL_MAX; level++)
		if (temperature < g_temp_level_threshold_table[level])
			break;

	return level;
}

/* used in thermal_sysfs.c to filter tzd temperature */
void thermal_update_level(struct thermal_zone_device *tz,
			  int *temperature)
{
#ifndef CONFIG_THERMAL_EMULATION
	enum thermal_level level;

	if (!tz || !temperature) {
		pr_err("%s: invalid input!\n", __func__);
		return;
	}
	if (!g_tmdev->tsens_level || !check_temperature_hidden(tz))
		return;
	if (check_temperature_no_show(tz)) {
		*temperature = 0;
	} else {
		level = get_temperature_level(tz, *temperature);
		*temperature = g_temp_level_table[level];
	}
#endif
}

void print_temp_trace(struct thermal_zone_device *tz, int temp)
{
#ifdef trace_perf
	char reg_temp[THERMAL_ZONE_NAME_LENGTH] = {0};
	int ret, print_temp = temp, indx = 0;
#ifndef CONFIG_THERMAL_EMULATION
	enum thermal_level level = THERMAL_LEVEL_NORMAL;
#endif

	if (!tz) {
		pr_err("%s: invalid input!\n", __func__);
		return;
	}
	/* soc temp */
	if (tz->is_soc_thermal) {
		ret = snprintf_s(reg_temp, sizeof(reg_temp),
				 (sizeof(reg_temp) - 1), "soc_temp");
#ifndef CONFIG_THERMAL_EMULATION
		if (g_tmdev->tsens_level) {
			level = get_temperature_level(tz, temp / CELSIUS_SCALING);
			print_temp = g_temp_level_table[level] * CELSIUS_SCALING;
		}
#endif
	/* ip temp */
	} else if (check_tz_type((const char **)g_trace_ip_print_list,
				  ARRAY_SIZE(g_trace_ip_print_list),
				  tz->type, &indx)) {
#ifndef CONFIG_THERMAL_EMULATION
		if (g_tmdev->tsens_level)
			return;
#endif
		ret = snprintf_s(reg_temp, sizeof(reg_temp), (sizeof(reg_temp) - 1),
				 "%s_temp", g_trace_ip_print_list[indx]);
	/* board temp */
	} else if (tz->is_board_thermal) {
		ret = snprintf_s(reg_temp, sizeof(reg_temp),
				 (sizeof(reg_temp) - 1), "board_temp");
	/* shell temp */
	} else if (check_tz_type((const char **)g_trace_shell_print_list,
				  ARRAY_SIZE(g_trace_shell_print_list),
				  tz->type, &indx)) {
		ret = snprintf_s(reg_temp, sizeof(reg_temp), (sizeof(reg_temp) - 1),
				 "%s_temp", g_trace_shell_print_list[indx]);
	} else {
		return;
	}
	if (ret < 0) {
		pr_err("%s: compose reg_temp fail!\n", __func__);
		return;
	}
	trace_perf(clock_set_rate, reg_temp, print_temp,
		   raw_smp_processor_id());
#endif
}

void tsen_debug(int onoff)
{
	g_tsensor_debug = onoff;
}

ssize_t trip_point_type_activate(struct device *dev,
				 struct device_attribute *attr, const char *buf,
				 size_t count)
{
	struct thermal_zone_device *tz = to_thermal_zone(dev);
	int trip = 0, result = 0;

	if (!tz->ops->activate_trip_type)
		return -EPERM;

	if (!sscanf_s(attr->attr.name, "trip_point_%d_type", &trip))
		return -EINVAL;

	if (!strncmp(buf, "enabled", strlen("enabled"))) {
		result =
			tz->ops->activate_trip_type(tz, trip, THERMAL_TRIP_ACTIVATION_ENABLED);
	} else if (!strncmp(buf, "disabled", strlen("disabled"))) {
		result =
			tz->ops->activate_trip_type(tz, trip, THERMAL_TRIP_ACTIVATION_DISABLED);
	} else {
		result = -EINVAL;
	}
	if (result)
		return result;
	return count;
}

#ifndef CONFIG_THERMAL_TSENSOR_GET_TEMP
static int tsens_tz_code_to_degc(int adc_val, int sensor_type)
{
	int temp = 0;
	int adc_start_value, adc_end_value, adc_delta_value;

	if (g_tmdev == NULL)
		goto ERR_OR;

	adc_delta_value = 0;
	if (sensor_type == TYPE_TSENSOR) {
		adc_start_value = (int)g_tmdev->adc_start_value;
		adc_end_value = (int)g_tmdev->adc_end_value;
		adc_delta_value = (int)g_tmdev->adc_delta_value;
	} else if (sensor_type == TYPE_PVTSENSOR) {
		adc_start_value = (int)g_tmdev->pvtsensor_adc_start_value;
		adc_end_value = (int)g_tmdev->pvtsensor_adc_end_value;
	} else {
		adc_start_value = 0;
		adc_end_value = 0;
	}

	if (adc_start_value > adc_val || adc_val > adc_end_value ||
	    adc_start_value == 0 || adc_end_value == 0)
		goto ERR_OR;
	temp = ((adc_val - adc_start_value) *
		(TSENS_TEMP_END_VALUE - TSENS_TEMP_START_VALUE)) /
		(adc_end_value - adc_start_value) - adc_delta_value;
	temp = temp + TSENS_TEMP_START_VALUE;
#ifdef CONFIG_THERMAL_TSENS_TEMP_V2
	temp = ((adc_val - adc_start_value) *
		(TSENS_TEMP_END_VALUE_1 - TSENS_TEMP_START_VALUE_1)) /
		(adc_end_value - adc_start_value) - adc_delta_value;
	temp = temp + TSENS_TEMP_START_VALUE_1;
#endif
#if (defined CONFIG_TSENS_TEMP_SAPPHIRE) && (defined CONFIG_VENDOR_EFUSE)
	if (is_chip_ver2()) {
		adc_start_value = (int)g_tmdev->adc_sup_start_value;
		adc_end_value = (int)g_tmdev->adc_sup_end_value;
		adc_delta_value = (int)g_tmdev->adc_sup_delta_value;

		temp = ((adc_val - adc_start_value) *
		(TSENS_TEMP_END_VALUE_1 - TSENS_TEMP_START_VALUE_1)) /
		(adc_end_value - adc_start_value) - adc_delta_value;
		temp = temp + TSENS_TEMP_START_VALUE_1;
	}
#endif
	return temp;

ERR_OR:
	pr_debug("adc_to_temp_conversion failed\n");
	return temp;
}
#endif

#ifdef CONFIG_THERMAL_SUPPORT_FFA
static int ffa_tsens_sync_request(u64 _function_id, u64 _arg0, u64 _arg1, u64 _arg2)
{
	int ret;
	struct ffa_send_direct_data args = {
		.data0 = (unsigned long)_function_id,
		.data1 = (unsigned long)_arg0,
		.data2 = (unsigned long)_arg1,
		.data3 = (unsigned long)_arg2,
	};

	ret = ffa_platdrv_send_msg(&args);
	return (int)args.data1;
}
#endif
/*lint -e715*/
noinline int atfd_readtemp_smc(u64 _function_id, u64 _arg0, u64 _arg1, u64 _arg2)
{
#ifdef CONFIG_THERMAL_SUPPORT_FFA
	return ffa_tsens_sync_request(_function_id, _arg0, _arg1, _arg2);
#else
	register u64 function_id asm("x0") = _function_id;
	register u64 arg0 asm("x1") = _arg0;
	register u64 arg1 asm("x2") = _arg1;
	register u64 arg2 asm("x3") = _arg2;
	asm volatile (
		__asmeq("%0", "x0")
		__asmeq("%1", "x1")
		__asmeq("%2", "x2")
		__asmeq("%3", "x3")
#ifndef CONFIG_LIBLINUX
		"smc #0\n"
#else
		"svc #0x4200\n"
#endif
		: "+r" (function_id)
		: "r" (arg0), "r" (arg1), "r" (arg2));

	return (int)function_id;
#endif
}

/*lint +e715*/

int sec_readtemp_read(u64 reg_address)
{
	return atfd_readtemp_smc(READTEMP_SVC_REG_RD, reg_address, 0UL, 0UL);
}

#ifndef CONFIG_THERMAL_TSENSOR_GET_TEMP
static int tsens_tz_get_temp(struct thermal_zone_device *thermal,
			     int *temp)
{
	struct tsens_tm_device_sensor *tm_sensor = thermal->devdata;
	int adc_value = 0;

	if (tm_sensor == NULL || tm_sensor->mode != THERMAL_DEVICE_ENABLED ||
	    temp == NULL)
		return -EINVAL;

	if (tm_sensor->reg_no >= 0 && tm_sensor->reg_no < g_tmdev->tsens_num_sensor) {
		adc_value = sec_readtemp_read(tm_sensor->reg_no);
		*temp = tsens_tz_code_to_degc(adc_value, tm_sensor->sensor_type);
		print_temp_trace(thermal, *temp);
	} else {
		*temp = TSENS_TEMP_START_VALUE;
		#if (defined CONFIG_TSENS_TEMP_SAPPHIRE) && (defined CONFIG_VENDOR_EFUSE)
			if (is_chip_ver2())
				*temp = TSENS_TEMP_START_VALUE_1;
		#endif
	}

	return 0;
}
#else
static int tsens_tz_get_temp(struct thermal_zone_device *thermal,
			     int *temp)
{
	struct tsens_tm_device_sensor *tm_sensor = thermal->devdata;

	if (tm_sensor == NULL || tm_sensor->mode != THERMAL_DEVICE_ENABLED ||
	    temp == NULL)
		return -EINVAL;

	if (tm_sensor->reg_no >= 0 && tm_sensor->reg_no < g_tmdev->tsens_num_sensor) {
		*temp = sec_readtemp_read(tm_sensor->reg_no);
		print_temp_trace(thermal, *temp);
	} else {
		*temp = TSENS_TEMP_ERR;
	}

	return 0;
}
#endif

/* add for IPA */
int tsens_get_temp(u32 sensor, int *temp)
{
	int i;
	struct thermal_zone_device *thermal = NULL;
	int ret = -EINVAL;
	int tmp = 0;

	for (i = 0; i < g_tmdev->tsens_num_sensor; i++) {
		if (sensor == g_tmdev->sensor[i].sensor_num) {
			thermal = g_tmdev->sensor[i].tz_dev;
			ret = tsens_tz_get_temp(thermal, &tmp);
			if (tmp < 0)
				tmp = 0;
			*temp = tmp * CELSIUS_SCALING;
		}
	}

	return ret;
}

int ipa_get_tsensor_id(const char *name)
{
	int ret = -ENODEV;
	int id;

	if (name == NULL) {
		pr_err("%s:name == NULL!\n", __func__);
		return ret;
	}

	pr_debug("IPA tsensor_num = %d\n", g_tmdev->tsens_num_sensor);
	for (id = 0; id < g_tmdev->tsens_num_sensor; id++) {
		pr_debug("IPA: sensor_name = %s, g_tsensor_name %d = %s\n",
			 name, id, g_tsensor_name[id]);

		if (strncmp(name, g_tsensor_name[id],
			    strlen(g_tsensor_name[id])) == 0) {
			ret = id;
			pr_debug("sensor_id = %d\n", ret);
			return ret;
		}
	}

#ifdef CONFIG_THERMAL_SHELL
	if (strncmp(name, IPA_SENSOR_SHELL, strlen(IPA_SENSOR_SHELL)) == 0)
		ret = IPA_SENSOR_SHELLID;
#endif

	return ret;
}
EXPORT_SYMBOL_GPL(ipa_get_tsensor_id);

int ipa_get_sensor_value(u32 sensor, int *val)
{
	int ret = -EINVAL;
	u32 sensor_num = sizeof(g_tsensor_name) / sizeof(char *);

	if (val == NULL) {
		pr_err("%s parm null\n", __func__);
		return ret;
	}

	if (sensor < sensor_num)
		ret = tsens_get_temp(sensor, val);

	return ret;
}
EXPORT_SYMBOL_GPL(ipa_get_sensor_value);
/* end of add for IPA */

static int tsens_tz_get_trip_type(struct thermal_zone_device *thermal,
				  int trip, enum thermal_trip_type *type)
{
	struct tsens_tm_device_sensor *tm_sensor = thermal->devdata;
	int ret = 0;

	if (tm_sensor == NULL || trip < 0 || type == NULL)
		return -EINVAL;

	switch (trip) {
#ifdef CONFIG_THERMAL_TRIP
	case TSENS_TRIP_THROTTLING:
		*type = THERMAL_TRIP_THROTTLING;
		break;
	case TSENS_TRIP_SHUTDOWN:
		*type = THERMAL_TRIP_SHUTDOWN;
		break;
	case TSENS_TRIP_BELOW_VR_MIN:
		*type = THERMAL_TRIP_BELOW_VR_MIN;
		break;
#endif
	default:
		ret = -EINVAL;
	}

	return ret;
}

/*lint -e764 -e527 -esym(764,527,*)*/
static int tsens_tz_get_trip_temp(struct thermal_zone_device *thermal,
				  int trip, int *temp)
{
	struct tsens_tm_device_sensor *tm_sensor = thermal->devdata;
	int ret = 0;

	if (tm_sensor == NULL || trip < 0 || temp == NULL)
		return -EINVAL;

	switch (trip) {
#ifdef CONFIG_THERMAL_TRIP
	case TSENS_TRIP_THROTTLING:
		*temp = tm_sensor->temp_throttling;
		break;
	case TSENS_TRIP_SHUTDOWN:
		*temp = tm_sensor->temp_shutdown;
		break;
	case TSENS_TRIP_BELOW_VR_MIN:
		*temp = tm_sensor->temp_below_vr_min;
		break;
#endif
	default:
		ret = -EINVAL;
	}

	return ret;
}

/*lint -e764 -e527 +esym(764,527,*)*/

static struct thermal_zone_device_ops tsens_thermal_zone_ops = {
	.get_temp = tsens_tz_get_temp,
	.get_trip_type = tsens_tz_get_trip_type,
	.get_trip_temp = tsens_tz_get_trip_temp,
};

#ifndef CONFIG_THERMAL_TSENSOR_GET_TEMP
static void get_sup_tsens_inf(struct platform_device *pdev)
{
	int rc;
	struct tsens_inf inf;
	const struct device_node *of_node = pdev->dev.of_node; /*lint !e578*/

	g_tmdev->adc_sup_start_value = g_tmdev->adc_start_value;
	g_tmdev->adc_sup_end_value = g_tmdev->adc_end_value;
	g_tmdev->adc_sup_delta_value = g_tmdev->adc_delta_value;
	g_tmdev->adc_sup_start_value_v2 = g_tmdev->adc_start_value;
	g_tmdev->adc_sup_end_value_v2 = g_tmdev->adc_end_value;
	g_tmdev->adc_sup_delta_value_v2 = g_tmdev->adc_delta_value;
	rc = of_property_read_u32(of_node, "ithermal,tsensor_adc_sup_start_value", &(inf.register_info));
	if (rc == 0)
		g_tmdev->adc_sup_start_value = inf.register_info;
	rc = of_property_read_u32(of_node, "ithermal,tsensor_adc_sup_end_value", &(inf.register_info));
	if (rc == 0)
		g_tmdev->adc_sup_end_value = inf.register_info;
	rc = of_property_read_u32(of_node, "ithermal,tsensor_adc_sup_delta_value", &(inf.register_info));
	if (rc == 0)
		g_tmdev->adc_sup_delta_value = inf.register_info;
	rc = of_property_read_u32(of_node, "ithermal,tsensor_adc_sup_start_value_v2", &(inf.register_info));
	if (rc == 0)
		g_tmdev->adc_sup_start_value_v2 = inf.register_info;
	rc = of_property_read_u32(of_node, "ithermal,tsensor_adc_sup_end_value_v2", &(inf.register_info));
	if (rc == 0)
		g_tmdev->adc_sup_end_value_v2 = inf.register_info;
	rc = of_property_read_u32(of_node, "ithermal,tsensor_adc_sup_delta_value_v2", &(inf.register_info));
	if (rc == 0)
		g_tmdev->adc_sup_delta_value_v2 = inf.register_info;
#if (defined CONFIG_TSENS_TEMP_SAPPHIRE) && (defined CONFIG_VENDOR_EFUSE)
	if (is_chip_ver2() && (get_tsensor_version() == TSENSOR_VERSION_2)) {
		g_tmdev->adc_sup_start_value = g_tmdev->adc_sup_start_value_v2;
		g_tmdev->adc_sup_end_value = g_tmdev->adc_sup_end_value_v2;
		g_tmdev->adc_sup_delta_value = g_tmdev->adc_sup_delta_value_v2;
	}
#endif
}
#endif

static int get_device_tree_data(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	const struct device_node *of_node = pdev->dev.of_node; /*lint !e578*/
	struct tsens_inf inf = {0};
	int i, rc, j = 0, tsensor_level = 0;

	rc = of_property_read_s32(of_node, "ithermal,sensors", &(inf.tsens_num_sensors)); /* parse tsensor number */
	if (rc != 0) {
		dev_err(&pdev->dev, "missing sensor number\n");
		return -ENODEV;
	}
	g_tmdev = devm_kzalloc(&pdev->dev, sizeof(struct tsens_tm_device) + inf.tsens_num_sensors *
			       sizeof(struct tsens_tm_device_sensor), GFP_KERNEL);
	if (g_tmdev == NULL) {
		dev_err(&pdev->dev, "kzalloc() failed.\n");
		return -ENOMEM;
	}
	g_tmdev->tsens_num_sensor = inf.tsens_num_sensors;

	for (i = 0; i < inf.tsens_num_sensors; i++) {
		rc = of_property_read_string_index(of_node, "ithermal,tsensor_name", i, &g_tsensor_name[i]);
		if (rc != 0) {
			pr_err("%s g_tsensor_name %d read err\n", __func__, i);
			return -EINVAL;
		}
	}
	rc = of_property_read_u32_array(of_node, "ithermal,tsensor_mode", g_tsensor_mode, inf.tsens_num_sensors);
	if (rc != 0) {
		pr_err("%s g_tsensor_mode read err\n", __func__);
		return -EINVAL;
	}
	rc = of_property_read_u32(of_node, "ithermal,tsensor_level", &tsensor_level);
	if (rc != 0 || tsensor_level == 0) {
		g_tmdev->tsens_level = false;
	} else if (tsensor_level == 1) {
		g_tmdev->tsens_level = true;
	} else {
		pr_err("%s tsensor_level read err\n", __func__);
		return -EINVAL;
	}
#ifndef CONFIG_THERMAL_TSENSOR_GET_TEMP
	rc = of_property_read_u32(of_node, "ithermal,tsensor_adc_start_value", &(inf.register_info));
	if (rc != 0) {
		dev_err(dev, "no ithermal,tsensor_adc_start_value!\n");
		return -EINVAL;
	}
	g_tmdev->adc_start_value = inf.register_info;
	rc = of_property_read_u32(of_node, "ithermal,tsensor_adc_end_value", &(inf.register_info));
	if (rc != 0) {
		dev_err(dev, "no ithermal,tsensor_adc_end_value!\n");
		return -EINVAL;
	}
	g_tmdev->adc_end_value = inf.register_info;
	rc = of_property_read_u32(of_node, "ithermal,tsensor_adc_delta_value", &(inf.register_info));
	if (rc != 0) {
		dev_err(dev, "no ithermal,tsensor_adc_delta_value!\n");
		g_tmdev->adc_delta_value = 0;
	} else {
		g_tmdev->adc_delta_value = inf.register_info;
	}
	rc = of_property_read_u32(of_node, "ithermal,pvtsensor_adc_start_value", &(inf.register_info));
	if (rc != 0) {
		dev_err(dev, "no ithermal,pvtsensor_adc_start_value!\n");
		g_tmdev->pvtsensor_adc_start_value = 0;
	} else {
		g_tmdev->pvtsensor_adc_start_value = inf.register_info;
	}
	rc = of_property_read_u32(of_node, "ithermal,pvtsensor_adc_end_value", &(inf.register_info));
	if (rc != 0) {
		dev_err(dev, "no ithermal,pvtsensor_adc_end_value!\n");
		g_tmdev->pvtsensor_adc_end_value = 0;
	} else {
		g_tmdev->pvtsensor_adc_end_value = inf.register_info;
	}
	get_sup_tsens_inf(pdev);
#endif

#ifdef CONFIG_THERMAL_MNTN_DMD
	parse_temp_dmd_parameter(of_node);
#endif

	g_tmdev->pdev = pdev;
	return 0;
}

static int tsens_tm_probe(struct platform_device *pdev)
{
	int rc = 0;

	if (g_tmdev != NULL) {
		dev_err(&pdev->dev, "TSENS device already in use\n");
		return -EBUSY;
	}

	if (pdev->dev.of_node != NULL) {
		rc = get_device_tree_data(pdev);
		if (rc != 0) {
			dev_err(&pdev->dev, "Error reading TSENS DT\n");
			return rc;
		}
	} else {
		return -ENODEV;
	}

	platform_set_drvdata(pdev, g_tmdev);
	return 0;
}

static void get_detect_tsensor(struct platform_device *pdev, int i)
{
	struct device_node *node = pdev->dev.of_node;
	char reg_no_name[MAX_SENSOR_NUM] = {0};
	char sensor_type_name[MAX_SENSOR_NUM] = {0};
	int reg_no = 0;
	int sensor_type = 0;
	int rc;

	rc = snprintf_s(reg_no_name, sizeof(reg_no_name),
			(sizeof(reg_no_name) - 1),
			"ithermal,detect_%s_regno", g_tsensor_name[i]);
	if (rc < 0)
		dev_err(&pdev->dev, "snprintf_s for regno fail!\n");
	rc = of_property_read_s32(node, reg_no_name, &reg_no);
	if (rc != 0) {
		dev_err(&pdev->dev, "%s node not found!\n", reg_no_name);
		reg_no = -1;
	}
	g_tmdev->sensor[i].reg_no = reg_no;

	(void)memset_s(sensor_type_name, sizeof(sensor_type_name),
		       0, sizeof(sensor_type_name));
	rc = snprintf_s(sensor_type_name, sizeof(sensor_type_name),
			(sizeof(sensor_type_name) - 1),
			"ithermal,detect_%s_sensortype",
			g_tsensor_name[i]);
	if (rc < 0)
		dev_err(&pdev->dev, "snprintf_s for sensortype fail!\n");
	rc = of_property_read_s32(node, sensor_type_name, &sensor_type);
	if (rc != 0) {
		dev_err(&pdev->dev, "%s node not found, use default type!\n",
			sensor_type_name);
		sensor_type = 0; /* default tsensor */
	}
	g_tmdev->sensor[i].sensor_type = sensor_type;
}

#ifdef CONFIG_THERMAL_TRIP
static void get_node_property(struct platform_device *pdev, int i,
			      struct device_node *np)
{
	s32 temp_throttling, temp_shutdown, temp_below_vr_min;
	int rc;

	rc = of_property_read_s32(np, "temp_throttling",
				  &temp_throttling);
	if (rc != 0) {
		dev_err(&pdev->dev, "temp_throttling node not found!\n");
		temp_throttling = 0;
	}
	g_tmdev->sensor[i].temp_throttling = temp_throttling;

	rc = of_property_read_s32(np, "temp_shutdown",
				  &temp_shutdown);
	if (rc != 0) {
		dev_err(&pdev->dev, "temp_shutdown node not found!\n");
		temp_shutdown = 0;
	}
	g_tmdev->sensor[i].temp_shutdown = temp_shutdown;

	rc = of_property_read_s32(np, "temp_below_vr_min",
				  &temp_below_vr_min);
	if (rc != 0) {
		dev_err(&pdev->dev, "temp_below_vr_min node not found!\n");
		temp_below_vr_min = 0;
	}
	g_tmdev->sensor[i].temp_below_vr_min = temp_below_vr_min;
}
#endif

static int _tsens_register_thermal(void)
{
	int rc, i, j;
	unsigned int trips_num = 0;
#ifdef CONFIG_THERMAL_TRIP
	struct device_node *np = NULL;
	char node_name[30] = {0};
#endif

	if (g_tmdev == NULL) {
		pr_err("TSENS early init not done!\n");
		return -ENODEV;
	}

	for (i = 0, j = 0; i < g_tmdev->tsens_num_sensor; i++, j++) {
		unsigned int mask = 0;

		g_tmdev->sensor[i].mode = g_tsensor_mode[i];
		g_tmdev->sensor[i].sensor_num = (unsigned int)i;
		get_detect_tsensor(g_tmdev->pdev, i);
#ifdef CONFIG_THERMAL_TRIP
		(void)memset_s((void *)node_name, sizeof(node_name), 0,
			       sizeof(node_name));
		rc = snprintf_s(node_name, sizeof(node_name),
				(sizeof(node_name) - 1), "ithermal_tsens_%s",
				g_tsensor_name[i]);
		if (rc < 0)
			dev_err(&g_tmdev->pdev->dev, "snprintf_s for nodename fail!\n");
		np = of_find_node_by_name(g_tmdev->pdev->dev.of_node, node_name);
		if (np != NULL) {
			get_node_property(g_tmdev->pdev, i, np);
			of_node_put(np);
			trips_num = TSENS_TRIP_NUM;
		} else {
			trips_num = TSENS_TRIP_ORIGNUM;
		}
#else
		trips_num = TSENS_TRIP_ORIGNUM;
#endif
		mask |= (unsigned int)((1 << trips_num) - 1);

		g_tmdev->sensor[i].tz_dev =
			thermal_zone_device_register(g_tsensor_name[i],
						     trips_num, (int)mask,
						     &g_tmdev->sensor[i],
						     &tsens_thermal_zone_ops,
						     NULL, 0, 0);
		if (IS_ERR(g_tmdev->sensor[i].tz_dev)) {
			dev_err(&g_tmdev->pdev->dev, "Tsensor \
				thermal_zone_device_register failed\n");
			rc = -ENODEV;
			goto register_fail;
		}
	}

	platform_set_drvdata(g_tmdev->pdev, g_tmdev);

	return 0;

register_fail:
	for (i = 0; i < j; i++)
		thermal_zone_device_unregister(g_tmdev->sensor[i].tz_dev);
	return rc;
}

static int tsens_tm_remove(struct platform_device *pdev)
{
	int i;
	struct tsens_tm_device *tmdev = platform_get_drvdata(pdev);

	if (tmdev == NULL)
		return -1;

	for (i = 0; i < tmdev->tsens_num_sensor; i++)
		thermal_zone_device_unregister(tmdev->sensor[i].tz_dev);

	platform_set_drvdata(pdev, NULL);

	return 0;
}

static struct of_device_id tsens_match[] = {
	{ .compatible = "ithermal,tsens", },
	{}
};

static struct platform_driver tsens_tm_driver = {
	.probe = tsens_tm_probe,
	.remove = tsens_tm_remove,
	.driver = {
		.name = "ithermal-tsens",
		.owner = THIS_MODULE,
		.of_match_table = tsens_match,
	},
};

static int __init tsens_tm_init_driver(void)
{
	return platform_driver_register(&tsens_tm_driver);
}
arch_initcall(tsens_tm_init_driver);

static int __init tsens_thermal_register(void)
{
	return _tsens_register_thermal();
}
module_init(tsens_thermal_register);

static void __exit _tsens_tm_remove(void)
{
	platform_driver_unregister(&tsens_tm_driver);
}
module_exit(_tsens_tm_remove);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("thermal tsens module driver");
