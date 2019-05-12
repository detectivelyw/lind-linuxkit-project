extern int kernel_init_done;
int printk(const char *fmt, ...);
/*
 * proc/fs/generic.c --- generic routines for the proc-fs
 *
 * This file contains generic proc-fs routines for handling
 * directories and files.
 * 
 * Copyright (C) 1991, 1992 Linus Torvalds.
 * Copyright (C) 1997 Theodore Ts'o
 */

#include <linux/errno.h>
#include <linux/time.h>
#include <linux/proc_fs.h>
#include <linux/stat.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/mount.h>
#include <linux/init.h>
#include <linux/idr.h>
#include <linux/bitops.h>
#include <linux/spinlock.h>
#include <linux/completion.h>
#include <linux/uaccess.h>

#include "internal.h"

static DEFINE_RWLOCK(proc_subdir_lock);

static int proc_match(unsigned int len, const char *name, struct proc_dir_entry *de)
{
	if (len < de->namelen)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -1;
}
	if (len > de->namelen)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return 1;
}

	return memcmp(name, de->name, len);
}

static struct proc_dir_entry *pde_subdir_first(struct proc_dir_entry *dir)
{
	return rb_entry_safe(rb_first_cached(&dir->subdir),
			     struct proc_dir_entry, subdir_node);
}

static struct proc_dir_entry *pde_subdir_next(struct proc_dir_entry *dir)
{
	return rb_entry_safe(rb_next(&dir->subdir_node), struct proc_dir_entry,
			     subdir_node);
}

static struct proc_dir_entry *pde_subdir_find(struct proc_dir_entry *dir,
					      const char *name,
					      unsigned int len)
{
	struct rb_node *node = dir->subdir.rb_root.rb_node;

	while (node) {
		struct proc_dir_entry *de = rb_entry(node,
						     struct proc_dir_entry,
						     subdir_node);
		int result = proc_match(len, name, de);

		if (result < 0)
			node = node->rb_left;
		else if (result > 0)
			node = node->rb_right;
		else
			return de;
	}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return NULL;
}

static bool pde_subdir_insert(struct proc_dir_entry *dir,
			      struct proc_dir_entry *de)
{
	struct rb_root_cached *root = &dir->subdir;
	struct rb_node **new = &root->rb_root.rb_node, *parent = NULL;
	bool leftmost = true;

	/* Figure out where to put new node */
	while (*new) {
		struct proc_dir_entry *this = rb_entry(*new,
						       struct proc_dir_entry,
						       subdir_node);
		int result = proc_match(de->namelen, de->name, this);

		parent = *new;
		if (result < 0)
			new = &(*new)->rb_left;
		else if (result > 0) {
			new = &(*new)->rb_right;
			leftmost = false;
		} else
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return false;
}
	}

	/* Add new node and rebalance tree. */
	rb_link_node(&de->subdir_node, parent, new);
	rb_insert_color_cached(&de->subdir_node, root, leftmost);
	return true;
}

static int proc_notify_change(struct dentry *dentry, struct iattr *iattr)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct inode *inode = d_inode(dentry);
	struct proc_dir_entry *de = PDE(inode);
	int error;

	error = setattr_prepare(dentry, iattr);
	if (error)
		return error;

	setattr_copy(inode, iattr);
	mark_inode_dirty(inode);

	proc_set_user(de, inode->i_uid, inode->i_gid);
	de->mode = inode->i_mode;
	return 0;
}

static int proc_getattr(const struct path *path, struct kstat *stat,
			u32 request_mask, unsigned int query_flags)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct inode *inode = d_inode(path->dentry);
	struct proc_dir_entry *de = PDE(inode);
	if (de && de->nlink)
		set_nlink(inode, de->nlink);

	generic_fillattr(inode, stat);
	return 0;
}

static const struct inode_operations proc_file_inode_operations = {
	.setattr	= proc_notify_change,
};

/*
 * This function parses a name such as "tty/driver/serial", and
 * returns the struct proc_dir_entry for "/proc/tty/driver", and
 * returns "serial" in residual.
 */
static int __xlate_proc_name(const char *name, struct proc_dir_entry **ret,
			     const char **residual)
{
	const char     		*cp = name, *next;
	struct proc_dir_entry	*de;
	unsigned int		len;

	de = *ret;
	if (!de)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		de = &proc_root;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	while (1) {
		next = strchr(cp, '/');
		if (!next)
			break;

		len = next - cp;
		de = pde_subdir_find(de, cp, len);
		if (!de) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			WARN(1, "name '%s'\n", name);
			return -ENOENT;
		}
		cp += len + 1;
	}
	*residual = cp;
	*ret = de;
	return 0;
}

