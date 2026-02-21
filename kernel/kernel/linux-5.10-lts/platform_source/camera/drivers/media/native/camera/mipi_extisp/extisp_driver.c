#include <linux/dma-buf.h>
#include <linux/kconfig.h>
#include <linux/kthread.h>
#include <linux/videodev2.h>
#include <linux/timekeeping.h>
#include "cam_intf.h"
#include <linux/regulator/consumer.h>
#include <linux/gpio.h>
#include <linux/pm_wakeup.h>
#include <dsm/dsm_pub.h>

#include "extisp_driver.h"

#define extisp_err(fmt, args...) \
do { \
	pr_err("[external_isp err] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define extisp_info(fmt, args...) \
do { \
	pr_info("[external_isp info] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define extisp_warn(fmt, args...) \
do { \
	pr_warn("[external_isp warn] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define sd_to_extisp_ctrl(i) container_of(i, extisp_data_ctrl_t, subdev)

static int g_external_camera_ref = 0;
static struct mutex g_mutex_ref;

exitisp_reg_ops_t *get_xc8132_reg_ops(void);
exitisp_reg_ops_t *get_fic7609_reg_ops(void);

#define LOG_BUF_SIZE 512
#ifdef CONFIG_HISYSEVENT
struct error_no_map {
	int error_no;
	char *name;
};
static struct error_no_map g_extisp_error_no_map[] = {
	{DSM_FLASH_OPEN_SHOTR_ERROR_NO, "CAMERA_FLASH_SHORT_OR_CIRCUIT"},
};
static int extisp_errorno_to_str(int error_no, char *str, int buff_len)
{
	int i;
	int extisp_error_no_map_len = sizeof(g_extisp_error_no_map) / sizeof(struct error_no_map);
	if (str == NULL) {
		extisp_err("[E]str is null\n");
		return -EINVAL;
	}
	for (i = 0; i < extisp_error_no_map_len; i++) {
		if (g_extisp_error_no_map[i].error_no == error_no) {
			if (strlen(g_extisp_error_no_map[i].name) >= buff_len) {
				extisp_err("[E]length error\n");
				return -EINVAL;
			}
			if (strncpy_s(str, buff_len, g_extisp_error_no_map[i].name, strlen(g_extisp_error_no_map[i].name)) != EOK) {
				extisp_err("[E]cpy error\n");
				return -EINVAL;
			}
			str[buff_len - 1] = '\0';
			extisp_info("[I]dsm extisp_errorno_to_str str=%s\n", str);
			return 0;
		}
	}
	return -EINVAL;
}
static struct dsm_client_ops hisi_dsm_ops_extisp = {
	.poll_state = NULL,
	.dump_func = NULL,
	.errorno_to_str = extisp_errorno_to_str,
};
#endif
static struct dsm_dev dev_extisp = {
#ifdef CONFIG_HISYSEVENT
	.name = "CAM_DRV",
#else
	.name = "dsm_extisp",
#endif
	.device_name = "extisp_subdev",
	.ic_name = "extisp",
	.module_name = NULL,
#ifdef CONFIG_HISYSEVENT
    .fops = &hisi_dsm_ops_extisp,
#else
	.fops = NULL,
#endif
	.buff_size = LOG_BUF_SIZE,
};

static void extisp_dsm_client_notify(extisp_data_ctrl_t *ctrl, int dsm_id, const char *fmt, ...)
{
	va_list ap;
	int size, retry = 2;
	char buf[LOG_BUF_SIZE] = {0};

	extisp_info("Enter");
	if (ctrl == NULL )
		return;
 
	if (!ctrl->dsm_c) {
		extisp_err("dsm_c is null ptr");
		return;
	}

	if (fmt != NULL) {
		va_start(ap, fmt);
		size = vsnprintf_s(buf, LOG_BUF_SIZE, LOG_BUF_SIZE - 1, fmt, ap);
		va_end(ap);
		if (size < 0) {
			extisp_err("buf copy failed");
			return;
		}
	}

	while (retry > 0) {
		if (!dsm_client_ocuppy(ctrl->dsm_c)) {
			dsm_client_record(ctrl->dsm_c, buf);
			dsm_client_notify(ctrl->dsm_c, dsm_id);
			extisp_info("report dmd success: %d",dsm_id);
			break;
		}
		dsm_client_unocuppy(ctrl->dsm_c);
		retry--;
	}
	if (retry == 0)
		extisp_err("dsm client ocuppy, dsm notify failed, dsm_id=%d", dsm_id);
}

static int do_external_isp_config_power(
    uint32_t *ppower_cnt,
    int powerup_or_down,
    struct power_setting_group *ppower_group)
{
    int ret = -1;

    if (!ppower_cnt || !ppower_group)
        return -EINVAL;

    if (powerup_or_down) {
        if (*ppower_cnt != 0) {
            extisp_info("powered before cnt:%u", (*ppower_cnt));
            ++(*ppower_cnt);
            return 0;
        }
        ret = enable_power_setting_utils(ppower_group);
        if (ret) {
            extisp_err("power up fail");
            return ret;
        }
        ++(*ppower_cnt);
        extisp_info("power up success. cnt:%u", *ppower_cnt);
    } else {
        if (*ppower_cnt == 0) {
            extisp_info("was not powered before cnt:%u", *ppower_cnt);
            return 0;
        }

        if (*ppower_cnt > 1) {
            extisp_info("powered before cnt:%u", *ppower_cnt);
            --(*ppower_cnt);
            return 0;
        }
        extisp_info("powered cnt:%u", *ppower_cnt);
        disable_power_setting_utils(ppower_group);
        --(*ppower_cnt);
        ret = 0;
        extisp_info( "power off success. cnt:%u", *ppower_cnt);
    }

    return ret;
}

static sensor_data_ctrl_t *get_sensor_data_ctrl(extisp_data_ctrl_t *ctrl, int sensor_idx)
{
    struct platform_device *sensor_dev = NULL;
    sensor_data_ctrl_t *sensor_ctrl = NULL;
    if (!ctrl) {
        extisp_err("ctrl null.");
        return NULL;
    }

    if (sensor_idx < 0 || sensor_idx >= ctrl->sensor_num) {
        extisp_err("Invalid sensor index.");
        return NULL;
    }

    sensor_dev = ctrl->sensor_pdev[sensor_idx];
    if (!sensor_dev) {
        return NULL;
    }
    sensor_ctrl = platform_get_drvdata(sensor_dev);
    if (!sensor_ctrl) {
        return NULL;
    }

    return sensor_ctrl;
}

static int external_isp_config_power(extisp_data_ctrl_t *ctrl, void *data)
{
    int ret = -1;
    extisp_cfg_data_t *cdata = NULL;
    extisp_power_cfg_t *power_cfg = NULL;

    extisp_info("enter");

    if (!ctrl || !data)
        return -EINVAL;

    cdata = (extisp_cfg_data_t *)data;
	power_cfg = &cdata->cfg.power_cfg;
    if (!power_cfg)
        return -EINVAL;

    extisp_info("cfg_type:%d, powerup_or_down:%d .", power_cfg->cfg_type, power_cfg->powerup_or_down);
    if (power_cfg->cfg_type == CFG_TYPE_ISP) {
        extisp_info("CFG_TYPE_ISP, power_cnt:%d.", ctrl->power_cnt);
        ret = do_external_isp_config_power(&ctrl->power_cnt, power_cfg->powerup_or_down, &ctrl->power_group);
        if (ret == 0) {
            if (ctrl->power_cnt == 1 && power_cfg->powerup_or_down == 1) {
                __pm_stay_awake(ctrl->power_ws);
            } else if (ctrl->power_cnt == 0 && power_cfg->powerup_or_down == 0) {
                __pm_relax(ctrl->power_ws);
            }
        }
    } else if (power_cfg->cfg_type == CFG_TYPE_SENSOR) {
        sensor_data_ctrl_t *sensor_ctrl = get_sensor_data_ctrl(ctrl, power_cfg->parameter);
        if (!sensor_ctrl) return -EINVAL;
        extisp_info("CFG_TYPE_SENSOR, power_cnt:%d,sensor_id:%d .",
                 sensor_ctrl->power_cnt, power_cfg->parameter);
        ret = do_external_isp_config_power(&sensor_ctrl->power_cnt, power_cfg->powerup_or_down, &sensor_ctrl->power_group);
    }

    if (ret < 0) {
        extisp_dsm_client_notify(ctrl, DSM_FLASH_OPEN_SHOTR_ERROR_NO,
            "mipi external isp power-%d fail, ret:%d.", power_cfg->powerup_or_down, ret);
    }
    return ret;
}

static int external_isp_rw_des_reg(extisp_data_ctrl_t *ctrl, void *data)
{
    int ret = -1;
    extisp_cfg_data_t *cdata = NULL;
    extisp_reg_rw_cfg_t *reg_cfg = NULL;
    extisp_info("enter");

    if (!ctrl || !data)
        return -EINVAL;

    cdata = (extisp_cfg_data_t *)data;
    reg_cfg = &cdata->cfg.reg_rw_cfg;
    if (!reg_cfg)
        return -EINVAL;

    if (ctrl->extisp_reg_ops &&
        ctrl->extisp_reg_ops->rw_des_reg &&
        ctrl->i2c_c)
        ret = ctrl->extisp_reg_ops->rw_des_reg(ctrl->i2c_c, reg_cfg);

    if (ret < 0) {
        extisp_dsm_client_notify(ctrl, DSM_FLASH_OPEN_SHOTR_ERROR_NO,
            "mipi external isp i2c transfer error, ret:%d.", ret);
    }
    return ret;
}

static int external_isp_get_sensor_info(extisp_data_ctrl_t *ctrl, void *data)
{
    int i = -1;
    sensor_data_ctrl_t *sensor_ctrl = NULL;
    extisp_cfg_data_t *cdata = NULL;
    extisp_sensor_info_list_t *sensorinfo_list = NULL;
    extisp_info("enter");

    if (!ctrl || !data)
        goto get_sensor_info_done;

    cdata = (extisp_cfg_data_t *)data;
	sensorinfo_list = &cdata->cfg.sensorinfo_list;
    if (!sensorinfo_list) {
        extisp_err("sensorinfo_list is null.");
		goto get_sensor_info_done;
    }

    sensorinfo_list->num = ctrl->sensor_num;

    for (i = 0; i < sensorinfo_list->num; i++) {
        sensor_ctrl = get_sensor_data_ctrl(ctrl, i);
        if (!sensor_ctrl) continue;
        sensorinfo_list->info[i] = sensor_ctrl->sensorinfo;
    }

get_sensor_info_done:
    extisp_info("exit");
    return 0;
}

static int external_isp_get_isp_index(extisp_data_ctrl_t *ctrl, void *data)
{
    extisp_cfg_data_t *cdata = NULL;

    if (!ctrl || !data)
        return -EINVAL;

    cdata = (extisp_cfg_data_t *)data;
	cdata->index = ctrl->isp_idx;
    extisp_info("isp_idx:%d", cdata->index);

    return 0;
}

static int i2a_extisp_config(extisp_data_ctrl_t *ctrl, void *argp)
{
	int ret = 0;
	extisp_cfg_data_t *data = NULL;

	if (!ctrl || !argp)
		return -EINVAL;

	data = (extisp_cfg_data_t *)argp;
	extisp_info("i2a extisp cfgtype = %d", data->cfgtype);
	switch (data->cfgtype) {
		case EXTISP_CONFIG_POWER:
			ret = external_isp_config_power(ctrl, argp);
			break;
		case EXTISP_CONFIG_RW_DES_REG:
			ret = external_isp_rw_des_reg(ctrl, argp);
			break;
		case EXTISP_CONFIG_GET_SENSOR_INFO:
			ret = external_isp_get_sensor_info(ctrl, argp);
			break;
        case EXTISP_CONFIG_GET_ISP_INDEX:
			ret = external_isp_get_isp_index(ctrl, argp);
			break;
		default:
			extisp_err("%s cfgtype %d is error", __func__, data->cfgtype);
			break;
	}
	extisp_info("%s exit %d", __func__, ret);
	return ret;
}

static int external_isp_release(extisp_data_ctrl_t *ctrl)
{
    int ret = -1, i = -1;
    sensor_data_ctrl_t *sensor_ctrl = NULL;
    extisp_info("enter");

    if (!ctrl)
	    return -EINVAL;

    mutex_lock(&g_mutex_ref);
    g_external_camera_ref--;
    extisp_info("release cnt[%d]", g_external_camera_ref);
    if (g_external_camera_ref > 0) {
        mutex_unlock(&g_mutex_ref);
        return EOK;
    }

    if (ctrl->isp_idx == ISP_IDX_XXW) {
        // isp power down.
        while (ctrl->power_cnt) {
            ret = do_external_isp_config_power(&ctrl->power_cnt, 0, &ctrl->power_group);
            if (ret == 0 && ctrl->power_cnt == 0)
                __pm_relax(ctrl->power_ws);
            else
                break; // Exit the loop if an error occurs
        }

        // sensor power down.
        for (i = 0; i < ctrl->sensor_num; i++) {
            sensor_ctrl = get_sensor_data_ctrl(ctrl, i);
            if (!sensor_ctrl) continue;
            while (sensor_ctrl->power_cnt) {
                ret = do_external_isp_config_power(&sensor_ctrl->power_cnt, 0, &sensor_ctrl->power_group);
                if (ret != 0)
                    break; // Exit the loop if an error occurs
            }
        }
    } else if (ctrl->isp_idx == ISP_IDX_YHW) {
        // isp reset
        do_isp_rst_after_release(&ctrl->power_group);

        // sensor power down.
        for (i = 0; i < ctrl->sensor_num; i++) {
            sensor_ctrl = get_sensor_data_ctrl(ctrl, i);
            if (!sensor_ctrl) continue;
            while (sensor_ctrl->power_cnt) {
                ret = do_external_isp_config_power(&sensor_ctrl->power_cnt, 0, &sensor_ctrl->power_group);
                if (ret != 0)
                    break; // Exit the loop if an error occurs
            }
        }

        // isp power down.
        while (ctrl->power_cnt) {
            ret = do_external_isp_config_power(&ctrl->power_cnt, 0, &ctrl->power_group);
            if (ret == 0 && ctrl->power_cnt == 0)
                __pm_relax(ctrl->power_ws);
            else
                break; // Exit the loop if an error occurs
        }
    }

    mutex_unlock(&g_mutex_ref);

    extisp_info("exit");
    return ret;
}

static long mipi_extisp_subdev_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	long rc = -1;
	extisp_data_ctrl_t *ctrl = NULL;

	if (!sd) {
		extisp_err("sd is NULL\n");
		return -EINVAL;
	}

	ctrl = sd_to_extisp_ctrl(sd);
	if (!ctrl) {
		extisp_err("ctrl is NULL\n");
		return -EINVAL;
	}
	extisp_info("extisp cmd = %x", cmd);
	switch (cmd) {
		case HWSENSOR_IOCTL_EXTISP_CFG:
			rc = i2a_extisp_config(ctrl, arg);
			break;
		default:
			extisp_err("invalid IOCTL CMD = %x\n", cmd);
			break;
	}
	return rc;
}

static int mipi_extisp_subdev_internal_close(struct v4l2_subdev *sd,
	struct v4l2_subdev_fh *fh)
{
	int rc = 0;
	extisp_data_ctrl_t *ctrl = NULL;
	if (!sd) {
		extisp_err("sd is NULL\n");
		return -EINVAL;
	}
	ctrl = sd_to_extisp_ctrl(sd);
	if (!ctrl) {
		extisp_err(" get extisp_data_ctrl_t error");
		return -EINVAL;
	}

	rc = external_isp_release(ctrl);
	if (rc < 0)
		extisp_err(" external_isp_release failed with error code %d", rc);
	else
		extisp_info(" enter %s external_isp_release successful!return value %d",__func__, rc);
	return rc;
}

static int mipi_extisp_subdev_open(
	struct v4l2_subdev *sd,
	struct v4l2_subdev_fh *fh)
{
	mutex_lock(&g_mutex_ref);
	g_external_camera_ref++;
	extisp_info("open cnt[%d]", g_external_camera_ref);
	if (g_external_camera_ref > 1) {
		mutex_unlock(&g_mutex_ref);
		return EOK;
	}
	mutex_unlock(&g_mutex_ref);
	return 0;
}

static int mipi_extisp_subdev_close(
	struct v4l2_subdev *sd,
	struct v4l2_subdev_fh *fh)
{
	mipi_extisp_subdev_internal_close(sd, fh);
	return 0;
}

static struct v4l2_subdev_core_ops s_subdev_core_ops_mipi_extisp = {
	.ioctl = mipi_extisp_subdev_ioctl,
};

static struct v4l2_subdev_ops s_subdev_ops_mipi_extisp = {
	.core = &s_subdev_core_ops_mipi_extisp,
};

static struct v4l2_subdev_internal_ops s_subdev_internal_ops_mipi_extisp = {
	.open  = mipi_extisp_subdev_open,
	.close = mipi_extisp_subdev_close,
};

static int extisp_get_dt_data(
    struct platform_device *pdev,
    extisp_data_ctrl_t *ctrl)
{
    struct device_node *of_node = NULL;
    struct i2c_client *i2c_c = NULL;
    struct device_node *np_i2c = NULL;
    struct device_node *np_sensor = NULL;
    int ret, i = -1;

    if (!pdev || !ctrl)
        return -EINVAL;

    of_node = pdev->dev.of_node;
    ret = of_property_read_string(of_node, "huawei,extisp_name", &ctrl->extisp_name);
    if (ret < 0) {
	    extisp_err("get extisp name failed.");
	    return ret;
    }
    ret = of_property_read_u32(of_node, "huawei,isp_idx", &ctrl->isp_idx);
    if (ret < 0) {
	    extisp_err("get isp_idx failed.");
	    return ret;
    }
    ret = of_property_read_u32(pdev->dev.of_node, "ispidx_gpiono", &ctrl->isp_index_gpiono);
    if (ret < 0) {
        extisp_err("get ispidx_gpiono fail");
        return ret;
    }

    // get i2c device
    np_i2c = of_parse_phandle(of_node, "i2c_client", 0);
    if (!np_i2c) { // probably this sensor dev don't use i2c
		extisp_err("OF no i2c_client property");
		return 0;
    }
    i2c_c = of_find_i2c_device_by_node(np_i2c);
    if (!i2c_c) {
	    extisp_err("OF i2c client find failed");
	    return -1;
    }
    ctrl->i2c_c = i2c_c;

    // get sensor device
    for (i = 0; i < SENSOR_NUM_MAX; i++) {
        np_sensor = of_parse_phandle(of_node, "sensor_device", i);
        if(!np_sensor) {
            extisp_info("get np_sensor-%d end, sensor_num:%d.", i, ctrl->sensor_num);
            break;
        }
        ctrl->sensor_pdev[i] = of_find_device_by_node(np_sensor);
        extisp_info("sensor_pdev[%d]", i);
        ctrl->sensor_num++;
    }

    return 0;
}

static int extisp_chip_matchid(
    struct platform_device *pdev,
    extisp_data_ctrl_t *ctrl)
{
    int ret = -1;
    extisp_info("enter.");

    // get extisp reg ops.
    if (ctrl->isp_idx == ISP_IDX_XXW)
        ctrl->extisp_reg_ops = get_xc8132_reg_ops();
    else if (ctrl->isp_idx == ISP_IDX_YHW)
        ctrl->extisp_reg_ops = get_fic7609_reg_ops();

    ret = get_power_setting_utils(&pdev->dev, &ctrl->power_group);
    if (ret) {
        extisp_err("get_power_setting fail.");
        return ret;
    }

    if (ctrl->extisp_reg_ops == NULL)
        return -EINVAL;

    // match isp id.
   if (ctrl->extisp_reg_ops->match_id) {
        // isp power up
        ret = do_external_isp_config_power(&ctrl->power_cnt, 1, &ctrl->power_group);
        if (ret != 0) {
            extisp_err("%s power up fail.", ctrl->extisp_name);
            goto out_free_power_settings;
        }

        // isp match id.
        ret = ctrl->extisp_reg_ops->match_id(ctrl);
        if (ret != 0) {
            extisp_err("%s match id fail.", ctrl->extisp_name);
            extisp_dsm_client_notify(ctrl, DSM_FLASH_OPEN_SHOTR_ERROR_NO,
                "mipi external isp match id fail, ret:%d.", ret);
            goto out_isp_powerdown;
        }

        // isp power off.
        ret = do_external_isp_config_power(&ctrl->power_cnt, 0, &ctrl->power_group);
        if (ret != 0) {
            extisp_err("%s power down fail.", ctrl->extisp_name);
        }
    }
    else {
        extisp_err("%s no match id func.", ctrl->extisp_name);
    }

    return 0;

out_isp_powerdown:
    do_external_isp_config_power(&ctrl->power_cnt, 0, &ctrl->power_group);
out_free_power_settings:
    put_power_setting_utils(&pdev->dev, &ctrl->power_group);
    return ret;
}

static int external_isp_platform_probe(struct platform_device* pdev)
{
    extisp_data_ctrl_t *ctrl = NULL;
    struct v4l2_subdev *subdev = NULL;
    int ret = -1;

    extisp_info("enter.");
    if (pdev == NULL) {
	    extisp_err("pdev is null.\n");
	    goto fail_out;
    }
    ctrl = devm_kzalloc(&pdev->dev, sizeof(extisp_data_ctrl_t), GFP_KERNEL);
    if (!ctrl) {
	    dev_err(&pdev->dev, "kzalloc des ctrl fail");
	    goto fail_out;
    }

    ret = extisp_get_dt_data(pdev, ctrl);
    if (ret < 0) {
	    dev_err(&pdev->dev,"extisp dt data init fail");
	    goto fail_out_free;
    }

    ret = extisp_chip_matchid(pdev, ctrl);
    if (ret != 0) {
        extisp_err("%s extisp_chip_matchid fail.", ctrl->extisp_name);
        goto fail_out_free;
    }

    subdev = &ctrl->subdev;
    mutex_init(&ctrl->lock);
    v4l2_subdev_init(subdev, &s_subdev_ops_mipi_extisp);
    subdev->internal_ops = &s_subdev_internal_ops_mipi_extisp;
    ret = memcpy_s(&subdev->name[0], sizeof(subdev->name),
		    ctrl->extisp_name, strlen(ctrl->extisp_name));
    if (ret != 0) {
	    extisp_err("memcpy_s fail");
	    goto fail_out_free;
    }
    subdev->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
    v4l2_set_subdevdata(subdev, ctrl);
    init_subdev_media_entity(subdev, CAM_SUBDEV_MIPI_EXTISP);
    cam_cfgdev_register_subdev(subdev, CAM_SUBDEV_MIPI_EXTISP);
    subdev->devnode->lock = &ctrl->lock;
    cam_dev_create(&pdev->dev, &ctrl->cam_dev_num);

    extisp_info("start fill ctrl.");
    ctrl->extisp_pdev = pdev;
    ctrl->power_cnt = 0;

    ctrl->dsm_c = dsm_register_client(&dev_extisp);

    ctrl->power_ws = wakeup_source_register(&pdev->dev, "extisp_power_wakelock");
    if (!ctrl->power_ws) {
		extisp_err("wakeup source register failed");
		return -1;
	}
    platform_set_drvdata(pdev, ctrl);

    return 0;

fail_out_free:
    devm_kfree(&pdev->dev, ctrl);
fail_out:
    return ret;
}

static int external_isp_platform_remove(struct platform_device* pdev)
{
    extisp_data_ctrl_t *ctrl = NULL;
	dev_info(&pdev->dev, "removing  ext isp device");
    ctrl = platform_get_drvdata(pdev);
    if (ctrl && ctrl->dsm_c) {
		dsm_unregister_client(ctrl->dsm_c, &dev_extisp);
		ctrl->dsm_c = NULL;
	}
    devm_kfree(&pdev->dev, ctrl);
	return 0;
}


static const struct of_device_id external_isp_dt_match[] =
{
	{
		.compatible = "vendor,external_isp",
	},
	{
	},
};

MODULE_DEVICE_TABLE(of, external_isp_dt_match);

static struct platform_driver external_isp_platform_driver = {
	.driver = {
		.name           = "vendor,external_isp",
		.owner          = THIS_MODULE,
		.of_match_table = external_isp_dt_match,
	},

	.probe = external_isp_platform_probe,
	.remove = external_isp_platform_remove,
};

static int __init external_isp_drv_init(void)
{
	extisp_info("enter ..");
	return platform_driver_register(&external_isp_platform_driver);
}

static void __exit external_isp_drv_exit(void)
{
    platform_driver_unregister(&external_isp_platform_driver);
}

module_init(external_isp_drv_init);
module_exit(external_isp_drv_exit);
MODULE_DESCRIPTION("external isp driver");
MODULE_LICENSE("GPL v2");
