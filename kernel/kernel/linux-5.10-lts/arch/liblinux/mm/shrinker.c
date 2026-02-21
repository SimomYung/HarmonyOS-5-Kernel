// SPDX-License-Identifier: GPL-2.0-only
/*
 *  part of vmscan.c which use to register shrinker and invoke shrinkers
 */

#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/swap.h>
#ifdef CONFIG_LIBLINUX_SYS_UNION_SHRINKER
#include <linux/cpuset.h>
#include <linux/memcontrol.h>
#include <linux/mm_inline.h>

#if defined(CONFIG_PROC_FS)
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#endif

#include "internal.h" /* mm/internal.h */
#include "mm_internal.h" /* liblinux/mm/mm_internal.h */
#endif

#include "page_alloc.h"

static unsigned long shrinker_query(void *priv)
{
	unsigned long ret;
	struct shrinker *shrinker = (struct shrinker *)priv;
	struct shrink_control shrinkctl = {
		.gfp_mask = GFP_KERNEL,
		.nr_to_scan = 0,
	};

	if (shrinker == NULL) {
		pr_err("invalid shrinker from sub_shrinker\n");
		return 0;
	}

	ret = shrinker->count_objects(shrinker, &shrinkctl);
	if (ret == SHRINK_EMPTY)
		ret = 0;

	return ret;
}

static unsigned long shrinker_shrink(void *priv, int max_nr)
{
	unsigned long ret;
	struct shrinker *shrinker = (struct shrinker *)priv;
	struct shrink_control shrinkctl = {
		.gfp_mask = GFP_KERNEL,
		.nr_to_scan = max_nr,
	};

	if (shrinker == NULL) {
		pr_err("invalid shrinker from sub_shrinker\n");
		return 0;
	}

	ret = shrinker->scan_objects(shrinker, &shrinkctl);
	if (ret == SHRINK_STOP)
		ret = 0;
	return ret;
}

#ifndef CONFIG_LIBLINUX_SYS_UNION_SHRINKER
static void register_pool_sub_shrink(struct shrinker* shrinker)
{
	struct liblinux_pal_sub_shrinker sub_shrinker;
	struct liblinux_page_allocator *page_pool_main = NULL;

	sub_shrinker.priv = shrinker;
	sub_shrinker.query = shrinker_query;
	sub_shrinker.shrink = shrinker_shrink;
	if (!shrinker->is_stand_alone) {
		page_pool_main = default_page_allocator();
	}

	liblinux_pal_sub_shrinker_register(page_pool_main, sub_shrinker);
}

static void unregister_pool_sub_shrink(struct shrinker* shrinker)
{
	struct liblinux_pal_sub_shrinker sub_shrinker;
	struct liblinux_page_allocator *page_pool_main = NULL;

	sub_shrinker.priv = shrinker;
	if (!shrinker->is_stand_alone) {
		page_pool_main = default_page_allocator();
	}

	liblinux_pal_sub_shrinker_unregister(page_pool_main, sub_shrinker);
}

int prealloc_shrinker(struct shrinker *shrinker)
{
	unsigned int size = sizeof(*shrinker->nr_deferred);

	if (shrinker->flags & SHRINKER_NUMA_AWARE)
		size *= nr_node_ids;

	shrinker->nr_deferred = kzalloc(size, GFP_KERNEL);
	if (!shrinker->nr_deferred)
		return -ENOMEM;

	return 0;
}

void free_prealloced_shrinker(struct shrinker *shrinker)
{
	if (!shrinker->nr_deferred)
		return;

	kfree(shrinker->nr_deferred);
	shrinker->nr_deferred = NULL;
}

void register_shrinker_prepared(struct shrinker *shrinker)
{
	register_pool_sub_shrink(shrinker);
}

int register_shrinker(struct shrinker *shrinker)
{
	int err = prealloc_shrinker(shrinker);

	if (err)
		return err;

	register_shrinker_prepared(shrinker);
	return 0;
}
EXPORT_SYMBOL(register_shrinker);

