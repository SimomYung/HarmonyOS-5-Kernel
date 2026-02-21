/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "session_manager.h"
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/spinlock_types.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <asm/cacheflush.h>
#include <linux/kthread.h>
#include <linux/atomic.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/thread_info.h>
#include <linux/completion.h>

#include <securec.h>
#include "smc_smp.h"
#include "mem.h"
#include "gp_ops.h"
#include "teek_client_constants.h"
#include "client_hash_auth.h"
#include "mailbox_mempool.h"
#include "tc_client_driver.h"
#include "internal_functions.h"
#include "dynamic_ion_mem.h"
#include "ko_adapt.h"
#include "teek_app_load.h"
#include "tc_freeze.h"
#include "tc_ns_log.h"
#include "tc_current_info.h"
#include "tc_linux_def.h"
#include "auth_path_impl.h"
#include "agent.h"

#ifdef CONFIG_LIBLINUX
#define VENDOR_PATH "/vendor/"
#define SYSTEM_PATH "/system/"
#endif
#ifdef CONFIG_CRL_PATH
#include "tz_update_crl.h"
uint8_t g_update_crl_flag = 0;
#endif

static DEFINE_MUTEX(g_load_app_lock);
#define MAX_REF_COUNT (255)

#ifdef CONFIG_ENABLE_TA_CTRL
#define VENDOR_FILE_TYPE 0
#define SYSTEM_FILE_TYPE 1
#define MAX_RETRY_TIME   3
static const char *g_systemTaVerCtrlFileList[] = {
	"/system/etc/ta_ctrl.ctrl",
};
static const char *g_vendorTaVerCtrlFileList[] = {
	"/vendor/etc/ta_ctrl.ctrl",
	"/vendor/root/res/ta_ctrl.ctrl",
};
#endif

/* record all service node and need mutex to avoid race */
struct list_head g_service_list;
DEFINE_MUTEX(g_service_list_lock);

void init_srvc_list(void)
{
	INIT_LIST_HEAD(&g_service_list);
}

void get_session_struct(struct tc_ns_session *session)
{
	if (!session)
		return;

	atomic_inc(&session->usage);
}

void put_session_struct(struct tc_ns_session *session)
{
	if (!session || !atomic_dec_and_test(&session->usage))
		return;

	if (memset_s(session, sizeof(*session), 0, sizeof(*session)) != 0)
		tloge("Caution, memset failed!\n");
	tz_free(session);
}

void get_service_struct(struct tc_ns_service *service)
{
	if (!service)
		return;

	atomic_inc(&service->usage);
	tlogd("service->usage = %d\n", atomic_read(&service->usage));
}

void put_service_struct(struct tc_ns_service *service)
{
	if (!service)
		return;

	tlogd("service->usage = %d\n", atomic_read(&service->usage));
	mutex_lock(&g_service_list_lock);
	if (atomic_dec_and_test(&service->usage)) {
		tlogd("del service [0x%x] from service list\n",
			*(uint32_t *)service->uuid);
		list_del(&service->head);
		tz_free(service);
	}
	mutex_unlock(&g_service_list_lock);
}

static int add_service_to_dev(struct tc_ns_dev_file *dev,
	struct tc_ns_service *service)
{
	uint32_t i;

	if (!dev || !service)
		return -EINVAL;

	for (i = 0; i < SERVICES_MAX_COUNT; i++) {
		if (!dev->services[i]) {
			tlogd("add service %u to %u\n", i, dev->dev_file_id);
			dev->services[i] = service;
			dev->service_ref[i] = 1;
			return 0;
		}
	}
	return -EFAULT;
}

static void tz_srv_sess_dump(const char *param)
{
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };

	(void)param;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_DUMP_SRV_SESS;
	smc_cmd.cmd_type = CMD_TYPE_GLOBAL;

	livepatch_down_read_sem();
	if (tc_ns_smc(&smc_cmd) != 0)
		tloge("send dump service session failed\n");
	livepatch_up_read_sem();
}

void dump_services_status(const char *param)
{
	struct tc_ns_service *service = NULL;

	(void)param;
	mutex_lock(&g_service_list_lock);
	tlogi("show service list:\n");
	list_for_each_entry(service, &g_service_list, head) {
		tlogi("uuid-%x, usage=%d\n", *(uint32_t *)service->uuid,
			atomic_read(&service->usage));
	}
	mutex_unlock(&g_service_list_lock);

	tz_srv_sess_dump(param);
}

static void del_service_from_dev(struct tc_ns_dev_file *dev,
	struct tc_ns_service *service)
{
	uint32_t i;

	for (i = 0; i < SERVICES_MAX_COUNT; i++) {
		if (dev->services[i] == service) {
			tlogd("dev service ref-%u = %u\n", i,
				dev->service_ref[i]);
			if (dev->service_ref[i] == 0) {
				tloge("Caution! No service to be deleted!\n");
				break;
			}
			dev->service_ref[i]--;
			if (dev->service_ref[i] == 0) {
				tlogd("del service %u from %u\n",
					i, dev->dev_file_id);
				dev->services[i] = NULL;
				put_service_struct(service);
			}
			break;
		}
	}
}

struct tc_ns_session *tc_find_session_withowner(
	const struct list_head *session_list,
	unsigned int session_id, const struct tc_ns_dev_file *dev_file)
{
	struct tc_ns_session *session = NULL;

	if (!session_list || !dev_file) {
		tloge("session list or dev is null\n");
		return NULL;
	}

	list_for_each_entry(session, session_list, head) {
		if (session->session_id == session_id &&
			session->owner == dev_file)
			return session;
	}
	return NULL;
}

struct tc_ns_service *tc_find_service_in_dev(const struct tc_ns_dev_file *dev,
	const unsigned char *uuid, int uuid_size)
{
	uint32_t i;

	if (!dev || !uuid || uuid_size != UUID_LEN)
		return NULL;

	for (i = 0; i < SERVICES_MAX_COUNT; i++) {
		if (dev->services[i] != NULL &&
			memcmp(dev->services[i]->uuid, uuid, UUID_LEN) == 0)
			return dev->services[i];
	}
	return NULL;
}

struct tc_ns_session *tc_find_session_by_uuid(unsigned int dev_file_id,
	const struct tc_ns_smc_cmd *cmd)
{
	struct tc_ns_dev_file *dev_file = NULL;
	struct tc_ns_service *service = NULL;
	struct tc_ns_session *session = NULL;

	if (!cmd) {
		tloge("parameter is null pointer!\n");
		return NULL;
	}

	dev_file = tc_find_dev_file(dev_file_id);
	if (!dev_file) {
		tloge("can't find dev file!\n");
		return NULL;
	}

	mutex_lock(&dev_file->service_lock);
	service = tc_find_service_in_dev(dev_file, cmd->uuid, UUID_LEN);
	get_service_struct(service);
	mutex_unlock(&dev_file->service_lock);
	if (!service) {
		tloge("can't find service!\n");
		return NULL;
	}

	mutex_lock(&service->session_lock);
	session = tc_find_session_withowner(&service->session_list,
		cmd->context_id, dev_file);
	get_session_struct(session);
	mutex_unlock(&service->session_lock);
	put_service_struct(service);
	if (!session) {
		tloge("can't find session-0x%x!\n", cmd->context_id);
		return NULL;
	}
	return session;
}

