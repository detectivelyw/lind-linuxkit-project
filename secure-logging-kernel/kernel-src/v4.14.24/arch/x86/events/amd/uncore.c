extern int kernel_init_done;
int printk(const char *fmt, ...);
/*
 * Copyright (C) 2013 Advanced Micro Devices, Inc.
 *
 * Author: Jacob Shin <jacob.shin@amd.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/perf_event.h>
#include <linux/percpu.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/cpu.h>
#include <linux/cpumask.h>

#include <asm/cpufeature.h>
#include <asm/perf_event.h>
#include <asm/msr.h>

#define NUM_COUNTERS_NB		4
#define NUM_COUNTERS_L2		4
#define NUM_COUNTERS_L3		6
#define MAX_COUNTERS		6

#define RDPMC_BASE_NB		6
#define RDPMC_BASE_LLC		10

#define COUNTER_SHIFT		16

#undef pr_fmt
#define pr_fmt(fmt)	"amd_uncore: " fmt

static int num_counters_llc;
static int num_counters_nb;

static HLIST_HEAD(uncore_unused_list);

struct amd_uncore {
	int id;
	int refcnt;
	int cpu;
	int num_counters;
	int rdpmc_base;
	u32 msr_base;
	cpumask_t *active_mask;
	struct pmu *pmu;
	struct perf_event *events[MAX_COUNTERS];
	struct hlist_node node;
};

static struct amd_uncore * __percpu *amd_uncore_nb;
static struct amd_uncore * __percpu *amd_uncore_llc;

static struct pmu amd_nb_pmu;
static struct pmu amd_llc_pmu;

static cpumask_t amd_nb_active_mask;
static cpumask_t amd_llc_active_mask;

static bool is_nb_event(struct perf_event *event)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return event->pmu->type == amd_nb_pmu.type;
}

static bool is_llc_event(struct perf_event *event)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return event->pmu->type == amd_llc_pmu.type;
}

static struct amd_uncore *event_to_amd_uncore(struct perf_event *event)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (is_nb_event(event) && amd_uncore_nb)
		return *per_cpu_ptr(amd_uncore_nb, event->cpu);
	else if (is_llc_event(event) && amd_uncore_llc)
		return *per_cpu_ptr(amd_uncore_llc, event->cpu);

	return NULL;
}

static void amd_uncore_read(struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;
	u64 prev, new;
	s64 delta;

	/*
	 * since we do not enable counter overflow interrupts,
	 * we do not have to worry about prev_count changing on us
	 */

	prev = local64_read(&hwc->prev_count);
	rdpmcl(hwc->event_base_rdpmc, new);
	local64_set(&hwc->prev_count, new);
	delta = (new << COUNTER_SHIFT) - (prev << COUNTER_SHIFT);
	delta >>= COUNTER_SHIFT;
	local64_add(delta, &event->count);
}

static void amd_uncore_start(struct perf_event *event, int flags)
{
	struct hw_perf_event *hwc = &event->hw;

	if (flags & PERF_EF_RELOAD)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		wrmsrl(hwc->event_base, (u64)local64_read(&hwc->prev_count));
}

	hwc->state = 0;
	wrmsrl(hwc->config_base, (hwc->config | ARCH_PERFMON_EVENTSEL_ENABLE));
	perf_event_update_userpage(event);
}

static void amd_uncore_stop(struct perf_event *event, int flags)
{
	struct hw_perf_event *hwc = &event->hw;

	wrmsrl(hwc->config_base, hwc->config);
	hwc->state |= PERF_HES_STOPPED;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if ((flags & PERF_EF_UPDATE) && !(hwc->state & PERF_HES_UPTODATE)) {
		amd_uncore_read(event);
		hwc->state |= PERF_HES_UPTODATE;
	}
}

