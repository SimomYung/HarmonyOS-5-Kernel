/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: Network physical layer management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 05 15:20:20 2022
 */
#include <udk/net/phy.h>

#include <udk/of.h>
#include <udk/log.h>
#include <udk/phy.h>
#include <udk/mdio.h>
#include <udk/timer.h>
#include <udk/mutex.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/bitops.h>
#include <udk/securec.h>
#include <udk/net/mii.h>
#include <udk/lib/dlist.h>
#include <udk/lib/string.h>
#include <internal/of.h>
#include <internal/phy.h>
#include <internal/init.h>

/* phy auto-negotiation abilities */
#define BASE_ANEG_ABILITY	(BMSR_ANEGCAPABLE |		\
				 BMSR_100FULL | BMSR_100HALF |	\
				 BMSR_10FULL | BMSR_10HALF)
#define EXPAND_ANEG_ABILITY	(ESTATUS_1000_TFULL | ESTATUS_1000_THALF)
#define EXPAND_ANEG_SHIFT	16
#define	BASE_ADVERTISE_MASK	(ADVERTISE_10HALF | ADVERTISE_10FULL |		\
				 ADVERTISE_100HALF | ADVERTISE_100FULL |	\
				 ADVERTISE_100BASE4 | ADVERTISE_PAUSE_CAP |	\
				 ADVERTISE_PAUSE_ASYM)
#define EXPAND_ADVERTISE_MASK	(ADVERTISE_1000FULL | ADVERTISE_1000HALF)

#define LINK_STATUS_CHECK_INTERVAL	1000
#define LINK_STATUS_DOWN		0
#define LINK_STATUS_UP			(BMSR_ANEGCOMPLETE | BMSR_LSTATUS)

#define UDK_PHY_SETTING(s, d, abl) { .speed = SPEED_ ## s,	\
				     .duplex = DUPLEX_ ## d,	\
				     .ability = abl		\
				   }
struct udk_phy_setting {
	int speed;
	int duplex;
	unsigned int ability;
};

static const struct udk_phy_setting settings[] = {
	 UDK_PHY_SETTING(1000, FULL, (ESTATUS_1000_TFULL << EXPAND_ANEG_SHIFT)),
	 UDK_PHY_SETTING(1000, HALF, (ESTATUS_1000_THALF << EXPAND_ANEG_SHIFT)),
	 UDK_PHY_SETTING(100,  FULL, BMSR_100FULL),
	 UDK_PHY_SETTING(100,  HALF, BMSR_100HALF),
	 UDK_PHY_SETTING(10,   FULL, BMSR_10FULL),
	 UDK_PHY_SETTING(10,   HALF, BMSR_10HALF),
};

enum netphy_state {
	NETPHY_INVAL,
	NETPHY_READY,
	NETPHY_DOWN,
	NETPHY_UP,
};

struct netphy_manager {
	struct dlist_node head;
	struct udk_mutex lock;
};

struct netphy_entry {
	bool linked;
	struct udk_netphy *phy;
	enum netphy_state state;
	struct dlist_node node;
	struct udk_timer checker;
	struct udk_net_device *netdev;
	udk_netphy_adjust_link_func adjust_link;
};

static struct netphy_manager netphy_mgr;

static inline void netphy_mgr_lock(void)
{
	udk_mutex_lock(&netphy_mgr.lock);
}

static inline void netphy_mgr_unlock(void)
{
	udk_mutex_unlock(&netphy_mgr.lock);
}

static inline bool is_netphy_valid(const struct udk_netphy *phy)
{
	return (phy != NULL) && (phy->desc.name != NULL) &&
	       (phy->desc.name[0] != '\0') && (phy->mdio != NULL);
}

static struct netphy_entry *netphy_entry_lookup(const char *name)
{
	struct netphy_entry *entry = NULL;

	dlist_for_each_entry(entry, &netphy_mgr.head,
			     struct netphy_entry, node) {
		if ((entry->phy != NULL) &&
		    (entry->phy->desc.name != NULL) &&
		    (strcmp(entry->phy->desc.name, name) == 0)) {
			return entry;
		}
	}

	return NULL;
}

static inline struct udk_netphy *desc_netphy_of(struct udk_phy_desc *desc)
{
	return container_of(desc, struct udk_netphy, desc);
}

static inline int netphy_read(struct udk_netphy *netphy, unsigned int regnum)
{
	return udk_mdio_read(netphy->mdio, netphy->id, regnum);
}

