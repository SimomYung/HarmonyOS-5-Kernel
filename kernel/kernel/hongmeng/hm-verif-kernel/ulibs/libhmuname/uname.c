/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HM LDK uname
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 9 09:45:52 2022
 */

#include <vfs.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>

#define HM_OSRELEASE_PATH	"/proc/devhost/root/sys/kernel/osrelease"
#define DATA_SIZE	50

int uname(struct utsname *__name)
{
	int ret = 0;
	char data[DATA_SIZE] = {0};
	int fd = -1;
	ssize_t len = 0;

	fd = vfs_open(HM_OSRELEASE_PATH, O_RDONLY, 0);
	if (fd < 0) {
		hm_error("HM-LDK osrelease open '%s' failed: %d\n", HM_OSRELEASE_PATH, fd);
		return -hmerrno2posix(fd);
	}

	len = vfs_read(fd, data, sizeof(data));
	if (len < 0) {
		hm_error("HM-LDK osrelease read failed: %ld\n", (long)len);
		(void)vfs_close(fd);
		return -EINVAL;
	}

	if (strncpy_s(__name->release, sizeof(__name->release), data, (size_t)len) != 0) {
		hm_error("HM-LDK copy failed: %d\n", ret);
		ret = -EINVAL;
	}

	(void)vfs_close(fd);
	return ret;
}