static int amd_uncore_add(struct perf_event *event, int flags)
{
	int i;
	struct amd_uncore *uncore = event_to_amd_uncore(event);
	struct hw_perf_event *hwc = &event->hw;

	/* are we already assigned? */
	if (hwc->idx != -1 && uncore->events[hwc->idx] == event)
		goto out;

	for (i = 0; i < uncore->num_counters; i++) {
		if (uncore->events[i] == event) {
			hwc->idx = i;
			goto out;
		}
	}

	/* if not, take the first available counter */
	hwc->idx = -1;
	for (i = 0; i < uncore->num_counters; i++) {
		if (cmpxchg(&uncore->events[i], NULL, event) == NULL) {
			hwc->idx = i;
			break;
		}
	}

out:
	if (hwc->idx == -1)
		return -EBUSY;

	hwc->config_base = uncore->msr_base + (2 * hwc->idx);
	hwc->event_base = uncore->msr_base + 1 + (2 * hwc->idx);
	hwc->event_base_rdpmc = uncore->rdpmc_base + hwc->idx;
	hwc->state = PERF_HES_UPTODATE | PERF_HES_STOPPED;

	if (flags & PERF_EF_START)
		amd_uncore_start(event, PERF_EF_RELOAD);

	return 0;
}

static void amd_uncore_del(struct perf_event *event, int flags)
{
	int i;
	struct amd_uncore *uncore = event_to_amd_uncore(event);
	struct hw_perf_event *hwc = &event->hw;

	amd_uncore_stop(event, PERF_EF_UPDATE);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	for (i = 0; i < uncore->num_counters; i++) {
		if (cmpxchg(&uncore->events[i], event, NULL) == event)
			break;
	}

	hwc->idx = -1;
}

static int amd_uncore_event_init(struct perf_event *event)
{
	struct amd_uncore *uncore;
	struct hw_perf_event *hwc = &event->hw;

	if (event->attr.type != event->pmu->type)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENOENT;
}

	/*
	 * NB and Last level cache counters (MSRs) are shared across all cores
	 * that share the same NB / Last level cache. Interrupts can be directed
	 * to a single target core, however, event counts generated by processes
	 * running on other cores cannot be masked out. So we do not support
	 * sampling and per-thread events.
	 */
	if (is_sampling_event(event) || event->attach_state & PERF_ATTACH_TASK)
		return -EINVAL;

	/* NB and Last level cache counters do not have usr/os/guest/host bits */
	if (event->attr.exclude_user || event->attr.exclude_kernel ||
	    event->attr.exclude_host || event->attr.exclude_guest)
		return -EINVAL;

	/* and we do not enable counter overflow interrupts */
	hwc->config = event->attr.config & AMD64_RAW_EVENT_MASK_NB;
	hwc->idx = -1;

	if (event->cpu < 0)
		return -EINVAL;

	uncore = event_to_amd_uncore(event);
	if (!uncore)
		return -ENODEV;

	/*
	 * since request can come in to any of the shared cores, we will remap
	 * to a single common cpu.
	 */
	event->cpu = uncore->cpu;

	return 0;
}

static ssize_t amd_uncore_attr_show_cpumask(struct device *dev,
					    struct device_attribute *attr,
					    char *buf)
{
	cpumask_t *active_mask;
	struct pmu *pmu = dev_get_drvdata(dev);

	if (pmu->type == amd_nb_pmu.type)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		active_mask = &amd_nb_active_mask;
}
	else if (pmu->type == amd_llc_pmu.type)
		active_mask = &amd_llc_active_mask;
	else
		return 0;

	return cpumap_print_to_pagebuf(true, buf, active_mask);
}
static DEVICE_ATTR(cpumask, S_IRUGO, amd_uncore_attr_show_cpumask, NULL);

static struct attribute *amd_uncore_attrs[] = {
	&dev_attr_cpumask.attr,
	NULL,
};

static struct attribute_group amd_uncore_attr_group = {
	.attrs = amd_uncore_attrs,
};

/*
 * Similar to PMU_FORMAT_ATTR but allowing for format_attr to be assigned based
 * on family
 */
#define AMD_FORMAT_ATTR(_dev, _name, _format)				     \
static ssize_t								     \
_dev##_show##_name(struct device *dev,					     \
		struct device_attribute *attr,				     \
		char *page)						     \
{									     \
	BUILD_BUG_ON(sizeof(_format) >= PAGE_SIZE);			     \
	return sprintf(page, _format "\n");				     \
}									     \
static struct device_attribute format_attr_##_dev##_name = __ATTR_RO(_dev);

