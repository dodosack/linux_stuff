// module.c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>

static int __init my_init(void)
{
    pr_info(">> Hello world init\n");
    return 0;
}

static void __exit my_exit(void)
{
    pr_info(">> Goodbye world exit\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dein Name");
MODULE_DESCRIPTION("Ein kleines Hello-World Testmodul");
