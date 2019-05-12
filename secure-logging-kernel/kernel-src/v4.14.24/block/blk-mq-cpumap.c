extern int kernel_init_done;
int printk(const char *fmt, ...);
/*
 * CPU <-> hardware queue mapping helpers
 *
 * Copyright (C) 2013-2014 Jens Axboe
 */
#include <linux/kernel.h>
#include <linux/threads.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/cpu.h>

#include <linux/blk-mq.h>
#include "blk.h"
#include "blk-mq.h"

static int cpu_to_queue_index(unsigned int nr_queues, const int cpu)
{
	/*
	 * Non present CPU will be mapped to queue index 0.
	 */
	if (!cpu_present(cpu))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return 0;
}
	return cpu % nr_queues;
}

static int get_first_sibling(unsigned int cpu)
{
	unsigned int ret;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	ret = cpumask_first(topology_sibling_cpumask(cpu));
	if (ret < nr_cpu_ids)
		return ret;

	return cpu;
}

int blk_mq_map_queues(struct blk_mq_tag_set *set)
{
	unsigned int *map = set->mq_map;
	unsigned int nr_queues = set->nr_hw_queues;
	unsigned int cpu, first_sibling;

	for_each_possible_cpu(cpu) {
		/*
		 * First do sequential mapping between CPUs and queues.
		 * In case we still have CPUs to map, and we have some number of
		 * threads per cores then map sibling threads to the same queue for
		 * performace optimizations.
		 */
		if (cpu < nr_queues) {
			map[cpu] = cpu_to_queue_index(nr_queues, cpu);
		} else {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			first_sibling = get_first_sibling(cpu);
			if (first_sibling == cpu)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				map[cpu] = cpu_to_queue_index(nr_queues, cpu);
}
			else
				map[cpu] = map[first_sibling];
		}
	}

	return 0;
}
EXPORT_SYMBOL_GPL(blk_mq_map_queues);

/*
 * We have no quick way of doing reverse lookups. This is only used at
 * queue init time, so runtime isn't important.
 */
int blk_mq_hw_queue_to_node(unsigned int *mq_map, unsigned int index)
{
	int i;

	for_each_possible_cpu(i) {
		if (index == mq_map[i])
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return local_memory_node(cpu_to_node(i));
}
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return NUMA_NO_NODE;
}
