#include <linux/module.h>

MODULE_AUTHOR("Luis R. Rodriguez");
MODULE_DESCRIPTION("Kernel compatibility module");
MODULE_LICENSE("GPL");

#ifndef COMPAT_BASE
#error "You need a COMPAT_BASE"
#endif

#ifndef COMPAT_BASE_TREE
#error "You need a COMPAT_BASE_TREE"
#endif

#ifndef COMPAT_BASE_TREE_VERSION
#error "You need a COMPAT_BASE_TREE_VERSION"
#endif

#ifndef COMPAT_VERSION
#error "You need a COMPAT_VERSION"
#endif

static char *compat_base = COMPAT_BASE;
static char *compat_base_tree = COMPAT_BASE_TREE;
static char *compat_base_tree_version = COMPAT_BASE_TREE_VERSION;
static char *compat_version = COMPAT_VERSION;

module_param(compat_base, charp, 0400);
MODULE_PARM_DESC(compat_base_tree,
		 "The upstream verion of compat.git used");

module_param(compat_base_tree, charp, 0400);
MODULE_PARM_DESC(compat_base_tree,
		 "The upstream tree used as base for this backport");

module_param(compat_base_tree_version, charp, 0400);
MODULE_PARM_DESC(compat_base_tree_version,
		 "The git-describe of the upstream base tree");

module_param(compat_version, charp, 0400);
MODULE_PARM_DESC(compat_version,
		 "Version of the kernel compat backport work");

void compat_dependency_symbol(void)
{
}
EXPORT_SYMBOL_GPL(compat_dependency_symbol);


#if defined(CONFIG_FW_LOADER) && (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,33))
int __init firmware_class_init(void);
void __exit firmware_class_exit(void);
#else
static inline int firmware_class_init(void)
{
	return 0;
}
static inline void firmware_class_exit(void) {}
#endif

static int __init compat_init(void)
{
	compat_pm_qos_power_init();
	compat_system_workqueue_create();
	init_compat_mmc_pm_flags();

	printk(KERN_INFO
	       COMPAT_PROJECT " backport release: "
	       COMPAT_VERSION
	       "\n");
	printk(KERN_INFO "Backport based on "
	       COMPAT_BASE_TREE " " COMPAT_BASE_TREE_VERSION
	       "\n");
	printk(KERN_INFO "compat.git: "
	       COMPAT_BASE_TREE "\n");

	firmware_class_init();
	return 0;
}
module_init(compat_init);

static void __exit compat_exit(void)
{
	compat_pm_qos_power_deinit();
	compat_system_workqueue_destroy();

	firmware_class_exit();
	return;
}
module_exit(compat_exit);

