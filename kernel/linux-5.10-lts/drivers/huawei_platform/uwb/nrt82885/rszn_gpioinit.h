
#ifndef _RSZNGPIOINIT_H_
#define _RSZNGPIOINIT_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/acpi.h>
#include <linux/wait.h>
#include <linux/interrupt.h>

#include <linux/cdev.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>

#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include "securec.h"

/* misc definition */
#define CLR_BIT 				0x0
#define SET_BIT 				0x1
#define UWB_SUCC				0
#define UWB_FAIL				(-1)
#define FORCE_UG_DELAY_MS	   	500
#define POWER_DELAY			 	200
#define WAKEUP_DELAY			3
#define RESET_ON_DELAY			3
#define RESET_DELAY			 	40
#define RESET_ON				0
#define RESET_OFF			   	1
#define UWB_GPIO_LOW			0
#define UWB_GPIO_HIGH		   	1

/* gpio active status definition */
#define POWER_POST_SLEEP        2
#define EXIT_SLEEP_RESET        2
#define POWER_ON				1
#define POWER_OFF			   	0
#define UWB_SWITCH_OPEN		 	1
#define UWB_SWITCH_CLOSE		0
#define WAIT_IRQ_ON			 	0
#define WAIT_IRQ_OFF			1
#define WAIT_IRQ_OTHER		  	(-1)

#ifndef unused
#define unused(arg)			 	(void)(arg)
#endif

struct rszn_data {
	dev_t devt;
	spinlock_t spi_lock;
	spinlock_t irq_lock;
	wait_queue_head_t wait;
	struct cdev cdev;
	struct device *device;
	struct spi_device *spi;
	struct list_head device_entry;

	/* TX/RX buffers are NULL unless this device is open (users > 0) */
	struct mutex buf_lock;
	struct mutex wait_lock;
	struct mutex wake_lock;
	unsigned int users;
	u8 *tx_buffer;
	u8 *rx_buffer;
	u32 speed_hz;
	u32 power;
	u32 wait_irq;
	int power_gpio;
	int reset_gpio;
	int wakeup_gpio;
	int cs_gpio;
	int irq_gpio;
	int irq;
};


typedef struct rszn_uwb_supply {
	struct regulator *ldo_reg54;
	struct regulator *ldo_reg53;
	struct regulator *ldo_reg25;
	struct regulator *ldo_reg3;
	struct clk *clk32kb;
	int gpio_enable2v85;
	int gpio_enable_rfpower;
	int freq2div38m;
	int clk_38m4_enable_reg;
} rszn_uwb_power_supply;

typedef struct rszn_pmu_reg_control {
	int addr;	/* reg address */
	int value;   /* bit position */
} rszn_pmu_reg_control_stu;


int rszn_uwb_open_init(struct rszn_data *spidev, rszn_uwb_power_supply uwbldo);

int rszn_uwb_close_deinit(struct rszn_data *spidev, rszn_uwb_power_supply uwbldo);

void rszn_uwb_hardware_reset(struct rszn_data *rszn);

int rszn_request_gpios(struct rszn_data *rszn, struct spi_device *spi);

int rszn_free_gpios(struct rszn_data *rszn);

int rszn_wakeup_low(struct rszn_data *rszn);

int rszn_wakeup_high(struct rszn_data *rszn);

void rszn_reset_low(struct rszn_data *rszn);

void rszn_reset_high(struct rszn_data *rszn);

void rszn_cs_low(struct rszn_data *rszn);

void rszn_cs_high(struct rszn_data *rszn);

int rszn_request_irq(struct rszn_data *rszn, struct spi_device *spi);

#endif