/* Used for each uncore counter type */
#define AMD_ATTRIBUTE(_name)						     \
static struct attribute *amd_uncore_format_attr_##_name[] = {		     \
	&format_attr_event_##_name.attr,				     \
	&format_attr_umask.attr,					     \
	NULL,								     \
};									     \
static struct attribute_group amd_uncore_format_group_##_name = {	     \
	.name = "format",						     \
	.attrs = amd_uncore_format_attr_##_name,			     \
};									     \
static const struct attribute_group *amd_uncore_attr_groups_##_name[] = {    \
	&amd_uncore_attr_group,						     \
	&amd_uncore_format_group_##_name,				     \
	NULL,								     \
};

AMD_FORMAT_ATTR(event, , "config:0-7,32-35");
AMD_FORMAT_ATTR(umask, , "config:8-15");
AMD_FORMAT_ATTR(event, _df, "config:0-7,32-35,59-60");
AMD_FORMAT_ATTR(event, _l3, "config:0-7");
AMD_ATTRIBUTE(df);
AMD_ATTRIBUTE(l3);

static struct pmu amd_nb_pmu = {
	.task_ctx_nr	= perf_invalid_context,
	.event_init	= amd_uncore_event_init,
	.add		= amd_uncore_add,
	.del		= amd_uncore_del,
	.start		= amd_uncore_start,
	.stop		= amd_uncore_stop,
	.read		= amd_uncore_read,
};

static struct pmu amd_llc_pmu = {
	.task_ctx_nr	= perf_invalid_context,
	.event_init	= amd_uncore_event_init,
	.add		= amd_uncore_add,
	.del		= amd_uncore_del,
	.start		= amd_uncore_start,
	.stop		= amd_uncore_stop,
	.read		= amd_uncore_read,
};

static struct amd_uncore *amd_uncore_alloc(unsigned int cpu)
{
	return kzalloc_node(sizeof(struct amd_uncore), GFP_KERNEL,
			cpu_to_node(cpu));
}

static int amd_uncore_cpu_up_prepare(unsigned int cpu)
{
	struct amd_uncore *uncore_nb = NULL, *uncore_llc;

	if (amd_uncore_nb) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		uncore_nb = amd_uncore_alloc(cpu);
		if (!uncore_nb)
			goto fail;
		uncore_nb->cpu = cpu;
		uncore_nb->num_counters = num_counters_nb;
		uncore_nb->rdpmc_base = RDPMC_BASE_NB;
		uncore_nb->msr_base = MSR_F15H_NB_PERF_CTL;
		uncore_nb->active_mask = &amd_nb_active_mask;
		uncore_nb->pmu = &amd_nb_pmu;
		uncore_nb->id = -1;
		*per_cpu_ptr(amd_uncore_nb, cpu) = uncore_nb;
	}

	if (amd_uncore_llc) {
		uncore_llc = amd_uncore_alloc(cpu);
		if (!uncore_llc)
			goto fail;
		uncore_llc->cpu = cpu;
		uncore_llc->num_counters = num_counters_llc;
		uncore_llc->rdpmc_base = RDPMC_BASE_LLC;
		uncore_llc->msr_base = MSR_F16H_L2I_PERF_CTL;
		uncore_llc->active_mask = &amd_llc_active_mask;
		uncore_llc->pmu = &amd_llc_pmu;
		uncore_llc->id = -1;
		*per_cpu_ptr(amd_uncore_llc, cpu) = uncore_llc;
	}

	return 0;

fail:
	if (amd_uncore_nb)
		*per_cpu_ptr(amd_uncore_nb, cpu) = NULL;
	kfree(uncore_nb);
	return -ENOMEM;
}

static struct amd_uncore *
amd_uncore_find_online_sibling(struct amd_uncore *this,
			       struct amd_uncore * __percpu *uncores)
{
	unsigned int cpu;
	struct amd_uncore *that;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	for_each_online_cpu(cpu) {
		that = *per_cpu_ptr(uncores, cpu);

		if (!that)
			continue;

