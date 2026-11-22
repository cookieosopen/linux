/*
 * CookieOS MT - Modular Technology
 * MT Core Kernel Module
 *
 * SPDX-License-Identifier: GPL-2.0
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define MT_VERSION "11.0"

static int __init mtcore_init(void)
{
	pr_info("CookieOS MT: MT Core loaded (version %s)\n",
		MT_VERSION);

	return 0;
}

static void __exit mtcore_exit(void)
{
	pr_info("CookieOS MT: MT Core unloaded\n");
}

module_init(mtcore_init);
module_exit(mtcore_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CookieOS");
MODULE_DESCRIPTION("CookieOS MT Core");
MODULE_VERSION(MT_VERSION);