static int tc_ns_need_load_image(const struct tc_ns_dev_file *dev_file,
	const unsigned char *uuid, unsigned int uuid_len, struct tc_ns_client_return *tee_ret)
{
	int ret, smc_ret;
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
	struct mb_cmd_pack *mb_pack = NULL;
	char *mb_param = NULL;

	mb_pack = mailbox_alloc_cmd_pack();
	if (!mb_pack) {
		tloge("alloc mb pack failed\n");
		return -ENOMEM;
	}
	mb_param = mailbox_copy_alloc(uuid, uuid_len);
	if (!mb_param) {
		tloge("alloc mb param failed\n");
		ret = -ENOMEM;
		goto clean;
	}
	mb_pack->operation.paramtypes = TEEC_MEMREF_TEMP_INOUT;
	mb_pack->operation.params[0].memref.buffer =
		(uint32_t)mailbox_virt_to_phys((uintptr_t)mb_param);
	mb_pack->operation.buffer_h_addr[0] =
		(uint32_t)(mailbox_virt_to_phys((uintptr_t)mb_param) >> ADDR_TRANS_NUM);
	mb_pack->operation.params[0].memref.size = SZ_4K;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_NEED_LOAD_APP;
	smc_cmd.cmd_type = CMD_TYPE_GLOBAL;
	smc_cmd.dev_file_id = dev_file->dev_file_id;
#ifdef CONFIG_CONFIDENTIAL_CONTAINER
	smc_cmd.nsid = dev_file->nsid;
#endif
	smc_cmd.context_id = 0;
	smc_cmd.operation_phys = (uint32_t)mailbox_virt_to_phys((uintptr_t)&mb_pack->operation);
	smc_cmd.operation_h_phys =
		(uint32_t)(mailbox_virt_to_phys((uintptr_t)&mb_pack->operation) >> ADDR_TRANS_NUM);

	smc_ret = tc_ns_smc(&smc_cmd);
	if (smc_ret != 0) {
		tloge("smc call returns error ret 0x%x\n", smc_ret);
		if (smc_cmd.err_origin != TEEC_ORIGIN_COMMS && tee_ret != NULL) {
			tee_ret->origin = smc_cmd.err_origin;
			tee_ret->code = smc_ret;
		}
		ret = -EFAULT;
		goto clean;
	} else {
		ret = *(int *)mb_param;
	}
clean:
	if (mb_param)
		mailbox_free(mb_param);
	mailbox_free(mb_pack);

	return ret;
}

bool is_kernel_mode(uint8_t kernel_api)
{
	switch (kernel_api) {
	case TEE_REQ_FROM_USER_MODE:
	case TEE_REQ_FROM_CONTAINER_USER_MODE:
		return false;
	case TEE_REQ_FROM_KERNEL_MODE:
		return true;
	default:
		tlogd("invalid kernel api [%u]\n", kernel_api);
		return false;
	}
}

static int init_ioctl_arg(const struct tc_ns_dev_file *dev_file, const void __user *argp,
	const struct load_secfile_ioctl_struct *k_argp, struct load_secfile_ioctl_struct *ioctl_arg)
{
	if (!dev_file) {
		tloge("Invalid dev file params !\n");
		return -EINVAL;
	}

	if (!is_kernel_mode(dev_file->kernel_api)) {
		if (!argp) {
			tloge("Invalid argp params !\n");
			return -EINVAL;
		}

		if (copy_from_user(ioctl_arg, argp, sizeof(*ioctl_arg)) != 0) {
			tloge("copy from user failed\n");
			return -ENOMEM;
		}
	} else {
		if (!k_argp) {
			tloge("Invalid kargp params !\n");
			return -EINVAL;
		}

		if (memcpy_s(ioctl_arg, sizeof(*ioctl_arg), k_argp, sizeof(*ioctl_arg)) != EOK) {
			tloge("memcpy arg failed\n");
			return -ENOMEM;
		}
	}

	return 0;
}

int tc_ns_load_secfile(struct tc_ns_dev_file *dev_file,
	void __user *argp, const struct load_secfile_ioctl_struct *k_argp, bool is_from_client_node)
{
	int ret;
	struct load_secfile_ioctl_struct ioctl_arg = { {0}, {0}, {NULL} };
	bool load = true;
	void *file_addr = NULL;

	ret = init_ioctl_arg(dev_file, argp, k_argp, &ioctl_arg);
	if (ret != 0) {
		tloge("init ioctl arg failed, ret %d\n", ret);
		return ret;
	}

	if (ioctl_arg.sec_file_info.secfile_type >= LOAD_TYPE_MAX ||
	    ioctl_arg.sec_file_info.secfile_type == LOAD_PATCH) {
		tloge("invalid secfile type: %d!", ioctl_arg.sec_file_info.secfile_type);
		return -EINVAL;
	}

	ret = lock_freezable(&g_load_app_lock);
	if (ret != 0)
		return ret;
	if (is_from_client_node) {
		if (ioctl_arg.sec_file_info.secfile_type != LOAD_TA &&
			ioctl_arg.sec_file_info.secfile_type != LOAD_LIB) {
			tloge("this node does not allow this type of file to be loaded\n");
			mutex_unlock(&g_load_app_lock);
			return -EINVAL;
		}
	}

	if (ioctl_arg.sec_file_info.secfile_type == LOAD_TA) {
		ret = tc_ns_need_load_image(dev_file, ioctl_arg.uuid, (unsigned int)UUID_LEN, NULL);
		if (ret != 1) /* 1 means we need to load image */
			load = false;
	}

	if (load) {
		file_addr = (void *)(uintptr_t)(ioctl_arg.memref.file_addr |
			(((uint64_t)ioctl_arg.memref.file_h_addr) << ADDR_TRANS_NUM));
		ret = tc_ns_load_image(dev_file, file_addr, &ioctl_arg.sec_file_info, NULL, false);
		if (ret != 0)
			tloge("load TA secfile: %d failed, ret = 0x%x\n",
				ioctl_arg.sec_file_info.secfile_type, ret);
	}
	mutex_unlock(&g_load_app_lock);
	if (!is_kernel_mode(dev_file->kernel_api)) {
		if (copy_to_user(argp, &ioctl_arg, sizeof(ioctl_arg)) != 0)
			tloge("copy to user failed\n");
	}
	return ret;
}

#ifdef CONFIG_AUTH_SUPPORT_UNAME
static int set_login_information_uname(struct tc_ns_dev_file *dev_file, uint32_t uid)
{
	char uname[MAX_NAME_LENGTH] = { 0 };
	uint32_t username_len = 0;
	int ret = tc_ns_get_uname(uid, uname, sizeof(uname), &username_len);
	if (ret < 0 || username_len >= MAX_NAME_LENGTH) {
		tloge("get user name filed\n");
		return -EFAULT;
	}
	if (memcpy_s(dev_file->pub_key, MAX_PUBKEY_LEN, uname, username_len)) {
		tloge("failed to copy username, pub key len=%u\n", dev_file->pub_key_len);
		return -EFAULT;
	}
	/* use pub_key to store username info */
	dev_file->pub_key_len = username_len;
	return 0;
}
#else
static int set_login_information_uid(struct tc_ns_dev_file *dev_file, uint32_t ca_uid)
{
	if (memcpy_s(dev_file->pub_key, MAX_PUBKEY_LEN, &ca_uid, sizeof(ca_uid)) != 0) {
		tloge("failed to copy pubkey, pub key len=%u\n",
				dev_file->pub_key_len);
		return -EFAULT;
	}
	dev_file->pub_key_len = sizeof(ca_uid);
	return 0;
}
#endif

/*
 * Modify the client context so params id 2 and 3 contain temp pointers to the
 * public key and package name for the open session. This is used for the
 * TEEC_LOGIN_IDENTIFY open session method
 */
