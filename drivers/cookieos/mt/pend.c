// SPDX-License-Identifier: GPL-2.0
/*
 *
 * linux/drivers/cookieos/mt/pend.c
 * Description:
 *  Add pending status for function objects
 *
*/

#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <ckos_pend.h>

struct mt_pending_object {
    void (*object)(void);
    struct list_head list;
};

static LIST_HEAD(mt_pending_objects);
static DEFINE_MUTEX(mt_pending_lock);

int MT_InitPendingStatus(void (*object)(void))
{
    struct mt_pending_object *entry;

    if (!object)
        return -EINVAL;

    mutex_lock(&mt_pending_lock);

    list_for_each_entry(entry, &mt_pending_objects, list) {
        if (entry->object == object) {
            mutex_unlock(&mt_pending_lock);
            return -EEXIST;
        }
    }

    entry = kmalloc(sizeof(*entry), GFP_KERNEL);
    if (!entry) {
        mutex_unlock(&mt_pending_lock);
        return -ENOMEM;
    }

    entry->object = object;
    list_add_tail(&entry->list, &mt_pending_objects);

    mutex_unlock(&mt_pending_lock);

    return 0;
}

int MT_UnlockPendingStatus(void (*object)(void))
{
    struct mt_pending_object *entry;

    if (!object)
        return -EINVAL;

    mutex_lock(&mt_pending_lock);

    list_for_each_entry(entry, &mt_pending_objects, list) {
        if (entry->object == object) {
            list_del(&entry->list);
            mutex_unlock(&mt_pending_lock);

            object();

            kfree(entry);
            return 0;
        }
    }

    mutex_unlock(&mt_pending_lock);

    return -ENOENT;
}