/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define inode and file security field management functions
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 22 15:09:52 CST 2023
 */
#include "types.h"
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <libsec/chk.h>
#include <libsec/component.h>
#include <libhmseharmony/avc.h>
#include <libhmseharmony/flask.h>
#include <libhmseharmony/perm_file.h>
#include <libhmseharmony/seharmony.h>
#include <libhmseharmony/services.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_io/type.h>
#include <sys/ioctl.h>
#include <devhost/file.h>
#include <devhost/device.h>
#include <devhost/security_seharmony.h>
#include "security_internal.h"
#include "seharmony.h"
#include "overlay.h"

static int seharmony_devhost_file_alloc(const struct sec_chk_cred *cred, struct filp_node *node)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	node->security.sel.fsid = seharmony_cred->sid;
	node->security.sel.vsid = 0;
	return E_HM_OK;
}

/**
 * struct seharmony_devhost_log_data - Data for audit log.
 * @devinfo:	the object's device
 * @node:	the object's filp_node
 * @type:	log type, HMSEL_AUDIT_FILE or HMSEL_AUDIT_IOCTL
 * @cmd:	ioctl cmd, for xperms check
 */
struct seharmony_devhost_log_data {
	const struct device_info *devinfo;
	const struct filp_node *node;
	unsigned int type;
	unsigned int cmd;
};

static void seharmony_devhost_audit_log_common(const struct seharmony_devhost_log_data *data,
					     struct hmsel_audit_info *ai)
{
	const struct device_info *devinfo = data->devinfo;
	size_t len = ai->len;
	int ret;

	if (unlikely(devinfo == NULL))
		return;

	if (devinfo->devname) {
		ret = snprintf_s(ai->buf, len, len - 1, " name=\"%s\" dev=\"%s\" major=%u minor=%u",
				 devinfo->devname, devinfo->devname, MAJOR(devinfo->p->devt),
				 MINOR(devinfo->p->devt));
		if (ret < 0) {
			hm_warn("devhost: seharmony: %s audit log create failed.\n", __func__);
			return;
		}
		ai_update(ret, ai);
	} else if (devhost_device_is_anonymous(devinfo)) {
		ret = snprintf_s(ai->buf, len, len - 1, " name=anon dev=anon ino=anon");
		if (ret < 0) {
			hm_warn("devhost: seharmony: %s audit log create failed.\n", __func__);
			return;
		}
		ai_update(ret, ai);
	}
}

static void seharmony_devhost_audit_log_ioctl(const struct seharmony_devhost_log_data *data,
					    struct hmsel_audit_info *ai)
{
	int ret;
	size_t len = ai->len;
	seharmony_devhost_audit_log_common(data, ai);
	ret = snprintf_s(ai->buf, len, len - 1, " ioctlcmd=0x%hx", (uint16_t)data->cmd);
	if (ret < 0) {
		hm_warn("devhost: seharmony: %s audit log create failed.\n", __func__);
		return;
	}
	ai_update(ret, ai);
}

static int seharmony_devhost_audit(const void *c, const void *args, struct hmsel_audit_info *ai)
{
	const struct seharmony_devhost_log_data *data = (const struct seharmony_devhost_log_data *)args;
	UNUSED(c);

	if (unlikely(ai == NULL)) {
		return E_HM_INVAL;
	}

	switch (data->type) {
	case HMSEL_AUDIT_FILE:
		seharmony_devhost_audit_log_common(data, ai);
		break;
	case HMSEL_AUDIT_IOCTL:
		seharmony_devhost_audit_log_ioctl(data, ai);
		break;
	default:
		break;
	}
	return E_HM_OK;
}

static inline bool is_device_info_sel_init(struct device_info_sec *devinfo_cred)
{
	return devinfo_cred->sel.initialized == DEVICE_INFO_SEL_INITIALIZED;
}

static inline bool is_devinfo_check_skip(struct filp_node *node)
{
	if (devhost_device_is_anonymous(node->devinfo)) {
		/* skip checking anonymous device */
		return true;
	}

	if (!is_device_info_sel_init(node->security.device_info_sec)) {
		/* skip checking when inode uninitialized */
		return true;
	}

	return false;
}

