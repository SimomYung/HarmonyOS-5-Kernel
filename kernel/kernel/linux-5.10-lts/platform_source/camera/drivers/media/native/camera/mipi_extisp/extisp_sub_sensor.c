
#include "extisp_driver.h"

#define sensor_err(fmt, args...) \
do { \
	pr_err("[extisp_sensor err] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define sensor_info(fmt, args...) \
do { \
	pr_info("[extisp_sensor info] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define sensor_warn(fmt, args...) \
do { \
	pr_warn("[extisp_sensor warn] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

static int sub_sensor_get_dt_data(struct platform_device *pdev, sensor_data_ctrl_t *sub_sensor)
{
    struct device_node *of_node = NULL;
	sensor_info_t *sub_sensor_info = NULL;
    int ret = -1;
    const char *sensor_name = "null";

    if (!pdev || !sub_sensor)
		return -EINVAL;
    of_node = pdev->dev.of_node;

	sub_sensor_info = &sub_sensor->sensorinfo;
    if (!sub_sensor_info)
        return -EINVAL;

    ret = of_property_read_string(of_node, "sensor_name", &sensor_name);
    if (ret < 0) {
		sensor_err("get sensor name failed.");
		return ret;
	}
    ret = memcpy_s(sub_sensor_info->sensor_name, SENSOR_NAME_LEN_MAX,
        sensor_name, strlen(sensor_name));
    if (ret != 0) {
		sensor_err("memcpy_s fail");
		return ret;
	}
    sensor_info("get sensor name done, ret:%d, sensor name:%s,%s ",
        ret, sub_sensor_info->sensor_name, sensor_name);

    ret = of_property_read_u32(of_node, "position", &sub_sensor_info->position);
    if (ret < 0) {
		sensor_err("get sensor position failed.");
		return ret;
	}

    ret = of_property_read_u32(of_node, "sensor_idx", &sub_sensor->sensor_idx);
    if (ret < 0) {
		sensor_err("get sensor_idx failed.");
		return ret;
	}

    ret = of_property_read_u32(of_node, "id", &sub_sensor_info->id);
    if (ret < 0) {
		sensor_err("get sensor id failed.");
		return ret;
	}

    return 0;
}

static int extisp_sensor_platform_probe(struct platform_device* pdev)
{
	sensor_data_ctrl_t *sub_sensor = NULL;
	int ret = -1;

    sensor_info("enter.");
    if (pdev == NULL) {
		sensor_err("pdev is null.\n");
		goto fail_out;
	}
    sub_sensor = devm_kzalloc(&pdev->dev, sizeof(sensor_data_ctrl_t), GFP_KERNEL);
	if (!sub_sensor) {
		sensor_err("kzalloc des sub_sensor fail");
		goto fail_out;
	}

	sensor_info("start sub_sensor_get_dt_data.");
	ret = sub_sensor_get_dt_data(pdev, sub_sensor);
	if (ret < 0) {
		sensor_err("sensor dt data init fail");
		goto fail_out_free;
	}

	ret = get_power_setting_utils(&pdev->dev, &sub_sensor->power_group);
    if (ret) {
        sensor_err("get_power_setting fail.");
        goto fail_out_free;
    }

	sub_sensor->dev = &pdev->dev;
	sub_sensor->power_cnt = 0;

	platform_set_drvdata(pdev, sub_sensor);

	return 0;

fail_out_free:
    devm_kfree(&pdev->dev, sub_sensor);
fail_out:
    return ret;
}

static int extisp_sensor_platform_remove(struct platform_device* pdev)
{
	dev_info(&pdev->dev, "removing extisp_sensor");
	return 0;
}


static const struct of_device_id extisp_sensor_dt_match[] =
{
	{
		.compatible = "vendor,extisp_sensor",
	},
};

MODULE_DEVICE_TABLE(of, i2a_des_ser_dt_match);

static struct platform_driver extisp_sensor_platform_driver = {
	.driver = {
		.name           = "vendor,extisp_sensor",
		.owner          = THIS_MODULE,
		.of_match_table = extisp_sensor_dt_match,
	},

	.probe = extisp_sensor_platform_probe,
	.remove = extisp_sensor_platform_remove,
};

static int __init extisp_sensor_drv_init(void)
{
    sensor_info("enter.");
	return platform_driver_register(&extisp_sensor_platform_driver);
}

static void __exit extisp_sensor_drv_exit(void)
{
    platform_driver_unregister(&extisp_sensor_platform_driver);
}

module_init(extisp_sensor_drv_init);
module_exit(extisp_sensor_drv_exit);
MODULE_DESCRIPTION("external isp sub sensor driver");
MODULE_LICENSE("GPL v2");
