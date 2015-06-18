#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

#define BUFFER_SIZE 12
#define DEBUG

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacob Stuart");
MODULE_DESCRIPTION("Generate Pseudo Random Characters");

static void
crandom_err(char *msg)
{
	pr_err("crandom: %s\n", msg);
}

static void
crandom_base64(unsigned char* byte_to_encode)
{
	*byte_to_encode = *byte_to_encode;
}

static ssize_t
crandom_read(struct file *filp, char *buffer,
		size_t length, loff_t *offset)
{
	int i;
	char *gen_buf = (char *) kmalloc(length, GFP_KERNEL);

	for (i = 0; i < length; i++) {
		gen_buf[i] = 'a' + i;
		crandom_base64(&gen_buf[i]);
	}

	if (*offset != 0)
		return 0;

	if (copy_to_user(buffer, gen_buf, length))
		return -EINVAL;

	*offset = length;
	return length;
}

static ssize_t
crandom_write(struct file *filp, const char *buffer,
		size_t length, loff_t *offset)
{
	return length;
}

int
crandom_open(struct inode *inode, struct file *file)
{
	return 0;
}

int
crandom_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations fops = {
	.read = crandom_read,
	.write = crandom_write,
	.open = crandom_open,
	.release = crandom_release
};

static struct miscdevice hello_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "crandom",
	.fops = &fops,
	.mode = S_IRWXUGO
};

static int __init
crandom_init(void)
{
	int ret = 0;

	ret = misc_register(&hello_device);

	if (ret)
		crandom_err("Unable to Register\n");


	return ret;
}

static void __exit
crandom_exit(void)
{
	misc_deregister(&hello_device);
	pr_err("HELLOWORLD: Destroyed\n");
}

module_init(crandom_init);
module_exit(crandom_exit);