static int xlate_proc_name(const char *name, struct proc_dir_entry **ret,
			   const char **residual)
{
	int rv;

	read_lock(&proc_subdir_lock);
	rv = __xlate_proc_name(name, ret, residual);
	read_unlock(&proc_subdir_lock);
	return rv;
}

static DEFINE_IDA(proc_inum_ida);

#define PROC_DYNAMIC_FIRST 0xF0000000U

/*
 * Return an inode number between PROC_DYNAMIC_FIRST and
 * 0xffffffff, or zero on failure.
 */
int proc_alloc_inum(unsigned int *inum)
{
	int i;

	i = ida_simple_get(&proc_inum_ida, 0, UINT_MAX - PROC_DYNAMIC_FIRST + 1,
			   GFP_KERNEL);
	if (i < 0)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return i;
}

	*inum = PROC_DYNAMIC_FIRST + (unsigned int)i;
	return 0;
}

void proc_free_inum(unsigned int inum)
{
	ida_simple_remove(&proc_inum_ida, inum - PROC_DYNAMIC_FIRST);
}

/*
 * Don't create negative dentries here, return -ENOENT by hand
 * instead.
 */
struct dentry *proc_lookup_de(struct proc_dir_entry *de, struct inode *dir,
		struct dentry *dentry)
{
	struct inode *inode;

	read_lock(&proc_subdir_lock);
	de = pde_subdir_find(de, dentry->d_name.name, dentry->d_name.len);
	if (de) {
		pde_get(de);
		read_unlock(&proc_subdir_lock);
		inode = proc_get_inode(dir->i_sb, de);
		if (!inode)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return ERR_PTR(-ENOMEM);
}
		d_set_d_op(dentry, &simple_dentry_operations);
		d_add(dentry, inode);
		return NULL;
	}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	read_unlock(&proc_subdir_lock);
	return ERR_PTR(-ENOENT);
}

struct dentry *proc_lookup(struct inode *dir, struct dentry *dentry,
		unsigned int flags)
{
	return proc_lookup_de(PDE(dir), dir, dentry);
}

/*
 * This returns non-zero if at EOF, so that the /proc
 * root directory can use this and check if it should
 * continue with the <pid> entries..
 *
 * Note that the VFS-layer doesn't care about the return
 * value of the readdir() call, as long as it's non-negative
 * for success..
 */
int proc_readdir_de(struct proc_dir_entry *de, struct file *file,
		    struct dir_context *ctx)
{
	int i;

	if (!dir_emit_dots(file, ctx))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return 0;
}

	read_lock(&proc_subdir_lock);
	de = pde_subdir_first(de);
	i = ctx->pos - 2;
	for (;;) {
		if (!de) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			read_unlock(&proc_subdir_lock);
			return 0;
		}
		if (!i)
			break;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		de = pde_subdir_next(de);
		i--;
	}

	do {
		struct proc_dir_entry *next;
		pde_get(de);
		read_unlock(&proc_subdir_lock);
		if (!dir_emit(ctx, de->name, de->namelen,
			    de->low_ino, de->mode >> 12)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			pde_put(de);
			return 0;
		}
		read_lock(&proc_subdir_lock);
		ctx->pos++;
		next = pde_subdir_next(de);
		pde_put(de);
		de = next;
	} while (de);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	read_unlock(&proc_subdir_lock);
	return 1;
}

int proc_readdir(struct file *file, struct dir_context *ctx)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct inode *inode = file_inode(file);

	return proc_readdir_de(PDE(inode), file, ctx);
}

/*
 * These are the generic /proc directory operations. They
 * use the in-memory "struct proc_dir_entry" tree to parse
 * the /proc directory.
 */
static const struct file_operations proc_dir_operations = {
	.llseek			= generic_file_llseek,
	.read			= generic_read_dir,
	.iterate_shared		= proc_readdir,
};

/*
 * proc directories can do almost nothing..
 */
static const struct inode_operations proc_dir_inode_operations = {
	.lookup		= proc_lookup,
	.getattr	= proc_getattr,
	.setattr	= proc_notify_change,
};

static int proc_register(struct proc_dir_entry * dir, struct proc_dir_entry * dp)
{
	int ret;

	ret = proc_alloc_inum(&dp->low_ino);
	if (ret)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ret;
}

	write_lock(&proc_subdir_lock);
	dp->parent = dir;
	if (pde_subdir_insert(dir, dp) == false) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		WARN(1, "proc_dir_entry '%s/%s' already registered\n",
		     dir->name, dp->name);
		write_unlock(&proc_subdir_lock);
		proc_free_inum(dp->low_ino);
		return -EEXIST;
	}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	write_unlock(&proc_subdir_lock);

	return 0;
}

