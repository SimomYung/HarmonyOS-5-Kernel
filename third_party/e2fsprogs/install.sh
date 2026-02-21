#!/bin/bash
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation version 2.1
# of the License.
#
# Copyright(c) 2023 Huawei Device Co., Ltd.

set -e
cd $1
touch test.lock
(
    flock -x 200
if [ -d "e2fsprogs" ];then
    exit 0
fi
tar xvf e2fsprogs.tar.xz
cd $1/e2fsprogs
patch -p1 < $1/0001-e2fsprogs-set-hugefile-from-4T-to-1T-in-hugefile-tes.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0002-libss-add-newer-libreadline.so.8-to-dlopen-path.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0003-tests-update-expect-files-for-f_mmp_garbage.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0004-tests-update-expect-files-for-f_large_dir-and-f_larg.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0005-resize2fs-resize2fs-disk-hardlinks-will-be-error.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0006-e2fsck-exit-journal-recovery-when-find-EIO-ENOMEM-er.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0007-e2fsck-exit-journal-recovery-when-jounral-superblock.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0008-e2fsck-add-env-param-E2FS_UNRELIABLE_IO-to-fi.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0009-e2mmpstatus.8.in-detele-filesystem-can-be-UUID-or-LA.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0010-tests-update-expect-file-for-u_direct_io.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0011-libext2fs-don-t-old-the-CACHE_MTX-while-doing-I-O.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0012-tests-fix-ACL-printing-tests.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0013-e2fsck-do-not-clean-up-file-acl-if-the-inode-is-trun.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0014-e2fsck-handle-level-is-overflow-in-ext2fs_extent_get.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0015-libext2fs-add-sanity-check-to-extent-manipulation.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0016-e2fsprogs-add-sw64.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0017-tune2fs-do-not-change-j_tail_sequence-in-journal-sup.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0018-debugfs-teach-logdump-the-n-num_trans-option.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0019-tune2fs-fix-tune2fs-segfault-when-ext2fs_run_ext3_jo.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0020-tune2fs-tune2fs_main-should-return-rc-when-some-erro.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0021-tune2fs-exit-directly-when-fs-freed-in-ext2fs_run_ext3_journal.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0022-unix_io.c-fix-deadlock-problem-in-unix_write_blk64.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0023-debugfs-fix-repeated-output-problem-with-logdump-O-n.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0024-tune2fs-check-return-value-of-ext2fs_mmp_update2-in-.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0025-mmp-fix-wrong-comparison-in-ext2fs_mmp_stop.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0026-misc-fsck.c-Processes-may-kill-other-processes.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0027-dumpe2fs-resize2fs-avoid-memory-leak-on-error-path.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0028-libext2fs-fix-memory-leak-in-error-path-while-openin.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0029-e2fsck-avoid-theoretical-null-dereference-in-end_pro.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0030-e2fsck-fix-potential-out-of-bounds-read-in-inc_ea_in.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0031-e2fsck-avoid-out-of-bounds-write-for-very-deep-exten.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0032-e2fsck-fix-potential-fencepost-error-in-e2fsck_shoul.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0033-libext2fs-fix-potential-integer-overflow-in-bitmap-a.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0034-tune2fs-fix-an-error-message.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0035-e2fsck-don-t-allow-journal-inode-to-have-encrypt-fla.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0036-lib-ext2fs-fix-unbalanced-mutex-unlock-for-BOUNCE_MT.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0037-libext2fs-fix-ext2fs_compare_generic_bmap-logic.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0038-Quiet-unused-variable-warnings.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0039-ext2fs-Use-64bit-lseek-when-_FILE_OFFSET_BITS-is-64.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0040-e2fsck-fix-bad-htree-checksums-in-preen-mode.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/0041-debugfs-Fix-infinite-loop-when-dump-log.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/1001-image-make.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/1002-add-header-file-to-musl-compile-mk2efs.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/1003-add-dac-config.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/1004-modify-code-to-compile.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/1005-read-vfat-chinese-label.patch --fuzz=0 --no-backup-if-mismatch
exit 0
)200>$1/test.lock
