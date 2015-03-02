#include "linux/init.h"
#include "linux/module.h"

static int hello_init(void)
{
	printk(KERN_ALERT "i am happy Hello to device driver\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Good Bye kernel\n");
}

module_init(hello_init);
module_exit(hello_exit);