void unregister_shrinker(struct shrinker *shrinker)
{
	if (!shrinker->nr_deferred)
		return;

	unregister_pool_sub_shrink(shrinker);
	kfree(shrinker->nr_deferred);
	shrinker->nr_deferred = NULL;
}
EXPORT_SYMBOL(unregister_shrinker);

#else /* CONFIG_LIBLINUX_SYS_UNION_SHRINKER */

static LIST_HEAD(shrinker_list);
static DECLARE_RWSEM(shrinker_rwsem);
#define SHRINK_BATCH 128
atomic_long_t g_shrinkers_shrinked_pages = ATOMIC_LONG_INIT(0);

static bool shrink_zone_is_suspend(struct zone *zone)
{
	unsigned long wmark = high_wmark_pages(zone);
	unsigned long free = zone_page_state(zone, NR_FREE_PAGES);
	if (free > wmark)
		return true;
	else
		return false;
}

static unsigned long do_shrink_slab(struct shrink_control *shrinkctl,
				    struct shrinker *shrinker, int priority)
{
	unsigned long freed = 0;
	unsigned long long delta;
	long total_scan;
	long freeable;
	long nr;
	long new_nr;
	int nid = shrinkctl->nid;
	long batch_size = shrinker->batch ? shrinker->batch
					  : SHRINK_BATCH;
	long scanned = 0, next_deferred;

	if (!(shrinker->flags & SHRINKER_NUMA_AWARE))
		nid = 0;

	freeable = shrinker->count_objects(shrinker, shrinkctl);
	if (freeable == 0 || freeable == SHRINK_EMPTY)
		return freeable;

	if (unlikely(freeable < 0)) {
		pr_err("shrink_slab: %pS freeable = %ld is negative, nr_deferred = %ld\n",
			shrinker->scan_objects, freeable,
			atomic_long_read(&shrinker->nr_deferred[nid]));
		return 0;
	}

	/*
	 * copy the current shrinker scan count into a local variable
	 * and zero it so that other concurrent shrinker invocations
	 * don't also do this scanning work.
	 */
	nr = atomic_long_xchg(&shrinker->nr_deferred[nid], 0);

	total_scan = nr;
	if (shrinker->seeks) {
		delta = freeable >> priority;
		delta *= 4;
		do_div(delta, shrinker->seeks);
	} else {
		/*
		 * These objects don't require any IO to create. Trim
		 * them aggressively under memory pressure to keep
		 * them from causing refetches in the IO caches.
		 */
		delta = freeable / 2;
	}

	total_scan += delta;
	if (total_scan < 0) {
		pr_err("shrink_slab: %pS negative objects to delete nr=%ld\n",
		       shrinker->scan_objects, total_scan);
		total_scan = freeable;
		next_deferred = nr;
	} else
		next_deferred = total_scan;

	/*
	 * We need to avoid excessive windup on filesystem shrinkers
	 * due to large numbers of GFP_NOFS allocations causing the
	 * shrinkers to return -1 all the time. This results in a large
	 * nr being built up so when a shrink that can do some work
	 * comes along it empties the entire cache due to nr >>>
	 * freeable. This is bad for sustaining a working set in
	 * memory.
	 *
	 * Hence only allow the shrinker to scan the entire cache when
	 * a large delta change is calculated directly.
	 */
	if (delta < freeable / 4)
		total_scan = min(total_scan, freeable / 2);

	/*
	 * Avoid risking looping forever due to too large nr value:
	 * never try to free more than twice the estimate number of
	 * freeable entries.
	 */
	if (total_scan > freeable * 2)
		total_scan = freeable * 2;

