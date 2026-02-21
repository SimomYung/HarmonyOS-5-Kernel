/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 06:59:36 2019
 */

#include <vfs.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/mount.h>
#include <libtest/test.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <libsysif/sysmgr/api.h>
#include <libhmucap/ucap.h>
#include <libstrict/strict.h>
#include <libvsyscall/hmvsyscall.h>
#include <libhmsrv_io/io.h>
#include <libhmsrv_io/mgmt.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_procmgr.h>

#define  ROOT_DEVID           (0)
#define  DEFAULT_CNODE_IDX    (~0U)
#define  FAT_DEV_BLKTEST1     "/dev/udkblk1"
#define  FAT_DEV_BLKTEST      "/dev/udkblk"
#define  FAT_MOUNT_POINT      "/udkblk"
#define  FAT_WR_FILE          "/udkblk/fat.txt"
#define  BIGFILE_LEN          (512 * 20 + 1)
#define  FS_TYPE              "vfat"
#define  NR_SECT_DISK         8192
#define  NR_SECT_PART         8191

static int udk_test_remove_test_file(const char *name, const char *test_case)
{
	int ret;

	ret = remove(name);
	if (ret) {
		hm_error("%s: remove %s failed: %d\n ",
			 test_case, name, errno);
		posix2hmerrno(ret);
	}

	return ret;
}

static int udk_test_rmdir_test_dir(const char *name, const char *test_case)
{
	int ret;

	ret = rmdir(name);
	if (ret) {
		hm_error("%s: rmdir %s failed: %d\n ", test_case, name, errno);
		posix2hmerrno(ret);
	}

	return ret;
}

