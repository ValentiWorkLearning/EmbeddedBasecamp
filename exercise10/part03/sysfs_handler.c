#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/device/class.h>

#include "convertor_mode_enum.h"
#include <linux/kernel.h>

int convertToEnum(const char*  input_buffer, ConversionMode_t* conversion_mode)
{
    int32_t value = 0;
    kstrtos32(input_buffer,10,&value);

    if(value < NoneConversion || value >= EnumEnd)
        return -1;
    *conversion_mode = (ConversionMode_t)(value);
    return 0;
}

static conversion_mode_changed_cb_t on_conversion_mode_changed_cb;

void register_on_conversion_mode_changed_cb(conversion_mode_changed_cb_t callback)
{
    on_conversion_mode_changed_cb = callback;
}

#define LEN_MSG 15
static char buf_msg[ LEN_MSG + 1 ] = "Hello_sysfs\n";

/* <linux/device.h>
LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32)
struct class_attribute {
   struct attribute attr;
   ssize_t (*show)(struct class *class, struct class_attribute *attr, char *buf);
   ssize_t (*store)(struct class *class, struct class_attribute *attr,
                    const char *buf, size_t count);
};
LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,32)
struct class_attribute {
   struct attribute attr;
   ssize_t (*show)(struct class *class, char *buf);
   ssize_t (*store)(struct class *class, const char *buf, size_t count);
};
*/

/* sysfs show() method. Calls the show() method corresponding to the individual sysfs file */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32)
static ssize_t conv_conf_show( struct class *class, struct class_attribute *attr, char *buf ) {
#else
static ssize_t x_show( struct class *class, char *buf ) {
#endif
   strcpy( buf, buf_msg );
   printk( "read %ld\n", (long)strlen( buf ) );
   return strlen( buf );
}

/* sysfs store() method. Calls the store() method corresponding to the individual sysfs file */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32)
static ssize_t conv_conf_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
#else
static ssize_t x_store( struct class *class, const char *buf, size_t count ) {
#endif
   if(count > LEN_MSG)
    return -EIO;

   printk( "write %ld\n", (long)count );
   strncpy( buf_msg, buf, count );
   buf_msg[ count ] = '\0';

   ConversionMode_t conversion_mode = NoneConversion;
   convertToEnum(buf_msg,&conversion_mode);
   if(on_conversion_mode_changed_cb)
   {
       on_conversion_mode_changed_cb(conversion_mode);
   }
   return count;
}

/*
// #define __ATTR(_name, _mode, _show, _store) {				\
// 	.attr = {.name = __stringify(_name),				\
// 		 .mode = VERIFY_OCTAL_PERMISSIONS(_mode) },		\
// 	.show	= _show,						\
// 	.store	= _store,						\
// }

// #define __ATTR_RW(_name) __ATTR(_name, 0644, _name##_show, _name##_store)

// #define CLASS_ATTR_RW(_name) \
// 	struct class_attribute class_attr_##_name = __ATTR_RW(_name)
// #define CLASS_ATTR_RO(_name) \
// 	struct class_attribute class_attr_##_name = __ATTR_RO(_name)
// #define CLASS_ATTR_WO(_name) \
// 	struct class_attribute class_attr_##_name = __ATTR_WO(_name)
*/

CLASS_ATTR_RW(conv_conf);

static struct class *x_class;

int init_sysfs_handler(void)
{
    int res;
   x_class = class_create( THIS_MODULE, "conv-class" );
   if( IS_ERR( x_class ) ) printk( "bad class create\n" );
   res = class_create_file( x_class, &class_attr_conv_conf );
/* <linux/device.h>
extern int __must_check class_create_file(struct class *class, const struct class_attribute *attr); */
   printk( "'conv_conf' module initialized\n" );
   return res;
}

void sysfs_handler_cleanup(void)
{
/* <linux/device.h>
extern void class_remove_file(struct class *class, const struct class_attribute *attr); */
   class_remove_file( x_class, &class_attr_conv_conf );
   class_destroy( x_class );
   return;
}