		if (this == that)
			continue;

		if (this->id == that->id) {
			hlist_add_head(&this->node, &uncore_unused_list);
			this = that;
			break;
		}
	}

	this->refcnt++;
	return this;
}

static int amd_uncore_cpu_starting(unsigned int cpu)
{
	unsigned int eax, ebx, ecx, edx;
	struct amd_uncore *uncore;

	if (amd_uncore_nb) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		uncore = *per_cpu_ptr(amd_uncore_nb, cpu);
		cpuid(0x8000001e, &eax, &ebx, &ecx, &edx);
		uncore->id = ecx & 0xff;

		uncore = amd_uncore_find_online_sibling(uncore, amd_uncore_nb);
		*per_cpu_ptr(amd_uncore_nb, cpu) = uncore;
	}

	if (amd_uncore_llc) {
		unsigned int apicid = cpu_data(cpu).apicid;
		unsigned int nshared, subleaf, prev_eax = 0;

		uncore = *per_cpu_ptr(amd_uncore_llc, cpu);
		/*
		 * Iterate over Cache Topology Definition leaves until no
		 * more cache descriptions are available.
		 */
		for (subleaf = 0; subleaf < 5; subleaf++) {
			cpuid_count(0x8000001d, subleaf, &eax, &ebx, &ecx, &edx);

			/* EAX[0:4] gives type of cache */
			if (!(eax & 0x1f))
				break;

			prev_eax = eax;
		}
		nshared = ((prev_eax >> 14) & 0xfff) + 1;

		uncore->id = apicid - (apicid % nshared);

		uncore = amd_uncore_find_online_sibling(uncore, amd_uncore_llc);
		*per_cpu_ptr(amd_uncore_llc, cpu) = uncore;
	}

	return 0;
}

static void uncore_clean_online(void)
{
	struct amd_uncore *uncore;
	struct hlist_node *n;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	hlist_for_each_entry_safe(uncore, n, &uncore_unused_list, node) {
		hlist_del(&uncore->node);
		kfree(uncore);
	}
}

static void uncore_online(unsigned int cpu,
			  struct amd_uncore * __percpu *uncores)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct amd_uncore *uncore = *per_cpu_ptr(uncores, cpu);

	uncore_clean_online();

	if (cpu == uncore->cpu)
		cpumask_set_cpu(cpu, uncore->active_mask);
}

static int amd_uncore_cpu_online(unsigned int cpu)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (amd_uncore_nb)
		uncore_online(cpu, amd_uncore_nb);

	if (amd_uncore_llc)
		uncore_online(cpu, amd_uncore_llc);

	return 0;
}

static void uncore_down_prepare(unsigned int cpu,
				struct amd_uncore * __percpu *uncores)
{
	unsigned int i;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct amd_uncore *this = *per_cpu_ptr(uncores, cpu);

	if (this->cpu != cpu)
		return;

	/* this cpu is going down, migrate to a shared sibling if possible */
	for_each_online_cpu(i) {
		struct amd_uncore *that = *per_cpu_ptr(uncores, i);

		if (cpu == i)
			continue;

		if (this == that) {
			perf_pmu_migrate_context(this->pmu, cpu, i);
			cpumask_clear_cpu(cpu, that->active_mask);
			cpumask_set_cpu(i, that->active_mask);
			that->cpu = i;
			break;
		}
	}
}

static int amd_uncore_cpu_down_prepare(unsigned int cpu)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (amd_uncore_nb)
		uncore_down_prepare(cpu, amd_uncore_nb);

	if (amd_uncore_llc)
		uncore_down_prepare(cpu, amd_uncore_llc);

	return 0;
}

static void uncore_dead(unsigned int cpu, struct amd_uncore * __percpu *uncores)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct amd_uncore *uncore = *per_cpu_ptr(uncores, cpu);

	if (cpu == uncore->cpu)
		cpumask_clear_cpu(cpu, uncore->active_mask);

	if (!--uncore->refcnt)
		kfree(uncore);
	*per_cpu_ptr(uncores, cpu) = NULL;
}

