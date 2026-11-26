// SPDX-License-Identifier: GPL-2.0
/*
 * CookieOS driver initialization
 *
 * drivers/cookieos/init.c
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define COOKIEOS_DEVICE_NAME "cookieos"

static dev_t cookieos_devt;
static struct cdev cookieos_cdev;
static struct class *cookieos_class;
static struct device *cookieos_device;

static ssize_t cookieos_read(struct file *file,
                             char __user *buf,
                             size_t count,
                             loff_t *ppos)
{
    static const char message[] = "CookieOS 11\n";

    return simple_read_from_buffer(
        buf, count, ppos, message, sizeof(message) - 1
    );
}

static const struct file_operations cookieos_fops = {
    .owner = THIS_MODULE,
    .read = cookieos_read,
    .llseek = default_llseek,
};

static int __init cookieos_init(void)
{
    int ret;

    pr_info("CookieOS: Initializing drivers\n");

    ret = alloc_chrdev_region(&cookieos_devt, 0, 1,
                              COOKIEOS_DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&cookieos_cdev, &cookieos_fops);

    ret = cdev_add(&cookieos_cdev, cookieos_devt, 1);
    if (ret)
        goto err_unregister;

    cookieos_class = class_create(COOKIEOS_DEVICE_NAME);
    if (IS_ERR(cookieos_class)) {
        ret = PTR_ERR(cookieos_class);
        goto err_cdev;
    }

    cookieos_device = device_create(
        cookieos_class,
        NULL,
        cookieos_devt,
        NULL,
        COOKIEOS_DEVICE_NAME
    );

    if (IS_ERR(cookieos_device)) {
        ret = PTR_ERR(cookieos_device);
        goto err_class;
    }

    pr_info("CookieOS: Registered /dev/%s\n",
            COOKIEOS_DEVICE_NAME);

    return 0;

err_class:
    class_destroy(cookieos_class);

err_cdev:
    cdev_del(&cookieos_cdev);

err_unregister:
    unregister_chrdev_region(cookieos_devt, 1);

    return ret;
}

subsys_initcall(cookieos_init);