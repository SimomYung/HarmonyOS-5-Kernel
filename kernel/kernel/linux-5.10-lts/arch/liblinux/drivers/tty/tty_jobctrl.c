// SPDX-License-Identifier: GPL-2.0-only
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/tty.h>

#include "tty.h"

int __tty_check_change(struct tty_struct *tty, int sig)
{
	int sid;
	int pgid;
	int ret = 0;

	ret = liblinux_pal_get_sid_pgid(&sid, &pgid);
	if (ret < 0)
		return -EIO;

	if (sid != tty->hmsid) {
		pr_debug("tty: current process does not belong to current tty, "
			 "pid=%d, sid=%d\n", current->pid, sid);
		return 0;
	}

	if (tty->hmpgrp != 0 && pgid != tty->hmpgrp) {
		if (liblinux_pal_signal_isignored(pgid, sig)) {
			if (sig == SIGTTIN)
				ret = -EIO;
		} else if (liblinux_pal_current_pgrp_isorphaned()) {
			ret = -EIO;
		} else {
			tty_debug(tty, "send sig %d to pgid %d which is not foregound process\n",
				  sig, -pgid);
			liblinux_pal_process_signal_with_pid(-pgid, sig);
			ret = -ERESTARTSYS;
		}
	}

	if (tty->hmpgrp == 0)
		tty_warn(tty, "sig=%d, tty->hmpgrp is NULL!\n", sig);

	return ret;
}

int tty_check_change(struct tty_struct *tty)
{
	return __tty_check_change(tty, SIGTTOU);
}
EXPORT_SYMBOL(tty_check_change);

/* the caller function should tty_lock*/
void tty_open_proc_set_tty(struct file *filp, struct tty_struct *tty)
{
	int rsid;
	int ret;
	struct tty_info info;

	rsid = liblinux_pal_get_sid();
	if (rsid < 0)
		return;

	if (rsid == tty->hmsid)
		return;

	if ((filp->f_mode & FMODE_READ) == 0)
		return;

	info.tty_sid = tty->hmsid;
	info.tty_dev_no = (unsigned int)tty_devnum(tty);
	info.tty_name = tty->name;
	info.f_mode = filp->f_mode & FMODE_READ;
	ret = liblinux_pal_set_control_terminal(&info, 0);
	if (ret == 0) {
		tty->hmsid = rsid;
		tty->hmpgrp = rsid;
	}
}

struct tty_struct *get_current_tty(void)
{
	dev_t devno;
	int ret;
	struct tty_struct *tty = NULL;

	ret = liblinux_pal_get_control_terminal_devno(&devno);
	if (ret < 0) {
		return NULL;
	}

	tty = __lookup_tty(devno);
	if (IS_ERR(tty)) {
		return NULL;
	}

	return tty;
}
EXPORT_SYMBOL_GPL(get_current_tty);

void session_clear_tty(struct pid *session) {}

int tty_signal_session_leader(struct tty_struct *tty, int exit_session)
{
	if (tty->hmsid) {
		liblinux_pal_process_signal_with_pid(-tty->hmsid, SIGHUP);
		liblinux_pal_process_signal_with_pid(-tty->hmsid, SIGCONT);
	}

	if (exit_session && tty->hmpgrp && tty->hmpgrp != tty->hmsid) {
		liblinux_pal_process_signal_with_pid(-tty->hmpgrp, SIGHUP);
	}

	return 0;
}

static int tiocsctty(struct tty_struct *tty, struct file *file, int arg)
{
	int ret = 0;
	int rsid;
	struct tty_info info;

	rsid = liblinux_pal_get_sid();
	if (rsid < 0)
		return -ESRCH;

	info.tty_sid = tty->hmsid;
	info.tty_dev_no = (unsigned int)tty_devnum(tty);
	info.tty_name = tty->name;
	info.f_mode = file->f_mode & FMODE_READ;
	tty_lock(tty);
	ret = liblinux_pal_set_control_terminal(&info, arg);
	if (ret < 0) {
		tty_debug(tty, "set terminal failed, sid=%d, tty name=%s, devnum=%u, f_mode=%u, arg=%d\n",
			  tty->hmsid, tty->name, (unsigned int)tty_devnum(tty), info.f_mode, arg);
	} else {
		tty->hmsid = rsid;
		tty->hmpgrp = rsid;
	}
	tty_unlock(tty);
	return ret;
}

struct pid *tty_get_pgrp(struct tty_struct *tty)
{
	return NULL;
}
EXPORT_SYMBOL_GPL(tty_get_pgrp);

void no_tty(void) {}

static int tiocgpgrp(struct tty_struct *tty, struct tty_struct *real_tty, pid_t __user *p)
{
	int ret;
	int pgrp;
	int rsid;
	int tty_pgrp;

	if (tty != real_tty) {
		pgrp = real_tty->hmpgrp;
		goto out;
	}

	rsid = liblinux_pal_get_sid();
	if (rsid < 0)
		return rsid;

	tty_lock(tty);
	if (rsid != tty->hmsid) {
		tty_unlock(tty);
		return -ENOTTY;
	}
	tty_pgrp = tty->hmpgrp;
	tty_unlock(tty);
	pgrp = liblinux_pal_get_pgrp(rsid);
	if (pgrp < 0)
		return pgrp;

	if (tty_pgrp != pgrp)
		return -EIO;
out:
	ret = put_user((pid_t)pgrp, p);
	if (ret != 0)
		tty_debug(tty, "put_user failed, ret=%d\n", ret);

	return ret;
}

static int tiocspgrp(struct tty_struct *tty, struct tty_struct *real_tty, pid_t __user *p)
{
	int ret;
	pid_t pgrp = 0;
	int rsid;

	if (get_user(pgrp, p))
		return -EFAULT;

	if (pgrp < 0)
		return -EINVAL;

	rsid = liblinux_pal_get_sid();
	if (rsid < 0)
		return rsid;

	tty_lock(real_tty);
	if (rsid != real_tty->hmsid) {
		tty_unlock(real_tty);
		return -ENOTTY;
	}
	tty_unlock(real_tty);

	ret = liblinux_pal_set_pgrp(rsid, (int)pgrp);
	if (ret != 0) {
		tty_err(real_tty, "cannot set pgrp, ret=%d\n", ret);
		return ret;
	}
	tty_lock(real_tty);
	real_tty->hmpgrp = (int)pgrp;
	tty_unlock(real_tty);
	return ret;
}

static int tiocgsid(struct tty_struct *tty, struct tty_struct *real_tty, pid_t __user *p)
{
	int rsid;

	if (tty != real_tty) {
		rsid = real_tty->hmsid;
		goto out;
	}

	rsid = liblinux_pal_get_sid();
	if (rsid < 0)
		return rsid;

	tty_lock(tty);
	if (rsid != tty->hmsid) {
		tty_unlock(tty);
		return -ENOTTY;
	}
	tty_unlock(tty);
out:
	return put_user((pid_t)rsid, p);
}

long tty_jobctrl_ioctl(struct tty_struct *tty, struct tty_struct *real_tty,
		       struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *p = (void __user *)arg;

	switch (cmd) {
	case TIOCSCTTY:
		return tiocsctty(real_tty, file, arg);
	case TIOCGPGRP:
		return tiocgpgrp(tty, real_tty, p);
	case TIOCSPGRP:
		return tiocspgrp(tty, real_tty, p);
	case TIOCGSID:
		return tiocgsid(tty, real_tty, p);
	}
	return -ENOIOCTLCMD;
}