static int amd_uncore_cpu_dead(unsigned int cpu)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (amd_uncore_nb)
		uncore_dead(cpu, amd_uncore_nb);

	if (amd_uncore_llc)
		uncore_dead(cpu, amd_uncore_llc);

	return 0;
}

static int __init amd_uncore_init(void)
{
	int ret = -ENODEV;

	if (boot_cpu_data.x86_vendor != X86_VENDOR_AMD)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENODEV;
}

	if (!boot_cpu_has(X86_FEATURE_TOPOEXT))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENODEV;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (boot_cpu_data.x86 == 0x17) {
		/*
		 * For F17h, the Northbridge counters are repurposed as Data
		 * Fabric counters. Also, L3 counters are supported too. The PMUs
		 * are exported based on  family as either L2 or L3 and NB or DF.
		 */
		num_counters_nb		  = NUM_COUNTERS_NB;
		num_counters_llc	  = NUM_COUNTERS_L3;
		amd_nb_pmu.name		  = "amd_df";
		amd_llc_pmu.name	  = "amd_l3";
		format_attr_event_df.show = &event_show_df;
		format_attr_event_l3.show = &event_show_l3;
	} else {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		num_counters_nb		  = NUM_COUNTERS_NB;
		num_counters_llc	  = NUM_COUNTERS_L2;
		amd_nb_pmu.name		  = "amd_nb";
		amd_llc_pmu.name	  = "amd_l2";
		format_attr_event_df	  = format_attr_event;
		format_attr_event_l3	  = format_attr_event;
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	amd_nb_pmu.attr_groups	= amd_uncore_attr_groups_df;
	amd_llc_pmu.attr_groups = amd_uncore_attr_groups_l3;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (boot_cpu_has(X86_FEATURE_PERFCTR_NB)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		amd_uncore_nb = alloc_percpu(struct amd_uncore *);
		if (!amd_uncore_nb) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			ret = -ENOMEM;
			goto fail_nb;
		}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		ret = perf_pmu_register(&amd_nb_pmu, amd_nb_pmu.name, -1);
		if (ret)
			goto fail_nb;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		pr_info("AMD NB counters detected\n");
		ret = 0;
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (boot_cpu_has(X86_FEATURE_PERFCTR_LLC)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		amd_uncore_llc = alloc_percpu(struct amd_uncore *);
		if (!amd_uncore_llc) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			ret = -ENOMEM;
			goto fail_llc;
		}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		ret = perf_pmu_register(&amd_llc_pmu, amd_llc_pmu.name, -1);
		if (ret)
			goto fail_llc;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		pr_info("AMD LLC counters detected\n");
		ret = 0;
	}

	/*
	 * Install callbacks. Core will call them for each online cpu.
	 */
	if (cpuhp_setup_state(CPUHP_PERF_X86_AMD_UNCORE_PREP,
			      "perf/x86/amd/uncore:prepare",
			      amd_uncore_cpu_up_prepare, amd_uncore_cpu_dead))
		goto fail_llc;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (cpuhp_setup_state(CPUHP_AP_PERF_X86_AMD_UNCORE_STARTING,
			      "perf/x86/amd/uncore:starting",
			      amd_uncore_cpu_starting, NULL))
		goto fail_prep;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (cpuhp_setup_state(CPUHP_AP_PERF_X86_AMD_UNCORE_ONLINE,
			      "perf/x86/amd/uncore:online",
			      amd_uncore_cpu_online,
			      amd_uncore_cpu_down_prepare))
		goto fail_start;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return 0;

fail_start:
	cpuhp_remove_state(CPUHP_AP_PERF_X86_AMD_UNCORE_STARTING);
fail_prep:
	cpuhp_remove_state(CPUHP_PERF_X86_AMD_UNCORE_PREP);
fail_llc:
	if (boot_cpu_has(X86_FEATURE_PERFCTR_NB))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		perf_pmu_unregister(&amd_nb_pmu);
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (amd_uncore_llc)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		free_percpu(amd_uncore_llc);
}
fail_nb:
	if (amd_uncore_nb)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		free_percpu(amd_uncore_nb);
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return ret;
}
device_initcall(amd_uncore_init);