static struct proc_dir_entry *__proc_create(struct proc_dir_entry **parent,
					  const char *name,
					  umode_t mode,
					  nlink_t nlink)
{
	struct proc_dir_entry *ent = NULL;
	const char *fn;
	struct qstr qstr;

	if (xlate_proc_name(name, parent, &fn) != 0)
		goto out;
	qstr.name = fn;
	qstr.len = strlen(fn);
	if (qstr.len == 0 || qstr.len >= 256) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		WARN(1, "name len %u\n", qstr.len);
		return NULL;
	}
	if (*parent == &proc_root && name_to_int(&qstr) != ~0U) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		WARN(1, "create '/proc/%s' by hand\n", qstr.name);
		return NULL;
	}
	if (is_empty_pde(*parent)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		WARN(1, "attempt to add to permanently empty directory");
		return NULL;
	}

	ent = kzalloc(sizeof(struct proc_dir_entry) + qstr.len + 1, GFP_KERNEL);
	if (!ent)
		goto out;

	memcpy(ent->name, fn, qstr.len + 1);
	ent->namelen = qstr.len;
	ent->mode = mode;
	ent->nlink = nlink;
	ent->subdir = RB_ROOT_CACHED;
	atomic_set(&ent->count, 1);
	spin_lock_init(&ent->pde_unload_lock);
	INIT_LIST_HEAD(&ent->pde_openers);
	proc_set_user(ent, (*parent)->uid, (*parent)->gid);

out:
	return ent;
}

struct proc_dir_entry *proc_symlink(const char *name,
		struct proc_dir_entry *parent, const char *dest)
{
	struct proc_dir_entry *ent;

	ent = __proc_create(&parent, name,
			  (S_IFLNK | S_IRUGO | S_IWUGO | S_IXUGO),1);

	if (ent) {
		ent->data = kmalloc((ent->size=strlen(dest))+1, GFP_KERNEL);
		if (ent->data) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			strcpy((char*)ent->data,dest);
			ent->proc_iops = &proc_link_inode_operations;
			if (proc_register(parent, ent) < 0) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				kfree(ent->data);
				kfree(ent);
				ent = NULL;
			}
		} else {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			kfree(ent);
			ent = NULL;
		}
	}
	return ent;
}
EXPORT_SYMBOL(proc_symlink);

struct proc_dir_entry *proc_mkdir_data(const char *name, umode_t mode,
		struct proc_dir_entry *parent, void *data)
{
	struct proc_dir_entry *ent;

	if (mode == 0)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		mode = S_IRUGO | S_IXUGO;
}

	ent = __proc_create(&parent, name, S_IFDIR | mode, 2);
	if (ent) {
		ent->data = data;
		ent->proc_fops = &proc_dir_operations;
		ent->proc_iops = &proc_dir_inode_operations;
		parent->nlink++;
		if (proc_register(parent, ent) < 0) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			kfree(ent);
			parent->nlink--;
			ent = NULL;
		}
	}
	return ent;
}
EXPORT_SYMBOL_GPL(proc_mkdir_data);

struct proc_dir_entry *proc_mkdir_mode(const char *name, umode_t mode,
				       struct proc_dir_entry *parent)
{
	return proc_mkdir_data(name, mode, parent, NULL);
}
EXPORT_SYMBOL(proc_mkdir_mode);

struct proc_dir_entry *proc_mkdir(const char *name,
		struct proc_dir_entry *parent)
{
	return proc_mkdir_data(name, 0, parent, NULL);
}
EXPORT_SYMBOL(proc_mkdir);

struct proc_dir_entry *proc_create_mount_point(const char *name)
{
	umode_t mode = S_IFDIR | S_IRUGO | S_IXUGO;
	struct proc_dir_entry *ent, *parent = NULL;

	ent = __proc_create(&parent, name, mode, 2);
	if (ent) {
		ent->data = NULL;
		ent->proc_fops = NULL;
		ent->proc_iops = NULL;
		parent->nlink++;
		if (proc_register(parent, ent) < 0) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			kfree(ent);
			parent->nlink--;
			ent = NULL;
		}
	}
	return ent;
}
EXPORT_SYMBOL(proc_create_mount_point);

