// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init.h>
#include <linux/export.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/hash.h>
#include <linux/bitops.h>
#include <linux/dcache.h>

#include <trace/hooks/liblinux.h>
#include <asm/word-at-a-time.h>
#ifdef HASH_MIX
#elif defined(CONFIG_64BIT)
#define HASH_MIX(x, y, a)       \
		(       x ^= (a),       \
		y ^= x, x = rol64(x,12),\
		x += y, y = rol64(y,45),\
		y *= 9                  )

static inline unsigned int fold_hash(unsigned long x, unsigned long y)
{
	y ^= x * GOLDEN_RATIO_64;
	y *= GOLDEN_RATIO_64;
	return y >> 32;
}
#else   /* 32-bit case */
#define HASH_MIX(x, y, a)       \
		(       x ^= (a),       \
		y ^= x, x = rol32(x, 7),\
		x += y, y = rol32(y,20),\
		y *= 9                  )

static inline unsigned int fold_hash(unsigned long x, unsigned long y)
{
	/* Use arch-optimized multiply if one exists */
	return __hash_32(y ^ __hash_32(x));
}
#endif

int __override inode_permission(struct inode *inode, int mask)
{
	return 0;
}

static inline int d_revalidate(struct dentry *dentry, unsigned int flags)
{
	if (unlikely(dentry->d_flags & DCACHE_OP_REVALIDATE))
		return dentry->d_op->d_revalidate(dentry, flags);
	else
		return 1;
}
/*
 * This looks up the name in dcache and possibly revalidates the found dentry.
 * NULL is returned if the dentry does not exist in the cache.
 */
static struct dentry *lookup_dcache(const struct qstr *name,
				    struct dentry *dir,
				    unsigned int flags)
{
	struct dentry *dentry = d_lookup(dir, name);
	if (dentry) {
		int error = d_revalidate(dentry, flags);
		if (unlikely(error <= 0)) {
			if (!error)
				d_invalidate(dentry);
			dput(dentry);
			return ERR_PTR(error);
		}
	}
	return dentry;
}

static
struct dentry *__lookup_hash(const struct qstr *name,
		struct dentry *base, unsigned int flags)
{
	struct dentry *dentry = lookup_dcache(name, base, flags);
	struct dentry *old;
	struct inode *dir = base->d_inode;

	if (dentry)
		return dentry;

	/* Don't create child dentry for a dead directory. */
	if (unlikely(IS_DEADDIR(dir)))
		return ERR_PTR(-ENOENT);

	dentry = d_alloc(base, name);
	if (unlikely(!dentry))
		return ERR_PTR(-ENOMEM);

	old = dir->i_op->lookup(dir, dentry, flags);
	if (unlikely(old)) {
		dput(dentry);
		dentry = old;
	}
	return dentry;
}
/*
 * Calculate the length and hash of the path component, and
 * return the "hash_len" as the result.
 */
static inline u64 hash_name(const void *salt, const char *name)
{
	unsigned long a = 0, b, x = 0, y = (unsigned long)salt;
	unsigned long adata, bdata, mask, len;
	const struct word_at_a_time constants = WORD_AT_A_TIME_CONSTANTS;
	unsigned long name_len = strlen(name);
	unsigned long tmp_len = 0;
	const char *tmp = NULL;

	len = 0;
	goto inside;

	do {
		HASH_MIX(x, y, a);
		len += sizeof(unsigned long);
inside:
		/* Handle possible out-of-bounds access situations */
		tmp = name + len;
		tmp_len = name_len - len;
		if (tmp_len < sizeof(unsigned long)) {
			a = 0;
			memcpy(&a, tmp, tmp_len);
		} else {
			a = load_unaligned_zeropad(tmp);
		}
		b = a ^ REPEAT_BYTE('/');
	} while (!(has_zero(a, &adata, &constants) | has_zero(b, &bdata, &constants)));

	adata = prep_zero_mask(a, adata, &constants);
	bdata = prep_zero_mask(b, bdata, &constants);
	mask = create_zero_mask(adata | bdata);
	x ^= a & zero_bytemask(mask);

	return hashlen_create(fold_hash(x, y), len + find_zero(mask));
}

static void hook_user_path_create(void *args, const char __user *pathname, struct path *path,
				 unsigned int lookup_flags, struct dentry **pdentry)
{
	struct filename *name = getname(pathname);
	struct qstr last;
	char *parent_path;

	if (IS_ERR(name)) {
		*pdentry = ERR_PTR(-ENOENT);
		return;
	}
	last.name = kbasename(name->name);
	if (last.name == name->name) {
		*pdentry = ERR_PTR(-ENOENT);
		goto fail;
	}

	parent_path = kstrndup(name->name, strlen(name->name) - strlen(last.name) - 1, GFP_KERNEL);
	if (parent_path == NULL) {
		*pdentry = ERR_PTR(-ENOMEM);
		goto fail;
	}
	lookup_flags &= LOOKUP_REVAL;
	path->dentry = liblinux_pal_gdentry(parent_path);
	if (path->dentry == NULL) {
		*pdentry = ERR_PTR(-ENOENT);
		goto out;
	}

	path->mnt = NULL;
	last.hash_len = hash_name(path->dentry, last.name);
	lookup_flags |= LOOKUP_CREATE | LOOKUP_EXCL;
	inode_lock_nested(path->dentry->d_inode, I_MUTEX_PARENT);
	*pdentry = __lookup_hash(&last, path->dentry, lookup_flags);
	if (IS_ERR(*pdentry))
		inode_unlock(path->dentry->d_inode);

out:
	kfree(parent_path);
fail:
	putname(name);
}
INIT_VENDOR_HOOK(ldk_vh_user_path_create, hook_user_path_create);

static void hook_user_path_at(void *args, const char __user *pathname, struct path *path, int *ret)
{
	struct filename *name = getname(pathname);

	if (IS_ERR(name)) {
		*ret = PTR_ERR(name);
		return;
	}

	if (path == NULL) {
		*ret = -ENOENT;
		goto out;
	}

	path->dentry = liblinux_pal_gdentry(name->name);
	if (path->dentry == NULL) {
		*ret = -ENOENT;
	} else {
		*ret = 0;
		path->mnt = NULL;
	}

out:
	putname(name);
}
INIT_VENDOR_HOOK(ldk_vh_user_path_at, hook_user_path_at);

static void hook_inode_permission(void *args, const char *pathname, bool user, int mask, int *ret)
{
	struct filename *name;

	if (user)
		name = getname(pathname);
	else
		name = getname_kernel(pathname);

	if (IS_ERR(name)) {
		*ret = PTR_ERR(name);
		return;
	}

	*ret = liblinux_pal_ipermission(name->name, mask);
	putname(name);
}
INIT_VENDOR_HOOK(ldk_vh_inode_permission, hook_inode_permission);

static void hook_touch_atime(void *args, struct path *path)
{
	struct inode *inode = d_inode(path->dentry);
	inode->i_flags |= S_NOATIME;
}
INIT_VENDOR_HOOK(ldk_vh_touch_atime, hook_touch_atime);

static void hook_done_path_create(void *args, struct path *path, struct dentry *dentry)
{
	dput(dentry);
	inode_unlock(path->dentry->d_inode);
	path_put(path);
}
INIT_VENDOR_HOOK(ldk_vh_done_path_create, hook_done_path_create);
