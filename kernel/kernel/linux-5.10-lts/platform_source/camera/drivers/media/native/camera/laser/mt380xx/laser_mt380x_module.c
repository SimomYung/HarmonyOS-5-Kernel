/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: laser mt380x driver
 * Create: 2025-05-16
 */

#include "laser_mt380x_module.h"
#include "hw_laser_dmd.h"

#define DRIVER_NAME "vendor,mt380x"

static laser_module_intf_t laser_devices[] = {
	{ "MT380X", &mt380x_probe, &mt380x_remove, &mt380x_ioctl },
};

static int laser_index = -1;

static long hw_laser_mt380x_ioctl(void *hw_data, unsigned int cmd, void *p)
{
	int rc = 0;

	if (!hw_data) {
		laser_err("parameter error");
		return -EINVAL;
	}

	if (laser_index >= 0 && laser_index < ARRAY_SIZE(laser_devices))
		rc = laser_devices[laser_index].laser_ioctl(hw_data, cmd, p);

	return rc;
}

static int laser_data_remove(struct i2c_client *client)
{
	int rc = 0;

	if (!client) {
		laser_err("parameter error");
		return -EINVAL;
	}

	if (laser_index >= 0 && laser_index < ARRAY_SIZE(laser_devices))
		rc = laser_devices[laser_index].data_remove(client);

	return rc;
}

static int laser_data_init(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	/* try sub devices */
	int rc = 0;
	int i = 0;

	if (!client || !id) {
		laser_err("parameter error");
		return -EINVAL;
	}

	for (i = 0; i < ARRAY_SIZE(laser_devices); i++) {
		rc = laser_devices[i].data_init(client, id);
		if (rc == 0)
			break;
	}

	if (i == ARRAY_SIZE(laser_devices)) {
		report_laser_probe_state(HWLASER_MT380X, PROBE_FAIL);
		laser_err("No mt laser probe success");
		return -1;
	}

	laser_index = i;
	laser_info("probed success sensor index = %d", i);
	return 0;
}

hw_laser_fn_t laser_mt380x_module_fn = {
	.laser_ioctl = hw_laser_mt380x_ioctl,
};

static hw_laser_ctrl_t laser_mt380x_module_ctrl = {
	.func_tbl = &laser_mt380x_module_fn,
	.data = NULL,
};

static const struct i2c_device_id laser_mt380x_module_id[] = {
	{ "mt380x",  (unsigned long)&laser_mt380x_module_ctrl},
	{ }, /* sentinel */
};

MODULE_DEVICE_TABLE(i2c, laser_mt380x_module_id);

static const struct of_device_id laser_mt380x_module_of_match[] = {
	{ .compatible = "vendor,mt380x", },
	{ }, /* sentinel */
};
MODULE_DEVICE_TABLE(of, laser_mt380x_module_of_match);

static int laser_mt380x_module_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int rc = 0;
	int match_times = 3;

	if (!client || !id)
		return -EINVAL;

	laser_info("module probe enter");
	do {
		match_times--;
		rc = laser_data_init(client, id);
	} while (rc != 0 && match_times > 0);
	if (rc == 0) {
		laser_info("laser_data_init success");
		rc = laser_probe(client, id);
	}

	return rc;
}

static int laser_mt380x_module_remove(struct i2c_client *client)
{
	int rc = 0;

	if (!client)
		return -EINVAL;

	laser_info("module remove enter");
	rc = laser_data_remove(client);
	if (rc == 0)
		rc = laser_remove(client);
	return rc;
}

static struct i2c_driver laser_mt380x_module_i2c_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = of_match_ptr(laser_mt380x_module_of_match),
	},
	.probe = laser_mt380x_module_probe,
	.remove = laser_mt380x_module_remove,
	.id_table = laser_mt380x_module_id,
};

static int __init laser_mt380x_module_init(void)
{
	laser_info("%s enter\n", __func__);
	return i2c_add_driver(&laser_mt380x_module_i2c_driver);
}

static void __exit laser_mt380x_module_exit(void)
{
	laser_info("%s enter", __func__);
	i2c_del_driver(&laser_mt380x_module_i2c_driver);
}

module_init(laser_mt380x_module_init);
module_exit(laser_mt380x_module_exit);
MODULE_DESCRIPTION("laser MT380X driver");
MODULE_LICENSE("GPL v2");
