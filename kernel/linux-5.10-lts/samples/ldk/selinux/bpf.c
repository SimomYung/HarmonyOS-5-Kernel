// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/bpf.h>
#include <security.h>
#include <objsec.h>

#include "sec.h"
#include "av_permissions.h"

int __override security_bpf(int cmd, union bpf_attr *attr, unsigned int size)
{
	unsigned int sid;
	int ret;

	ret = sec_current_task_sid(&sid);
	if (ret == 0) {
		switch (cmd) {
		case BPF_MAP_CREATE:
			ret = sec_avc_perm(sid, SECCLASS_BPF, BPF__MAP_CREATE);
			break;
		case BPF_PROG_LOAD:
			ret = sec_avc_perm(sid, SECCLASS_BPF, BPF__PROG_LOAD);
			break;
		default:
			ret = 0;
			break;
		}
	}

	return ret;
}

static unsigned int bpf_map_fmode_to_av(fmode_t fmode)
{
	unsigned int av = 0;

	if (fmode & FMODE_READ)
		av |= BPF__MAP_READ;
	if (fmode & FMODE_WRITE)
		av |= BPF__MAP_WRITE;
	return av;
}

int __override security_bpf_map(struct bpf_map *map, fmode_t fmode)
{
	struct bpf_security_struct *bpfsec;

	bpfsec = map->security;
	return sec_avc_perm(bpfsec->sid, SECCLASS_BPF, bpf_map_fmode_to_av(fmode));
}

int __override security_bpf_prog(struct bpf_prog *prog)
{
	struct bpf_security_struct *bpfsec;

	bpfsec = prog->aux->security;
	return sec_avc_perm(bpfsec->sid, SECCLASS_BPF, BPF__PROG_RUN);
}

int __override security_bpf_map_alloc(struct bpf_map *map)
{
	struct bpf_security_struct *bpfsec;
	int ret;

	bpfsec = kzalloc(sizeof(*bpfsec), GFP_KERNEL);
	if (!bpfsec)
		return -ENOMEM;

	ret = sec_current_task_sid(&bpfsec->sid);
	if (ret == 0)
		map->security = bpfsec;
	else
		kfree(bpfsec);

	return ret;
}

void __override security_bpf_map_free(struct bpf_map *map)
{
	struct bpf_security_struct *bpfsec = map->security;

	map->security = NULL;
	kfree(bpfsec);
}

void __override security_bpf_prog_free(struct bpf_prog_aux *aux)
{
	struct bpf_security_struct *bpfsec = aux->security;

	aux->security = NULL;
	kfree(bpfsec);
}

int __override security_bpf_prog_alloc(struct bpf_prog_aux *aux)
{
	struct bpf_security_struct *bpfsec;
	int ret;

	bpfsec = kzalloc(sizeof(*bpfsec), GFP_KERNEL);
	if (!bpfsec)
		return -ENOMEM;

	ret = sec_current_task_sid(&bpfsec->sid);
	if (ret == 0)
		aux->security = bpfsec;
	else
		kfree(bpfsec);

	return ret;
}
