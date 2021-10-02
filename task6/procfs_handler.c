#include <linux/proc_fs.h>
#include "module_common.h"

#define PROCFS_BUFFER_FORMAT_SIZE 10
#define PROCFS_REQUIRED_PARAMS 3

static char proc_buffer[PROCFS_REQUIRED_PARAMS][PROCFS_BUFFER_FORMAT_SIZE];

#define MESSENGER_TOTAL_SIZE_PARAM 0
#define MESSENGER_REMAINING_SIZE_PARAM 1
#define MESSENGER_MESSAGES_COUNT_PARAM 2

#define PROC_DIRECTORY "messenger_dir"
#define PROC_FILENAME_TOTAL_SIZE "message_buffer_total_size"
#define PROC_FILENAME_REMAINING_SIZE "message_buffer_remaining"
#define PROC_FILENAME_TOTAL_MESSAGES_PROCEEDED "total_messages_proceeded"

static size_t proc_msg_length[PROCFS_REQUIRED_PARAMS];
static size_t proc_msg_read_pos[PROCFS_REQUIRED_PARAMS];

static struct proc_dir_entry *proc_dir_messenger_root;
static struct proc_dir_entry *proc_files[PROCFS_REQUIRED_PARAMS];

static ssize_t proc_buffer_total_size_read(struct file *file_p,
					   char __user *buffer, size_t length,
					   loff_t *offset);
static ssize_t proc_buffer_remaining_size_read(struct file *file_p,
					       char __user *buffer,
					       size_t length, loff_t *offset);
static ssize_t proc_total_messages_count_read(struct file *file_p,
					      char __user *buffer,
					      size_t length, loff_t *offset);

static struct proc_ops read_buffer_size_total_entry_ops = {
	.proc_read = proc_buffer_total_size_read
};

static struct proc_ops read_buffer_remaining_entry_ops = {
	.proc_read = proc_buffer_remaining_size_read
};

static struct proc_ops read_buffer_total_messages_proceeded_ops = {
	.proc_read = proc_total_messages_count_read
};

int initialize_procfs_handler(void)
{
	proc_dir_messenger_root = proc_mkdir(PROC_DIRECTORY, NULL);
	if (NULL == proc_dir_messenger_root)
		return -EFAULT;
	proc_files[MESSENGER_TOTAL_SIZE_PARAM] = proc_create(
		PROC_FILENAME_TOTAL_SIZE, S_IFREG | S_IRUGO | S_IWUGO,
		proc_dir_messenger_root, &read_buffer_size_total_entry_ops);

	if (NULL == proc_files[MESSENGER_TOTAL_SIZE_PARAM])
		return -EFAULT;

	proc_files[MESSENGER_REMAINING_SIZE_PARAM] = proc_create(
		PROC_FILENAME_REMAINING_SIZE, S_IFREG | S_IRUGO | S_IWUGO,
		proc_dir_messenger_root, &read_buffer_remaining_entry_ops);

	if (NULL == proc_files[MESSENGER_REMAINING_SIZE_PARAM])
		return -EFAULT;

	proc_files[MESSENGER_MESSAGES_COUNT_PARAM] =
		proc_create(PROC_FILENAME_TOTAL_MESSAGES_PROCEEDED,
			    S_IFREG | S_IRUGO | S_IWUGO,
			    proc_dir_messenger_root,
			    &read_buffer_total_messages_proceeded_ops);

	if (NULL == proc_files[MESSENGER_MESSAGES_COUNT_PARAM])
		return -EFAULT;

	return 0;
}

void cleanup_procfs_entries(void)
{
	if (proc_files[MESSENGER_TOTAL_SIZE_PARAM]) {
		remove_proc_entry(PROC_FILENAME_TOTAL_SIZE,
				  proc_dir_messenger_root);
		proc_files[MESSENGER_TOTAL_SIZE_PARAM] = NULL;
	}
	if (proc_files[MESSENGER_REMAINING_SIZE_PARAM]) {
		remove_proc_entry(PROC_FILENAME_REMAINING_SIZE,
				  proc_dir_messenger_root);
		proc_files[MESSENGER_REMAINING_SIZE_PARAM] = NULL;
	}
	if (proc_files[MESSENGER_MESSAGES_COUNT_PARAM]) {
		remove_proc_entry(PROC_FILENAME_TOTAL_MESSAGES_PROCEEDED,
				  proc_dir_messenger_root);
		proc_files[MESSENGER_MESSAGES_COUNT_PARAM] = NULL;
	}

	if (proc_dir_messenger_root) {
		remove_proc_entry(PROC_DIRECTORY, NULL);
		proc_dir_messenger_root = NULL;
	}
}