static inline int netphy_write(struct udk_netphy *netphy, unsigned int regnum,
			       unsigned int val)
{
	return udk_mdio_write(netphy->mdio, netphy->id, regnum,
			      (unsigned short)val);
}

static int netphy_set_bits(struct udk_netphy *phy, unsigned int reg,
			   unsigned int mask, unsigned int val)
{
	int rd_val;
	unsigned int wr_val;

	rd_val = netphy_read(phy, reg);
	if (rd_val < 0) {
		return rd_val;
	}
	wr_val = udk_set_bits((unsigned int)rd_val, val, mask, 0);
	if (wr_val == (unsigned int)rd_val) {
		return 0;
	}

	return netphy_write(phy, reg, wr_val);
}

static unsigned int __limit_max_speed(struct udk_netphy *phy)
{
	struct udk_dt_node *node = NULL;
	unsigned int max_speed = 0;
	unsigned int max_supported = 0;
	int ret;

	node = udk_of_find_node_by_name(udk_of_soc_root(), phy->desc.name);
	if (node == NULL) {
		return ~max_supported;
	}
	ret = udk_of_prop_read_u32(node, "max-speed", &max_speed);
	if (ret < 0) {
		return ~max_supported;
	}
	udk_info("get phy max_speed=%u\n", max_speed);
	if (max_speed >= SPEED_10) {
		max_supported |= (BMSR_10FULL | BMSR_10HALF);
	}
	if (max_speed >= SPEED_100) {
		max_supported |= (BMSR_100FULL | BMSR_100HALF);
	}
	if (max_speed >= SPEED_1000) {
		max_supported |= ((ESTATUS_1000_TFULL | ESTATUS_1000_THALF) << EXPAND_ANEG_SHIFT);
	}

	return max_supported;
}

static unsigned int aneg_abilities_support_get(struct udk_netphy *phy, unsigned int base_ability,
					       unsigned int ex_ability)
{
	unsigned int supported;
	unsigned int max_supported;

	supported = UDK_BIT_LS(ex_ability, EXPAND_ANEG_SHIFT) | base_ability;
	max_supported = __limit_max_speed(phy);

	return supported & max_supported;
}

static int aneg_abilities_init(struct udk_netphy *phy)
{
	int val;
	unsigned int base_ability = 0;
	unsigned int ex_ability = 0;

	val = netphy_read(phy, MII_BMSR);
	if (val < 0) {
		udk_warn("read MII_BMSR register failed: %s\n",
			 strerror(-val));
		return val;
	}
	base_ability = UDK_MASK((unsigned int)val,
				BASE_ANEG_ABILITY);
	if (((unsigned int)val & BMSR_ESTATEN) != 0) {
		val = netphy_read(phy, MII_ESTATUS);
		if (val < 0) {
			udk_warn("read MII_ESTATUS register failed: %s\n",
				 strerror(-val));
			return val;
		}
		ex_ability = UDK_MASK((unsigned int)val,
				      EXPAND_ANEG_ABILITY);
	}
	phy->supported = aneg_abilities_support_get(phy, base_ability, ex_ability);
	phy->advertising = phy->supported;
	phy->autoneg = (((unsigned int)base_ability & BMSR_ANEGCAPABLE) == BMSR_ANEGCAPABLE) ?
			AUTONEG_ENABLE : AUTONEG_DISABLE;

	return 0;
}

static int base_advertise_config(struct udk_netphy *phy)
{
	int err;
	unsigned int advertise = 0;
	unsigned int abilities = phy->advertising;

	if (UDK_MASK(abilities, BMSR_100FULL) != 0) {
		advertise |= ADVERTISE_100FULL;
	}
	if (UDK_MASK(abilities, BMSR_100HALF) != 0) {
		advertise |= ADVERTISE_100HALF;
	}
	if (UDK_MASK(abilities, BMSR_10FULL) != 0) {
		advertise |= ADVERTISE_10FULL;
	}
	if (UDK_MASK(abilities, BMSR_10HALF) != 0) {
		advertise |= ADVERTISE_10HALF;
	}
	/*
	 * Write advertise to MII_ADVERTISE register to configure auto-negotiation.
	 */
	err = netphy_set_bits(phy, MII_ADVERTISE, BASE_ADVERTISE_MASK, advertise);
	if (err < 0) {
		return err;
	}

	return 0;
}

