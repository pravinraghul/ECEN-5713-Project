#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DEVICE_NAME "spichar"

struct spichar_device {
    struct spi_device *spi;
    struct cdev cdev;
    struct class *class;
    dev_t devt;
};

static struct spichar_device spi_dev;

static int spichar_open(struct inode *inode, struct file *filp) {
    filp->private_data = &spi_dev;
    return 0;
}

static ssize_t spichar_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
    struct spichar_device *dev = filp->private_data;
    char data[32];  // Example buffer
    int ret;

    ret = spi_read(dev->spi, data, count);
    if (ret < 0)
        return ret;

    if (copy_to_user(buf, data, count))
        return -EFAULT;

    return count;
}

static ssize_t spichar_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
    struct spichar_device *dev = filp->private_data;
    char data[32];
    int ret;

    if (copy_from_user(data, buf, count))
        return -EFAULT;

    ret = spi_write(dev->spi, data, count);
    if (ret < 0)
        return ret;

    return count;
}

static const struct file_operations spichar_fops = {
    .owner = THIS_MODULE,
    .open = spichar_open,
    .read = spichar_read,
    .write = spichar_write,
};

static int spichar_probe(struct spi_device *spi) {
    int ret;
    
    spi_dev.spi = spi;
    cdev_init(&spi_dev.cdev, &spichar_fops);
    ret = cdev_add(&spi_dev.cdev, spi_dev.devt, 1);
    if (ret)
        return ret;

    spi_dev.class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(spi_dev.class))
        return PTR_ERR(spi_dev.class);

    device_create(spi_dev.class, NULL, spi_dev.devt, NULL, DEVICE_NAME);

    return 0;
}

static int spichar_remove(struct spi_device *spi) {
    device_destroy(spi_dev.class, spi_dev.devt);
    class_destroy(spi_dev.class);
    cdev_del(&spi_dev.cdev);
    return 0;
}

static struct spi_driver spichar_driver = {
    .driver = {
        .name = DEVICE_NAME,
        .owner = THIS_MODULE,
    },
    .probe = spichar_probe,
    .remove = spichar_remove,
};

static int __init spichar_init(void) {
    int ret;

    ret = alloc_chrdev_region(&spi_dev.devt, 0, 1, DEVICE_NAME);
    if (ret < 0)
        return ret;

    return spi_register_driver(&spichar_driver);
}

static void __exit spichar_exit(void) {
    spi_unregister_driver(&spichar_driver);
    unregister_chrdev_region(spi_dev.devt, 1);
}

module_init(spichar_init);
module_exit(spichar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("SPI Character Device Driver");
