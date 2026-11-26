// SPDX-License-Identifier: GPL-2.0
/*
 *
 * drivers/cookieos/mt/sys.c
 * Description:
 *  CookieOS Modular Technology sysfs interface.
 *
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/utsname.h>

static struct kobject *mt_kobj;

/*
 * version
 *
 * Displays the running kernel release.
 */
static ssize_t version_show(struct kobject *kobj,
                            struct kobj_attribute *attr,
                            char *buf)
{
    return sysfs_emit(buf, "%s\n", init_uts_ns.name.release);
}

/*
 * state
 *
 * Displays the MT subsystem state.
 */
static ssize_t state_show(struct kobject *kobj,
                          struct kobj_attribute *attr,
                          char *buf)
{
    return sysfs_emit(buf, "initialized\n");
}

/*
 * system
 *
 * Displays basic system information.
 */
static ssize_t system_show(struct kobject *kobj,
                           struct kobj_attribute *attr,
                           char *buf)
{
    return sysfs_emit(buf, "%s %s %s\n",
                      init_uts_ns.name.sysname,
                      init_uts_ns.name.release,
                      init_uts_ns.name.machine);
}

static struct kobj_attribute version_attr =
    __ATTR_RO(version);

static struct kobj_attribute state_attr =
    __ATTR_RO(state);

static struct kobj_attribute system_attr =
    __ATTR_RO(system);

static struct attribute *mt_attrs[] = {
    &version_attr.attr,
    &state_attr.attr,
    &system_attr.attr,
    NULL,
};

static const struct attribute_group mt_attr_group = {
    .attrs = mt_attrs,
};

/*
 * MT_InitSys
 *
 * Initialize the MT sysfs interface.
 */
static int __init MT_InitSys(void)
{
    int ret;

    mt_kobj = kobject_create_and_add("mt", kernel_kobj);
    if (!mt_kobj) {
        pr_err("MT: Failed to create sysfs directory\n");
        return -ENOMEM;
    }

    ret = sysfs_create_group(mt_kobj, &mt_attr_group);
    if (ret) {
        pr_err("MT: Failed to create sysfs attributes: %d\n", ret);
        kobject_put(mt_kobj);
        mt_kobj = NULL;
        return ret;
    }

    pr_info("MT: sysfs interface initialized\n");

    return 0;
}

late_initcall(MT_InitSys);
