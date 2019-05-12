extern int kernel_init_done;
int printk(const char *fmt, ...);
/*
 *  linux/fs/binfmt_script.c
 *
 *  Copyright (C) 1996  Martin von Löwis
 *  original #!-checking implemented by tytso.
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/binfmts.h>
#include <linux/init.h>
#include <linux/file.h>
#include <linux/err.h>
#include <linux/fs.h>

static int load_script(struct linux_binprm *bprm)
{
	const char *i_arg, *i_name;
	char *cp;
	struct file *file;
	int retval;

	if ((bprm->buf[0] != '#') || (bprm->buf[1] != '!'))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENOEXEC;
}

	/*
	 * If the script filename will be inaccessible after exec, typically
	 * because it is a "/dev/fd/<fd>/.." path against an O_CLOEXEC fd, give
	 * up now (on the assumption that the interpreter will want to load
	 * this file).
	 */
	if (bprm->interp_flags & BINPRM_FLAGS_PATH_INACCESSIBLE)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENOENT;
}

	/*
	 * This section does the #! interpretation.
	 * Sorta complicated, but hopefully it will work.  -TYT
	 */

	allow_write_access(bprm->file);
	fput(bprm->file);
	bprm->file = NULL;

	bprm->buf[BINPRM_BUF_SIZE - 1] = '\0';
	if ((cp = strchr(bprm->buf, '\n')) == NULL)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		cp = bprm->buf+BINPRM_BUF_SIZE-1;
}
	*cp = '\0';
	while (cp > bprm->buf) {
		cp--;
		if ((*cp == ' ') || (*cp == '\t'))
			*cp = '\0';
		else
			break;
	}
	for (cp = bprm->buf+2; (*cp == ' ') || (*cp == '\t'); cp++);
	if (*cp == '\0')
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENOEXEC; /* No interpreter name found */
}
	i_name = cp;
	i_arg = NULL;
	for ( ; *cp && (*cp != ' ') && (*cp != '\t'); cp++)
		/* nothing */ ;
	while ((*cp == ' ') || (*cp == '\t'))
		*cp++ = '\0';
	if (*cp)
		i_arg = cp;
	/*
	 * OK, we've parsed out the interpreter name and
	 * (optional) argument.
	 * Splice in (1) the interpreter's name for argv[0]
	 *           (2) (optional) argument to interpreter
	 *           (3) filename of shell script (replace argv[0])
	 *
	 * This is done in reverse order, because of how the
	 * user environment and arguments are stored.
	 */
	retval = remove_arg_zero(bprm);
	if (retval)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return retval;
}
	retval = copy_strings_kernel(1, &bprm->interp, bprm);
	if (retval < 0)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return retval;
}
	bprm->argc++;
	if (i_arg) {
		retval = copy_strings_kernel(1, &i_arg, bprm);
		if (retval < 0)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return retval;
}
		bprm->argc++;
	}
	retval = copy_strings_kernel(1, &i_name, bprm);
	if (retval)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return retval;
}
	bprm->argc++;
	retval = bprm_change_interp(i_name, bprm);
	if (retval < 0)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return retval;
}

	/*
	 * OK, now restart the process with the interpreter's dentry.
	 */
	file = open_exec(i_name);
	if (IS_ERR(file))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return PTR_ERR(file);
}

	bprm->file = file;
	retval = prepare_binprm(bprm);
	if (retval < 0)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return retval;
}
	return search_binary_handler(bprm);
}

static struct linux_binfmt script_format = {
	.module		= THIS_MODULE,
	.load_binary	= load_script,
};

static int __init init_script_binfmt(void)
{
	register_binfmt(&script_format);
	return 0;
}

static void __exit exit_script_binfmt(void)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	unregister_binfmt(&script_format);
}

core_initcall(init_script_binfmt);
module_exit(exit_script_binfmt);
MODULE_LICENSE("GPL");