	/*
	 * Normally, we should not scan less than batch_size objects in one
	 * pass to avoid too frequent shrinker calls, but if the slab has less
	 * than batch_size objects in total and we are really tight on memory,
	 * we will try to reclaim all available objects, otherwise we can end
	 * up failing allocations although there are plenty of reclaimable
	 * objects spread over several slabs with usage less than the
	 * batch_size.
	 *
	 * We detect the "tight on memory" situations by looking at the total
	 * number of objects we want to scan (total_scan). If it is greater
	 * than the total number of objects on slab (freeable), we must be
	 * scanning at high prio and therefore should try to reclaim as much as
	 * possible.
	 */
	while (total_scan >= batch_size ||
	       total_scan >= freeable) {
		unsigned long ret;
		unsigned long nr_to_scan = min(batch_size, total_scan);

		shrinkctl->nr_to_scan = nr_to_scan;
		shrinkctl->nr_scanned = nr_to_scan;
		ret = shrinker->scan_objects(shrinker, shrinkctl);
		if (ret == SHRINK_STOP)
			break;

		freed += ret;
		count_vm_events(SLABS_SCANNED, shrinkctl->nr_scanned);
		total_scan -= shrinkctl->nr_scanned;
		scanned += shrinkctl->nr_scanned;

		cond_resched();
	}

	if (next_deferred >= scanned)
		next_deferred -= scanned;
	else
		next_deferred = 0;
	/*
	 * move the unused scan count back into the shrinker in a
	 * manner that handles concurrent updates. If we exhausted the
	 * scan, there is no need to do an update.
	 */
	if (next_deferred > 0)
		new_nr = atomic_long_add_return(next_deferred,
						&shrinker->nr_deferred[nid]);
	else
		new_nr = atomic_long_read(&shrinker->nr_deferred[nid]);

	return freed;
}

static unsigned long shrink_zone(struct zone *zone, struct scan_control *sc)
{
	unsigned long ret, freed = 0;
	struct shrinker *shrinker;

	if (!down_read_trylock(&shrinker_rwsem))
		goto out;

	list_for_each_entry(shrinker, &shrinker_list, list) {
		struct shrink_control shctl = {
			.gfp_mask = sc->gfp_mask,
			.nid = zone->zone_pgdat->node_id,
		};

		if(shrink_zone_is_suspend(zone))
			break;

		ret = do_shrink_slab(&shctl, shrinker, sc->priority);
		if (ret == SHRINK_EMPTY)
			ret = 0;

		freed += ret;

		/*
		 * Bail out if someone want to register a new shrinker to
		 * prevent the registration from being stalled for long periods
		 * by parallel ongoing shrinking.
		 */
		if (rwsem_is_contended(&shrinker_rwsem)) {
			freed = freed ? : 1;
			break;
		}
	}

	up_read(&shrinker_rwsem);
out:
	cond_resched();
	return freed;
}

void shrink_zones(struct zonelist *zonelist, struct scan_control *sc)
{
	struct zoneref *z;
	struct zone *zone;
	pg_data_t *last_pgdat = NULL;

	for_each_zone_zonelist_nodemask(zone, z, zonelist,
					sc->reclaim_idx, sc->nodemask) {
		/*
		 * Shrink each node in the zonelist once. If the
		 * zonelist is ordered by zone (not the default) then a
		 * node may be shrunk multiple times but in that case
		 * the user prefers lower zones being preserved.
		*/
		if (zone->zone_pgdat == last_pgdat)
			continue;

		last_pgdat = zone->zone_pgdat;
		sc->nr_reclaimed += shrink_zone(zone, sc);
	}
	atomic_long_add(sc->nr_reclaimed, &g_shrinkers_shrinked_pages);
}

/*
 * Add a shrinker callback to be called from the vm.
 */
int prealloc_shrinker(struct shrinker *shrinker)
{
	unsigned int size = sizeof(*shrinker->nr_deferred);

	if (shrinker->flags & SHRINKER_NUMA_AWARE)
		size *= nr_node_ids;

	shrinker->nr_deferred = kzalloc(size, GFP_KERNEL);
	if (!shrinker->nr_deferred)
		return -ENOMEM;

	return 0;

free_deferred:
	kfree(shrinker->nr_deferred);
	shrinker->nr_deferred = NULL;
	return -ENOMEM;
}