/**
 * @brief avc check file operations perms common
 *
 * @param cred sec_chk_cred
 * @param devinfo the device_info to be check, as linux inode
 * @param node the filp_node to be check, as linux file
 * @param ssid subject sid from process
 * @param tsid object sid from file's fsid or inode's sid
 * @param tclass always HM_SECCLASS_CHR_FILE in devhost fastpath device
 * @param av hmsel_av_t fops perm, like HM_FILE__OPEN
 * @param ltype audit type, HMSEL_AUDIT_FILE or HMSEL_AUDIT_IOCTL
 * @return int check result, E_HM_OK if is permitted.
 */
static int seharmony_audit_avc_perm(const struct sec_chk_cred *cred, struct device_info *devinfo,
				  struct filp_node *node, hmsel_sid_t ssid, hmsel_sid_t tsid,
				  hmsel_class_t tclass, hmsel_av_t av, unsigned int ltype)
{
	struct seharmony_devhost_log_data data = {
		.devinfo = devinfo,
		.node = node,
		.type = ltype,
	};
	struct hmsel_sid_context scon = {
		.ssid = ssid,
		.tsid = tsid,
		.tclass = tclass,
	};
	struct sec_sel_audit ad = {
		.func = seharmony_devhost_audit,
		.cred = cred,
		.data = &data,
	};

	return hmsel_avc_query(scon, av, &ad);
}

static int seharmony_devhost_file_opened(const struct sec_chk_cred *cred, struct filp_node *node)
{
	struct device_info *devinfo = node->devinfo;
	struct device_info_sec *devinfo_cred = node->security.device_info_sec;

	if (!is_device_info_sel_init(devinfo_cred)) {
		node->security.sel.vsid = 0;
		hm_warn("devhost: seharmony: open device=%s without seharmony check.\n", devinfo->devname);
		return E_HM_OK;
	}

	node->security.sel.vsid = devinfo_cred_read_sid(devinfo_cred);
	return E_HM_OK;
}

/**
 * check only file fsid
*/
static int seharmony_check_file_desc(const struct sec_chk_cred *cred, struct filp_node *node,
				   unsigned int ltype)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	hmsel_sid_t fsid = node->security.sel.fsid;

	if (seharmony_cred->sid == fsid) {
		return E_HM_OK;
	}

	return seharmony_audit_avc_perm(cred, node->devinfo, node, seharmony_cred->sid, fsid,
				      HM_SECCLASS_FD, HM_FD__USE, ltype);
}

/**
 * check both file fsid and vnode sid
*/
static int seharmony_check_file_slow(const struct sec_chk_cred *cred, struct filp_node *node,
				   hmsel_av_t req)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	struct device_info_sec *devinfo_cred = node->security.device_info_sec;
	int ret = seharmony_check_file_desc(cred, node, HMSEL_AUDIT_FILE);
	if (ret != E_HM_OK) {
		return ret;
	}

	if (is_devinfo_check_skip(node)) {
		return E_HM_OK;
	}

	return seharmony_audit_avc_perm(cred, node->devinfo, node, seharmony_cred->sid, devinfo_cred_read_sid(devinfo_cred),
				      devinfo_cred->sel.sclass, req, HMSEL_AUDIT_FILE);
}

/**
 * only for ioctl, check vnode sid
*/

/**
 * @brief avc check ioctl xperms, object using inode sid
 *
 * @param cred sec_chk_cred
 * @param devinfo the device_info to be check, as linux inode
 * @param node the filp_node to be check, as linux file
 * @param req hmsel_av_t perm, always HM_FILE__IOCTL
 * @param cmd ioctl cmd, using for check xperms
 * @return int check result, E_HM_OK if is permitted.
 */
static int seharmony_devhost_xperm_check(const struct sec_chk_cred *cred, struct device_info *devinfo,
				       struct filp_node *node, hmsel_av_t req, uint16_t cmd)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	struct device_info_sec *devinfo_cred = node->security.device_info_sec;
	uint8_t driver = cmd >> 8;
	uint8_t xperms = cmd & 0xff;
	struct seharmony_devhost_log_data data = {
		.devinfo = devinfo,
		.node = node,
		.type = HMSEL_AUDIT_IOCTL,
		.cmd = (unsigned int)cmd,
	};
	struct hmsel_sid_context scon = {
		.ssid = seharmony_cred->sid,
		.tsid = devinfo_cred_read_sid(devinfo_cred),
		.tclass = devinfo_cred->sel.sclass,
	};
	struct sec_sel_audit ad = {
		.func = seharmony_devhost_audit,
		.cred = cred,
		.data = &data,
	};

	return hmsel_avc_xperms_query(scon, req, driver, xperms, &ad);
}

