#ifndef POWER_UTILS_H
#define POWER_UTILS_H

struct power_setting;
struct power_setting_group {
	struct power_setting *settings;
	int nsettings;
};

int get_power_setting_utils(struct device *dev, struct power_setting_group *grp);
int put_power_setting_utils(struct device *dev, struct power_setting_group *grp);
int enable_power_setting_utils(struct power_setting_group *grp);
void disable_power_setting_utils(struct power_setting_group *grp);
void do_isp_rst_after_release(struct power_setting_group *grp);
#endif /* end of include guard: POWER_UTILS_H */
