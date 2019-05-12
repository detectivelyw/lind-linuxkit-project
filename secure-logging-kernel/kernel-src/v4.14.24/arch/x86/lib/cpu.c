extern int kernel_init_done;
int printk(const char *fmt, ...);
#include <linux/types.h>
#include <linux/export.h>

unsigned int x86_family(unsigned int sig)
{
	unsigned int x86;

	x86 = (sig >> 8) & 0xf;

	if (x86 == 0xf)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		x86 += (sig >> 20) & 0xff;
}

	return x86;
}
EXPORT_SYMBOL_GPL(x86_family);

unsigned int x86_model(unsigned int sig)
{
	unsigned int fam, model;

	fam = x86_family(sig);

	model = (sig >> 4) & 0xf;

	if (fam >= 0x6)
		model += ((sig >> 16) & 0xf) << 4;

	return model;
}
EXPORT_SYMBOL_GPL(x86_model);

unsigned int x86_stepping(unsigned int sig)
{
	return sig & 0xf;
}
EXPORT_SYMBOL_GPL(x86_stepping);
