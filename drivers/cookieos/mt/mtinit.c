#include <linux/init.h>
#include <linux/kernel.h>

static int __init cookieos_mt_init(void)
{
    pr_info("CookieOS MT: Initializing Modular Technology...\n");

    pr_info("CookieOS MT: Initialization complete.\n");

    return 0;
}

subsys_initcall(cookieos_mt_init);