static int set_login_information(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context)
{
	uint64_t size_addr, buffer_addr;
	/* The daemon has failed to get login information or not supplied */
	if (dev_file->pkg_name_len == 0)
		return -EINVAL;
	/*
	 * The 3rd parameter buffer points to the pkg name buffer in the
	 * device file pointer
	 * get package name len and package name
	 */
	size_addr = (__u64)(uintptr_t)&dev_file->pkg_name_len;
	buffer_addr = (__u64)(uintptr_t)dev_file->pkg_name;
	context->params[3].memref.size_addr = (__u32)size_addr;
	context->params[3].memref.size_h_addr = (__u32)(size_addr >> ADDR_TRANS_NUM);
	context->params[3].memref.buffer = (__u32)buffer_addr;
	context->params[3].memref.buffer_h_addr = (__u32)(buffer_addr >> ADDR_TRANS_NUM);

	/* Set public key len and public key */
	if (dev_file->pub_key_len == 0) {
		/* If get public key failed, then get uid in kernel */
		uint32_t ca_uid = current_uid().val;
		if (ca_uid == (uint32_t)(-1)) {
			tloge("failed to get uid of the task\n");
			goto error;
		}
#ifdef CONFIG_AUTH_SUPPORT_UNAME
		if (set_login_information_uname(dev_file, ca_uid) != 0)
			goto error;
#else
		if (set_login_information_uid(dev_file, ca_uid) != 0)
			goto error;
#endif
#ifdef CONFIG_AUTH_HASH
		dev_file->pkg_name_len = (uint32_t)strlen((unsigned char *)dev_file->pkg_name);
#endif
	}
	size_addr = (__u64)(uintptr_t)&dev_file->pub_key_len;
	buffer_addr = (__u64)(uintptr_t)dev_file->pub_key;
	context->params[2].memref.size_addr = (__u32)size_addr;
	context->params[2].memref.size_h_addr = (__u32)(size_addr >> ADDR_TRANS_NUM);
	context->params[2].memref.buffer = (__u32)buffer_addr;
	context->params[2].memref.buffer_h_addr = (__u32)(buffer_addr >> ADDR_TRANS_NUM);
	/* Now we mark the 2 parameters as input temp buffers */
	context->param_types = teec_param_types(
		teec_param_type_get(context->param_types, 0),
		teec_param_type_get(context->param_types, 1),
		TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
#ifdef CONFIG_AUTH_HASH
	if(set_login_information_hash(dev_file) != 0) {
		tloge("set login information hash failed\n");
		goto error;
	}
#endif
	return 0;
error:
	return -EFAULT;
}

static int check_login_method(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context, uint8_t *flags)
{
	int ret;

	if (!dev_file || !context || !flags)
		return -EFAULT;

	if (is_tee_rebooting()) {
		context->returns.code = TEE_ERROR_IS_DEAD;
		/* when ret > 0, use context return code */
		return EFAULT;
	}

	if (context->login.method != TEEC_LOGIN_IDENTIFY) {
		tloge("login method is not supported\n");
		return -EINVAL;
	}

#ifdef CONFIG_LIBLINUX
	if (dev_file->kernel_api == TEE_REQ_FROM_CONTAINER_USER_MODE) {
#ifdef CONFIG_TA_GET_CA_USERID
		if ((dev_file->pkg_name_len <= CA_USERID_SIZE) ||
			(dev_file->pkg_name_len >= MAX_PACKAGE_NAME_LEN + CA_USERID_SIZE)) {
			tloge("check pkg_name length failed with %u\n", dev_file->pkg_name_len);
			return -EINVAL;
		}
		uint8_t *container_user_id_pos = (uint8_t *)dev_file->pkg_name + dev_file->pkg_name_len - CA_USERID_SIZE;
		uint32_t container_user_id = *(uint32_t *)container_user_id_pos;
#endif
		ret = liblinux_pal_get_current_exec_path(dev_file->pkg_name, MAX_PACKAGE_NAME_LEN);
		if (ret != 0) {
			tloge(" get container exec path failed with %d\n", ret);
			return ret;
		}
		/* dev_file->pkg_name max len is MAX_PACKAGE_NAME_LEN + CA_USERID_SIZE */
		dev_file->pkg_name_len = strnlen(dev_file->pkg_name, MAX_PACKAGE_NAME_LEN);
		if ((dev_file->pkg_name_len == 0) || (dev_file->pkg_name_len == MAX_PACKAGE_NAME_LEN)) {
			tloge("get container exec path length failed with %u\n", dev_file->pkg_name_len);
			return -EINVAL;
		}
#ifdef CONFIG_TA_GET_CA_USERID
		container_user_id_pos = (uint8_t *)dev_file->pkg_name + dev_file->pkg_name_len;
		*(uint32_t *)container_user_id_pos = container_user_id;
		dev_file->pkg_name_len += CA_USERID_SIZE;
#endif
	}
#endif

	tlogd("login method is IDENTIFY\n");

	ret = set_login_information(dev_file, context);
	if (ret != 0) {
		tloge("set login information failed ret =%d\n", ret);
		return ret;
	}
	*flags |= TC_CALL_LOGIN;

	return 0;
}

static struct tc_ns_service *tc_ref_service_in_dev(struct tc_ns_dev_file *dev,
	const unsigned char *uuid, int uuid_size, unsigned int nsid, bool *is_full)
{
	uint32_t i;

	if (uuid_size != UUID_LEN)
		return NULL;

	for (i = 0; i < SERVICES_MAX_COUNT; i++) {
		if (dev->services[i] != NULL && dev->services[i]->nsid == nsid &&
			memcmp(dev->services[i]->uuid, uuid, UUID_LEN) == 0) {
			if (dev->service_ref[i] == MAX_REF_COUNT) {
				*is_full = true;
				return NULL;
			}
			dev->service_ref[i]++;
			return dev->services[i];
		}
	}
	return NULL;
}

static int tc_ns_service_init(const unsigned char *uuid, uint32_t uuid_len,
	struct tc_ns_service **new_service)
{
	int ret = 0;
	struct tc_ns_service *service = NULL;

	if (!uuid || !new_service || uuid_len != UUID_LEN)
		return -EINVAL;

	service = tz_alloc(sizeof(*service));
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)service)) {
		tloge("kzalloc failed\n");
		ret = -ENOMEM;
		return ret;
	}

	if (memcpy_s(service->uuid, sizeof(service->uuid), uuid, uuid_len) != 0) {
		tz_free(service);
		return -EFAULT;
	}

#ifdef CONFIG_CONFIDENTIAL_CONTAINER
	service->nsid = task_active_pid_ns(current)->ns.inum;
#else
	/* when container is not existed, set nsid as default num from linux/proc_ns.h */
	service->nsid = PROC_PID_INIT_INO;
#endif
	INIT_LIST_HEAD(&service->session_list);
	mutex_init(&service->session_lock);
	list_add_tail(&service->head, &g_service_list);
	tlogd("add service: 0x%x to service list\n", *(const uint32_t *)uuid);
	atomic_set(&service->usage, 1);
	mutex_init(&service->operation_lock);
	*new_service = service;

	return ret;
}

static struct tc_ns_service *tc_find_service_from_all(
	const unsigned char *uuid, uint32_t uuid_len, uint32_t nsid)
{
	struct tc_ns_service *service = NULL;

	if (!uuid || uuid_len != UUID_LEN)
		return NULL;

	list_for_each_entry(service, &g_service_list, head) {
		if (memcmp(service->uuid, uuid, sizeof(service->uuid)) == 0 && service->nsid == nsid)
			return service;
	}

	return NULL;
}

