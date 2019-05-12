extern int kernel_init_done;
int printk(const char *fmt, ...);
// SPDX-License-Identifier: GPL-2.0
#include "sched.h"

/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */

#ifdef CONFIG_SMP
static int
select_task_rq_stop(struct task_struct *p, int cpu, int sd_flag, int flags)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return task_cpu(p); /* stop tasks as never migrate */
}
#endif /* CONFIG_SMP */

static void
check_preempt_curr_stop(struct rq *rq, struct task_struct *p, int flags)
{
	/* we're never preempted */
}

static struct task_struct *
pick_next_task_stop(struct rq *rq, struct task_struct *prev, struct rq_flags *rf)
{
	struct task_struct *stop = rq->stop;

	if (!stop || !task_on_rq_queued(stop))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return NULL;
}

	put_prev_task(rq, prev);

	stop->se.exec_start = rq_clock_task(rq);

	return stop;
}

static void
enqueue_task_stop(struct rq *rq, struct task_struct *p, int flags)
{
	add_nr_running(rq, 1);
}

static void
dequeue_task_stop(struct rq *rq, struct task_struct *p, int flags)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	sub_nr_running(rq, 1);
}

static void yield_task_stop(struct rq *rq)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	BUG(); /* the stop task should never yield, its pointless. */
}

static void put_prev_task_stop(struct rq *rq, struct task_struct *prev)
{
	struct task_struct *curr = rq->curr;
	u64 delta_exec;

	delta_exec = rq_clock_task(rq) - curr->se.exec_start;
	if (unlikely((s64)delta_exec < 0))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		delta_exec = 0;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	schedstat_set(curr->se.statistics.exec_max,
			max(curr->se.statistics.exec_max, delta_exec));

	curr->se.sum_exec_runtime += delta_exec;
	account_group_exec_runtime(curr, delta_exec);

	curr->se.exec_start = rq_clock_task(rq);
	cpuacct_charge(curr, delta_exec);
}

static void task_tick_stop(struct rq *rq, struct task_struct *curr, int queued)
{
}

static void set_curr_task_stop(struct rq *rq)
{
	struct task_struct *stop = rq->stop;

	stop->se.exec_start = rq_clock_task(rq);
}

static void switched_to_stop(struct rq *rq, struct task_struct *p)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	BUG(); /* its impossible to change to this class */
}

static void
prio_changed_stop(struct rq *rq, struct task_struct *p, int oldprio)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	BUG(); /* how!?, what priority? */
}

static unsigned int
get_rr_interval_stop(struct rq *rq, struct task_struct *task)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return 0;
}

static void update_curr_stop(struct rq *rq)
{
}

/*
 * Simple, special scheduling class for the per-CPU stop tasks:
 */
const struct sched_class stop_sched_class = {
	.next			= &dl_sched_class,

	.enqueue_task		= enqueue_task_stop,
	.dequeue_task		= dequeue_task_stop,
	.yield_task		= yield_task_stop,

	.check_preempt_curr	= check_preempt_curr_stop,

	.pick_next_task		= pick_next_task_stop,
	.put_prev_task		= put_prev_task_stop,

#ifdef CONFIG_SMP
	.select_task_rq		= select_task_rq_stop,
	.set_cpus_allowed	= set_cpus_allowed_common,
#endif

	.set_curr_task          = set_curr_task_stop,
	.task_tick		= task_tick_stop,

	.get_rr_interval	= get_rr_interval_stop,

	.prio_changed		= prio_changed_stop,
	.switched_to		= switched_to_stop,
	.update_curr		= update_curr_stop,
};
