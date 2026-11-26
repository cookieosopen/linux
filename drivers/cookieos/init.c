// SPDX-License-Identifier: GPL-2.0
/*
 * CookieOS driver initialization
 *
 * drivers/cookieos/init.c
*/

#include <linux/init.h>
#include <linux/kernel.h>

/**
 * DO:
 *  Initialize built in CookieOS drivers here
*/

static int __init cookieos_init(void)
{
    pr_info("CookieOS: Initializing drivers\n");

    return 0;
}

static void __exit cookieos_exit(void)
{
    pr_info("CookieOS: Driver initialization complete\n");
}

subsys_initcall(cookieos_init);