static struct tc_ns_service *find_service(struct tc_ns_dev_file *dev_file,
	const struct tc_ns_client_context *context)
{
	int ret;
	struct tc_ns_service *service = NULL;
	bool is_full = false;
#ifdef CONFIG_CONFIDENTIAL_CONTAINER
	unsigned int nsid = task_active_pid_ns(current)->ns.inum;
#else
	unsigned int nsid = PROC_PID_INIT_INO;
#endif

	mutex_lock(&dev_file->service_lock);
	service = tc_ref_service_in_dev(dev_file, context->uuid,
		UUID_LEN, nsid, &is_full);
	/* if service has been opened in this dev or ref cnt is full */
	if (service || is_full) {
		/*
		 * If service has been reference by this dev, find service in dev
		 * will incre ref count to declaim there's how many callers to
		 * this service from the dev, instead of incre service->usage.
		 * While close session, dev->service_ref[i] will decre and till
		 * it get to 0, put service struct will be called.
		 */
		mutex_unlock(&dev_file->service_lock);
		return service;
	}

	mutex_lock(&g_service_list_lock);
	service = tc_find_service_from_all(context->uuid, UUID_LEN, nsid);
	/* if service has been opened in other dev */
	if (service) {
		get_service_struct(service);
		mutex_unlock(&g_service_list_lock);
		goto add_service;
	}
	/* Create a new service if we couldn't find it in list */
	ret = tc_ns_service_init(context->uuid, UUID_LEN, &service);
	/* unlock after init to make sure find service from all is correct */
	mutex_unlock(&g_service_list_lock);
	if (ret != 0) {
		tloge("service init failed");
		mutex_unlock(&dev_file->service_lock);
		return NULL;
	}
add_service:
	ret = add_service_to_dev(dev_file, service);
	mutex_unlock(&dev_file->service_lock);
	if (ret != 0) {
		/*
		 * for new srvc, match init usage to 1;
		 * for srvc already exist, match get;
		 */
		put_service_struct(service);
		service = NULL;
		tloge("fail to add service to dev\n");
		return NULL;
	}
	return service;
}

static bool is_valid_ta_size(const char *file_buffer, unsigned int file_size)
{
	if (!file_buffer || file_size == 0) {
		tloge("invalid load ta size\n");
		return false;
	}

	if (file_size > SZ_8M) {
		tloge("not support TA larger than 8M, size=%u\n", file_size);
		return false;
	}
	return true;
}

static int alloc_for_load_image(struct load_img_params *params)
{
	/* we will try any possible to alloc mailbox mem to load TA */
	for (; params->mb_load_size > 0; params->mb_load_size >>= 1) {
		params->mb_load_mem = mailbox_alloc(params->mb_load_size, 0);
		if (params->mb_load_mem)
			break;
		tlogw("alloc mem size=%u for TA load mem fail\n",
			params->mb_load_size);
	}

	if (!params->mb_load_mem) {
		tloge("alloc TA load mem failed\n");
		return -ENOMEM;
	}

	params->mb_pack = mailbox_alloc_cmd_pack();
	if (!params->mb_pack) {
		mailbox_free(params->mb_load_mem);
		params->mb_load_mem = NULL;
		tloge("alloc mb pack failed\n");
		return -ENOMEM;
	}

	params->uuid_return = mailbox_alloc(sizeof(*(params->uuid_return)), 0);
	if (!params->uuid_return) {
		mailbox_free(params->mb_load_mem);
		params->mb_load_mem = NULL;
		mailbox_free(params->mb_pack);
		params->mb_pack = NULL;
		tloge("alloc uuid failed\n");
		return -ENOMEM;
	}
	return 0;
}

static void pack_load_frame_cmd(uint32_t load_size,
	const struct load_img_params *params, struct tc_ns_smc_cmd *smc_cmd)
{
	struct mb_cmd_pack *mb_pack = params->mb_pack;
	char *mb_load_mem = params->mb_load_mem;
	struct tc_uuid *uuid_return = params->uuid_return;

	mb_pack->operation.params[0].memref.buffer =
		mailbox_virt_to_phys((uintptr_t)mb_load_mem);
	mb_pack->operation.buffer_h_addr[0] =
		(uint64_t)mailbox_virt_to_phys((uintptr_t)mb_load_mem) >> ADDR_TRANS_NUM;
	mb_pack->operation.params[0].memref.size = load_size + sizeof(int);
	mb_pack->operation.params[2].memref.buffer =
		mailbox_virt_to_phys((uintptr_t)uuid_return);
	mb_pack->operation.buffer_h_addr[2] =
		(uint64_t)mailbox_virt_to_phys((uintptr_t)uuid_return) >> ADDR_TRANS_NUM;
	mb_pack->operation.params[2].memref.size = sizeof(*uuid_return);
	mb_pack->operation.paramtypes = teec_param_types(TEEC_MEMREF_TEMP_INPUT,
		TEEC_VALUE_INOUT, TEEC_MEMREF_TEMP_OUTPUT, TEEC_VALUE_INOUT);

	smc_cmd->cmd_type = CMD_TYPE_GLOBAL;
	smc_cmd->cmd_id = GLOBAL_CMD_ID_LOAD_SECURE_APP;
	smc_cmd->context_id = 0;
	smc_cmd->operation_phys = mailbox_virt_to_phys((uintptr_t)&mb_pack->operation);
	smc_cmd->operation_h_phys =
		(uint64_t)mailbox_virt_to_phys((uintptr_t)&mb_pack->operation) >> ADDR_TRANS_NUM;
}

static int32_t load_image_copy_file(struct load_img_params *params, uint32_t load_size,
	int32_t load_flag, uint32_t loaded_size, bool is_kernal_load)
{
	if (is_kernel_mode(params->dev_file->kernel_api) || is_kernal_load) {
		if (memcpy_s(params->mb_load_mem + sizeof(load_flag),
			params->mb_load_size - sizeof(load_flag),
			params->file_buffer + loaded_size, load_size) != 0) {
			tloge("memcpy file buf get fail\n");
			return  -EFAULT;
		}
		return 0;
	}
	if (copy_from_user(params->mb_load_mem + sizeof(load_flag),
		(const void __user *)(params->file_buffer + loaded_size), load_size) != 0) {
		tloge("file buf get fail\n");
		return  -EFAULT;
	}
	return 0;
}

static int load_image_by_frame(struct load_img_params *params, unsigned int load_times,
	struct tc_ns_client_return *tee_ret, struct sec_file_info *sec_file_info, bool is_kernal_load)
{
	char *p = params->mb_load_mem;
	uint32_t load_size;
	int load_flag = 1; /* 0:it's last block, 1:not last block */
	uint32_t loaded_size = 0;
	unsigned int index;
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
	int smc_ret;