static ssize_t proc_buffer_total_size_read(struct file *file_p,
					   char __user *buffer, size_t length,
					   loff_t *offset)
{
	size_t left;

	if (length > (proc_msg_length[MESSENGER_TOTAL_SIZE_PARAM] -
		      proc_msg_read_pos[MESSENGER_TOTAL_SIZE_PARAM]))
		length = (proc_msg_length[MESSENGER_TOTAL_SIZE_PARAM] -
			  proc_msg_read_pos[MESSENGER_TOTAL_SIZE_PARAM]);

	left = copy_to_user(
		buffer,
		&proc_buffer[MESSENGER_TOTAL_SIZE_PARAM]
			    [proc_msg_read_pos[MESSENGER_TOTAL_SIZE_PARAM]],
		length);

	proc_msg_read_pos[MESSENGER_TOTAL_SIZE_PARAM] += length - left;

	if (left)
		printk(KERN_ERR MODULE_TAG
		       "failed to read %zu from %zu chars\n",
		       left, length);
	else
		printk(KERN_NOTICE MODULE_TAG "read %zu chars\n", length);

	return length - left;
}
static ssize_t proc_buffer_remaining_size_read(struct file *file_p,
					       char __user *buffer,
					       size_t length, loff_t *offset)
{
	size_t left;

	if (length > (proc_msg_length[MESSENGER_REMAINING_SIZE_PARAM] -
		      proc_msg_read_pos[MESSENGER_REMAINING_SIZE_PARAM]))
		length = (proc_msg_length[MESSENGER_REMAINING_SIZE_PARAM] -
			  proc_msg_read_pos[MESSENGER_REMAINING_SIZE_PARAM]);

	left = copy_to_user(
		buffer,
		&proc_buffer[MESSENGER_REMAINING_SIZE_PARAM]
			    [proc_msg_read_pos[MESSENGER_TOTAL_SIZE_PARAM]],
		length);

	proc_msg_read_pos[MESSENGER_REMAINING_SIZE_PARAM] += length - left;

	if (left)
		printk(KERN_ERR MODULE_TAG
		       "failed to read %zu from %zu chars\n",
		       left, length);
	else
		printk(KERN_NOTICE MODULE_TAG "read %zu chars\n", length);

	return length - left;
}
static ssize_t proc_total_messages_count_read(struct file *file_p,
					      char __user *buffer,
					      size_t length, loff_t *offset)
{
	size_t left;

	if (length > (proc_msg_length[MESSENGER_MESSAGES_COUNT_PARAM] -
		      proc_msg_read_pos[MESSENGER_MESSAGES_COUNT_PARAM]))
		length = (proc_msg_length[MESSENGER_MESSAGES_COUNT_PARAM] -
			  proc_msg_read_pos[MESSENGER_MESSAGES_COUNT_PARAM]);

	left = copy_to_user(
		buffer,
		&proc_buffer[MESSENGER_MESSAGES_COUNT_PARAM]
			    [proc_msg_read_pos[MESSENGER_MESSAGES_COUNT_PARAM]],
		length);

	proc_msg_read_pos[MESSENGER_MESSAGES_COUNT_PARAM] += length - left;

	if (left)
		printk(KERN_ERR MODULE_TAG
		       "failed to read %zu from %zu chars\n",
		       left, length);
	else
		printk(KERN_NOTICE MODULE_TAG "read %zu chars\n", length);

	return length - left;
}

void update_buffer_total_size(ssize_t current_buffer_size)
{
}

void update_remaining_buffer_size(ssize_t remainig_buffer_size)
{
}

void update_total_messages_count(ssize_t total_messages_count)
{
}