static int expand_advertise_config(struct udk_netphy *phy)
{
	int err;
	unsigned int advertise = 0;
	unsigned int abilities = phy->advertising >> EXPAND_ANEG_SHIFT;

	if (UDK_MASK(abilities, ESTATUS_1000_TFULL) != 0) {
		advertise |= ADVERTISE_1000FULL;
	}
	if (UDK_MASK(abilities, ESTATUS_1000_THALF) != 0) {
		advertise |= ADVERTISE_1000HALF;
	}
	/*
	 * The expansions of auto-negotiation configurations
	 * should be write to MII_CTRL1000 register.
	 */
	err = netphy_set_bits(phy, MII_CTRL1000, EXPAND_ADVERTISE_MASK, advertise);
	if (err < 0) {
		return err;
	}

	return 0;
}

static void autoneg_status_set(struct udk_netphy *phy)
{
	const struct udk_phy_setting *p = settings;
	unsigned int abilities = phy->advertising & phy->lp_advertising;

	for (size_t i = 0UL; i < ARRAY_SIZE(settings); i++, p++) {
		if (UDK_MASK(abilities, p->ability) != 0) {
			phy->speed = p->speed;
			phy->duplex = p->duplex;
			return;
		}
	}

	phy->speed = SPEED_UNKNOWN;
	phy->duplex = DUPLEX_UNKNOWN;
}

static void fixed_status_set(struct udk_netphy *phy, unsigned int ctrl)
{
	if ((ctrl & BMCR_FULLDPLX) != 0) {
		phy->duplex = DUPLEX_FULL;
	} else {
		phy->duplex = DUPLEX_HALF;
	}

	switch (ctrl & BMCR_SPEED_MASK) {
	case BMCR_SPEED1000:
		phy->speed = SPEED_1000;
		break;
	case BMCR_SPEED100:
		phy->speed = SPEED_100;
		break;
	default:
		phy->speed = SPEED_10;
	}
}

static unsigned int bmcr_encode_by_speed_and_duplex(int speed, int duplex)
{
	unsigned int ctl;

	switch(speed) {
	case SPEED_1000:
		ctl = BMCR_SPEED1000;
		break;
	case SPEED_100:
		ctl = BMCR_SPEED100;
		break;
	case SPEED_10:
		/* fall through */
	default:
		ctl = BMCR_SPEED10;
		break;
	}

	if (duplex == DUPLEX_FULL) {
		ctl |= BMCR_FULLDPLX;
	}

	return ctl;
}

static int link_partner_update(struct udk_netphy *phy, unsigned int status)
{
	int lpa;
	unsigned int lp_advertising = 0;

	if (phy->autoneg == AUTONEG_ENABLE) {
		if ((status & BMSR_ESTATEN) != 0) {
			lpa = netphy_read(phy, MII_STAT1000);
			if (lpa < 0) {
				udk_warn("read MII_STAT1000 register failed\n");
				return lpa;
			}
			if ((lpa & LPA_1000MSFAIL) != 0) {
				udk_warn("Master/Slave resolution failed\n");
				return -ENOLINK;
			}
			if ((lpa & LPA_1000HALF) != 0) {
				lp_advertising |= ESTATUS_1000_THALF << EXPAND_ANEG_SHIFT;
			}
			if ((lpa & LPA_1000FULL) != 0) {
				lp_advertising |= ESTATUS_1000_TFULL << EXPAND_ANEG_SHIFT;
			}
			lpa = netphy_read(phy, MII_LPA);
			if (lpa < 0) {
				udk_warn("read MII_LPA register failed\n");
				return lpa;
			}
			lp_advertising |= ((lpa & ADVERTISE_10HALF) != 0) ? BMSR_10HALF : 0;
			lp_advertising |= ((lpa & ADVERTISE_10FULL) != 0) ? BMSR_10FULL : 0;
			lp_advertising |= ((lpa & ADVERTISE_100HALF) != 0) ? BMSR_100HALF : 0;
			lp_advertising |= ((lpa & ADVERTISE_100FULL) != 0) ? BMSR_100FULL : 0;
		}
	} else {
		lp_advertising = 0U;
	}
	phy->lp_advertising = lp_advertising;

	return 0;
}

static inline int netphy_restart_aneg(struct udk_netphy *phy)
{
	return netphy_set_bits(phy, MII_BMCR,
			       BMCR_ISOLATE | BMCR_ANENABLE | BMCR_ANRESTART,
			       BMCR_ANENABLE | BMCR_ANRESTART);
}

