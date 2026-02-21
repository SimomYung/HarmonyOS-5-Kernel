// SPDX-License-Identifier: GPL-2.0-only
#include <linux/err.h>
#include <liblinux/unimpl.h>

/* fs/aio.c */
LIBLINUX_EXPORTED_UNIMPL(kiocb_set_cancel_fn);

/* fs/block_dev.c */
LIBLINUX_EXPORTED_UNIMPL_RET(fsync_bdev, int, 0);
LIBLINUX_EXPORTED_UNIMPL_RET(sync_blockdev, int, 0);
LIBLINUX_EXPORTED_UNIMPL_RET(thaw_bdev, int, -ENOSYS);

/* fs/eventfd.c */
LIBLINUX_GPL_EXPORTED_UNIMPL(eventfd_ctx_put);
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(eventfd_ctx_fdget, void *, ERR_PTR(-ENOSYS));
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(eventfd_signal, __u64, 0);

/* fs/eventpoll.c */
LIBLINUX_UNIMPL(eventpoll_release_file);

/* fs/exec.c */
LIBLINUX_UNIMPL_RET(path_noexec, bool, false);

/* fs/file.c */
LIBLINUX_UNIMPL_RET(__fdget_raw, unsigned long, 0);
LIBLINUX_UNIMPL_RET(__fdget_pos, unsigned long, 0);
LIBLINUX_UNIMPL(__f_unlock_pos);
LIBLINUX_EXPORTED_UNIMPL_RET(fget_raw, void *, NULL);
LIBLINUX_UNIMPL_RET(__get_unused_fd_flags, int, -ENOSYS);
LIBLINUX_UNIMPL_RET(__receive_fd, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(iterate_fd, int, 0)
LIBLINUX_UNIMPL_RET(__close_range, int, -ENOSYS);
LIBLINUX_UNIMPL_RET(get_files_struct, void *, NULL);
LIBLINUX_UNIMPL(put_files_struct);

/* fs/fs-writeback.c */
LIBLINUX_EXPORTED_UNIMPL(inode_io_list_del);
LIBLINUX_EXPORTED_UNIMPL_RET(sync_inode_metadata, int, -ENOSYS);
LIBLINUX_UNIMPL(wb_workfn);
LIBLINUX_EXPORTED_UNIMPL_RET(write_inode_now, int, -ENOSYS);

/* fs/io-wq.c */
LIBLINUX_UNIMPL(io_wq_worker_running)
LIBLINUX_UNIMPL(io_wq_worker_sleeping)

/* fs/locks.c */
LIBLINUX_UNIMPL(locks_free_lock_context);
LIBLINUX_UNIMPL_RET(locks_mandatory_locked, int, 0);
LIBLINUX_UNIMPL(locks_remove_file);
LIBLINUX_EXPORTED_UNIMPL_RET(__break_lease, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(locks_mandatory_area, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL(locks_remove_posix);

/* fs/mpage.c */
LIBLINUX_EXPORTED_UNIMPL_RET(mpage_writepage, int, -ENOSYS);

/* fs/notify/fsnotify.c */
LIBLINUX_EXPORTED_UNIMPL_RET(fsnotify, int, -ENOSYS);
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(fsnotify_get_cookie, int, 0);
LIBLINUX_UNIMPL(fsnotify_sb_delete);
LIBLINUX_GPL_EXPORTED_UNIMPL(__fsnotify_inode_delete);
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(__fsnotify_parent, int, -ENOSYS);
LIBLINUX_UNIMPL(__fsnotify_vfsmount_delete);

/* fs/read_write.c */
LIBLINUX_UNIMPL_RET(generic_file_mmap, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(generic_file_read_iter, long, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(generic_file_write_iter, long, -ENOSYS);

/* fs/splice.o */
LIBLINUX_EXPORTED_UNIMPL_RET(iter_file_splice_write, long, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(generic_file_splice_read, long, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(do_splice_direct, long, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(generic_splice_sendpage, long, -ENOSYS);
LIBLINUX_UNIMPL_RET(splice_grow_spd, int, 0);
LIBLINUX_UNIMPL(splice_shrink_spd);
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(splice_to_pipe, long, -ENOSYS);
LIBLINUX_UNIMPL_RET(splice_from_pipe, ssize_t, -ENOSYS);

/* fs/sync.c */
LIBLINUX_EXPORTED_UNIMPL_RET(sync_filesystem, int, 0);
LIBLINUX_UNIMPL_RET(ksys_fsync, int, 0);
LIBLINUX_UNIMPL(ksys_sync);

/* fs/fcntl.c */
LIBLINUX_EXPORTED_UNIMPL_RET(fasync_helper, int, -1)
LIBLINUX_EXPORTED_UNIMPL(kill_fasync)
LIBLINUX_UNIMPL_RET(f_getown, int, -1);
LIBLINUX_EXPORTED_UNIMPL_RET(f_setown, int, -ENOSYS);
LIBLINUX_UNIMPL_RET(send_sigurg, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL(__f_setown)

/* fs/proc/base.c */
LIBLINUX_UNIMPL(proc_pid_evict_inode);
LIBLINUX_UNIMPL_RET(proc_pid_lookup, void *, ERR_PTR(-ENOENT));
LIBLINUX_UNIMPL_RET(proc_pid_readdir, int, -ENOSYS);

/* fs/proc/self.c */
LIBLINUX_UNIMPL_RET(proc_setup_self, int, 0);
LIBLINUX_UNIMPL_RET(proc_setup_thread_self, int, 0);

/* fs/select.c */
LIBLINUX_UNIMPL_RET(poll_select_set_timeout, int, -ENOSYS);

/* fs/nsfs.c */
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(open_related_ns, int, -ENOSYS);
LIBLINUX_UNIMPL_RET(proc_ns_fget, void *, NULL);
LIBLINUX_UNIMPL_RET(ns_get_path_cb, int, -ENOENT);
LIBLINUX_UNIMPL_RET(ns_match, bool, false);

/* fs/crypto/ */
LIBLINUX_EXPORTED_UNIMPL(fscrypt_enqueue_decrypt_work);
LIBLINUX_EXPORTED_UNIMPL_RET(fscrypt_decrypt_pagecache_blocks, int, -ENOSYS);
LIBLINUX_UNIMPL(fscrypt_sb_free);

/* fs/notify/dnotify/dnotify.c */
LIBLINUX_UNIMPL(dnotify_flush);

/* fs/drop_caches.c */
LIBLINUX_UNIMPL(cpa_drop_pagecache);

/* fs/file.c */
LIBLINUX_UNIMPL_RET(replace_fd, int, -ENOSYS);
