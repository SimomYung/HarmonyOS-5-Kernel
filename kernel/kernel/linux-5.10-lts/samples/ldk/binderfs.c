// SPDX-License-Identifier: GPL-2.0
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsync/raw_mutex_guard.h>

#include <devhost/log.h>
#include <vfs.h>

static struct raw_mutex g_binderfs_mutex = RAW_MUTEX_INITIALIZER;
static rref_t g_fs_rref = 0ULL;

bool check_binder_calling_work(int pid)
{
	int has_calling_work = 1; /* assume process have binder work for error */
	int err = E_HM_OK;
	rref_t rref;

	raw_mutex_lock(&g_binderfs_mutex);
	if (unlikely(g_fs_rref == 0ULL)) {
		err = hm_path_acquire(FSMGR_PATH_ACTV, &g_fs_rref);
		if (err != E_HM_OK) {
			dh_warn("failed to acquire actv vfs rref: %s\n",
				hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		rref = g_fs_rref;
	}
	raw_mutex_unlock(&g_binderfs_mutex);

	if (err == E_HM_OK) {
		err = vfs_proc_has_binder_work(rref, pid, &has_calling_work);
		if (err != E_HM_OK) {
			dh_error("check proc has binder work failed: %s\n",
				 hmstrerror(err));
			raw_mutex_lock(&g_binderfs_mutex);
			(void)hm_path_release(FSMGR_PATH_ACTV, g_fs_rref);
			g_fs_rref = 0ULL;
			raw_mutex_unlock(&g_binderfs_mutex);
		}
	}

	return (has_calling_work != 0);
}

EXPORT_SYMBOL_GPL(hwbinderinfo);
EXPORT_SYMBOL_GPL(hwkillinfo);
EXPORT_SYMBOL_GPL(hw_freeze_binder_info);
EXPORT_SYMBOL_GPL(hw_request_anco_thaw);
EXPORT_SYMBOL_GPL(check_binder_calling_work);
