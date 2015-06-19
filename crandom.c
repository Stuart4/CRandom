#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/random.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacob Stuart");
MODULE_DESCRIPTION("Generate Pseudo Random Characters");

static char
crandom_encode(unsigned int num)
{
	/* 511 % 73 == 0, meaning there is no modulo bias */
	num %= 73;

	if (num < 26) {
		return 'a' + num;
	} else if (num < 52) {
		return 'A' + num - 26;
	} else if (num < 62) {
		return '0' + num - 52;
	} else {
		switch (num) {
		case (62):
			return '!';
		case (63):
			return '@';
		case (64):
			return '#';
		case (65):
			return '$';
		case (66):
			return '%';
		case (67):
			return '^';
		case (68):
			return '&';
		case (69):
			return '*';
		case (70):
			return '=';
		case (71):
			return '+';
		case (72):
			return '?';
		default:
			/* impossible */
			return ' ';
		}
	}
}

static void
crandom_get_char(char *bucket)
{
	unsigned int val;

	/* grab one pseudo random byte */
	prandom_bytes(&val, sizeof(val));

	/* encode to a character */
	*bucket = crandom_encode(val);
}

static ssize_t
crandom_read(struct file *filp, char *buffer,
		size_t length, loff_t *offset)
{
	int i;
	int ret = length;
	char *gen_buf = kmalloc(length, GFP_KERNEL);

	for (i = 0; i < length; i++)
		crandom_get_char(gen_buf + i);

	if (copy_to_user(buffer, gen_buf, length)) {
		ret = -EFAULT;
		goto exit;
	}

exit:
	kfree(gen_buf);
	return length;
}

static ssize_t
crandom_write(struct file *filp, const char *buffer,
		size_t length, loff_t *offset)
{
	/* make everyone happy */
	return length;
}

static const struct file_operations fops = {
	.read = crandom_read,
	.write = crandom_write,
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
		pr_err("crandom: Unable to register as misc device.\n");


	return ret;
}

static void __exit
crandom_exit(void)
{
	misc_deregister(&hello_device);
}

module_init(crandom_init);
module_exit(crandom_exit);