static int nego_start_auto(struct udk_netphy *phy)
{
	int err;

	phy->advertising = UDK_MASK(phy->advertising, phy->supported);
	err = base_advertise_config(phy);
	if (err < 0) {
		return err;
	}
	err = expand_advertise_config(phy);
	if (err < 0) {
		return err;
	}

	return netphy_restart_aneg(phy);
}

static const struct udk_phy_setting *
nego_lookup_valid_setting(int speed, int duplex, unsigned int ability)
{
	const struct udk_phy_setting *p = settings;
	const struct udk_phy_setting *match= NULL, *last = NULL;

	for (size_t i = 0UL; i < ARRAY_SIZE(settings); i++, p++) {
		if (UDK_MASK(ability, p->ability) != 0) {
			last = p;
			if (p->speed == speed && p->duplex == duplex) {
				match = p;
				break;
			}
			if (match == NULL && p->speed <= speed) {
				/* the nearest speed is chosen */
				match = p;
			}
			if (p->speed < speed) {
				break;
			}
		}
	}

	if (match == NULL) {
		/* the smallest speed is chosen */
		match = last;
	}

	return match;
}

static int nego_get_valid_settings(struct udk_netphy *phy)
{
	const struct udk_phy_setting *setting;

	setting = nego_lookup_valid_setting(phy->speed, phy->duplex,
					      phy->supported);
	if (setting == NULL) {
		udk_error("find invalid phy setting\n");
		return -EINVAL;
	}
	phy->speed = setting->speed;
	phy->duplex = setting->duplex;
	udk_info("find appropriate phy setting, speed=%d, duplex=%d\n",
		 phy->speed, phy->duplex);

	return 0;
}

static int netphy_start_forced(struct udk_netphy *phy)
{
	unsigned int ctl;

	ctl = bmcr_encode_by_speed_and_duplex(phy->speed, phy->duplex);

	return netphy_set_bits(phy, MII_BMCR,
			       ~(BMCR_ISOLATE | BMCR_LSB | BMCP_POWER_DOWN), ctl);
}

static int nego_start_by_manual(struct udk_netphy *phy)
{
	int err;

	err = nego_get_valid_settings(phy);
	if (err < 0) {
		udk_error("negotiation process get valid settings failed, err=%s\n",
			   strerror(-err));
		return err;
	}

	err = netphy_start_forced(phy);
	if (err < 0) {
		udk_error("net phy start forced failed, err=%s\n",
			  strerror(-err));
		return err;
	}

	return 0;
}

static int netphy_start_nego(struct udk_phy_desc *desc)
{
	int err;
	struct udk_netphy *phy = desc_netphy_of(desc);

	if (phy == NULL) {
		return -EINVAL;
	}

	if (phy->autoneg == AUTONEG_DISABLE) {
		err = nego_start_by_manual(phy);
		if (err < 0) {
			udk_error("net phy %s negotiation by manual failed: %s\n",
				  desc->name, strerror(-err));
		}
		return err;
	}

	err = nego_start_auto(phy);
	if (err < 0) {
		udk_error("net phy %s auto negotiation failed: %s\n",
			  desc->name, strerror(-err));
		return err;
	}

	return 0;
}

static void link_status_notify(struct netphy_entry *entry)
{
	if (entry->adjust_link != NULL) {
		entry->adjust_link(entry->netdev);
	}
}

static inline void link_status_set(struct netphy_entry *entry,
				   unsigned int ctrl,
				   unsigned int status)
{
	struct udk_netphy *phy = entry->phy;

	if (UDK_MASK(status, LINK_STATUS_UP) == LINK_STATUS_UP) {
		entry->linked = true;
	} else {
		entry->linked = false;
	}

	phy->autoneg = ((ctrl & BMCR_ANENABLE) != 0) ? AUTONEG_ENABLE : AUTONEG_DISABLE;
}

static int link_mode_update(struct udk_netphy *phy, unsigned int ctrl,
			    unsigned int status)
{
	int err;

	if ((status & BMSR_ANEGCOMPLETE) == 0) {
		phy->speed = SPEED_UNKNOWN;
		phy->duplex = DUPLEX_UNKNOWN;
		phy->lp_advertising = 0U;
		return 0;
	}

	err = link_partner_update(phy, status);
	if (err < 0) {
		return err;
	}

	if (phy->autoneg == AUTONEG_ENABLE) {
		autoneg_status_set(phy);
	} else {
		fixed_status_set(phy, ctrl);
	}

	return 0;
}

