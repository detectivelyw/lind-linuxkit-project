extern int kernel_init_done;
int printk(const char *fmt, ...);
// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irqnr.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/*
 * /proc/interrupts
 */
static void *int_seq_start(struct seq_file *f, loff_t *pos)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return (*pos <= nr_irqs) ? pos : NULL;
}

static void *int_seq_next(struct seq_file *f, void *v, loff_t *pos)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	(*pos)++;
	if (*pos > nr_irqs)
		return NULL;
	return pos;
}

static void int_seq_stop(struct seq_file *f, void *v)
{
	/* Nothing to do */
}

static const struct seq_operations int_seq_ops = {
	.start = int_seq_start,
	.next  = int_seq_next,
	.stop  = int_seq_stop,
	.show  = show_interrupts
};

static int interrupts_open(struct inode *inode, struct file *filp)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return seq_open(filp, &int_seq_ops);
}

static const struct file_operations proc_interrupts_operations = {
	.open		= interrupts_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static int __init proc_interrupts_init(void)
{
	proc_create("interrupts", 0, NULL, &proc_interrupts_operations);
	return 0;
}
fs_initcall(proc_interrupts_init);