	for (index = 0; index < load_times; index++) {
		smc_cmd.err_origin = TEEC_ORIGIN_COMMS;
		if (index == (load_times - 1)) {
			load_flag = 0;
			load_size = params->file_size - loaded_size;
		} else {
			load_size = params->mb_load_size - sizeof(load_flag);
		}
		*(int *)p = load_flag;
		if (load_size > params->mb_load_size - sizeof(load_flag)) {
			tloge("invalid load size %u/%u\n", load_size, params->mb_load_size);
			return  -EINVAL;
		}

		if (load_image_copy_file(params, load_size, load_flag, loaded_size,
			is_kernal_load) != 0)
			return -EFAULT;

		pack_load_frame_cmd(load_size, params, &smc_cmd);
		params->mb_pack->operation.params[3].value.a = index;
		params->mb_pack->operation.params[1].value.a = sec_file_info->secfile_type;
		smc_cmd.dev_file_id = params->dev_file->dev_file_id;
#ifdef CONFIG_CONFIDENTIAL_CONTAINER
		smc_cmd.nsid = params->dev_file->nsid;
#endif
		smc_ret = tc_ns_smc(&smc_cmd);
		tlogd("configid=%u, ret=%d, load_flag=%d, index=%u\n",
			params->mb_pack->operation.params[1].value.a, smc_ret, load_flag, index);

		if (smc_ret != 0) {
			if (tee_ret != NULL) {
				tee_ret->code = smc_ret;
				tee_ret->origin = smc_cmd.err_origin;
			}
			sec_file_info->sec_load_err = (int32_t)params->mb_pack->operation.params[3].value.b;
			return -EFAULT;
		}

		if ((smc_ret == 0) && (load_flag == 0) &&
			(load_image_for_ion(params, tee_ret ? (int32_t *)&tee_ret->origin : NULL) != 0))
				return -EPERM;

		loaded_size += load_size;
	}
	return 0;
}

int tc_ns_load_image_with_lock(struct tc_ns_dev_file *dev, const char *file_buffer,
	unsigned int file_size, enum secfile_type_t type)
{
	int ret;
	struct sec_file_info sec_file = {0, 0, 0};

	if (!dev || !file_buffer) {
		tloge("dev or file buffer is NULL!\n");
		return -EINVAL;
	}

	sec_file.secfile_type = type;
	sec_file.file_size = file_size;

	ret = lock_freezable(&g_load_app_lock);
	if (ret != 0)
		return ret;
	ret = tc_ns_load_image(dev, file_buffer, &sec_file, NULL, false);
	mutex_unlock(&g_load_app_lock);

	return ret;
}

static void free_load_image_buffer(struct load_img_params *params)
{
	mailbox_free(params->mb_load_mem);
	mailbox_free(params->mb_pack);
	mailbox_free(params->uuid_return);
}

/*
 * Notice:
 * Calling this function requires g_load_app_lock protection.
 * Concurrent loading is not supported.
 */
int load_image(struct load_img_params *params,
	struct sec_file_info *sec_file_info, struct tc_ns_client_return *tee_ret, bool is_kernal_load)
{
	int ret;
	unsigned int load_times;
	unsigned int file_size;

	/* tee_ret can be null */
	if (params == NULL || sec_file_info == NULL)
		return -1;

	file_size = params->file_size;

	params->mb_load_size = (file_size > (SZ_1M - sizeof(int))) ?
		SZ_1M : ALIGN(file_size, SZ_4K);

	ret = alloc_for_load_image(params);
	if (ret != 0) {
		tloge("Alloc load image buf fail!\n");
		return ret;
	}

	if (params->mb_load_size <= sizeof(int)) {
		tloge("mb load size is too small!\n");
		free_load_image_buffer(params);
		return -ENOMEM;
	}

	load_times = file_size / (params->mb_load_size - sizeof(int));
	if ((file_size % (params->mb_load_size - sizeof(int))) != 0)
		load_times += 1;

	ret = load_image_by_frame(params, load_times, tee_ret, sec_file_info, is_kernal_load);
	if (ret != 0) {
		tloge("load image by frame fail!\n");
		free_load_image_buffer(params);
		return ret;
	}

	free_load_image_buffer(params);
	return 0;
}

#ifdef CONFIG_ENABLE_TA_CTRL
static int32_t tz_load_ctrl_file(const char *ta_ctrl_file, uint32_t file_type_index)
{
	char *file_buf = NULL;
	uint32_t file_size = 0;
	bool load_app_flag = false;
	int32_t ret = -1;
	struct ta_ctrl_info ta_ctrl = {NULL, 0, 0};
	load_app_flag = tzdriver_load_file(ta_ctrl_file, &file_buf, &file_size);
	if (load_app_flag == false) {
		tloge("teek load app failed, ta_ctrl_file=%s\n", ta_ctrl_file);
		goto clean;
	}

	ta_ctrl.buffer = file_buf;
	ta_ctrl.size = file_size;
	ta_ctrl.file_type_index = file_type_index;

	ret = tc_ns_set_tavertion_ctrl(&ta_ctrl, GLOBAL_CMD_ID_TA_CONTROL_VERSION);
	if (ret != 0)
		tloge("load file failed, file name=%s, ret=%d\n", ta_ctrl_file, ret);
	else
		tlogi("load file sucess, file name=%s\n", ta_ctrl_file);
clean:
	teek_free_app(load_app_flag, &file_buf);
	return ret;
}

int32_t tz_load_ta_ctrl(const char *file_name, uint32_t file_type)
{
	int32_t retry_times = 0;
	bool success_flag = false;
	int32_t ret = -1;
	while (retry_times < MAX_RETRY_TIME && !success_flag) {
		ret = tz_load_ctrl_file(file_name, file_type);
		if (ret == 0)
			success_flag = true;
		else
			retry_times++;
	}
	return ret;
}

void tz_load_ta_ctrl_file(void)
{
	static bool sendCtrlFlag = false;
	if (sendCtrlFlag) {
		return;
	}

	int32_t ret = tz_load_ta_ctrl(g_systemTaVerCtrlFileList[0], SYSTEM_FILE_TYPE);
	if (ret != 0) {
		sendCtrlFlag = true;
		return;
	}

	uint32_t i;
	for (i = 0; i < sizeof(g_vendorTaVerCtrlFileList) / sizeof(g_vendorTaVerCtrlFileList[0]); i++) {
		if (tz_load_ta_ctrl(g_vendorTaVerCtrlFileList[i], VENDOR_FILE_TYPE) == 0) {
			break;
		}
	}
	sendCtrlFlag = true;
}
#endif

int tc_ns_load_image(struct tc_ns_dev_file *dev, const char *file_buffer,
	struct sec_file_info *sec_file_info, struct tc_ns_client_return *tee_ret, bool is_kernal_load)
{
	unsigned int file_size;
	struct load_img_params params = { dev, file_buffer, 0, NULL, NULL, NULL, 0 };
#ifdef CONFIG_ENABLE_TA_CTRL
	tz_load_ta_ctrl_file();
#endif
	if (!dev || !file_buffer || !sec_file_info) {
		tloge("dev or file buffer or sec_file_info is NULL!\n");
		return -EINVAL;
	}

	file_size = sec_file_info->file_size;
	params.file_size = file_size;

#ifdef CONFIG_CRL_PATH
	if (g_update_crl_flag == 0) {
		if (tz_update_crl(CONFIG_CRL_PATH, dev) != 0) {
			tloge("tzdriver updates main crl failed\n");
			if (tz_update_crl(CONFIG_CRL_BAK_PATH, dev) != 0) {
				tloge("tzdriver updates backup crl failed\n");
			} else {
				g_update_crl_flag = 1;
				tloge("tzdriver updates backup crl successfully\n");
			}
		} else {
			g_update_crl_flag = 1;
			tloge("tzdriver updates main crl successfully\n");
		}
	}
#endif
	if (!is_valid_ta_size(file_buffer, file_size))
		return -EINVAL;

	return load_image(&params, sec_file_info, tee_ret, is_kernal_load);
}

