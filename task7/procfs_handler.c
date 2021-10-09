#include "procfs_handler.h"

#include <linux/types.h>
#include <linux/proc_fs.h>

#include "module_common.h"

#define PROCFS_BUFFER_FORMAT_SIZE 64
#define PROCFS_REQUIRED_PARAMS 2

static char proc_buffer[PROCFS_REQUIRED_PARAMS][PROCFS_BUFFER_FORMAT_SIZE];

#define TIME_MODULE_ABSOLUTE_TIME_PARAM 0
#define TIME_MODULE_LAST_USED_TIME_PARAM 1

#define PROC_DIRECTORY "time_processing_module"

#define PROC_ABSOLUTE_TIME_NAME "absolute_time"
#define PROS_LAST_USED_NAME "last_accessed"

static size_t proc_msg_length[PROCFS_REQUIRED_PARAMS];
static size_t proc_msg_read_pos[PROCFS_REQUIRED_PARAMS];

static struct proc_dir_entry *proc_dir_time_module_root;
static struct proc_dir_entry *proc_files[PROCFS_REQUIRED_PARAMS];

static on_procfs_entry_has_been_used_cb procfs_entry_used_cb = NULL;
static on_procfs_entry_request_absolute_time_cb
	procfs_request_absolute_time_callback = NULL;

static void emit_event_procfs_entry_used(void);
static void emit_procfs_absolute_time_request(void);

static ssize_t proc_buffer_absolute_time_read(struct file *file_p,
					      char __user *buffer,
					      size_t length, loff_t *offset);
static ssize_t proc_buffer_last_accessed_read(struct file *file_p,
					      char __user *buffer,
					      size_t length, loff_t *offset);

static struct proc_ops absolute_time_entry_ops = {
	.proc_read = proc_buffer_absolute_time_read
};

static struct proc_ops last_accessed_entry_ops = {
	.proc_read = proc_buffer_last_accessed_read
};

int init_procfs_handler(void)
{
	proc_dir_time_module_root = proc_mkdir(PROC_DIRECTORY, NULL);

	if (NULL == proc_dir_time_module_root)
		return -EFAULT;
	proc_files[TIME_MODULE_ABSOLUTE_TIME_PARAM] = proc_create(
		PROC_ABSOLUTE_TIME_NAME, S_IFREG | S_IRUGO | S_IWUGO,
		proc_dir_time_module_root, &absolute_time_entry_ops);

	if (NULL == proc_files[TIME_MODULE_ABSOLUTE_TIME_PARAM])
		return -EFAULT;

	proc_files[TIME_MODULE_LAST_USED_TIME_PARAM] =
		proc_create(PROS_LAST_USED_NAME, S_IFREG | S_IRUGO | S_IWUGO,
			    proc_dir_time_module_root,
			    &last_accessed_entry_ops);

	if (NULL == proc_files[TIME_MODULE_LAST_USED_TIME_PARAM])
		return -EFAULT;

	return 0;
}

void cleanup_procfs_handler(void)
{
	if (proc_files[TIME_MODULE_ABSOLUTE_TIME_PARAM]) {
		remove_proc_entry(PROC_ABSOLUTE_TIME_NAME,
				  proc_dir_time_module_root);
		proc_files[TIME_MODULE_ABSOLUTE_TIME_PARAM] = NULL;
	}
	if (proc_files[TIME_MODULE_LAST_USED_TIME_PARAM]) {
		remove_proc_entry(PROS_LAST_USED_NAME,
				  proc_dir_time_module_root);
		proc_files[TIME_MODULE_LAST_USED_TIME_PARAM] = NULL;
	}
	if (proc_dir_time_module_root) {
		remove_proc_entry(PROC_DIRECTORY, NULL);
		proc_dir_time_module_root = NULL;
	}
}

