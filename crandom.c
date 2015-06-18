#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/random.h>

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

static char
crandom_encode(unsigned int num)
{
	num %= 62;

	if (num < 26) {
		return 'a' + num;
	} else if (num < 52) {
		return 'A' + num - 26;
	} else {
		return '0' + num - 52;
	}
}

static void
crandom_get_char(char *bucket)
{
	unsigned int val;

	for (;;) {
		prandom_bytes(&val, sizeof(val));
		if (val <= 248)
			break;
	}

	*bucket = crandom_encode(val);
}

static ssize_t
crandom_read(struct file *filp, char *buffer,
		size_t length, loff_t *offset)
{
	int i;
	char *gen_buf = (char *) kmalloc(length, GFP_KERNEL);

	for (i = 0; i < length; i++) {
		crandom_get_char(gen_buf + i);
	}


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