static int load_ta_image(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context)
{
	int ret;
	struct sec_file_info sec_file = {0, 0, 0};
	struct tc_ns_client_return tee_ret = {0};
	void *file_addr = NULL;
	bool is_kernal_load = false;

	tee_ret.origin = TEEC_ORIGIN_COMMS;

	ret = lock_freezable(&g_load_app_lock);
	if (ret != 0)
		return ret;
	ret = tc_ns_need_load_image(dev_file, context->uuid, (unsigned int)UUID_LEN, &tee_ret);
	if (ret == 1) { /* 1 means we need to load image */
		if ((!context->file_buffer) && (!load_ta_from_default_path(context, &is_kernal_load))) {
			tloge("context's file_buffer is NULL");
			mutex_unlock(&g_load_app_lock);
			return -1;
		}
		file_addr = (void *)(uintptr_t)(context->memref.file_addr |
			(((uint64_t)context->memref.file_h_addr) << ADDR_TRANS_NUM));
		sec_file.secfile_type = LOAD_TA;
		sec_file.file_size = context->file_size;
		ret = tc_ns_load_image(dev_file, file_addr, &sec_file, &tee_ret, is_kernal_load);
		if (ret != 0) {
			tloge("load image failed, ret=%x", ret);
			context->returns.code = tee_ret.code;
			if (tee_ret.origin != TEEC_ORIGIN_COMMS) {
				context->returns.origin = tee_ret.origin;
				ret = EFAULT;
			}
			teek_free_app(is_kernal_load, &(context->file_buffer));
			mutex_unlock(&g_load_app_lock);
			return ret;
		}
	}

	teek_free_app(is_kernal_load, &(context->file_buffer));
	if (ret != 0 && tee_ret.origin != TEEC_ORIGIN_COMMS) {
		context->returns.code = tee_ret.code;
		context->returns.origin = tee_ret.origin;
		ret = EFAULT;
	}
	mutex_unlock(&g_load_app_lock);

	return ret;
}

static void init_new_sess_node(struct tc_ns_dev_file *dev_file,
	const struct tc_ns_client_context *context,
	struct tc_ns_service *service,
	struct tc_ns_session *session)
{
	session->session_id = context->session_id;
	atomic_set(&session->usage, 1);
	session->owner = dev_file;

	session->wait_data.send_wait_flag = 0;
	init_waitqueue_head(&session->wait_data.send_cmd_wq);

	mutex_lock(&service->session_lock);
	list_add_tail(&session->head, &service->session_list);
	mutex_unlock(&service->session_lock);
}

#ifdef CONFIG_LIBLINUX
static int check_spec_path(char *path, int path_len)
{
	if (path == NULL || path_len <= 0 || path_len < strlen(VENDOR_PATH) || path_len < strlen(SYSTEM_PATH))
		return TEEC_ERROR_BAD_PARAMETERS;
	if (strncmp(path, VENDOR_PATH, strlen(VENDOR_PATH)) == 0 ||
	    strncmp(path, SYSTEM_PATH, strlen(SYSTEM_PATH)) == 0)
		return TEEC_SUCCESS;
	return TEEC_ERROR_NOT_SUPPORTED;
}

static int construct_hashdata(struct tc_ns_char_area *in_buf, struct tc_ns_char_area *path)
{
	int ret;
	char *output_buff = in_buf->addr;
	int output_size = in_buf->size;
	ret = memcpy_s(output_buff, output_size, path->addr, path->size);
	if (ret != EOK) {
		tloge("memcpy_s failed!\n");
		return TEEC_ERROR_SECURITY;
	}
	return TEEC_SUCCESS;
}

static void auth_hash_fill_pid(__u32 *pid, struct tc_ns_client_context *context)
{
	int ret;
	ret = check_cadaemon_auth();
	if (ret == CHECK_ACCESS_SUCC) {
		/* hidl path */
		*pid = context->calling_pid;
	} else {
		/* normal path */
		*pid = current->pid;
	}
}

static int calc_client_auth_hash_liblinux(struct tc_ns_client_context *context, struct tc_ns_session *session)
{
	if (current->mm == NULL)
		return TEEC_SUCCESS;
	int ret = 0;
	__u32 pid = 0;
	char path[MAX_PATH_LEN] = {0};
	struct tc_ns_char_area in_buf = {0};
	struct tc_ns_char_area ca_path = {0};
	auth_hash_fill_pid(&pid, context);
#ifndef CONFIG_ASAN_DEBUG
	ret = liblinux_pal_get_processname_by_tid(pid, path, MAX_PATH_LEN);
#else
	tloge("in CONFIG_ASAN_DEBUG mode. Change ret to SUCC\n");
#define DEBUG_ASAN_PATH "/vendor/bin/asan"
	const char *new_path = DEBUG_ASAN_PATH;
	(void)strncpy_s(path, MAX_PATH_LEN, new_path, strlen(new_path));
	ret = EOK;
#endif
	if (ret != EOK) {
		tloge("get ca path failed!\n");
		return TEEC_ERROR_EXCESS_DATA;
	}
	ret = check_spec_path(path, MAX_PATH_LEN);
	if (ret != TEEC_SUCCESS) {
		tloge("check path prefix failed!\n");
		return TEEC_ERROR_EXCESS_DATA;
	}
	ca_path.addr = path;
	ca_path.size = strnlen(path, MAX_PATH_LEN);
	in_buf.size = ca_path.size;
	in_buf.addr = kzalloc(in_buf.size, GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)in_buf.addr)) {
		tloge("indata kmalloc fail\n");
		return TEEC_ERROR_GENERIC;
	}
	ret = construct_hashdata(&in_buf, &ca_path);
	if (ret != 0) {
		tloge("construct hashdata failed\n");
		goto error;
	}
	ret = do_sha256((unsigned char *)in_buf.addr, in_buf.size,
			session->auth_hash_buf + MAX_SHA_256_SZ * NUM_OF_SO, SHA256_DIGEST_LENTH);
	if (ret != 0) {
		tloge("sha256 failed\n");
		goto error;
	}
	if (!ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)in_buf.addr))
		kfree(in_buf.addr);
	return TEEC_SUCCESS;
error:
	if (!ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)in_buf.addr))
		kfree(in_buf.addr);
	return ret;
}
#endif

static int proc_open_session(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context, struct tc_ns_service *service,
	struct tc_ns_session *session, uint8_t flags)
{
	struct tc_call_params params = {
		dev_file, context, session, flags
	};
	int ret = lock_freezable(&service->operation_lock);
	if (ret != 0)
		return ret;

	ret = load_ta_image(dev_file, context);
	if (ret != 0) {
		tloge("load ta image failed\n");
		mutex_unlock(&service->operation_lock);
		return ret;
	}

	ret = tc_client_call(&params);
	if (ret != 0) {
		/* Clean this session secure information */
		kill_ion_by_uuid((struct tc_uuid *)context->uuid);
		mutex_unlock(&service->operation_lock);
		tloge("smc call returns error, ret=0x%x\n", ret);
		return ret;
	}
	init_new_sess_node(dev_file, context, service, session);
	/*
	 * session_id in tee is unique, but in concurrency scene
	 * same session_id may appear in tzdriver, put session_list
	 * add/del in service->operation_lock can avoid it.
	 */
	mutex_unlock(&service->operation_lock);
	return ret;
}

static void clear_context_param(struct tc_ns_client_context *context)
{
	context->params[2].memref.size_addr = 0;
	context->params[2].memref.size_h_addr = 0;
	context->params[2].memref.buffer = 0;
	context->params[2].memref.buffer_h_addr = 0;
	context->params[3].memref.size_addr = 0;
	context->params[3].memref.size_h_addr = 0;
	context->params[3].memref.buffer = 0;
	context->params[3].memref.buffer_h_addr = 0;
}

