#include "dev.h"
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>      /* LINUX_VERSION_CODE */
#include <linux/init.h>

static int major = 0;
static int devices_minor_min = 0;
static int devices_minor_max = 3;

module_param( major, int, S_IRUGO );
module_param( devices_minor_min, int, S_IRUGO );
module_param( devices_minor_max, int, S_IRUGO );

MODULE_AUTHOR( "Timur G. Baymatov" );

static const struct file_operations dev_fops = {
   .owner = THIS_MODULE,
   .read  = dev_read,
};

#define MODNAME "my_dyndev_mod"

static struct cdev hcdev;
static struct class *devclass;

static int __init dev_init( void ) {
   int ret, i;
   dev_t dev;
   if( major != 0 ) {
      dev = MKDEV( major, devices_minor_min );
      ret = register_chrdev_region( dev, devices_minor_max, MODNAME );
   }
   else {
      ret = alloc_chrdev_region( &dev, devices_minor_min, devices_minor_max, MODNAME );
      major = MAJOR( dev );  // не забыть зафиксировать!
   }
   if( ret < 0 ) {
      printk( KERN_ERR "=== Can not register char device region\n" );
      goto err;
   }
   cdev_init( &hcdev, &dev_fops );
   hcdev.owner = THIS_MODULE;
   ret = cdev_add( &hcdev, dev, devices_minor_max );
   if( ret < 0 ) {
      unregister_chrdev_region( MKDEV( major, devices_minor_min ), devices_minor_max );
      printk( KERN_ERR "=== Can not add char device\n" );
      goto err;
   }
   devclass = class_create( THIS_MODULE, "dyn_class" ); /* struct class* */
   for( i = devices_minor_min; i < devices_minor_max; i++ ) {
#define DEVNAME "dyn"
      dev = MKDEV( major, devices_minor_min + i );
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,26)
/* struct device *device_create( struct class *cls, struct device *parent,
                                 dev_t devt, const char *fmt, ...); */
      device_create( devclass, NULL, dev, "%s_%d", DEVNAME, i );
#else
/* struct device *device_create( struct class *cls, struct device *parent,
                                 dev_t devt, void *drvdata, const char *fmt, ...); */
      device_create( devclass, NULL, dev, NULL, "%s_%d", DEVNAME, i );
#endif
   }
   printk( KERN_INFO "======== module installed %d:[%d-%d] ===========\n",
           MAJOR( dev ), devices_minor_min, MINOR( dev ) ); 
err:
   return ret;
}

static void __exit dev_exit( void ) {
   dev_t dev;
   int i;
   for( i = devices_minor_min; i < devices_minor_max; i++ ) {
      dev = MKDEV( major, devices_minor_min + i );
      device_destroy( devclass, dev );
   }
   class_destroy( devclass );
   cdev_del( &hcdev );
   unregister_chrdev_region( MKDEV( major, devices_minor_min ), devices_minor_max );
   printk( KERN_INFO "=============== module removed ==================\n" );
}
