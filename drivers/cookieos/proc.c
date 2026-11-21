#include <linux/cookieos.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/string.h>

struct cookieos_driver {
    struct list_head list;
    char *name;
};

static LIST_HEAD(cookieos_drivers);
static DEFINE_MUTEX(cookieos_lock);

int cookieos_driver_register(const char *name)
{
    struct cookieos_driver *driver;
    struct cookieos_driver *entry;

    if (!name || !*name)
        return -EINVAL;

    mutex_lock(&cookieos_lock);

    list_for_each_entry(entry, &cookieos_drivers, list) {
        if (!strcmp(entry->name, name)) {
            mutex_unlock(&cookieos_lock);
            return -EEXIST;
        }
    }

    driver = kzalloc(sizeof(*driver), GFP_KERNEL);
    if (!driver) {
        mutex_unlock(&cookieos_lock);
        return -ENOMEM;
    }

    driver->name = kstrdup(name, GFP_KERNEL);
    if (!driver->name) {
        kfree(driver);
        mutex_unlock(&cookieos_lock);
        return -ENOMEM;
    }

    list_add_tail(&driver->list, &cookieos_drivers);

    mutex_unlock(&cookieos_lock);

    return 0;
}
EXPORT_SYMBOL_GPL(cookieos_driver_register);

void cookieos_driver_unregister(const char *name)
{
    struct cookieos_driver *driver;
    struct cookieos_driver *next;

    if (!name)
        return;

    mutex_lock(&cookieos_lock);

    list_for_each_entry_safe(driver, next, &cookieos_drivers, list) {
        if (!strcmp(driver->name, name)) {
            list_del(&driver->list);
            kfree(driver->name);
            kfree(driver);
            break;
        }
    }

    mutex_unlock(&cookieos_lock);
}
EXPORT_SYMBOL_GPL(cookieos_driver_unregister);

static int cookieos_proc_show(struct seq_file *m, void *v)
{
    struct cookieos_driver *driver;

    seq_puts(m, "CookieOS 11\n");
    seq_puts(m, "Registered drivers:\n");

    mutex_lock(&cookieos_lock);

    list_for_each_entry(driver, &cookieos_drivers, list)
        seq_printf(m, "  %s: loaded\n", driver->name);

    mutex_unlock(&cookieos_lock);

    return 0;
}

static int cookieos_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, cookieos_proc_show, NULL);
}

static const struct proc_ops cookieos_proc_ops = {
    .proc_open    = cookieos_proc_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init cookieos_proc_init(void)
{
    if (!proc_create("cookieos", 0444, NULL, &cookieos_proc_ops))
        return -ENOMEM;

    pr_info("CookieOS: initialized /proc/cookieos\n");

    return 0;
}
subsys_initcall(cookieos_proc_init);