static ssize_t proc_buffer_absolute_time_read(struct file *file_p,
					      char __user *buffer,
					      size_t length, loff_t *offset)
{
	emit_procfs_absolute_time_request();
	emit_event_procfs_entry_used();

	size_t left;
	if (length > (proc_msg_length[TIME_MODULE_ABSOLUTE_TIME_PARAM] -
		      proc_msg_read_pos[TIME_MODULE_ABSOLUTE_TIME_PARAM]))
		length = (proc_msg_length[TIME_MODULE_ABSOLUTE_TIME_PARAM] -
			  proc_msg_read_pos[TIME_MODULE_ABSOLUTE_TIME_PARAM]);

	left = copy_to_user(
		buffer,
		&proc_buffer[TIME_MODULE_ABSOLUTE_TIME_PARAM]
			    [proc_msg_read_pos[TIME_MODULE_ABSOLUTE_TIME_PARAM]],
		length);

	proc_msg_read_pos[TIME_MODULE_ABSOLUTE_TIME_PARAM] += length - left;

	if (left)
		printk(KERN_ERR MODULE_TAG
		       "failed to read %zu from %zu chars\n",
		       left, length);
	else
		printk(KERN_NOTICE MODULE_TAG "read %zu chars\n", length);

	int read_length = length - left;
	if (read_length == 0)
		proc_msg_read_pos[TIME_MODULE_ABSOLUTE_TIME_PARAM] = 0;
	return read_length;
}
static ssize_t proc_buffer_last_accessed_read(struct file *file_p,
					      char __user *buffer,
					      size_t length, loff_t *offset)
{
	static bool ready_read = true;
	if (ready_read) {
		emit_event_procfs_entry_used();
		ready_read = false;
		proc_msg_read_pos[TIME_MODULE_LAST_USED_TIME_PARAM] = 0;
	}

	size_t left;

	if (length > (proc_msg_length[TIME_MODULE_LAST_USED_TIME_PARAM] -
		      proc_msg_read_pos[TIME_MODULE_LAST_USED_TIME_PARAM]))
		length = (proc_msg_length[TIME_MODULE_LAST_USED_TIME_PARAM] -
			  proc_msg_read_pos[TIME_MODULE_LAST_USED_TIME_PARAM]);

	left = copy_to_user(
		buffer,
		&proc_buffer[TIME_MODULE_LAST_USED_TIME_PARAM]
			    [proc_msg_read_pos[TIME_MODULE_LAST_USED_TIME_PARAM]],
		length);

	proc_msg_read_pos[TIME_MODULE_LAST_USED_TIME_PARAM] += length - left;

	if (left)
		printk(KERN_ERR MODULE_TAG
		       "failed to read %zu from %zu chars\n",
		       left, length);
	else
		printk(KERN_NOTICE MODULE_TAG "read %zu chars\n", length);

	int read_length = length - left;
	if (read_length == 0)
		ready_read = true;

	return read_length;
}

void set_procfs_entry_has_been_used_cb(on_procfs_entry_has_been_used_cb callback)
{
	procfs_entry_used_cb = callback;
}

void set_procfs_entry_request_absolute_current_time(
	on_procfs_entry_request_absolute_time_cb callback)
{
	procfs_request_absolute_time_callback = callback;
}

size_t procfs_get_last_used_total_buffer_size(void)
{
	return PROCFS_BUFFER_FORMAT_SIZE;
}
void procfs_set_last_used_message_size(size_t last_used_message_size)
{
	printk(KERN_NOTICE MODULE_TAG "procfs_set_last_used_message_size:%d",
	       last_used_message_size);

	proc_msg_length[TIME_MODULE_LAST_USED_TIME_PARAM] =
		last_used_message_size;
}
char *procfs_get_last_used_buffer(void)
{
	return proc_buffer[TIME_MODULE_LAST_USED_TIME_PARAM];
}

size_t procfs_get_absolute_time_buffer_size(void)
{
	return PROCFS_BUFFER_FORMAT_SIZE;
}
void procfs_set_absoulute_timer_message_size(size_t absolute_time_message_size)
{
	proc_msg_length[TIME_MODULE_ABSOLUTE_TIME_PARAM] =
		absolute_time_message_size;
}

char *procfs_get_absolute_time_buffer(void)
{
	return proc_buffer[TIME_MODULE_ABSOLUTE_TIME_PARAM];
}

static void emit_event_procfs_entry_used(void)
{
	if (procfs_entry_used_cb)
		procfs_entry_used_cb();
}
static void emit_procfs_absolute_time_request(void)
{
	if (procfs_request_absolute_time_callback)
		procfs_request_absolute_time_callback();
}