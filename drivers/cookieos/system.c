#include <linux/cookieos.h>
#include <linux/init.h>

static int __init cookieos_system_init(void)
{
    return cookieos_driver_register("system");
}
subsys_initcall(cookieos_system_init);