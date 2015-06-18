#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>

#define DEBUG
#define BUFFER_SIZE 12

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacob Stuart");
MODULE_DESCRIPTION("Hello World");

static const char *id_number = "e64c403f6ead";

static ssize_t
sample_char_read(struct file *filp, char *buffer,
		size_t length, loff_t *offset)
{
	if (length < sizeof(id_number))
		return -EINVAL;


	if (*offset != 0)
		return 0;

	if (copy_to_user(buffer, id_number, BUFFER_SIZE))
		return -EINVAL;
	*offset = length;
	return length;
}

static ssize_t
sample_char_write(struct file *filp, const char *buffer,
		size_t length, loff_t *offset)
{
	if (strncmp(buffer, id_number, length) == 0) {
		pr_err("HELLOWORLD: Success!");
		return length;
	}

	pr_err("HELLOWORLD: Wrong ID!");
	return -EINVAL;
}

int
sample_char_open(struct inode *inode, struct file *file)
{
	pr_err("HELLOWORLD: Opened\n");
	return 0;
}

int
sample_char_release(struct inode *inode, struct file *file)
{
	pr_err("HELLOWORLD: Release\n");
	return 0;
}

static const struct file_operations fops = {
	.read = sample_char_read,
	.write = sample_char_write,
	.open = sample_char_open,
	.release = sample_char_release
};

static struct miscdevice hello_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "eudyptula",
	.fops = &fops,
	.mode = S_IRWXUGO
};

static int __init
hello_init(void)
{
	int ret = 0;

	pr_err("HELLOWORLD: Initiated\n");

	ret = misc_register(&hello_device);
	if (ret)
		pr_err("HELLOWORLD: Unable to Register\n");
	else
		pr_err("HELLOWORLD: Registered as %d\n", hello_device.minor);


	return ret;
}

static void __exit
hello_exit(void)
{
	misc_deregister(&hello_device);
	pr_err("HELLOWORLD: Destroyed\n");
}

module_init(hello_init);
module_exit(hello_exit);