static int _seharmony_devhost_check_ioctl(const struct sec_chk_cred *cred, struct filp_node *node,
					unsigned int cmd)
{
	int ret = seharmony_check_file_desc(cred, node, HMSEL_AUDIT_IOCTL);
	if (ret != E_HM_OK) {
		return ret;
	}

	if (is_devinfo_check_skip(node)) {
		return E_HM_OK;
	}

	return seharmony_devhost_xperm_check(cred, node->devinfo, node, HM_FILE__IOCTL, (uint16_t)cmd);
}

static int seharmony_devhost_check_ioctl(const struct sec_chk_cred *cred, struct filp_node *node,
				       unsigned int cmd, unsigned long arg)
{
	int ret = E_HM_OK;
	UNUSED(arg);

	switch (cmd) {
	case FIONBIO:
		ret = seharmony_check_file_desc(cred, node, HMSEL_AUDIT_FILE);
		break;
	case FIONREAD:
		ret = seharmony_check_file_slow(cred, node, HM_FILE__GETATTR);
		break;
	case FIOCLEX:
	case FIONCLEX:
		if (!hmsel_policycap_ioctl_skip_cloexec()) {
			ret = _seharmony_devhost_check_ioctl(cred, node, cmd);
		}
		break;
	default:
		ret = _seharmony_devhost_check_ioctl(cred, node, cmd);
		break;
	}
	return ret;
}

static int seharmony_devhost_check_fcntl(const struct sec_chk_cred *cred, struct filp_node *node,
				       int cmd, unsigned long arg)
{
	int ret = E_HM_OK;

	switch (cmd) {
	case F_SETFL:
		if ((node->f_flags & O_APPEND) != 0 && (arg & O_APPEND) == 0) {
			ret = seharmony_check_file_slow(cred, node, HM_FILE__WRITE);
		} else {
			ret = seharmony_check_file_desc(cred, node, HMSEL_AUDIT_FILE);
		}
		break;
	case F_GETFL:
		ret = seharmony_check_file_desc(cred, node, HMSEL_AUDIT_FILE);
		break;
	default:
		break;
	}
	return ret;
}

static int seharmony_devhost_check_file_cache(struct sec_chk_cred *cred, struct filp_node *node,
					    hmsel_av_t av)
{
	int ret = E_HM_OK;
	struct requester req = {0};
	struct device_info *devinfo = node->devinfo;
	struct device_info_sec *devinfo_cred = node->security.device_info_sec;

	if (is_devinfo_check_skip(node)) {
		return E_HM_OK;
	}

	ret = requester_init(&req);
	if (ret != E_HM_OK) {
		hm_error("active cred check failed to get the requester. (ret=%s)\n",
			 hmstrerror(ret));
		return ret;
	}

	if (base_cred_to_sel(req.fwd_base_cred)->sid == node->security.sel.fsid &&
	    node->security.sel.vsid == devinfo_cred_read_sid(devinfo_cred)) {
		return E_HM_OK;
	}

	if (is_cnode_idx_err(cred->cnode_idx)) {
		sec_chk_cocred_init(cred, req.cnode_idx, &req);
	}

	return seharmony_check_file_slow(cred, node, av);
}

static int seharmony_devhost_check_fread(struct sec_chk_cred *cred, struct filp_node *node)
{
	return seharmony_devhost_check_file_cache(cred, node, HM_FILE__READ);
}

static int seharmony_devhost_check_fwrite(struct sec_chk_cred *cred, struct filp_node *node)
{
	return seharmony_devhost_check_file_cache(cred, node,
						(node->f_flags & O_APPEND) ? HM_FILE__APPEND : HM_FILE__WRITE);
}

SEC_DEFINE_COMPONENT(devhost, seharmony,
	sec_define_chk_method(devhost_file_alloc, seharmony_devhost_file_alloc),
	sec_define_chk_method(devhost_file_opened, seharmony_devhost_file_opened),
	sec_define_chk_method(devhost_ioctl, seharmony_devhost_check_ioctl),
	sec_define_chk_method(devhost_fcntl, seharmony_devhost_check_fcntl),
	sec_define_active_credchk_chk_method(devhost_fread, seharmony_devhost_check_fread),
	sec_define_active_credchk_chk_method(devhost_fwrite, seharmony_devhost_check_fwrite)
);
#define ENABLE_SEC_MOD_SEHARMONY
#include <generated/hmsd/module/devhost/devhost.h>