struct proc_dir_entry *proc_create_data(const char *name, umode_t mode,
					struct proc_dir_entry *parent,
					const struct file_operations *proc_fops,
					void *data)
{
	struct proc_dir_entry *pde;
	if ((mode & S_IFMT) == 0)
		mode |= S_IFREG;

	if (!S_ISREG(mode)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		WARN_ON(1);	/* use proc_mkdir() */
		return NULL;
	}

	BUG_ON(proc_fops == NULL);

	if ((mode & S_IALLUGO) == 0)
		mode |= S_IRUGO;
	pde = __proc_create(&parent, name, mode, 1);
	if (!pde)
		goto out;
	pde->proc_fops = proc_fops;
	pde->data = data;
	pde->proc_iops = &proc_file_inode_operations;
	if (proc_register(parent, pde) < 0)
		goto out_free;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return pde;
out_free:
	kfree(pde);
out:
	return NULL;
}
EXPORT_SYMBOL(proc_create_data);
 
struct proc_dir_entry *proc_create(const char *name, umode_t mode,
				   struct proc_dir_entry *parent,
				   const struct file_operations *proc_fops)
{
	return proc_create_data(name, mode, parent, proc_fops, NULL);
}
EXPORT_SYMBOL(proc_create);

void proc_set_size(struct proc_dir_entry *de, loff_t size)
{
	de->size = size;
}
EXPORT_SYMBOL(proc_set_size);

void proc_set_user(struct proc_dir_entry *de, kuid_t uid, kgid_t gid)
{
	de->uid = uid;
	de->gid = gid;
}
EXPORT_SYMBOL(proc_set_user);

static void free_proc_entry(struct proc_dir_entry *de)
{
	proc_free_inum(de->low_ino);

	if (S_ISLNK(de->mode))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		kfree(de->data);
}
	kfree(de);
}

void pde_put(struct proc_dir_entry *pde)
{
	if (atomic_dec_and_test(&pde->count))
		free_proc_entry(pde);
}

/*
 * Remove a /proc entry and free it if it's not currently in use.
 */
void remove_proc_entry(const char *name, struct proc_dir_entry *parent)
{
	struct proc_dir_entry *de = NULL;
	const char *fn = name;
	unsigned int len;

	write_lock(&proc_subdir_lock);
	if (__xlate_proc_name(name, &parent, &fn) != 0) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		write_unlock(&proc_subdir_lock);
		return;
	}
	len = strlen(fn);

	de = pde_subdir_find(parent, fn, len);
	if (de)
		rb_erase_cached(&de->subdir_node, &parent->subdir);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	write_unlock(&proc_subdir_lock);
	if (!de) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		WARN(1, "name '%s'\n", name);
		return;
	}

	proc_entry_rundown(de);

	if (S_ISDIR(de->mode))
		parent->nlink--;
	de->nlink = 0;
	WARN(pde_subdir_first(de),
	     "%s: removing non-empty directory '%s/%s', leaking at least '%s'\n",
	     __func__, de->parent->name, de->name, pde_subdir_first(de)->name);
	pde_put(de);
}
EXPORT_SYMBOL(remove_proc_entry);

int remove_proc_subtree(const char *name, struct proc_dir_entry *parent)
{
	struct proc_dir_entry *root = NULL, *de, *next;
	const char *fn = name;
	unsigned int len;

	write_lock(&proc_subdir_lock);
	if (__xlate_proc_name(name, &parent, &fn) != 0) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		write_unlock(&proc_subdir_lock);
		return -ENOENT;
	}
	len = strlen(fn);

	root = pde_subdir_find(parent, fn, len);
	if (!root) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		write_unlock(&proc_subdir_lock);
		return -ENOENT;
	}
	rb_erase_cached(&root->subdir_node, &parent->subdir);

	de = root;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	while (1) {
		next = pde_subdir_first(de);
		if (next) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			rb_erase_cached(&next->subdir_node, &de->subdir);
			de = next;
			continue;
		}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		write_unlock(&proc_subdir_lock);

		proc_entry_rundown(de);
		next = de->parent;
		if (S_ISDIR(de->mode))
			next->nlink--;
		de->nlink = 0;
		if (de == root)
			break;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		pde_put(de);

		write_lock(&proc_subdir_lock);
		de = next;
	}
	pde_put(root);
	return 0;
}
EXPORT_SYMBOL(remove_proc_subtree);

void *proc_get_parent_data(const struct inode *inode)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct proc_dir_entry *de = PDE(inode);
	return de->parent->data;
}
EXPORT_SYMBOL_GPL(proc_get_parent_data);

void proc_remove(struct proc_dir_entry *de)
{
	if (de)
		remove_proc_subtree(de->name, de->parent);
}
EXPORT_SYMBOL(proc_remove);

void *PDE_DATA(const struct inode *inode)
{
	return __PDE_DATA(inode);
}
EXPORT_SYMBOL(PDE_DATA);