static int link_status_update(struct netphy_entry *entry)
{
	bool cur_link;
	int ctrl, stat, err;
	struct udk_netphy *phy = entry->phy;

	cur_link = entry->linked;
	ctrl = netphy_read(phy, MII_BMCR);
	if (ctrl < 0) {
		return ctrl;
	}
	if (((unsigned int)ctrl & BMCR_ANRESTART) != 0U) {
		link_status_set(entry, (unsigned int)ctrl, LINK_STATUS_DOWN);
		return 0;
	}
	stat = netphy_read(phy, MII_BMSR);
	if (stat < 0) {
		return stat;
	}
	link_status_set(entry, (unsigned int)ctrl, (unsigned int)stat);
	if ((cur_link != entry->linked) && (entry->linked)) {
		err = link_mode_update(phy, ctrl, stat);
		if (err < 0) {
			return err;
		}
	}

	return 0;
}

static void link_status_check(unsigned long data)
{
	int err;
	struct netphy_entry *entry = (struct netphy_entry *)data;

	if (entry == NULL || entry->phy == NULL) {
		return;
	}
	if (entry->state != NETPHY_UP && entry->state != NETPHY_DOWN) {
		udk_warn("net phy %s is not started\n", entry->phy->desc.name);
		return;
	}
	err = link_status_update(entry);
	if (err < 0) {
		udk_warn("net phy %s update link failed: %s\n",
			 entry->phy->desc.name, strerror(-err));
		return;
	}
	if (entry->linked && entry->state != NETPHY_UP) {
		link_status_notify(entry);
		entry->state = NETPHY_UP;
		udk_info("net phy %s link up\n", entry->phy->desc.name);
	} else if (!entry->linked && entry->state != NETPHY_DOWN) {
		link_status_notify(entry);
		entry->state = NETPHY_DOWN;
		udk_info("net phy %s link down\n", entry->phy->desc.name);
	}
}

static int link_checker_start(struct netphy_entry *entry)
{
	return udk_timer_add(&entry->checker);
}

static int link_checker_stop(struct netphy_entry *entry)
{
	int err;

	err = udk_timer_del_sync(&entry->checker);
	if (err < 0) {
		return err;
	}

	return 0;
}

static void link_checker_init(struct netphy_entry *entry)
{
	udk_timer_init(&entry->checker, link_status_check,
		       ptr_to_ulong(entry), LINK_STATUS_CHECK_INTERVAL);
	udk_timer_setflags(&entry->checker, UDK_TIMER_FLAGS_PERIODIC);
}

static int netphy_hw_init(struct udk_phy_desc *desc)
{
	int err;
	struct udk_netphy *phy = desc_netphy_of(desc);

	if (phy->ops.config_init != NULL) {
		err = phy->ops.config_init(phy);
		if (err < 0) {
			udk_error("net phy %s config init failed: %s\n",
				  phy->desc.name, strerror(-err));
			return err;
		}
	}

	return 0;
}

