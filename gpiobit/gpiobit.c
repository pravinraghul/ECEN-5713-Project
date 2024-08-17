#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>

struct gpiobit_t {
    dev_t devnum;
    struct class *class;
    struct cdev cdev;
};

struct gpiobit_t gpiobit;

static int gpiobit_open(struct inode *device_file, struct file *instance);
static int gpiobit_close(struct inode *device_file, struct file *instance);
static ssize_t gpiobit_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs);

static struct file_operations gpiobit_fops = {
	.owner = THIS_MODULE,
	.open = gpiobit_open,
	.release = gpiobit_close,
	.write = gpiobit_write
};

static int gpiobit_pinsetup(void) {

	if(gpio_request(21, "gpio-clk")) {
		printk("Allocation failed for gpio-clk\n");
		return -1;
	}

	if(gpio_direction_output(21, 0)) {
		printk("Direction set output failed for gpio-clk\n");
		gpio_free(21);
		return -1;
	}

	if(gpio_request(26, "gpio-data")) {
		printk("Allocation failed for gpio-data\n");
		gpio_free(21);
		return -1;
	}

	if(gpio_direction_output(26, 0)) {
		printk("Direction set output failed for gpio-data\n");
		gpio_free(21);
		gpio_free(26);
		return -1;
	}

	return 0;
}

static void gpiobit_pinfree(void) {
	gpio_set_value(21, 0);
	gpio_set_value(26, 0);
	gpio_free(21);
	gpio_free(26);
}

int gpiobit_open(struct inode *device_file, struct file *instance) {
	printk("devnum - open was called!\n");
	return 0;
}

int gpiobit_close(struct inode *device_file, struct file *instance) {
	printk("devnum - close was called!\n");
	return 0;
}

void gpiobit_write_byte(char byte) {
	char value;
	int i;
	// printk("byte: %d\n", byte);
	for (i = 0; i < 8; i++) {
		value = (byte >> i) & 0x01;
		// printk("bit: %d, ", value);
		gpio_set_value(26, value); // set the data
		gpio_set_value(21, 1); // set clock 
		msleep(25);
		gpio_set_value(21, 0); // clear clock
		msleep(25);
	}
	printk("\n");
}

ssize_t gpiobit_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;
	char value;
	to_copy = min(count, sizeof(value));
	not_copied = copy_from_user(&value, user_buffer, to_copy);
	gpiobit_write_byte(value); // gpio write byte
	delta = to_copy - not_copied;
	return delta;
}

static int __init gpiobit_init(void)
{
    /* Allocate a device nr */
	if(alloc_chrdev_region(&gpiobit.devnum, 0, 1, "gpiobit") < 0) {
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", gpiobit.devnum >> 20, gpiobit.devnum && 0xfffff);

	/* Create device class */
	if((gpiobit.class = class_create(THIS_MODULE, "gpiobit_class")) == NULL) {
		printk("Device class can not be created!\n");
		goto class_cleanup;
	}

	/* create device file */
	if(device_create(gpiobit.class, NULL, gpiobit.devnum, NULL, "gpiobit") == NULL) {
		printk("Can not create device file!\n");
		goto device_cleanup;
	}
    
    /* Initialize device file */
	cdev_init(&gpiobit.cdev, &gpiobit_fops);

	/* Regisering device to kernel */
	if(cdev_add(&gpiobit.cdev, gpiobit.devnum, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto cdevadd_cleanup;
	}

	if (gpiobit_pinsetup() != 0)
		goto gpiopin_cleanup;

    printk("gpiobit module loaded\n");
    return 0;

gpiopin_cleanup:
	cdev_del(&gpiobit.cdev);
cdevadd_cleanup:
	device_destroy(gpiobit.class, gpiobit.devnum);
device_cleanup:
	class_destroy(gpiobit.class);
class_cleanup:
	unregister_chrdev_region(gpiobit.devnum, 1);
	return -1;
}

static void __exit gpiobit_exit(void)
{
	gpiobit_pinfree();
	cdev_del(&gpiobit.cdev);
	device_destroy(gpiobit.class, gpiobit.devnum);
	class_destroy(gpiobit.class);
	unregister_chrdev_region(gpiobit.devnum, 1);
	printk("gpiobit module unloaded\n");
}

module_init(gpiobit_init);
module_exit(gpiobit_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pravin Raghul S");
MODULE_DESCRIPTION("A Custom GPIO bit-banging driver");