static int udk_test_mount_fat(const char *fstype, int *mnt)
{
	int ret;

	ret = access(FAT_MOUNT_POINT, F_OK);
	if (ret != 0) {
		ret = mkdir(FAT_MOUNT_POINT, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (ret) {
			return posix2hmerrno(errno);
		}
	}
	*mnt = open(FAT_MOUNT_POINT, O_RDONLY);
	if (*mnt < 0) {
		return posix2hmerrno(errno);
	}

	/*mount fat /udkblk /dev/blktest1*/
	ret = mount(FAT_DEV_BLKTEST1, FAT_MOUNT_POINT, fstype, MS_SILENT, NULL);
	if (ret < 0) {
		hm_error("mount failed: errno:%d\n", errno);
		if (rmdir(FAT_MOUNT_POINT)) {
			hm_error("rmdir %s err, errno:%d\n",
				 FAT_MOUNT_POINT, errno);
		}
		return ret;
	}
	return ret;
}

static int udk_test_devmgr_insmod(const char *name, int devid)
{
	return hmsrv_io_mgmt_load_module(DM_MGMT_DEVID_TARGET(devid),
					 name, NULL);
}

static int udk_blk_test_rw(int *fatfd)
{
	int ret;
	int i;
	off64_t posr = 0;
	off64_t posw = 0;
	char *msgw = NULL;
	char *msgr = NULL;

	*fatfd = open(FAT_WR_FILE, O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (*fatfd < 0) {
		hm_error("create %s err,errno:%d\n", FAT_WR_FILE, errno);
		ret = E_HM_POSIX_NOENT;
		goto out;
	}

	msgw = (char *) malloc(BIGFILE_LEN);
	if (msgw == NULL) {
		hm_error("malloc failed.\n");
		ret = E_HM_NOMEM;
		goto out;
	}
	msgr = (char *) malloc(BIGFILE_LEN);
	if (msgr == NULL) {
		hm_error("malloc failed.\n");
		ret = E_HM_NOMEM;
		goto out;
	}

	for (i = 0; i < BIGFILE_LEN - 1; i++) {
		msgw[i] = 0x30 + i % 10;
	}
	msgw[BIGFILE_LEN - 1] = '\0';

	mem_zero_b(msgr, BIGFILE_LEN);

	posw = lseek(*fatfd, posw, SEEK_SET);
	if (posw < 0) {
		hm_error("lseek failed.\n");
		ret = E_HM_INVAL;
		goto out;
	}
	posw = write(*fatfd, msgw, BIGFILE_LEN);
	if (posw < 0) {
		hm_error("write failed.\n");
		ret = E_HM_INVAL;
		goto out;
	}

	hm_debug("blk_write_read_test read begin.\n");
	posr = lseek(*fatfd, posr, SEEK_SET);
	if (posr < 0) {
		hm_error("lseek failed.\n");
		ret = E_HM_INVAL;
		goto out;
	}
	posr = read(*fatfd, msgr, BIGFILE_LEN);
	if (posr < 0) {
		hm_error("read failed.\n");
		ret = E_HM_INVAL;
		goto out;
	}

	ret = strncmp(msgr, msgw, BIGFILE_LEN);
	if (ret) {
		hm_error("compare err! \n");
		ret = E_HM_INVAL;
	}
out:
	if (msgw != NULL) {
		free(msgw);
		msgw = NULL;
	}
	if (msgr != NULL) {
		free(msgr);
		msgr = NULL;
	}
	return ret;
}

static int udk_blk_test_ioctl(char *fname, unsigned long expected)
{
	unsigned long nblocks = 0;
	int fd = -1;
	int err = E_HM_OK;

	fd = open(fname, O_RDONLY);
	if (fd < 0) {
		if (errno == ENOENT) {
			return E_HM_OK;
		}
		hm_error("Failed to open %s errno %d\n", fname, errno);
		return E_HM_POSIX_NOENT;
	}

	if (ioctl(fd, BLKGETSIZE, &nblocks, sizeof(unsigned long)) < 0) {
		err = -errno;
		hm_error("Failed to ioctl %s errno %d\n", fname, errno);
		close(fd);
		return err;
	}
	close(fd);

	if (nblocks != expected) {
		hm_error("Incorrect nblocks %lu expected: %lu\n", nblocks,
			 expected);
		err = E_HM_INVAL;
	}

	return err;
}

static int udk_test_devmgr_rmmod(const char *name, unsigned int cnode_idx)
{
	return hmsrv_io_mgmt_unload_module(DM_MGMT_CIDX_TARGET(cnode_idx), name);
}

static int test_udk_block(void)
{
	const char *const mkfat_args[] = {"/usr/bin/mkfat.elf",
					  FAT_DEV_BLKTEST1, NULL};
	const char *const envp[] = {"PATH=/:/bin:/usr/bin", NULL};
	int mnt = -1;
	int fatfd = -1;
	pid_t pid = 0;
	int waitret = 0;
	int fd = -1;
	int ret;

	hm_debug("Begin to load udkblk-test.udk.\n");
	ret = udk_test_devmgr_insmod("udkblk-test.udk", ROOT_DEVID);
	if (ret != E_HM_OK) {
		hm_error("driver \"udkblk-test.udk\" failed: %d\n", ret);
		return ret;
	}

	hm_debug("Begin mkfat %s %s\n", FAT_MOUNT_POINT, FAT_DEV_BLKTEST1);
	fd = open(FAT_DEV_BLKTEST1, O_RDONLY);
	if (fd < 0) {
		hm_error("/dev/blktest1 is absent.\n");
		return fd;
	}

	ret = hm_spawn(&pid, mkfat_args[0], NULL, NULL, mkfat_args, envp);
	if (ret) {
		hm_error("Spawn mkfat.elf failed: %s\n", hmstrerror(ret));
		goto out;
	}

	/* wait mkfat.elf finish! */
	(void)hm_waitpid(pid, 0, &waitret);
	if (!WIFEXITED((unsigned int)waitret)) {
		hm_error("got wrong exit status: %d", waitret);
		goto out;
	}
	if (WEXITSTATUS(waitret)) {
		hm_error("mkfat failed: %d\n", WEXITSTATUS(waitret));
		goto out;
	}

	hm_debug("Begin mount fat %s %s\n", FAT_MOUNT_POINT, FAT_DEV_BLKTEST1);
	ret = udk_test_mount_fat(FS_TYPE, &mnt);
	if (ret != E_HM_OK) {
		hm_error("mount_fat failed.\n");
		goto out;
	}

	hm_debug("Begin write read file \n");
	ret = udk_blk_test_rw(&fatfd);
	if (ret != E_HM_OK) {
		hm_error("blk_write_read_test fat failed.\n");
		goto out;
	}

	hm_debug("Begin ioctl file %s \n", FAT_DEV_BLKTEST1);
	ret = udk_blk_test_ioctl(FAT_DEV_BLKTEST1, NR_SECT_PART);
	if (ret != E_HM_OK) {
		hm_error("blk_write_test_ioctl for device %s failed.\n",
			 FAT_DEV_BLKTEST1);
		goto out;
	}

	hm_debug("Begin ioctl file %s \n", FAT_DEV_BLKTEST);
	ret = udk_blk_test_ioctl(FAT_DEV_BLKTEST, NR_SECT_DISK);
	if (ret != E_HM_OK) {
		hm_error("blk_write_test_ioctl for device %s failed.\n",
			 FAT_DEV_BLKTEST);
	}

out:
	if (fatfd >= 0) {
		close(fatfd);
		ret = udk_test_remove_test_file(FAT_WR_FILE, "test_udk_block");
	}

	if (mnt >= 0) {
		close(mnt);
		hm_debug("begin unmount %s\n", FAT_MOUNT_POINT);
		ret = umount(FAT_MOUNT_POINT);
		if (ret < 0) {
			hm_error("umount fat failed. errno:%d\n", errno);
		}
		ret = udk_test_rmdir_test_dir(FAT_MOUNT_POINT,
					      "test_udk_block");
	}
	if (fd >= 0)
		close(fd);

	ret = udk_test_devmgr_rmmod("udkblk-test.udk", DEFAULT_CNODE_IDX);
	if (ret != E_HM_OK) {
		hm_error("failed unload \"udkblk-test.udk\", ret: %d\n", ret);
	}

	return ret;
}

int main(int argc, char **argv)
{
	int ret;
	__cpuset_t cpus = __cpuset_full;

	UNUSED(argc, argv);

	ret = set_process_affinity(cpus);
	if (ret) {
		return -1;
	}

	/* init devmgr mgmt */
	ret = hmsrv_io_init();
	if (ret < 0) {
		return -1;
	}

	ret = test_udk_block();
	if (ret != E_HM_OK) {
		return -1;
	}

	return 0;
}
