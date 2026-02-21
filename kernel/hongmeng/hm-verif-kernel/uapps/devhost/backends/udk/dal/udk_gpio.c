/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: UDK gpio subsystem framework
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 20 9:23:42 2021
 */

#include <string.h>

#include <udk/gpio.h>
#include <udk/log.h>
#include <udk/sync.h>
#include <udk/iolib.h>
#include <udk/errno.h>
#include <udk/securec.h>
#include <internal/init.h>

struct udk_gpio {
	unsigned int offset;
	struct udk_gpio_bank *bank;
};

struct gpio_bank_node {
	unsigned int pin_start;
	unsigned int pin_end;
	struct udk_gpio_bank *bank;
	struct dlist_node node;
};

struct gpio_banks {
	/* pin_max records the max pin_num, not total number of pins */
	unsigned int pin_max;
	struct dlist_node banks;
	struct udk_mutex mutex;
};

static struct gpio_banks banks_manager;

#define DEFINE_GPIO_GET_FUNCTION(name)					\
static int gpio_get_##name(struct udk_gpio *gpio)			\
{									\
	struct udk_gpio_bank *bank = gpio->bank;			\
	unsigned int offset = gpio->offset;				\
									\
	if ((bank == NULL) || (offset >= bank->pin_count)) {		\
		udk_warn("invalid gpio pin\n");				\
		return -EINVAL;						\
	}								\
	if ((bank->ops == NULL) || (bank->ops->get_##name == NULL)) {	\
		udk_warn("invalid gpio bank ops\n");			\
		return -EINVAL;						\
	}								\
									\
	return bank->ops->get_##name(bank, offset);			\
}

#define DEFINE_GPIO_SET_FUNCTION(name)					\
static int gpio_set_##name(struct udk_gpio *gpio, int name)		\
{									\
	struct udk_gpio_bank *bank = gpio->bank;			\
	unsigned int offset = gpio->offset;				\
									\
	if (!is_##name##_valid(name)) {					\
		return -EINVAL;						\
	}								\
	if ((bank == NULL) || (offset >= bank->pin_count)) {		\
		udk_warn("invalid gpio pin\n");				\
		return -EINVAL;						\
	}								\
	if ((bank->ops == NULL) || (bank->ops->set_##name == NULL)) {	\
		udk_warn("invalid gpio bank ops\n");			\
		return -EINVAL;						\
	}								\
									\
	return bank->ops->set_##name(bank, offset, name);		\
}

#define DEFINE_UDK_GPIO_GET_FUNCTION(name)		\
int udk_gpio_get_##name(struct udk_gpio *gpio)		\
{							\
	if (gpio == NULL) {				\
		return -EINVAL;				\
	}						\
							\
	return gpio_get_##name(gpio);			\
}

#define DEFINE_UDK_GPIO_SET_FUNCTION(name)			\
int udk_gpio_set_##name(struct udk_gpio *gpio, int name)	\
{								\
	int err;						\
								\
	if (gpio == NULL) {					\
		return -EINVAL;					\
	}							\
	err = gpio_set_##name(gpio, name);			\
	if (err < 0) {						\
		return err;					\
	}							\
								\
	return 0;						\
}

static inline void gpio_banks_lock(void)
{
	udk_mutex_lock(&banks_manager.mutex);
}

static inline void gpio_banks_unlock(void)
{
	udk_mutex_unlock(&banks_manager.mutex);
}

static void bank_node_init(struct gpio_bank_node *bank_node, struct udk_gpio_bank *bank)
{
	bank_node->pin_start = banks_manager.pin_max;
	bank_node->pin_end = bank_node->pin_start + bank->pin_count - 1;
	bank_node->bank = bank;
	dlist_init(&bank_node->node);
}

static int gpio_bank_check_valid(struct udk_gpio_bank *bank)
{
	struct gpio_bank_node *bank_node = NULL;

	if ((bank == NULL) || (bank->name == NULL) || (bank->pin_count == 0)) {
		udk_warn("invalid gpio bank\n");
		return -EINVAL;
	}
	/* check whether new bank will cause pin_max overflow */
	if ((banks_manager.pin_max + bank->pin_count) < banks_manager.pin_max) {
		udk_warn("pin_max overflow\n");
		return -EINVAL;
	}
	dlist_for_each_entry(bank_node, &banks_manager.banks,
			     struct gpio_bank_node, node) {
		if (bank_node->bank == bank) {
			udk_warn("bank already exist\n");
			return -EEXIST;
		}
	}

	return 0;
}

int udk_gpio_register(struct udk_gpio_bank *bank)
{
	int err;
	struct gpio_bank_node *bank_node = NULL;

	gpio_banks_lock();
	err = gpio_bank_check_valid(bank);
	if (err < 0) {
		gpio_banks_unlock();
		return err;
	}
	bank_node = (struct gpio_bank_node *)udk_malloc(sizeof(struct gpio_bank_node));
	if (bank_node == NULL) {
		gpio_banks_unlock();
		return -ENOMEM;
	}
	bank_node_init(bank_node, bank);
	dlist_insert_tail(&banks_manager.banks, &bank_node->node);
	banks_manager.pin_max += bank->pin_count;
	gpio_banks_unlock();
	udk_info("register udk gpio bank %s\n", bank->name);

	return 0;
}

int udk_gpio_unregister(struct udk_gpio_bank *bank)
{
	struct gpio_bank_node *bank_node = NULL;
	struct gpio_bank_node *bank_node_tmp = NULL;

	if ((bank == NULL) || (bank->name == NULL)) {
		udk_warn("invalid gpio bank\n");
		return -EINVAL;
	}
	gpio_banks_lock();
	dlist_for_each_entry_safe(bank_node, bank_node_tmp, &banks_manager.banks,
				  struct gpio_bank_node, node) {
		if (bank_node->bank == bank) {
			dlist_delete(&bank_node->node);
			gpio_banks_unlock();
			udk_info("unregister udk gpio bank %s\n", bank->name);
			udk_free(bank_node);
			return 0;
		}
	}
	gpio_banks_unlock();

	return -ENOENT;
}

static struct udk_gpio_bank *pin_bank_of(unsigned int pin, unsigned int *offset)
{
	unsigned int pin_offset;
	struct udk_gpio_bank *bank = NULL;
	struct gpio_bank_node *bank_node = NULL;

	gpio_banks_lock();
	if (pin >= banks_manager.pin_max) {
		udk_warn("invalid pin %u\n", pin);
		gpio_banks_unlock();
		return NULL;
	}
	dlist_for_each_entry(bank_node, &banks_manager.banks,
			     struct gpio_bank_node, node) {
		if ((pin >= bank_node->pin_start) && (pin <= bank_node->pin_end)) {
			bank = bank_node->bank;
			pin_offset = pin - bank_node->pin_start;
			break;
		}
	}
	gpio_banks_unlock();
	if (bank == NULL) {
		return NULL;
	}
	*offset = pin_offset;

	return bank;
}

struct udk_gpio *udk_gpio_request(unsigned int pin)
{
	int err;
	unsigned int offset;
	struct udk_gpio_bank *bank = NULL;
	struct udk_gpio *gpio = NULL;

	bank = pin_bank_of(pin, &offset);
	if (bank == NULL) {
		udk_warn("could not find bank of pin %u\n", pin);
		return NULL;
	}
	if ((bank->ops == NULL) || (bank->ops->request == NULL)) {
		udk_error("invalid gpio bank ops\n");
		return NULL;
	}
	gpio = (struct udk_gpio *)udk_malloc(sizeof(struct udk_gpio));
	if (gpio == NULL) {
		udk_error("udk gpio malloc failed\n");
		return NULL;
	}
	mem_zero_s(*gpio);
	gpio->offset = offset;
	gpio->bank = bank;
	err = bank->ops->request(bank, offset);
	if (err < 0) {
		udk_free(gpio);
		udk_warn("gpio request failed: %s\n", strerror(-err));
		return NULL;
	}

	return gpio;
}

int udk_gpio_free(struct udk_gpio *gpio)
{
	int err;
	unsigned int offset;
	struct udk_gpio_bank *bank = NULL;

	if (gpio == NULL) {
		return -EINVAL;
	}
	offset = gpio->offset;
	bank = gpio->bank;
	if ((bank == NULL) || (offset >= bank->pin_count)) {
		udk_warn("invalid gpio pin\n");
		return -EINVAL;
	}
	if ((bank->ops == NULL) || (bank->ops->free == NULL)) {
		udk_warn("invalid gpio bank ops\n");
		return -EINVAL;
	}
	err = bank->ops->free(bank, offset);
	if (err < 0) {
		udk_warn("gpio free failed: %s\n", strerror(-err));
		return err;
	}
	udk_free(gpio);

	return 0;
}

static inline bool is_level_valid(int level)
{
	if ((level <= UDK_GPIO_LEVEL_INVALID) ||
	    (level >= UDK_GPIO_LEVEL_MAX)) {
		return false;
	}

	return true;
}

DEFINE_GPIO_GET_FUNCTION(level);
DEFINE_GPIO_SET_FUNCTION(level);
DEFINE_UDK_GPIO_GET_FUNCTION(level);
DEFINE_UDK_GPIO_SET_FUNCTION(level);

static inline bool is_direction_valid(int direction)
{
	if ((direction <= UDK_GPIO_DIR_INVALID) ||
	    (direction >= UDK_GPIO_DIR_MAX)) {
		return false;
	}

	return true;
}

DEFINE_GPIO_GET_FUNCTION(direction);
DEFINE_GPIO_SET_FUNCTION(direction);
DEFINE_UDK_GPIO_GET_FUNCTION(direction);

int udk_gpio_set_input(struct udk_gpio *gpio)
{
	int err;

	if (gpio == NULL) {
		return -EINVAL;
	}
	err = gpio_set_direction(gpio, UDK_GPIO_INPUT);
	if (err < 0) {
		udk_warn("gpio %u set input direction failed: %s\n",
			  gpio->offset, strerror(-err));
		return err;
	}

	return 0;
}

int udk_gpio_set_output(struct udk_gpio *gpio, int level)
{
	int err;

	if (gpio == NULL) {
		return -EINVAL;
	}
	err = gpio_set_level(gpio, level);
	if (err < 0) {
		udk_warn("gpio %u set level failed: %s\n",
			  gpio->offset, strerror(-err));
		return err;
	}
	err = gpio_set_direction(gpio, UDK_GPIO_OUTPUT);
	if (err < 0) {
		udk_warn("gpio %u set output direction failed: %s\n",
			  gpio->offset, strerror(-err));
		return err;
	}

	return 0;
}

static inline bool is_bias_valid(int bias)
{
	if ((bias <= UDK_GPIO_BIAS_INVALID) || (bias >= UDK_GPIO_BIAS_MAX)) {
		return false;
	}

	return true;
}

DEFINE_GPIO_GET_FUNCTION(bias);
DEFINE_GPIO_SET_FUNCTION(bias);
DEFINE_UDK_GPIO_GET_FUNCTION(bias);
DEFINE_UDK_GPIO_SET_FUNCTION(bias);

int udk_gpio_get_mux(unsigned int pin)
{
	unsigned int offset;
	struct udk_gpio_bank *bank = NULL;

	bank = pin_bank_of(pin, &offset);
	if (bank == NULL) {
		udk_warn("could not find bank of pin %u\n", pin);
		return -EINVAL;
	}
	if ((bank->ops == NULL) || (bank->ops->get_mux == NULL)) {
		udk_error("invalid gpio bank ops\n");
		return -EINVAL;
	}

	return bank->ops->get_mux(bank, offset);
}

int udk_gpio_set_mux(unsigned int pin, int mux)
{
	unsigned int offset;
	struct udk_gpio_bank *bank = NULL;

	bank = pin_bank_of(pin, &offset);
	if (bank == NULL) {
		udk_warn("could not find bank of pin %u\n", pin);
		return -EINVAL;
	}
	if ((bank->ops == NULL) || (bank->ops->set_mux == NULL)) {
		udk_error("invalid gpio bank ops\n");
		return -EINVAL;
	}

	return bank->ops->set_mux(bank, offset, mux);
}

static int udk_gpio_init(void)
{
	banks_manager.pin_max = 0;
	dlist_init(&banks_manager.banks);
	udk_mutex_init(&banks_manager.mutex);

	return 0;
}
udk_init_call(udk_gpio_init);