static int native_netphy_init(struct udk_phy_desc *desc)
{
	struct netphy_entry *entry = NULL;
	int err;

	if (!is_phy_desc_valid(desc)) {
		udk_warn("init failed, invalid desc\n");
		return -EINVAL;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(desc->name);
	if (entry == NULL) {
		netphy_mgr_unlock();
		return -EINVAL;
	}
	netphy_mgr_unlock();
	if (entry->state != NETPHY_INVAL) {
		udk_warn("net phy %s is already inited\n", desc->name);
		return -EINVAL;
	}
	err = netphy_hw_init(desc);
	if (err < 0) {
		udk_error("net phy %s init hardware failed; %s\n",
			  desc->name, strerror(-err));
		return err;
	}
	entry->state = NETPHY_READY;

	return 0;
}

static int native_netphy_exit(struct udk_phy_desc *desc)
{
	struct netphy_entry *entry = NULL;

	if (!is_phy_desc_valid(desc)) {
		return -EINVAL;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(desc->name);
	if (entry == NULL) {
		netphy_mgr_unlock();
		return -EINVAL;
	}
	netphy_mgr_unlock();
	if (entry->state != NETPHY_READY) {
		udk_warn("net phy %s is not connected or already running\n", desc->name);
		return -EINVAL;
	}
	entry->state = NETPHY_INVAL;

	return 0;
}

static int native_netphy_start(struct udk_phy_desc *desc)
{
	struct netphy_entry *entry = NULL;
	int err;

	if (!is_phy_desc_valid(desc)) {
		udk_warn("start failed, invalid desc\n");
		return -EINVAL;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(desc->name);
	if (entry == NULL) {
		netphy_mgr_unlock();
		return -EINVAL;
	}
	netphy_mgr_unlock();
	if (entry->state != NETPHY_READY) {
		udk_error("net phy %s is not connected\n",
			  desc->name);
		return -EINVAL;
	}
	err = netphy_start_nego(desc);
	if (err < 0) {
		udk_error("net phy %s start negotiation failed: %s\n",
			  desc->name, strerror(-err));
		return err;
	}
	err = link_checker_start(entry);
	if (err < 0) {
		udk_error("net phy start state machine failed: %s\n",
			  strerror(-err));
		return err;
	}
	entry->state = NETPHY_DOWN;

	return 0;
}

static int native_netphy_stop(struct udk_phy_desc *desc)
{
	struct netphy_entry *entry = NULL;
	int err;

	if (!is_phy_desc_valid(desc)) {
		udk_warn("stop failed, invalid desc\n");
		return -EINVAL;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(desc->name);
	if (entry == NULL) {
		netphy_mgr_unlock();
		return -EINVAL;
	}
	netphy_mgr_unlock();
	if (entry->state != NETPHY_DOWN &&
	    entry->state != NETPHY_UP) {
		return 0;
	}
	err = link_checker_stop(entry);
	if (err < 0) {
		udk_error("delete net phy link checker failed: %s\n",
			  strerror(-err));
		return err;
	}
	entry->state = NETPHY_READY;

	return 0;
}

static int native_netphy_read(struct udk_phy_desc *desc, unsigned int regnum)
{
	struct udk_netphy *netphy = NULL;

	if (!is_phy_desc_valid(desc)) {
		udk_warn("read failed, invalid desc\n");
		return -EINVAL;
	}
	netphy = desc_netphy_of(desc);
	if (netphy->mdio == NULL) {
		return -EINVAL;
	}

	return netphy_read(netphy, regnum);
}

static int native_netphy_write(struct udk_phy_desc *desc, unsigned int regnum,
			       unsigned int val)
{
	struct udk_netphy *netphy = NULL;

	if (!is_phy_desc_valid(desc)) {
		udk_warn("write failed, invalid desc\n");
		return -EINVAL;
	}
	netphy = desc_netphy_of(desc);
	if (netphy->mdio == NULL) {
		return -EINVAL;
	}

	return netphy_write(netphy, regnum, val);
}

static int native_netphy_setmode(struct udk_phy_desc *desc,
				 enum udk_phy_mode mode,
				 int submode)
{
	struct udk_netphy *netphy = NULL;

	if (!is_phy_desc_valid(desc) || mode != UDK_PHY_MODE_ETHERNET ||
	    submode < (int)UDK_NETPHY_MODE_NA ||
	    submode >= (int)UDK_NETPHY_MODE_MAX) {
		return -EINVAL;
	}
	netphy = desc_netphy_of(desc);
	netphy->mode = (enum udk_netphy_mode)submode;

	return 0;
}

static int phy_register(struct udk_phy_desc *desc)
{
	desc->ops.init = native_netphy_init;
	desc->ops.exit = native_netphy_exit;
	desc->ops.start = native_netphy_start;
	desc->ops.stop = native_netphy_stop;
	desc->ops.read = native_netphy_read;
	desc->ops.write = native_netphy_write;
	desc->ops.set_mode = native_netphy_setmode;

	return udk_phy_register(desc);
}

int udk_netphy_register(struct udk_netphy *phy)
{
	struct netphy_entry *entry = NULL;
	int err;

	if (!is_netphy_valid(phy)) {
		return -EINVAL;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(phy->desc.name);
	if (entry != NULL) {
		udk_warn("netphy is aleady registered\n");
		netphy_mgr_unlock();
		return -EEXIST;
	}
	err = phy_register(&phy->desc);
	if (err < 0) {
		udk_error("register phy %s failed: %s\n", phy->desc.name,
			  strerror(-err));
		netphy_mgr_unlock();
		return err;
	}
	entry = (struct netphy_entry *)udk_malloc(sizeof(*entry));
	if (entry == NULL) {
		udk_error("net phy entry malloc failed\n");
		udk_phy_unregister(&phy->desc);
		netphy_mgr_unlock();
		return -ENOMEM;
	}
	mem_zero_s(*entry);
	err = aneg_abilities_init(phy);
	if (err < 0) {
		udk_phy_unregister(&phy->desc);
		udk_free(entry);
		netphy_mgr_unlock();
		return err;
	}
	entry->phy = phy;
	link_checker_init(entry);
	entry->state = NETPHY_INVAL;
	dlist_insert(&netphy_mgr.head, &entry->node);
	netphy_mgr_unlock();
	udk_info("udk net phy %s:%08x register\n", phy->desc.name, phy->id);

	return 0;
}

int udk_netphy_unregister(struct udk_netphy *phy)
{
	struct netphy_entry *entry = NULL;
	int err;

	if (!is_netphy_valid(phy)) {
		return -EINVAL;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(phy->desc.name);
	if (entry == NULL) {
		udk_warn("netphy is not registered\n");
		netphy_mgr_unlock();
		return -ENODEV;
	}
	err = udk_phy_unregister(&entry->phy->desc);
	if (err < 0) {
		udk_warn("unregister %s phy failed: %s\n",
			 phy->desc.name, strerror(-err));
		netphy_mgr_unlock();
		return err;
	}
	dlist_delete(&entry->node);
	udk_free(entry);
	netphy_mgr_unlock();

	return 0;
}

static int netphy_connect(struct udk_phy *phy,
			  struct netphy_entry *entry,
			  enum udk_netphy_mode mode,
			  struct udk_net_device *netdev,
			  udk_netphy_adjust_link_func adjust_link)
{
	int err;

	if (entry->state != NETPHY_INVAL) {
		udk_warn("net phy is already connected\n");
		return -EINVAL;
	}
	err = udk_phy_set_mode(phy, UDK_PHY_MODE_ETHERNET, mode);
	if (err < 0) {
		udk_warn("configure phy mode failed: %s\n", strerror(-err));
		return err;
	}
	err = udk_phy_init(phy);
	if (err < 0) {
		udk_warn("init netphy failed: %s\n", strerror(-err));
		return err;
	}
	entry->netdev = netdev;
	entry->adjust_link = adjust_link;
	udk_info("phy %s connect success\n", entry->phy->desc.name);

	return 0;
}

struct udk_phy *udk_netphy_connect(struct udk_net_device *netdev,
				   const char *name,
				   udk_netphy_adjust_link_func adjust_link,
				   enum udk_netphy_mode mode)
{
	struct udk_phy *phy = NULL;
	struct netphy_entry *entry = NULL;
	int err;

	if (name == NULL) {
		return NULL;
	}
	phy = udk_phy_get(name);
	if (phy == NULL) {
		return NULL;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(name);
	if (entry == NULL) {
		udk_warn("net phy no entry\n");
		udk_phy_put(phy);
		netphy_mgr_unlock();
		return NULL;
	}
	netphy_mgr_unlock();
	err = netphy_connect(phy, entry, mode, netdev, adjust_link);
	if (err < 0) {
		udk_phy_put(phy);
		return NULL;
	}

	return phy;
}

static void netphy_disconnect(struct udk_phy *phy,
			      struct netphy_entry *entry)
{
	int err;

	err = udk_phy_exit(phy);
	if (err < 0) {
		return;
	}
	entry->netdev = NULL;
	entry->adjust_link = NULL;
	(void)udk_phy_put(phy);
}

int udk_netphy_disconnect(struct udk_phy *phy)
{
	struct netphy_entry *entry = NULL;
	struct udk_phy_desc *desc = NULL;

	if (phy == NULL) {
		return -EINVAL;
	}
	desc = udk_phy_desc_of(phy);
	if (!is_phy_desc_valid(desc)) {
		return -EINVAL;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(desc->name);
	if (entry == NULL) {
		udk_warn("net phy no entry\n");
		netphy_mgr_unlock();
		return -ENOENT;
	}
	netphy_mgr_unlock();
	netphy_disconnect(phy, entry);

	return 0;
}

bool udk_netphy_is_linked(struct udk_phy *phy)
{
	bool linked;
	struct netphy_entry *entry = NULL;
	struct udk_phy_desc *desc = NULL;

	if (phy == NULL) {
		return false;
	}
	desc = udk_phy_desc_of(phy);
	if (!is_phy_desc_valid(desc)) {
		return false;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(desc->name);
	if (entry == NULL) {
		udk_warn("net phy no entry\n");
		netphy_mgr_unlock();
		return false;
	}
	linked = entry->linked;
	netphy_mgr_unlock();

	return linked;
}

static void netphy_ethool_link_settings_set(struct udk_netphy *phy,
					    const struct udk_ethtool_link_ksettings *ksetting)
{
	/* now only use speed/duplex/autoneg attr. */
	phy->speed = ksetting->base.speed;
	phy->duplex = ksetting->base.duplex;
	phy->autoneg = ksetting->base.autoneg;
}

static int netphy_link_settings_set(struct udk_phy_desc *desc, struct netphy_entry *entry,
				    const struct udk_ethtool_link_ksettings *ksetting)
{
	int err;
	struct udk_netphy *phy = desc_netphy_of(desc);

	if (entry->state == NETPHY_INVAL) {
		udk_error("net phy state is invalid\n");
		return -EINVAL;
	}

	netphy_ethool_link_settings_set(phy, ksetting);
	if (entry->state == NETPHY_READY) {
		udk_info("net phy state is ready\n");
		return 0;
	}
	entry->state = NETPHY_DOWN;
	err = netphy_start_nego(desc);
	if (err < 0) {
		udk_error("net phy start negotiation failed\n");
		return err;
	}

	return 0;
}

int udk_netphy_set_link_settings(struct udk_phy *phy, const struct udk_ethtool_link_ksettings *ksetting)
{
	int err;
	struct netphy_entry *entry = NULL;
	struct udk_phy_desc *desc = NULL;

	if (phy == NULL || ksetting == NULL) {
		return -EINVAL;
	}

	desc = udk_phy_desc_of(phy);
	if (!is_phy_desc_valid(desc)) {
		return -EINVAL;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(desc->name);
	if (entry == NULL) {
		udk_warn("net phy no entry\n");
		netphy_mgr_unlock();
		return -ENOENT;
	}
	netphy_mgr_unlock();

	phy_lock(phy);
	err = netphy_link_settings_set(desc, entry, ksetting);
	if (err < 0) {
		udk_warn("net phy set link settings failed: %s\n",
			 strerror(-err));
		phy_unlock(phy);
		return err;
	}
	phy_unlock(phy);

	return 0;
}

static void netphy_ethool_link_settings_get(struct udk_netphy *phy,
					    struct udk_ethtool_link_ksettings *ksetting)
{
	ksetting->link_modes.supported[0] = phy->supported;
	ksetting->link_modes.advertising[0] = phy->advertising;
	ksetting->link_modes.lp_advertising[0] = phy->lp_advertising;

	ksetting->base.speed = phy->speed;
	ksetting->base.duplex = phy->duplex;
	ksetting->base.autoneg = phy->autoneg;
}

static int netphy_link_settings_get(struct udk_phy_desc *desc, struct netphy_entry *entry,
				    struct udk_ethtool_link_ksettings *ksetting)
{
	struct udk_netphy *phy = desc_netphy_of(desc);

	if (entry->state != NETPHY_DOWN && entry->state != NETPHY_UP) {
		udk_error("net phy state is not started\n");
		return -EINVAL;
	}
	netphy_ethool_link_settings_get(phy, ksetting);

	return 0;
}

int udk_netphy_get_link_settings(struct udk_phy *phy, struct udk_ethtool_link_ksettings *ksetting)
{
	int err;
	struct netphy_entry *entry = NULL;
	struct udk_phy_desc *desc = NULL;

	if (phy == NULL || ksetting == NULL) {
		return -EINVAL;
	}

	desc = udk_phy_desc_of(phy);
	if (!is_phy_desc_valid(desc)) {
		return -EINVAL;
	}
	netphy_mgr_lock();
	entry = netphy_entry_lookup(desc->name);
	if (entry == NULL) {
		udk_warn("net phy no entry\n");
		netphy_mgr_unlock();
		return -ENOENT;
	}
	netphy_mgr_unlock();

	phy_lock(phy);
	err = netphy_link_settings_get(desc, entry, ksetting);
	if (err < 0) {
		udk_warn("net phy set link settings failed: %s\n",
			 strerror(-err));
		phy_unlock(phy);
		return err;
	}
	phy_unlock(phy);

	return 0;
}

static int netphy_init(void)
{
	udk_mutex_init(&netphy_mgr.lock);
	dlist_init(&netphy_mgr.head);

	return 0;
}
udk_init_call(netphy_init);