int tc_ns_open_session(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context)
{
	int ret;
	struct tc_ns_service *service = NULL;
	struct tc_ns_session *session = NULL;
	uint8_t flags = TC_CALL_GLOBAL;

	if (!dev_file || !context) {
		tloge("invalid dev_file or context\n");
		return -EINVAL;
	}

	ret = check_login_method(dev_file, context, &flags);
	if (ret != 0)
		goto err_clear_param;

	context->cmd_id = GLOBAL_CMD_ID_OPEN_SESSION;

	service = find_service(dev_file, context);
	if (!service) {
		tloge("find service failed\n");
		ret = -ENOMEM;
		goto err_clear_param;
	}

	session = tz_alloc(sizeof(*session));
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)session)) {
		tloge("kzalloc failed\n");
		mutex_lock(&dev_file->service_lock);
		del_service_from_dev(dev_file, service);
		mutex_unlock(&dev_file->service_lock);
		ret = -ENOMEM;
		goto err_clear_param;
	}
	mutex_init(&session->ta_session_lock);

#ifndef CONFIG_LIBLINUX
	ret = calc_client_auth_hash(dev_file, context, session);
	if (ret != 0) {
		tloge("calc client auth hash failed\n");
		goto err_free_rsrc;
	}
#else
	if (dev_file->kernel_api != TEE_REQ_FROM_CONTAINER_USER_MODE) {
		ret = calc_client_auth_hash_liblinux(context, session);
		if (ret != 0) {
			tloge("calc client auth hash failed\n");
			goto err_free_rsrc;
		}
	}
#endif

#ifdef CONFIG_CONFIDENTIAL_CONTAINER
	if (dev_file->nsid == 0)
		dev_file->nsid = task_active_pid_ns(current)->ns.inum;
#endif

	ret = proc_open_session(dev_file, context, service, session, flags);
	if (ret == 0)
		goto err_clear_param;
err_free_rsrc:
	mutex_lock(&dev_file->service_lock);
	del_service_from_dev(dev_file, service);
	mutex_unlock(&dev_file->service_lock);

	tz_free(session);
err_clear_param:
	clear_context_param(context);
	return ret;
}

static struct tc_ns_session *get_session(struct tc_ns_service *service,
	const struct tc_ns_dev_file *dev_file,
	const struct tc_ns_client_context *context)
{
	struct tc_ns_session *session = NULL;

	mutex_lock(&service->session_lock);
	session = tc_find_session_withowner(&service->session_list,
		context->session_id, dev_file);
	get_session_struct(session);
	mutex_unlock(&service->session_lock);

	return session;
}

static struct tc_ns_service *get_service(struct tc_ns_dev_file *dev_file,
	const struct tc_ns_client_context *context)
{
	struct tc_ns_service *service = NULL;

	mutex_lock(&dev_file->service_lock);
	service = tc_find_service_in_dev(dev_file, context->uuid, UUID_LEN);
	get_service_struct(service);
	mutex_unlock(&dev_file->service_lock);

	return service;
}

static int close_session(struct tc_ns_dev_file *dev,
	struct tc_ns_session *session, const unsigned char *uuid,
	unsigned int uuid_len, unsigned int session_id)
{
	struct tc_ns_client_context context;
	int ret;
	struct tc_call_params params = {
		dev, &context, session, 0
	};

	if (uuid_len != UUID_LEN)
		return -EINVAL;

	if (memset_s(&context, sizeof(context), 0, sizeof(context)) != 0)
		return -EFAULT;

	if (memcpy_s(context.uuid, sizeof(context.uuid), uuid, uuid_len) != 0)
		return -EFAULT;

	context.session_id = session_id;
	context.cmd_id = GLOBAL_CMD_ID_CLOSE_SESSION;
	params.flags = TC_CALL_GLOBAL | TC_CALL_SYNC;
	ret = tc_client_call(&params);
	if (ret != 0)
		tloge("close session failed, ret=0x%x\n", ret);

	kill_ion_by_uuid((struct tc_uuid *)context.uuid);
	return ret;
}

static void close_session_in_service_list(struct tc_ns_dev_file *dev,
	struct tc_ns_service *service)
{
	struct tc_ns_session *tmp_session = NULL;
	struct tc_ns_session *session = NULL;
	int ret;

	list_for_each_entry_safe(session, tmp_session,
		&service->session_list, head) {
		if (session->owner != dev)
			continue;
		ret = close_session(dev, session, service->uuid,
			(unsigned int)UUID_LEN, session->session_id);
		if (ret != 0)
			tloge("close session smc failed when close fd!\n");
		mutex_lock(&service->session_lock);
		list_del(&session->head);
		mutex_unlock(&service->session_lock);

		put_session_struct(session); /* pair with open session */
	}
}

static bool if_exist_unclosed_session(struct tc_ns_dev_file *dev)
{
	uint32_t index;

	for (index = 0; index < SERVICES_MAX_COUNT; index++) {
		if (dev->services[index] != NULL &&
			list_empty(&dev->services[index]->session_list) == 0)
			return true;
	}
	return false;
}

static int close_session_thread_fn(void *arg)
{
	struct tc_ns_dev_file *dev = arg;
	uint32_t index;
	struct tc_ns_service *service = NULL;

	/* close unclosed session */
	for (index = 0; index < SERVICES_MAX_COUNT; index++) {
		if (dev->services[index] != NULL &&
				list_empty(&dev->services[index]->session_list) == 0) {
				service = dev->services[index];

				mutex_lock(&service->operation_lock);
				close_session_in_service_list(dev, service);
				mutex_unlock(&service->operation_lock);

				put_service_struct(service); /* pair with open session */
			}
	}

	tlogd("complete close all unclosed session\n");
	complete(&dev->close_comp);
	return 0;
}

void close_unclosed_session_in_kthread(struct tc_ns_dev_file *dev)
{
	struct task_struct *close_thread = NULL;

	if (!dev) {
		tloge("dev is invalid\n");
		return;
	}

	if (!if_exist_unclosed_session(dev))
		return;

	/* when self recovery, release session in reboot interface */
	if (is_tee_rebooting())
		return;

#ifndef CONFIG_TA_AFFINITY
	close_session_thread_fn(dev);
	(void)close_thread;
#else
	/*
	 * the close(fd) is called automaticlly by kworker when CA process exit without close(fd).
	 * set affinity for kworker will course to BUG_ON, so we create a kthread.
	 */
	close_thread = kthread_create(close_session_thread_fn,
		dev, "close_fn_%6d", dev->dev_file_id);
	if (unlikely(IS_ERR_OR_NULL(close_thread))) {
		tloge("fail to create close session thread\n");
		return;
	}

	tz_kthread_bind_mask(close_thread);
	wake_up_process(close_thread);
	wait_for_completion(&dev->close_comp);
	tlogd("wait for completion success\n");
#endif
}