void free_prealloced_shrinker(struct shrinker *shrinker)
{
	if (!shrinker->nr_deferred)
		return;

	kfree(shrinker->nr_deferred);
	shrinker->nr_deferred = NULL;
}

void register_shrinker_prepared(struct shrinker *shrinker)
{
	down_write(&shrinker_rwsem);
	list_add_tail(&shrinker->list, &shrinker_list);
	shrinker->flags |= SHRINKER_REGISTERED;
	up_write(&shrinker_rwsem);
}

static int register_pool_sub_shrink(struct shrinker* shrinker)
{
	struct liblinux_pal_sub_shrinker sub_shrinker;

	sub_shrinker.priv = shrinker;
	sub_shrinker.query = shrinker_query;
	sub_shrinker.shrink = shrinker_shrink;

	liblinux_pal_sub_shrinker_register(NULL, sub_shrinker);

	return 0;
}

static void unregister_pool_sub_shrink(struct shrinker* shrinker)
{
	struct liblinux_pal_sub_shrinker sub_shrinker;

	sub_shrinker.priv = shrinker;

	liblinux_pal_sub_shrinker_unregister(NULL, sub_shrinker);
}

int register_shrinker(struct shrinker *shrinker)
{
	if (shrinker->is_stand_alone)
		return register_pool_sub_shrink(shrinker);

	int err = prealloc_shrinker(shrinker);
	if (err)
		return err;

	register_shrinker_prepared(shrinker);
	return 0;
}
EXPORT_SYMBOL(register_shrinker);

/*
 * Remove one
 */
void unregister_shrinker(struct shrinker *shrinker)
{
	if (shrinker->is_stand_alone)
		return unregister_pool_sub_shrink(shrinker);

	if (!(shrinker->flags & SHRINKER_REGISTERED))
		return;

	down_write(&shrinker_rwsem);
	list_del(&shrinker->list);
	shrinker->flags &= ~SHRINKER_REGISTERED;
	up_write(&shrinker_rwsem);

	kfree(shrinker->nr_deferred);
	shrinker->nr_deferred = NULL;
}
EXPORT_SYMBOL(unregister_shrinker);

#if defined(CONFIG_PROC_FS)
static int shrinker_show(struct seq_file *s, void *d)
{
	long total_freeable = 0;
	long freeable = 0;
	unsigned int nr_shrinkers = 0;
	struct shrinker *shrinker;

	seq_printf(s, "Shrinkers Freeable Pages: \n");

	if (!down_read_trylock(&shrinker_rwsem))
		goto out;

	list_for_each_entry(shrinker, &shrinker_list, list) {
		struct shrink_control shctl = {
			.gfp_mask = GFP_KERNEL,
			.nid = 0,
		};

		freeable = shrinker->count_objects(shrinker, &shctl);
		if (freeable > 0) {
			seq_printf(s, "  no.%d: %16ld\n", nr_shrinkers, freeable);
			total_freeable += freeable;
		}

		nr_shrinkers++;
	}

	up_read(&shrinker_rwsem);

	seq_printf(s, "Total shrinkers: %d, Total freeable pages: %16ld\n",
			   nr_shrinkers, total_freeable);
	seq_printf(s, "%16s %16lld\n", "Shrinkers shrinked pages",
			   atomic_long_read(&g_shrinkers_shrinked_pages));
out:
	cond_resched();
	return 0;
}
static int shrinker_open(struct inode *inode, struct file *file)
{
	return single_open(file, shrinker_show, inode->i_private);
}

static const struct proc_ops shrinker_proc_ops = {
	.proc_open	= shrinker_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

static int __init page_pool_proc_init(void)
{
	proc_create("shrinkers", S_IFREG | S_IRUGO, NULL, &shrinker_proc_ops);
	return 0;
}
late_initcall(page_pool_proc_init);
#endif

#endif /* CONFIG_LIBLINUX_SYS_UNION_SHRINKER */