int tc_ns_close_session(struct tc_ns_dev_file *dev_file, struct tc_ns_client_context *context)
{
	int ret = -EINVAL, ret2;
	struct tc_ns_service *service = NULL;
	struct tc_ns_session *session = NULL;

	if (!dev_file || !context) {
		tloge("invalid dev_file or context\n");
		return ret;
	}

	if (is_tee_rebooting()) {
		context->returns.code = TEE_ERROR_IS_DEAD;
		return TEE_ERROR_IS_DEAD;
	}

	service = get_service(dev_file, context);
	if (!service) {
		tloge("invalid service\n");
		return ret;
	}
	/*
	 * session_id in tee is unique, but in concurrency scene
	 * same session_id may appear in tzdriver, put session_list
	 * add/del in service->operation_lock can avoid it.
	 */
	if ((ret = lock_freezable(&service->operation_lock)) != 0) {
		put_service_struct(service);
		return ret;
	}
	session = get_session(service, dev_file, context);
	if (session) {
		if ((ret = lock_freezable(&session->ta_session_lock)) != 0) {
			put_session_struct(session);
			goto free_resource;
		}
		ret2 = close_session(dev_file, session, context->uuid,
			(unsigned int)UUID_LEN, context->session_id);
		mutex_unlock(&session->ta_session_lock);
		if (ret2 != 0)
			tloge("close session smc failed!\n");
		mutex_lock(&service->session_lock);
		list_del(&session->head);
		mutex_unlock(&service->session_lock);

		put_session_struct(session);
		put_session_struct(session); /* pair with open session */

		ret = 0;
		mutex_lock(&dev_file->service_lock);
		del_service_from_dev(dev_file, service);
		mutex_unlock(&dev_file->service_lock);
	} else {
		tloge("invalid session\n");
	}
free_resource:
	mutex_unlock(&service->operation_lock);
	put_service_struct(service);
	return ret;
}

static int check_param_types(struct tc_ns_client_context *context)
{
	int index;
	for (index = 0; index < TEE_PARAM_NUM; index++) {
		uint32_t param_type = teec_param_type_get(context->param_types, index);
		if (param_type == TEEC_MEMREF_REGISTER_INOUT) {
			tloge("invoke should not with register shm\n");
			return -EINVAL;
		}
	}
	return 0;
}

int tc_ns_send_cmd(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context)
{
	int ret = -EINVAL;
	struct tc_ns_service *service = NULL;
	struct tc_ns_session *session = NULL;
	struct tc_call_params params = {
		dev_file, context, NULL, 0
	};

	if (!dev_file || !context || (check_param_types(context) != 0)) {
		tloge("invalid dev_file or context or param_types\n");
		return ret;
	}

	if (is_tee_rebooting()) {
		context->returns.code = TEE_ERROR_IS_DEAD;
		return EFAULT;
	}

	service = get_service(dev_file, context);
	if (service) {
		session = get_session(service, dev_file, context);
		put_service_struct(service);
		if (session) {
			tlogd("send cmd find session id %x\n",
				context->session_id);
			goto find_session;
		}
		tloge("can't find session\n");
	} else {
		tloge("can't find service\n");
	}

	return ret;
find_session:
	ret = lock_freezable(&session->ta_session_lock);
	if (ret != 0) {
		put_session_struct(session);
		return ret;
	}
	params.sess = session;
	ret = tc_client_call(&params);
	mutex_unlock(&session->ta_session_lock);
	put_session_struct(session);
	if (ret != 0)
		tloge("smc call returns error, ret=0x%x\n", ret);
	return ret;
}

static int ioctl_session_send_cmd(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context, void *argp)
{
	int ret;

	ret = tc_ns_send_cmd(dev_file, context);
	if (ret != 0)
		tloge("send cmd failed ret is %d\n", ret);
	if (copy_to_user(argp, context, sizeof(*context)) != 0) {
		if (ret == 0)
			ret = -EFAULT;
	}
	return ret;
}

int tc_client_session_ioctl(struct file *file, unsigned int cmd,
	 unsigned long arg)
{
	int ret = -EINVAL;
	void *argp = (void __user *)(uintptr_t)arg;
	struct tc_ns_dev_file *dev_file = NULL;
	struct tc_ns_client_context context;

	if (!argp || !file) {
		tloge("invalid params\n");
		return -EINVAL;
	}

	dev_file = file->private_data;
	if (copy_from_user(&context, argp, sizeof(context)) != 0) {
		tloge("copy from user failed\n");
		return -EFAULT;
	}

	context.returns.origin = TEEC_ORIGIN_COMMS;
	switch (cmd) {
	case TC_NS_CLIENT_IOCTL_SES_OPEN_REQ:
		ret = tc_ns_open_session(dev_file, &context);
		if (ret != 0)
			tloge("open session failed ret is %d\n", ret);
		if (copy_to_user(argp, &context, sizeof(context)) != 0 && ret == 0)
			ret = -EFAULT;
		break;
	case TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ:
		ret = tc_ns_close_session(dev_file, &context);
		break;
	case TC_NS_CLIENT_IOCTL_SEND_CMD_REQ:
		tee_trace_add_event(INVOKE_CMD_START, 0);
		ret = ioctl_session_send_cmd(dev_file, &context, argp);
		tee_trace_add_event(INVOKE_CMD_END, 0);
		break;
	default:
		tloge("invalid cmd:0x%x!\n", cmd);
		return ret;
	}
	/*
	 * Don't leak ERESTARTSYS to user space.
	 *
	 * CloseSession is not reentrant, so convert to -EINTR.
	 * In other case, restart_syscall().
	 *
	 * It is better to call it right after the error code
	 * is generated (in tc_client_call), but kernel CAs are
	 * still exist when these words are written. Setting TIF
	 * flags for callers of those CAs is very hard to analysis.
	 *
	 * For kernel CA, when ERESTARTSYS is seen, loop in kernel
	 * instead of notifying user.
	 *
	 * P.S. ret code in this function is in mixed naming space.
	 * See the definition of ret. However, this function never
	 * return its default value, so using -EXXX is safe.
	 */
	if (ret == -ERESTARTSYS) {
		if (cmd == TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ)
			ret = -EINTR;
		else
			return restart_syscall();
	}
	return ret;
}

static void cleanup_session(struct tc_ns_service *service)
{
	struct tc_ns_session *session = NULL;
	struct tc_ns_session *session_tmp = NULL;

	if (!service)
		return;

	/* close unclosed session */
	if (list_empty(&service->session_list) == 0) {
		mutex_lock(&service->operation_lock);
		list_for_each_entry_safe(session, session_tmp, &service->session_list, head) {
			tlogd("clean up session %u\n", session->session_id);
			mutex_lock(&service->session_lock);
			list_del(&session->head);
			mutex_unlock(&service->session_lock);
			put_session_struct(session);
		}
		mutex_unlock(&service->operation_lock);
	}
	put_service_struct(service);

	return;
}

void free_all_session(void)
{
	struct tc_ns_dev_file *dev_file = NULL;
	struct tc_ns_dev_file *dev_file_tmp = NULL;
	struct tc_ns_dev_list *dev_list = NULL;
	int i;

	dev_list = get_dev_list();
	if (!dev_list) {
		tloge("cleanup session, dev list is null\n");
		return;
	}
	mutex_lock(&dev_list->dev_lock);
	list_for_each_entry_safe(dev_file, dev_file_tmp, &dev_list->dev_file_list, head) {
		mutex_lock(&dev_file->service_lock);
		for (i = 0; i < SERVICES_MAX_COUNT; i++) {
			if (dev_file->services[i] == NULL)
				continue;
			get_service_struct(dev_file->services[i]);
			/* avoid dead lock in close session */
			mutex_unlock(&dev_file->service_lock);
			cleanup_session(dev_file->services[i]);
			mutex_lock(&dev_file->service_lock);
			dev_file->services[i] = NULL;
		}
		mutex_unlock(&dev_file->service_lock);
	}
	mutex_unlock(&dev_list->dev_lock);
	return;
}

void load_image_lock(void)
{
	mutex_lock(&g_load_app_lock);
}

void load_image_unlock(void)
{
	mutex_unlock(&g_load_app_lock);
}
