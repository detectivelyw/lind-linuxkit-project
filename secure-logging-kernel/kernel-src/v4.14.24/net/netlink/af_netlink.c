extern int kernel_init_done;
int printk(const char *fmt, ...);
/*
 * NETLINK      Kernel-user communication protocol.
 *
 * 		Authors:	Alan Cox <alan@lxorguk.ukuu.org.uk>
 * 				Alexey Kuznetsov <kuznet@ms2.inr.ac.ru>
 * 				Patrick McHardy <kaber@trash.net>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Tue Jun 26 14:36:48 MEST 2001 Herbert "herp" Rosmanith
 *                               added netlink_proto_exit
 * Tue Jan 22 18:32:44 BRST 2002 Arnaldo C. de Melo <acme@conectiva.com.br>
 * 				 use nlk_sk, as sk->protinfo is on a diet 8)
 * Fri Jul 22 19:51:12 MEST 2005 Harald Welte <laforge@gnumonks.org>
 * 				 - inc module use count of module that owns
 * 				   the kernel socket in case userspace opens
 * 				   socket of same protocol
 * 				 - remove all module support, since netlink is
 * 				   mandatory if CONFIG_NET=y these days
 */

#include <linux/module.h>

#include <linux/capability.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/socket.h>
#include <linux/un.h>
#include <linux/fcntl.h>
#include <linux/termios.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/notifier.h>
#include <linux/security.h>
#include <linux/jhash.h>
#include <linux/jiffies.h>
#include <linux/random.h>
#include <linux/bitops.h>
#include <linux/mm.h>
#include <linux/types.h>
#include <linux/audit.h>
#include <linux/mutex.h>
#include <linux/vmalloc.h>
#include <linux/if_arp.h>
#include <linux/rhashtable.h>
#include <asm/cacheflush.h>
#include <linux/hash.h>
#include <linux/genetlink.h>
#include <linux/net_namespace.h>

#include <net/net_namespace.h>
#include <net/sock.h>
#include <net/scm.h>
#include <net/netlink.h>

#include "af_netlink.h"

struct listeners {
	struct rcu_head		rcu;
	unsigned long		masks[0];
};

/* state bits */
#define NETLINK_S_CONGESTED		0x0

static inline int netlink_is_kernel(struct sock *sk)
{
	return nlk_sk(sk)->flags & NETLINK_F_KERNEL_SOCKET;
}

struct netlink_table *nl_table __read_mostly;
EXPORT_SYMBOL_GPL(nl_table);

static DECLARE_WAIT_QUEUE_HEAD(nl_table_wait);

static struct lock_class_key nlk_cb_mutex_keys[MAX_LINKS];

static const char *const nlk_cb_mutex_key_strings[MAX_LINKS + 1] = {
	"nlk_cb_mutex-ROUTE",
	"nlk_cb_mutex-1",
	"nlk_cb_mutex-USERSOCK",
	"nlk_cb_mutex-FIREWALL",
	"nlk_cb_mutex-SOCK_DIAG",
	"nlk_cb_mutex-NFLOG",
	"nlk_cb_mutex-XFRM",
	"nlk_cb_mutex-SELINUX",
	"nlk_cb_mutex-ISCSI",
	"nlk_cb_mutex-AUDIT",
	"nlk_cb_mutex-FIB_LOOKUP",
	"nlk_cb_mutex-CONNECTOR",
	"nlk_cb_mutex-NETFILTER",
	"nlk_cb_mutex-IP6_FW",
	"nlk_cb_mutex-DNRTMSG",
	"nlk_cb_mutex-KOBJECT_UEVENT",
	"nlk_cb_mutex-GENERIC",
	"nlk_cb_mutex-17",
	"nlk_cb_mutex-SCSITRANSPORT",
	"nlk_cb_mutex-ECRYPTFS",
	"nlk_cb_mutex-RDMA",
	"nlk_cb_mutex-CRYPTO",
	"nlk_cb_mutex-SMC",
	"nlk_cb_mutex-23",
	"nlk_cb_mutex-24",
	"nlk_cb_mutex-25",
	"nlk_cb_mutex-26",
	"nlk_cb_mutex-27",
	"nlk_cb_mutex-28",
	"nlk_cb_mutex-29",
	"nlk_cb_mutex-30",
	"nlk_cb_mutex-31",
	"nlk_cb_mutex-MAX_LINKS"
};

static int netlink_dump(struct sock *sk);
static void netlink_skb_destructor(struct sk_buff *skb);

/* nl_table locking explained:
 * Lookup and traversal are protected with an RCU read-side lock. Insertion
 * and removal are protected with per bucket lock while using RCU list
 * modification primitives and may run in parallel to RCU protected lookups.
 * Destruction of the Netlink socket may only occur *after* nl_table_lock has
 * been acquired * either during or after the socket has been removed from
 * the list and after an RCU grace period.
 */
DEFINE_RWLOCK(nl_table_lock);
EXPORT_SYMBOL_GPL(nl_table_lock);
static atomic_t nl_table_users = ATOMIC_INIT(0);

#define nl_deref_protected(X) rcu_dereference_protected(X, lockdep_is_held(&nl_table_lock));

static BLOCKING_NOTIFIER_HEAD(netlink_chain);

static DEFINE_SPINLOCK(netlink_tap_lock);
static struct list_head netlink_tap_all __read_mostly;

static const struct rhashtable_params netlink_rhashtable_params;

static inline u32 netlink_group_mask(u32 group)
{
	return group ? 1 << (group - 1) : 0;
}

static struct sk_buff *netlink_to_full_skb(const struct sk_buff *skb,
					   gfp_t gfp_mask)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	unsigned int len = skb_end_offset(skb);
	struct sk_buff *new;

	new = alloc_skb(len, gfp_mask);
	if (new == NULL)
		return NULL;

	NETLINK_CB(new).portid = NETLINK_CB(skb).portid;
	NETLINK_CB(new).dst_group = NETLINK_CB(skb).dst_group;
	NETLINK_CB(new).creds = NETLINK_CB(skb).creds;

	skb_put_data(new, skb->data, len);
	return new;
}

int netlink_add_tap(struct netlink_tap *nt)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (unlikely(nt->dev->type != ARPHRD_NETLINK))
		return -EINVAL;

	spin_lock(&netlink_tap_lock);
	list_add_rcu(&nt->list, &netlink_tap_all);
	spin_unlock(&netlink_tap_lock);

	__module_get(nt->module);

	return 0;
}
EXPORT_SYMBOL_GPL(netlink_add_tap);

static int __netlink_remove_tap(struct netlink_tap *nt)
{
	bool found = false;
	struct netlink_tap *tmp;

	spin_lock(&netlink_tap_lock);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	list_for_each_entry(tmp, &netlink_tap_all, list) {
		if (nt == tmp) {
			list_del_rcu(&nt->list);
			found = true;
			goto out;
		}
	}

	pr_warn("__netlink_remove_tap: %p not found\n", nt);
out:
	spin_unlock(&netlink_tap_lock);

	if (found)
		module_put(nt->module);

	return found ? 0 : -ENODEV;
}

int netlink_remove_tap(struct netlink_tap *nt)
{
	int ret;

	ret = __netlink_remove_tap(nt);
	synchronize_net();

	return ret;
}
EXPORT_SYMBOL_GPL(netlink_remove_tap);

static bool netlink_filter_tap(const struct sk_buff *skb)
{
	struct sock *sk = skb->sk;

	/* We take the more conservative approach and
	 * whitelist socket protocols that may pass.
	 */
	switch (sk->sk_protocol) {
	case NETLINK_ROUTE:
	case NETLINK_USERSOCK:
	case NETLINK_SOCK_DIAG:
	case NETLINK_NFLOG:
	case NETLINK_XFRM:
	case NETLINK_FIB_LOOKUP:
	case NETLINK_NETFILTER:
	case NETLINK_GENERIC:
		return true;
	}

	return false;
}

static int __netlink_deliver_tap_skb(struct sk_buff *skb,
				     struct net_device *dev)
{
	struct sk_buff *nskb;
	struct sock *sk = skb->sk;
	int ret = -ENOMEM;

	if (!net_eq(dev_net(dev), sock_net(sk)))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return 0;
}

	dev_hold(dev);

	if (is_vmalloc_addr(skb->head))
		nskb = netlink_to_full_skb(skb, GFP_ATOMIC);
	else
		nskb = skb_clone(skb, GFP_ATOMIC);
	if (nskb) {
		nskb->dev = dev;
		nskb->protocol = htons((u16) sk->sk_protocol);
		nskb->pkt_type = netlink_is_kernel(sk) ?
				 PACKET_KERNEL : PACKET_USER;
		skb_reset_network_header(nskb);
		ret = dev_queue_xmit(nskb);
		if (unlikely(ret > 0))
			ret = net_xmit_errno(ret);
	}

	dev_put(dev);
	return ret;
}

static void __netlink_deliver_tap(struct sk_buff *skb)
{
	int ret;
	struct netlink_tap *tmp;

	if (!netlink_filter_tap(skb))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return;
}

	list_for_each_entry_rcu(tmp, &netlink_tap_all, list) {
		ret = __netlink_deliver_tap_skb(skb, tmp->dev);
		if (unlikely(ret))
			break;
	}
}

static void netlink_deliver_tap(struct sk_buff *skb)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	rcu_read_lock();

	if (unlikely(!list_empty(&netlink_tap_all)))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		__netlink_deliver_tap(skb);
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	rcu_read_unlock();
}

static void netlink_deliver_tap_kernel(struct sock *dst, struct sock *src,
				       struct sk_buff *skb)
{
	if (!(netlink_is_kernel(dst) && netlink_is_kernel(src)))
		netlink_deliver_tap(skb);
}

static void netlink_overrun(struct sock *sk)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = nlk_sk(sk);

	if (!(nlk->flags & NETLINK_F_RECV_NO_ENOBUFS)) {
		if (!test_and_set_bit(NETLINK_S_CONGESTED,
				      &nlk_sk(sk)->state)) {
			sk->sk_err = ENOBUFS;
			sk->sk_error_report(sk);
		}
	}
	atomic_inc(&sk->sk_drops);
}

static void netlink_rcv_wake(struct sock *sk)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = nlk_sk(sk);

	if (skb_queue_empty(&sk->sk_receive_queue))
		clear_bit(NETLINK_S_CONGESTED, &nlk->state);
	if (!test_bit(NETLINK_S_CONGESTED, &nlk->state))
		wake_up_interruptible(&nlk->wait);
}

static void netlink_skb_destructor(struct sk_buff *skb)
{
	if (is_vmalloc_addr(skb->head)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (!skb->cloned ||
		    !atomic_dec_return(&(skb_shinfo(skb)->dataref)))
			vfree(skb->head);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		skb->head = NULL;
	}
	if (skb->sk != NULL)
		sock_rfree(skb);
}

static void netlink_skb_set_owner_r(struct sk_buff *skb, struct sock *sk)
{
	WARN_ON(skb->sk != NULL);
	skb->sk = sk;
	skb->destructor = netlink_skb_destructor;
	atomic_add(skb->truesize, &sk->sk_rmem_alloc);
	sk_mem_charge(sk, skb->truesize);
}

static void netlink_sock_destruct(struct sock *sk)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = nlk_sk(sk);

	if (nlk->cb_running) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (nlk->cb.done)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			nlk->cb.done(&nlk->cb);
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		module_put(nlk->cb.module);
		kfree_skb(nlk->cb.skb);
	}

	skb_queue_purge(&sk->sk_receive_queue);

	if (!sock_flag(sk, SOCK_DEAD)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		printk(KERN_ERR "Freeing alive netlink socket %p\n", sk);
		return;
	}

	WARN_ON(atomic_read(&sk->sk_rmem_alloc));
	WARN_ON(refcount_read(&sk->sk_wmem_alloc));
	WARN_ON(nlk_sk(sk)->groups);
}

static void netlink_sock_destruct_work(struct work_struct *work)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = container_of(work, struct netlink_sock,
						work);

	sk_free(&nlk->sk);
}

/* This lock without WQ_FLAG_EXCLUSIVE is good on UP and it is _very_ bad on
 * SMP. Look, when several writers sleep and reader wakes them up, all but one
 * immediately hit write lock and grab all the cpus. Exclusive sleep solves
 * this, _but_ remember, it adds useless work on UP machines.
 */

void netlink_table_grab(void)
	__acquires(nl_table_lock)
{
	might_sleep();

	write_lock_irq(&nl_table_lock);

	if (atomic_read(&nl_table_users)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		DECLARE_WAITQUEUE(wait, current);

		add_wait_queue_exclusive(&nl_table_wait, &wait);
		for (;;) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			set_current_state(TASK_UNINTERRUPTIBLE);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			if (atomic_read(&nl_table_users) == 0)
				break;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			write_unlock_irq(&nl_table_lock);
			schedule();
			write_lock_irq(&nl_table_lock);
		}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		__set_current_state(TASK_RUNNING);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		remove_wait_queue(&nl_table_wait, &wait);
	}
}

void netlink_table_ungrab(void)
	__releases(nl_table_lock)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	write_unlock_irq(&nl_table_lock);
	wake_up(&nl_table_wait);
}

static inline void
netlink_lock_table(void)
{
	/* read_lock() synchronizes us to netlink_table_grab */

	read_lock(&nl_table_lock);
	atomic_inc(&nl_table_users);
	read_unlock(&nl_table_lock);
}

static inline void
netlink_unlock_table(void)
{
	if (atomic_dec_and_test(&nl_table_users))
		wake_up(&nl_table_wait);
}

struct netlink_compare_arg
{
	possible_net_t pnet;
	u32 portid;
};

/* Doing sizeof directly may yield 4 extra bytes on 64-bit. */
#define netlink_compare_arg_len \
	(offsetof(struct netlink_compare_arg, portid) + sizeof(u32))

static inline int netlink_compare(struct rhashtable_compare_arg *arg,
				  const void *ptr)
{
	const struct netlink_compare_arg *x = arg->key;
	const struct netlink_sock *nlk = ptr;

	return nlk->portid != x->portid ||
	       !net_eq(sock_net(&nlk->sk), read_pnet(&x->pnet));
}

static void netlink_compare_arg_init(struct netlink_compare_arg *arg,
				     struct net *net, u32 portid)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	memset(arg, 0, sizeof(*arg));
	write_pnet(&arg->pnet, net);
	arg->portid = portid;
}

static struct sock *__netlink_lookup(struct netlink_table *table, u32 portid,
				     struct net *net)
{
	struct netlink_compare_arg arg;

	netlink_compare_arg_init(&arg, net, portid);
	return rhashtable_lookup_fast(&table->hash, &arg,
				      netlink_rhashtable_params);
}

static int __netlink_insert(struct netlink_table *table, struct sock *sk)
{
	struct netlink_compare_arg arg;

	netlink_compare_arg_init(&arg, sock_net(sk), nlk_sk(sk)->portid);
	return rhashtable_lookup_insert_key(&table->hash, &arg,
					    &nlk_sk(sk)->node,
					    netlink_rhashtable_params);
}

static struct sock *netlink_lookup(struct net *net, int protocol, u32 portid)
{
	struct netlink_table *table = &nl_table[protocol];
	struct sock *sk;

	rcu_read_lock();
	sk = __netlink_lookup(table, portid, net);
	if (sk)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		sock_hold(sk);
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	rcu_read_unlock();

	return sk;
}

static const struct proto_ops netlink_ops;

static void
netlink_update_listeners(struct sock *sk)
{
	struct netlink_table *tbl = &nl_table[sk->sk_protocol];
	unsigned long mask;
	unsigned int i;
	struct listeners *listeners;

	listeners = nl_deref_protected(tbl->listeners);
	if (!listeners)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return;
}

	for (i = 0; i < NLGRPLONGS(tbl->groups); i++) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		mask = 0;
		sk_for_each_bound(sk, &tbl->mc_list) {
			if (i < NLGRPLONGS(nlk_sk(sk)->ngroups))
				mask |= nlk_sk(sk)->groups[i];
		}
		listeners->masks[i] = mask;
	}
	/* this function is only called with the netlink table "grabbed", which
	 * makes sure updates are visible before bind or setsockopt return. */
}

static int netlink_insert(struct sock *sk, u32 portid)
{
	struct netlink_table *table = &nl_table[sk->sk_protocol];
	int err;

	lock_sock(sk);

	err = nlk_sk(sk)->portid == portid ? 0 : -EBUSY;
	if (nlk_sk(sk)->bound)
		goto err;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	err = -ENOMEM;
	if (BITS_PER_LONG > 32 &&
	    unlikely(atomic_read(&table->hash.nelems) >= UINT_MAX))
		goto err;

	nlk_sk(sk)->portid = portid;
	sock_hold(sk);

	err = __netlink_insert(table, sk);
	if (err) {
		/* In case the hashtable backend returns with -EBUSY
		 * from here, it must not escape to the caller.
		 */
		if (unlikely(err == -EBUSY))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			err = -EOVERFLOW;
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (err == -EEXIST)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			err = -EADDRINUSE;
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		sock_put(sk);
		goto err;
	}

	/* We need to ensure that the socket is hashed and visible. */
	smp_wmb();
	nlk_sk(sk)->bound = portid;

err:
	release_sock(sk);
	return err;
}

static void netlink_remove(struct sock *sk)
{
	struct netlink_table *table;

	table = &nl_table[sk->sk_protocol];
	if (!rhashtable_remove_fast(&table->hash, &nlk_sk(sk)->node,
				    netlink_rhashtable_params)) {
		WARN_ON(refcount_read(&sk->sk_refcnt) == 1);
		__sock_put(sk);
	}

	netlink_table_grab();
	if (nlk_sk(sk)->subscriptions) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		__sk_del_bind_node(sk);
		netlink_update_listeners(sk);
	}
	if (sk->sk_protocol == NETLINK_GENERIC)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		atomic_inc(&genl_sk_destructing_cnt);
}
	netlink_table_ungrab();
}

static struct proto netlink_proto = {
	.name	  = "NETLINK",
	.owner	  = THIS_MODULE,
	.obj_size = sizeof(struct netlink_sock),
};

static int __netlink_create(struct net *net, struct socket *sock,
			    struct mutex *cb_mutex, int protocol,
			    int kern)
{
	struct sock *sk;
	struct netlink_sock *nlk;

	sock->ops = &netlink_ops;

	sk = sk_alloc(net, PF_NETLINK, GFP_KERNEL, &netlink_proto, kern);
	if (!sk)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENOMEM;
}

	sock_init_data(sock, sk);

	nlk = nlk_sk(sk);
	if (cb_mutex) {
		nlk->cb_mutex = cb_mutex;
	} else {
		nlk->cb_mutex = &nlk->cb_def_mutex;
		mutex_init(nlk->cb_mutex);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		lockdep_set_class_and_name(nlk->cb_mutex,
					   nlk_cb_mutex_keys + protocol,
					   nlk_cb_mutex_key_strings[protocol]);
	}
	init_waitqueue_head(&nlk->wait);

	sk->sk_destruct = netlink_sock_destruct;
	sk->sk_protocol = protocol;
	return 0;
}

static int netlink_create(struct net *net, struct socket *sock, int protocol,
			  int kern)
{
	struct module *module = NULL;
	struct mutex *cb_mutex;
	struct netlink_sock *nlk;
	int (*bind)(struct net *net, int group);
	void (*unbind)(struct net *net, int group);
	int err = 0;

	sock->state = SS_UNCONNECTED;

	if (sock->type != SOCK_RAW && sock->type != SOCK_DGRAM)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ESOCKTNOSUPPORT;
}

	if (protocol < 0 || protocol >= MAX_LINKS)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EPROTONOSUPPORT;
}

	netlink_lock_table();
#ifdef CONFIG_MODULES
	if (!nl_table[protocol].registered) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		netlink_unlock_table();
		request_module("net-pf-%d-proto-%d", PF_NETLINK, protocol);
		netlink_lock_table();
	}
#endif
	if (nl_table[protocol].registered &&
	    try_module_get(nl_table[protocol].module))
		module = nl_table[protocol].module;
	else
		err = -EPROTONOSUPPORT;
	cb_mutex = nl_table[protocol].cb_mutex;
	bind = nl_table[protocol].bind;
	unbind = nl_table[protocol].unbind;
	netlink_unlock_table();

	if (err < 0)
		goto out;

	err = __netlink_create(net, sock, cb_mutex, protocol, kern);
	if (err < 0)
		goto out_module;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	local_bh_disable();
	sock_prot_inuse_add(net, &netlink_proto, 1);
	local_bh_enable();

	nlk = nlk_sk(sock->sk);
	nlk->module = module;
	nlk->netlink_bind = bind;
	nlk->netlink_unbind = unbind;
out:
	return err;

out_module:
	module_put(module);
	goto out;
}

static void deferred_put_nlk_sk(struct rcu_head *head)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = container_of(head, struct netlink_sock, rcu);
	struct sock *sk = &nlk->sk;

	kfree(nlk->groups);
	nlk->groups = NULL;

	if (!refcount_dec_and_test(&sk->sk_refcnt))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return;
}

	if (nlk->cb_running && nlk->cb.done) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		INIT_WORK(&nlk->work, netlink_sock_destruct_work);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		schedule_work(&nlk->work);
		return;
	}

	sk_free(sk);
}

static int netlink_release(struct socket *sock)
{
	struct sock *sk = sock->sk;
	struct netlink_sock *nlk;

	if (!sk)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return 0;
}

	netlink_remove(sk);
	sock_orphan(sk);
	nlk = nlk_sk(sk);

	/*
	 * OK. Socket is unlinked, any packets that arrive now
	 * will be purged.
	 */

	/* must not acquire netlink_table_lock in any way again before unbind
	 * and notifying genetlink is done as otherwise it might deadlock
	 */
	if (nlk->netlink_unbind) {
		int i;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		for (i = 0; i < nlk->ngroups; i++)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			if (test_bit(i, nlk->groups))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				nlk->netlink_unbind(sock_net(sk), i + 1);
}
}
	}
	if (sk->sk_protocol == NETLINK_GENERIC &&
	    atomic_dec_return(&genl_sk_destructing_cnt) == 0)
		wake_up(&genl_sk_destructing_waitq);

	sock->sk = NULL;
	wake_up_interruptible_all(&nlk->wait);

	skb_queue_purge(&sk->sk_write_queue);

	if (nlk->portid && nlk->bound) {
		struct netlink_notify n = {
						.net = sock_net(sk),
						.protocol = sk->sk_protocol,
						.portid = nlk->portid,
					  };
		blocking_notifier_call_chain(&netlink_chain,
				NETLINK_URELEASE, &n);
	}

	module_put(nlk->module);

	if (netlink_is_kernel(sk)) {
		netlink_table_grab();
		BUG_ON(nl_table[sk->sk_protocol].registered == 0);
		if (--nl_table[sk->sk_protocol].registered == 0) {
			struct listeners *old;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			old = nl_deref_protected(nl_table[sk->sk_protocol].listeners);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			RCU_INIT_POINTER(nl_table[sk->sk_protocol].listeners, NULL);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			kfree_rcu(old, rcu);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			nl_table[sk->sk_protocol].module = NULL;
			nl_table[sk->sk_protocol].bind = NULL;
			nl_table[sk->sk_protocol].unbind = NULL;
			nl_table[sk->sk_protocol].flags = 0;
			nl_table[sk->sk_protocol].registered = 0;
		}
		netlink_table_ungrab();
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	local_bh_disable();
	sock_prot_inuse_add(sock_net(sk), &netlink_proto, -1);
	local_bh_enable();
	call_rcu(&nlk->rcu, deferred_put_nlk_sk);
	return 0;
}

static int netlink_autobind(struct socket *sock)
{
	struct sock *sk = sock->sk;
	struct net *net = sock_net(sk);
	struct netlink_table *table = &nl_table[sk->sk_protocol];
	s32 portid = task_tgid_vnr(current);
	int err;
	s32 rover = -4096;
	bool ok;

retry:
	cond_resched();
	rcu_read_lock();
	ok = !__netlink_lookup(table, portid, net);
	rcu_read_unlock();
	if (!ok) {
		/* Bind collision, search negative portid values. */
		if (rover == -4096)
			/* rover will be in range [S32_MIN, -4097] */
			rover = S32_MIN + prandom_u32_max(-4096 - S32_MIN);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		else if (rover >= -4096)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			rover = -4097;
}
		portid = rover--;
		goto retry;
	}

	err = netlink_insert(sk, portid);
	if (err == -EADDRINUSE)
		goto retry;

	/* If 2 threads race to autobind, that is fine.  */
	if (err == -EBUSY)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = 0;
}

	return err;
}

/**
 * __netlink_ns_capable - General netlink message capability test
 * @nsp: NETLINK_CB of the socket buffer holding a netlink command from userspace.
 * @user_ns: The user namespace of the capability to use
 * @cap: The capability to use
 *
 * Test to see if the opener of the socket we received the message
 * from had when the netlink socket was created and the sender of the
 * message has has the capability @cap in the user namespace @user_ns.
 */
bool __netlink_ns_capable(const struct netlink_skb_parms *nsp,
			struct user_namespace *user_ns, int cap)
{
	return ((nsp->flags & NETLINK_SKB_DST) ||
		file_ns_capable(nsp->sk->sk_socket->file, user_ns, cap)) &&
		ns_capable(user_ns, cap);
}
EXPORT_SYMBOL(__netlink_ns_capable);

/**
 * netlink_ns_capable - General netlink message capability test
 * @skb: socket buffer holding a netlink command from userspace
 * @user_ns: The user namespace of the capability to use
 * @cap: The capability to use
 *
 * Test to see if the opener of the socket we received the message
 * from had when the netlink socket was created and the sender of the
 * message has has the capability @cap in the user namespace @user_ns.
 */
bool netlink_ns_capable(const struct sk_buff *skb,
			struct user_namespace *user_ns, int cap)
{
	return __netlink_ns_capable(&NETLINK_CB(skb), user_ns, cap);
}
EXPORT_SYMBOL(netlink_ns_capable);

/**
 * netlink_capable - Netlink global message capability test
 * @skb: socket buffer holding a netlink command from userspace
 * @cap: The capability to use
 *
 * Test to see if the opener of the socket we received the message
 * from had when the netlink socket was created and the sender of the
 * message has has the capability @cap in all user namespaces.
 */
bool netlink_capable(const struct sk_buff *skb, int cap)
{
	return netlink_ns_capable(skb, &init_user_ns, cap);
}
EXPORT_SYMBOL(netlink_capable);

/**
 * netlink_net_capable - Netlink network namespace message capability test
 * @skb: socket buffer holding a netlink command from userspace
 * @cap: The capability to use
 *
 * Test to see if the opener of the socket we received the message
 * from had when the netlink socket was created and the sender of the
 * message has has the capability @cap over the network namespace of
 * the socket we received the message from.
 */
bool netlink_net_capable(const struct sk_buff *skb, int cap)
{
	return netlink_ns_capable(skb, sock_net(skb->sk)->user_ns, cap);
}
EXPORT_SYMBOL(netlink_net_capable);

static inline int netlink_allowed(const struct socket *sock, unsigned int flag)
{
	return (nl_table[sock->sk->sk_protocol].flags & flag) ||
		ns_capable(sock_net(sock->sk)->user_ns, CAP_NET_ADMIN);
}

static void
netlink_update_subscriptions(struct sock *sk, unsigned int subscriptions)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = nlk_sk(sk);

	if (nlk->subscriptions && !subscriptions)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		__sk_del_bind_node(sk);
}
	else if (!nlk->subscriptions && subscriptions)
		sk_add_bind_node(sk, &nl_table[sk->sk_protocol].mc_list);
	nlk->subscriptions = subscriptions;
}

static int netlink_realloc_groups(struct sock *sk)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = nlk_sk(sk);
	unsigned int groups;
	unsigned long *new_groups;
	int err = 0;

	netlink_table_grab();

	groups = nl_table[sk->sk_protocol].groups;
	if (!nl_table[sk->sk_protocol].registered) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = -ENOENT;
		goto out_unlock;
	}

	if (nlk->ngroups >= groups)
		goto out_unlock;

	new_groups = krealloc(nlk->groups, NLGRPSZ(groups), GFP_ATOMIC);
	if (new_groups == NULL) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = -ENOMEM;
		goto out_unlock;
	}
	memset((char *)new_groups + NLGRPSZ(nlk->ngroups), 0,
	       NLGRPSZ(groups) - NLGRPSZ(nlk->ngroups));

	nlk->groups = new_groups;
	nlk->ngroups = groups;
 out_unlock:
	netlink_table_ungrab();
	return err;
}

static void netlink_undo_bind(int group, long unsigned int groups,
			      struct sock *sk)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = nlk_sk(sk);
	int undo;

	if (!nlk->netlink_unbind)
		return;

	for (undo = 0; undo < group; undo++)
		if (test_bit(undo, &groups))
			nlk->netlink_unbind(sock_net(sk), undo + 1);
}

static int netlink_bind(struct socket *sock, struct sockaddr *addr,
			int addr_len)
{
	struct sock *sk = sock->sk;
	struct net *net = sock_net(sk);
	struct netlink_sock *nlk = nlk_sk(sk);
	struct sockaddr_nl *nladdr = (struct sockaddr_nl *)addr;
	int err = 0;
	long unsigned int groups = nladdr->nl_groups;
	bool bound;

	if (addr_len < sizeof(struct sockaddr_nl))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EINVAL;
}

	if (nladdr->nl_family != AF_NETLINK)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EINVAL;
}

	/* Only superuser is allowed to listen multicasts */
	if (groups) {
		if (!netlink_allowed(sock, NL_CFG_F_NONROOT_RECV))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return -EPERM;
}
		err = netlink_realloc_groups(sk);
		if (err)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return err;
}
	}

	bound = nlk->bound;
	if (bound) {
		/* Ensure nlk->portid is up-to-date. */
		smp_rmb();

		if (nladdr->nl_pid != nlk->portid)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return -EINVAL;
}
	}

	netlink_lock_table();
	if (nlk->netlink_bind && groups) {
		int group;

		for (group = 0; group < nlk->ngroups; group++) {
			if (!test_bit(group, &groups))
				continue;
			err = nlk->netlink_bind(net, group + 1);
			if (!err)
				continue;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			netlink_undo_bind(group, groups, sk);
			goto unlock;
		}
	}

	/* No need for barriers here as we return to user-space without
	 * using any of the bound attributes.
	 */
	if (!bound) {
		err = nladdr->nl_pid ?
			netlink_insert(sk, nladdr->nl_pid) :
			netlink_autobind(sock);
		if (err) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			netlink_undo_bind(nlk->ngroups, groups, sk);
			goto unlock;
		}
	}

	if (!groups && (nlk->groups == NULL || !(u32)nlk->groups[0]))
		goto unlock;
	netlink_unlock_table();

	netlink_table_grab();
	netlink_update_subscriptions(sk, nlk->subscriptions +
					 hweight32(groups) -
					 hweight32(nlk->groups[0]));
	nlk->groups[0] = (nlk->groups[0] & ~0xffffffffUL) | groups;
	netlink_update_listeners(sk);
	netlink_table_ungrab();

	return 0;

unlock:
	netlink_unlock_table();
	return err;
}

static int netlink_connect(struct socket *sock, struct sockaddr *addr,
			   int alen, int flags)
{
	int err = 0;
	struct sock *sk = sock->sk;
	struct netlink_sock *nlk = nlk_sk(sk);
	struct sockaddr_nl *nladdr = (struct sockaddr_nl *)addr;

	if (alen < sizeof(addr->sa_family))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EINVAL;
}

	if (addr->sa_family == AF_UNSPEC) {
		sk->sk_state	= NETLINK_UNCONNECTED;
		nlk->dst_portid	= 0;
		nlk->dst_group  = 0;
		return 0;
	}
	if (addr->sa_family != AF_NETLINK)
		return -EINVAL;

	if ((nladdr->nl_groups || nladdr->nl_pid) &&
	    !netlink_allowed(sock, NL_CFG_F_NONROOT_SEND))
		return -EPERM;

	/* No need for barriers here as we return to user-space without
	 * using any of the bound attributes.
	 */
	if (!nlk->bound)
		err = netlink_autobind(sock);

	if (err == 0) {
		sk->sk_state	= NETLINK_CONNECTED;
		nlk->dst_portid = nladdr->nl_pid;
		nlk->dst_group  = ffs(nladdr->nl_groups);
	}

	return err;
}

static int netlink_getname(struct socket *sock, struct sockaddr *addr,
			   int *addr_len, int peer)
{
	struct sock *sk = sock->sk;
	struct netlink_sock *nlk = nlk_sk(sk);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	DECLARE_SOCKADDR(struct sockaddr_nl *, nladdr, addr);

	nladdr->nl_family = AF_NETLINK;
	nladdr->nl_pad = 0;
	*addr_len = sizeof(*nladdr);

	if (peer) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		nladdr->nl_pid = nlk->dst_portid;
		nladdr->nl_groups = netlink_group_mask(nlk->dst_group);
	} else {
		nladdr->nl_pid = nlk->portid;
		netlink_lock_table();
		nladdr->nl_groups = nlk->groups ? nlk->groups[0] : 0;
		netlink_unlock_table();
	}
	return 0;
}

static int netlink_ioctl(struct socket *sock, unsigned int cmd,
			 unsigned long arg)
{
	/* try to hand this ioctl down to the NIC drivers.
	 */
	return -ENOIOCTLCMD;
}

static struct sock *netlink_getsockbyportid(struct sock *ssk, u32 portid)
{
	struct sock *sock;
	struct netlink_sock *nlk;

	sock = netlink_lookup(sock_net(ssk), ssk->sk_protocol, portid);
	if (!sock)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_PTR(-ECONNREFUSED);
}

	/* Don't bother queuing skb if kernel socket has no input function */
	nlk = nlk_sk(sock);
	if (sock->sk_state == NETLINK_CONNECTED &&
	    nlk->dst_portid != nlk_sk(ssk)->portid) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		sock_put(sock);
		return ERR_PTR(-ECONNREFUSED);
	}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return sock;
}

struct sock *netlink_getsockbyfilp(struct file *filp)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct inode *inode = file_inode(filp);
	struct sock *sock;

	if (!S_ISSOCK(inode->i_mode))
		return ERR_PTR(-ENOTSOCK);

	sock = SOCKET_I(inode)->sk;
	if (sock->sk_family != AF_NETLINK)
		return ERR_PTR(-EINVAL);

	sock_hold(sock);
	return sock;
}

static struct sk_buff *netlink_alloc_large_skb(unsigned int size,
					       int broadcast)
{
	struct sk_buff *skb;
	void *data;

	if (size <= NLMSG_GOODSIZE || broadcast)
		return alloc_skb(size, GFP_KERNEL);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	size = SKB_DATA_ALIGN(size) +
	       SKB_DATA_ALIGN(sizeof(struct skb_shared_info));

	data = vmalloc(size);
	if (data == NULL)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return NULL;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	skb = __build_skb(data, size);
	if (skb == NULL)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		vfree(data);
}
	else
		skb->destructor = netlink_skb_destructor;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return skb;
}

/*
 * Attach a skb to a netlink socket.
 * The caller must hold a reference to the destination socket. On error, the
 * reference is dropped. The skb is not send to the destination, just all
 * all error checks are performed and memory in the queue is reserved.
 * Return values:
 * < 0: error. skb freed, reference to sock dropped.
 * 0: continue
 * 1: repeat lookup - reference dropped while waiting for socket memory.
 */
int netlink_attachskb(struct sock *sk, struct sk_buff *skb,
		      long *timeo, struct sock *ssk)
{
	struct netlink_sock *nlk;

	nlk = nlk_sk(sk);

	if ((atomic_read(&sk->sk_rmem_alloc) > sk->sk_rcvbuf ||
	     test_bit(NETLINK_S_CONGESTED, &nlk->state))) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		DECLARE_WAITQUEUE(wait, current);
		if (!*timeo) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			if (!ssk || netlink_is_kernel(ssk))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				netlink_overrun(sk);
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			sock_put(sk);
			kfree_skb(skb);
			return -EAGAIN;
		}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		__set_current_state(TASK_INTERRUPTIBLE);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		add_wait_queue(&nlk->wait, &wait);

		if ((atomic_read(&sk->sk_rmem_alloc) > sk->sk_rcvbuf ||
		     test_bit(NETLINK_S_CONGESTED, &nlk->state)) &&
		    !sock_flag(sk, SOCK_DEAD))
			*timeo = schedule_timeout(*timeo);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		__set_current_state(TASK_RUNNING);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		remove_wait_queue(&nlk->wait, &wait);
		sock_put(sk);

		if (signal_pending(current)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			kfree_skb(skb);
			return sock_intr_errno(*timeo);
		}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return 1;
	}
	netlink_skb_set_owner_r(skb, sk);
	return 0;
}

static int __netlink_sendskb(struct sock *sk, struct sk_buff *skb)
{
	int len = skb->len;

	netlink_deliver_tap(skb);

	skb_queue_tail(&sk->sk_receive_queue, skb);
	sk->sk_data_ready(sk);
	return len;
}

int netlink_sendskb(struct sock *sk, struct sk_buff *skb)
{
	int len = __netlink_sendskb(sk, skb);

	sock_put(sk);
	return len;
}

void netlink_detachskb(struct sock *sk, struct sk_buff *skb)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	kfree_skb(skb);
	sock_put(sk);
}

static struct sk_buff *netlink_trim(struct sk_buff *skb, gfp_t allocation)
{
	int delta;

	WARN_ON(skb->sk != NULL);
	delta = skb->end - skb->tail;
	if (is_vmalloc_addr(skb->head) || delta * 2 < skb->truesize)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return skb;
}

	if (skb_shared(skb)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		struct sk_buff *nskb = skb_clone(skb, allocation);
		if (!nskb)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return skb;
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		consume_skb(skb);
		skb = nskb;
	}

	pskb_expand_head(skb, 0, -delta,
			 (allocation & ~__GFP_DIRECT_RECLAIM) |
			 __GFP_NOWARN | __GFP_NORETRY);
	return skb;
}

static int netlink_unicast_kernel(struct sock *sk, struct sk_buff *skb,
				  struct sock *ssk)
{
	int ret;
	struct netlink_sock *nlk = nlk_sk(sk);

	ret = -ECONNREFUSED;
	if (nlk->netlink_rcv != NULL) {
		ret = skb->len;
		netlink_skb_set_owner_r(skb, sk);
		NETLINK_CB(skb).sk = ssk;
		netlink_deliver_tap_kernel(sk, ssk, skb);
		nlk->netlink_rcv(skb);
		consume_skb(skb);
	} else {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		kfree_skb(skb);
	}
	sock_put(sk);
	return ret;
}

int netlink_unicast(struct sock *ssk, struct sk_buff *skb,
		    u32 portid, int nonblock)
{
	struct sock *sk;
	int err;
	long timeo;

	skb = netlink_trim(skb, gfp_any());

	timeo = sock_sndtimeo(ssk, nonblock);
retry:
	sk = netlink_getsockbyportid(ssk, portid);
	if (IS_ERR(sk)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		kfree_skb(skb);
		return PTR_ERR(sk);
	}
	if (netlink_is_kernel(sk))
		return netlink_unicast_kernel(sk, skb, ssk);

	if (sk_filter(sk, skb)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = skb->len;
		kfree_skb(skb);
		sock_put(sk);
		return err;
	}

	err = netlink_attachskb(sk, skb, &timeo, ssk);
	if (err == 1)
		goto retry;
	if (err)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return err;
}

	return netlink_sendskb(sk, skb);
}
EXPORT_SYMBOL(netlink_unicast);

int netlink_has_listeners(struct sock *sk, unsigned int group)
{
	int res = 0;
	struct listeners *listeners;

	BUG_ON(!netlink_is_kernel(sk));

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	rcu_read_lock();
	listeners = rcu_dereference(nl_table[sk->sk_protocol].listeners);

	if (listeners && group - 1 < nl_table[sk->sk_protocol].groups)
		res = test_bit(group - 1, listeners->masks);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	rcu_read_unlock();

	return res;
}
EXPORT_SYMBOL_GPL(netlink_has_listeners);

static int netlink_broadcast_deliver(struct sock *sk, struct sk_buff *skb)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = nlk_sk(sk);

	if (atomic_read(&sk->sk_rmem_alloc) <= sk->sk_rcvbuf &&
	    !test_bit(NETLINK_S_CONGESTED, &nlk->state)) {
		netlink_skb_set_owner_r(skb, sk);
		__netlink_sendskb(sk, skb);
		return atomic_read(&sk->sk_rmem_alloc) > (sk->sk_rcvbuf >> 1);
	}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return -1;
}

struct netlink_broadcast_data {
	struct sock *exclude_sk;
	struct net *net;
	u32 portid;
	u32 group;
	int failure;
	int delivery_failure;
	int congested;
	int delivered;
	gfp_t allocation;
	struct sk_buff *skb, *skb2;
	int (*tx_filter)(struct sock *dsk, struct sk_buff *skb, void *data);
	void *tx_data;
};

static void do_one_broadcast(struct sock *sk,
				    struct netlink_broadcast_data *p)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = nlk_sk(sk);
	int val;

	if (p->exclude_sk == sk)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return;
}

	if (nlk->portid == p->portid || p->group - 1 >= nlk->ngroups ||
	    !test_bit(p->group - 1, nlk->groups))
		return;

	if (!net_eq(sock_net(sk), p->net)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (!(nlk->flags & NETLINK_F_LISTEN_ALL_NSID))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (!peernet_has_id(sock_net(sk), p->net))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (!file_ns_capable(sk->sk_socket->file, p->net->user_ns,
				     CAP_NET_BROADCAST))
			return;
	}

	if (p->failure) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		netlink_overrun(sk);
		return;
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	sock_hold(sk);
	if (p->skb2 == NULL) {
		if (skb_shared(p->skb)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			p->skb2 = skb_clone(p->skb, p->allocation);
		} else {
			p->skb2 = skb_get(p->skb);
			/*
			 * skb ownership may have been set when
			 * delivered to a previous socket.
			 */
			skb_orphan(p->skb2);
		}
	}
	if (p->skb2 == NULL) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		netlink_overrun(sk);
		/* Clone failed. Notify ALL listeners. */
		p->failure = 1;
		if (nlk->flags & NETLINK_F_BROADCAST_SEND_ERROR)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			p->delivery_failure = 1;
}
		goto out;
	}
	if (p->tx_filter && p->tx_filter(sk, p->skb2, p->tx_data)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		kfree_skb(p->skb2);
		p->skb2 = NULL;
		goto out;
	}
	if (sk_filter(sk, p->skb2)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		kfree_skb(p->skb2);
		p->skb2 = NULL;
		goto out;
	}
	NETLINK_CB(p->skb2).nsid = peernet2id(sock_net(sk), p->net);
	if (NETLINK_CB(p->skb2).nsid != NETNSA_NSID_NOT_ASSIGNED)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		NETLINK_CB(p->skb2).nsid_is_set = true;
}
	val = netlink_broadcast_deliver(sk, p->skb2);
	if (val < 0) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		netlink_overrun(sk);
		if (nlk->flags & NETLINK_F_BROADCAST_SEND_ERROR)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			p->delivery_failure = 1;
}
	} else {
		p->congested |= val;
		p->delivered = 1;
		p->skb2 = NULL;
	}
out:
	sock_put(sk);
}

int netlink_broadcast_filtered(struct sock *ssk, struct sk_buff *skb, u32 portid,
	u32 group, gfp_t allocation,
	int (*filter)(struct sock *dsk, struct sk_buff *skb, void *data),
	void *filter_data)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct net *net = sock_net(ssk);
	struct netlink_broadcast_data info;
	struct sock *sk;

	skb = netlink_trim(skb, allocation);

	info.exclude_sk = ssk;
	info.net = net;
	info.portid = portid;
	info.group = group;
	info.failure = 0;
	info.delivery_failure = 0;
	info.congested = 0;
	info.delivered = 0;
	info.allocation = allocation;
	info.skb = skb;
	info.skb2 = NULL;
	info.tx_filter = filter;
	info.tx_data = filter_data;

	/* While we sleep in clone, do not allow to change socket list */

	netlink_lock_table();

	sk_for_each_bound(sk, &nl_table[ssk->sk_protocol].mc_list)
		do_one_broadcast(sk, &info);

	consume_skb(skb);

	netlink_unlock_table();

	if (info.delivery_failure) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		kfree_skb(info.skb2);
		return -ENOBUFS;
	}
	consume_skb(info.skb2);

	if (info.delivered) {
		if (info.congested && gfpflags_allow_blocking(allocation))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			yield();
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return 0;
	}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return -ESRCH;
}
EXPORT_SYMBOL(netlink_broadcast_filtered);

int netlink_broadcast(struct sock *ssk, struct sk_buff *skb, u32 portid,
		      u32 group, gfp_t allocation)
{
	return netlink_broadcast_filtered(ssk, skb, portid, group, allocation,
		NULL, NULL);
}
EXPORT_SYMBOL(netlink_broadcast);

struct netlink_set_err_data {
	struct sock *exclude_sk;
	u32 portid;
	u32 group;
	int code;
};

static int do_one_set_err(struct sock *sk, struct netlink_set_err_data *p)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = nlk_sk(sk);
	int ret = 0;

	if (sk == p->exclude_sk)
		goto out;

	if (!net_eq(sock_net(sk), sock_net(p->exclude_sk)))
		goto out;

	if (nlk->portid == p->portid || p->group - 1 >= nlk->ngroups ||
	    !test_bit(p->group - 1, nlk->groups))
		goto out;

	if (p->code == ENOBUFS && nlk->flags & NETLINK_F_RECV_NO_ENOBUFS) {
		ret = 1;
		goto out;
	}

	sk->sk_err = p->code;
	sk->sk_error_report(sk);
out:
	return ret;
}

/**
 * netlink_set_err - report error to broadcast listeners
 * @ssk: the kernel netlink socket, as returned by netlink_kernel_create()
 * @portid: the PORTID of a process that we want to skip (if any)
 * @group: the broadcast group that will notice the error
 * @code: error code, must be negative (as usual in kernelspace)
 *
 * This function returns the number of broadcast listeners that have set the
 * NETLINK_NO_ENOBUFS socket option.
 */
int netlink_set_err(struct sock *ssk, u32 portid, u32 group, int code)
{
	struct netlink_set_err_data info;
	struct sock *sk;
	int ret = 0;

	info.exclude_sk = ssk;
	info.portid = portid;
	info.group = group;
	/* sk->sk_err wants a positive error value */
	info.code = -code;

	read_lock(&nl_table_lock);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	sk_for_each_bound(sk, &nl_table[ssk->sk_protocol].mc_list)
		ret += do_one_set_err(sk, &info);

	read_unlock(&nl_table_lock);
	return ret;
}
EXPORT_SYMBOL(netlink_set_err);

/* must be called with netlink table grabbed */
static void netlink_update_socket_mc(struct netlink_sock *nlk,
				     unsigned int group,
				     int is_new)
{
	int old, new = !!is_new, subscriptions;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	old = test_bit(group - 1, nlk->groups);
	subscriptions = nlk->subscriptions - old + new;
	if (new)
		__set_bit(group - 1, nlk->groups);
	else
		__clear_bit(group - 1, nlk->groups);
	netlink_update_subscriptions(&nlk->sk, subscriptions);
	netlink_update_listeners(&nlk->sk);
}

static int netlink_setsockopt(struct socket *sock, int level, int optname,
			      char __user *optval, unsigned int optlen)
{
	struct sock *sk = sock->sk;
	struct netlink_sock *nlk = nlk_sk(sk);
	unsigned int val = 0;
	int err;

	if (level != SOL_NETLINK)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENOPROTOOPT;
}

	if (optlen >= sizeof(int) &&
	    get_user(val, (unsigned int __user *)optval))
		return -EFAULT;

	switch (optname) {
	case NETLINK_PKTINFO:
		if (val)
			nlk->flags |= NETLINK_F_RECV_PKTINFO;
		else
			nlk->flags &= ~NETLINK_F_RECV_PKTINFO;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = 0;
		break;
	case NETLINK_ADD_MEMBERSHIP:
	case NETLINK_DROP_MEMBERSHIP: {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (!netlink_allowed(sock, NL_CFG_F_NONROOT_RECV))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return -EPERM;
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = netlink_realloc_groups(sk);
		if (err)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return err;
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (!val || val - 1 >= nlk->ngroups)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return -EINVAL;
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (optname == NETLINK_ADD_MEMBERSHIP && nlk->netlink_bind) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			err = nlk->netlink_bind(sock_net(sk), val);
			if (err)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				return err;
}
		}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		netlink_table_grab();
		netlink_update_socket_mc(nlk, val,
					 optname == NETLINK_ADD_MEMBERSHIP);
		netlink_table_ungrab();
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (optname == NETLINK_DROP_MEMBERSHIP && nlk->netlink_unbind)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			nlk->netlink_unbind(sock_net(sk), val);
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = 0;
		break;
	}
	case NETLINK_BROADCAST_ERROR:
		if (val)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			nlk->flags |= NETLINK_F_BROADCAST_SEND_ERROR;
}
		else
			nlk->flags &= ~NETLINK_F_BROADCAST_SEND_ERROR;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = 0;
		break;
	case NETLINK_NO_ENOBUFS:
		if (val) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			nlk->flags |= NETLINK_F_RECV_NO_ENOBUFS;
			clear_bit(NETLINK_S_CONGESTED, &nlk->state);
			wake_up_interruptible(&nlk->wait);
		} else {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			nlk->flags &= ~NETLINK_F_RECV_NO_ENOBUFS;
		}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = 0;
		break;
	case NETLINK_LISTEN_ALL_NSID:
		if (!ns_capable(sock_net(sk)->user_ns, CAP_NET_BROADCAST))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return -EPERM;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (val)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			nlk->flags |= NETLINK_F_LISTEN_ALL_NSID;
}
		else
			nlk->flags &= ~NETLINK_F_LISTEN_ALL_NSID;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = 0;
		break;
	case NETLINK_CAP_ACK:
		if (val)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			nlk->flags |= NETLINK_F_CAP_ACK;
}
		else
			nlk->flags &= ~NETLINK_F_CAP_ACK;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = 0;
		break;
	case NETLINK_EXT_ACK:
		if (val)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			nlk->flags |= NETLINK_F_EXT_ACK;
}
		else
			nlk->flags &= ~NETLINK_F_EXT_ACK;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = 0;
		break;
	default:
		err = -ENOPROTOOPT;
	}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return err;
}

static int netlink_getsockopt(struct socket *sock, int level, int optname,
			      char __user *optval, int __user *optlen)
{
	struct sock *sk = sock->sk;
	struct netlink_sock *nlk = nlk_sk(sk);
	int len, val, err;

	if (level != SOL_NETLINK)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENOPROTOOPT;
}

	if (get_user(len, optlen))
		return -EFAULT;
	if (len < 0)
		return -EINVAL;

	switch (optname) {
	case NETLINK_PKTINFO:
		if (len < sizeof(int))
			return -EINVAL;
		len = sizeof(int);
		val = nlk->flags & NETLINK_F_RECV_PKTINFO ? 1 : 0;
		if (put_user(len, optlen) ||
		    put_user(val, optval))
			return -EFAULT;
		err = 0;
		break;
	case NETLINK_BROADCAST_ERROR:
		if (len < sizeof(int))
			return -EINVAL;
		len = sizeof(int);
		val = nlk->flags & NETLINK_F_BROADCAST_SEND_ERROR ? 1 : 0;
		if (put_user(len, optlen) ||
		    put_user(val, optval))
			return -EFAULT;
		err = 0;
		break;
	case NETLINK_NO_ENOBUFS:
		if (len < sizeof(int))
			return -EINVAL;
		len = sizeof(int);
		val = nlk->flags & NETLINK_F_RECV_NO_ENOBUFS ? 1 : 0;
		if (put_user(len, optlen) ||
		    put_user(val, optval))
			return -EFAULT;
		err = 0;
		break;
	case NETLINK_LIST_MEMBERSHIPS: {
		int pos, idx, shift;

		err = 0;
		netlink_lock_table();
		for (pos = 0; pos * 8 < nlk->ngroups; pos += sizeof(u32)) {
			if (len - pos < sizeof(u32))
				break;

			idx = pos / sizeof(unsigned long);
			shift = (pos % sizeof(unsigned long)) * 8;
			if (put_user((u32)(nlk->groups[idx] >> shift),
				     (u32 __user *)(optval + pos))) {
				err = -EFAULT;
				break;
			}
		}
		if (put_user(ALIGN(nlk->ngroups / 8, sizeof(u32)), optlen))
			err = -EFAULT;
		netlink_unlock_table();
		break;
	}
	case NETLINK_CAP_ACK:
		if (len < sizeof(int))
			return -EINVAL;
		len = sizeof(int);
		val = nlk->flags & NETLINK_F_CAP_ACK ? 1 : 0;
		if (put_user(len, optlen) ||
		    put_user(val, optval))
			return -EFAULT;
		err = 0;
		break;
	case NETLINK_EXT_ACK:
		if (len < sizeof(int))
			return -EINVAL;
		len = sizeof(int);
		val = nlk->flags & NETLINK_F_EXT_ACK ? 1 : 0;
		if (put_user(len, optlen) || put_user(val, optval))
			return -EFAULT;
		err = 0;
		break;
	default:
		err = -ENOPROTOOPT;
	}
	return err;
}

static void netlink_cmsg_recv_pktinfo(struct msghdr *msg, struct sk_buff *skb)
{
	struct nl_pktinfo info;

	info.group = NETLINK_CB(skb).dst_group;
	put_cmsg(msg, SOL_NETLINK, NETLINK_PKTINFO, sizeof(info), &info);
}

static void netlink_cmsg_listen_all_nsid(struct sock *sk, struct msghdr *msg,
					 struct sk_buff *skb)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (!NETLINK_CB(skb).nsid_is_set)
		return;

	put_cmsg(msg, SOL_NETLINK, NETLINK_LISTEN_ALL_NSID, sizeof(int),
		 &NETLINK_CB(skb).nsid);
}

static int netlink_sendmsg(struct socket *sock, struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
	struct netlink_sock *nlk = nlk_sk(sk);
	DECLARE_SOCKADDR(struct sockaddr_nl *, addr, msg->msg_name);
	u32 dst_portid;
	u32 dst_group;
	struct sk_buff *skb;
	int err;
	struct scm_cookie scm;
	u32 netlink_skb_flags = 0;

	if (msg->msg_flags&MSG_OOB)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EOPNOTSUPP;
}

	err = scm_send(sock, msg, &scm, true);
	if (err < 0)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return err;
}

	if (msg->msg_namelen) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = -EINVAL;
		if (addr->nl_family != AF_NETLINK)
			goto out;
		dst_portid = addr->nl_pid;
		dst_group = ffs(addr->nl_groups);
		err =  -EPERM;
		if ((dst_group || dst_portid) &&
		    !netlink_allowed(sock, NL_CFG_F_NONROOT_SEND))
			goto out;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		netlink_skb_flags |= NETLINK_SKB_DST;
	} else {
		dst_portid = nlk->dst_portid;
		dst_group = nlk->dst_group;
	}

	if (!nlk->bound) {
		err = netlink_autobind(sock);
		if (err)
			goto out;
	} else {
		/* Ensure nlk is hashed and visible. */
		smp_rmb();
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	err = -EMSGSIZE;
	if (len > sk->sk_sndbuf - 32)
		goto out;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	err = -ENOBUFS;
	skb = netlink_alloc_large_skb(len, dst_group);
	if (skb == NULL)
		goto out;

	NETLINK_CB(skb).portid	= nlk->portid;
	NETLINK_CB(skb).dst_group = dst_group;
	NETLINK_CB(skb).creds	= scm.creds;
	NETLINK_CB(skb).flags	= netlink_skb_flags;

	err = -EFAULT;
	if (memcpy_from_msg(skb_put(skb, len), msg, len)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		kfree_skb(skb);
		goto out;
	}

	err = security_netlink_send(sk, skb);
	if (err) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		kfree_skb(skb);
		goto out;
	}

	if (dst_group) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		refcount_inc(&skb->users);
		netlink_broadcast(sk, skb, dst_portid, dst_group, GFP_KERNEL);
	}
	err = netlink_unicast(sk, skb, dst_portid, msg->msg_flags&MSG_DONTWAIT);

out:
	scm_destroy(&scm);
	return err;
}

static int netlink_recvmsg(struct socket *sock, struct msghdr *msg, size_t len,
			   int flags)
{
	struct scm_cookie scm;
	struct sock *sk = sock->sk;
	struct netlink_sock *nlk = nlk_sk(sk);
	int noblock = flags&MSG_DONTWAIT;
	size_t copied;
	struct sk_buff *skb, *data_skb;
	int err, ret;

	if (flags&MSG_OOB)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EOPNOTSUPP;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	copied = 0;

	skb = skb_recv_datagram(sk, flags, noblock, &err);
	if (skb == NULL)
		goto out;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	data_skb = skb;

#ifdef CONFIG_COMPAT_NETLINK_MESSAGES
	if (unlikely(skb_shinfo(skb)->frag_list)) {
		/*
		 * If this skb has a frag_list, then here that means that we
		 * will have to use the frag_list skb's data for compat tasks
		 * and the regular skb's data for normal (non-compat) tasks.
		 *
		 * If we need to send the compat skb, assign it to the
		 * 'data_skb' variable so that it will be used below for data
		 * copying. We keep 'skb' for everything else, including
		 * freeing both later.
		 */
		if (flags & MSG_CMSG_COMPAT)
			data_skb = skb_shinfo(skb)->frag_list;
	}
#endif

	/* Record the max length of recvmsg() calls for future allocations */
	nlk->max_recvmsg_len = max(nlk->max_recvmsg_len, len);
	nlk->max_recvmsg_len = min_t(size_t, nlk->max_recvmsg_len,
				     SKB_WITH_OVERHEAD(32768));

	copied = data_skb->len;
	if (len < copied) {
		msg->msg_flags |= MSG_TRUNC;
		copied = len;
	}

	skb_reset_transport_header(data_skb);
	err = skb_copy_datagram_msg(data_skb, 0, msg, copied);

	if (msg->msg_name) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		DECLARE_SOCKADDR(struct sockaddr_nl *, addr, msg->msg_name);
		addr->nl_family = AF_NETLINK;
		addr->nl_pad    = 0;
		addr->nl_pid	= NETLINK_CB(skb).portid;
		addr->nl_groups	= netlink_group_mask(NETLINK_CB(skb).dst_group);
		msg->msg_namelen = sizeof(*addr);
	}

	if (nlk->flags & NETLINK_F_RECV_PKTINFO)
		netlink_cmsg_recv_pktinfo(msg, skb);
	if (nlk->flags & NETLINK_F_LISTEN_ALL_NSID)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		netlink_cmsg_listen_all_nsid(sk, msg, skb);
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	memset(&scm, 0, sizeof(scm));
	scm.creds = *NETLINK_CREDS(skb);
	if (flags & MSG_TRUNC)
		copied = data_skb->len;

	skb_free_datagram(sk, skb);

	if (nlk->cb_running &&
	    atomic_read(&sk->sk_rmem_alloc) <= sk->sk_rcvbuf / 2) {
		ret = netlink_dump(sk);
		if (ret) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			sk->sk_err = -ret;
			sk->sk_error_report(sk);
		}
	}

	scm_recv(sock, msg, &scm, flags);
out:
	netlink_rcv_wake(sk);
	return err ? : copied;
}

static void netlink_data_ready(struct sock *sk)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	BUG();
}

/*
 *	We export these functions to other modules. They provide a
 *	complete set of kernel non-blocking support for message
 *	queueing.
 */

struct sock *
__netlink_kernel_create(struct net *net, int unit, struct module *module,
			struct netlink_kernel_cfg *cfg)
{
	struct socket *sock;
	struct sock *sk;
	struct netlink_sock *nlk;
	struct listeners *listeners = NULL;
	struct mutex *cb_mutex = cfg ? cfg->cb_mutex : NULL;
	unsigned int groups;

	BUG_ON(!nl_table);

	if (unit < 0 || unit >= MAX_LINKS)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return NULL;
}

	if (sock_create_lite(PF_NETLINK, SOCK_DGRAM, unit, &sock))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return NULL;
}

	if (__netlink_create(net, sock, cb_mutex, unit, 1) < 0)
		goto out_sock_release_nosk;

	sk = sock->sk;

	if (!cfg || cfg->groups < 32)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		groups = 32;
}
	else
		groups = cfg->groups;

	listeners = kzalloc(sizeof(*listeners) + NLGRPSZ(groups), GFP_KERNEL);
	if (!listeners)
		goto out_sock_release;

	sk->sk_data_ready = netlink_data_ready;
	if (cfg && cfg->input)
		nlk_sk(sk)->netlink_rcv = cfg->input;

	if (netlink_insert(sk, 0))
		goto out_sock_release;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	nlk = nlk_sk(sk);
	nlk->flags |= NETLINK_F_KERNEL_SOCKET;

	netlink_table_grab();
	if (!nl_table[unit].registered) {
		nl_table[unit].groups = groups;
		rcu_assign_pointer(nl_table[unit].listeners, listeners);
		nl_table[unit].cb_mutex = cb_mutex;
		nl_table[unit].module = module;
		if (cfg) {
			nl_table[unit].bind = cfg->bind;
			nl_table[unit].unbind = cfg->unbind;
			nl_table[unit].flags = cfg->flags;
			if (cfg->compare)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				nl_table[unit].compare = cfg->compare;
}
		}
		nl_table[unit].registered = 1;
	} else {
		kfree(listeners);
		nl_table[unit].registered++;
	}
	netlink_table_ungrab();
	return sk;

out_sock_release:
	kfree(listeners);
	netlink_kernel_release(sk);
	return NULL;

out_sock_release_nosk:
	sock_release(sock);
	return NULL;
}
EXPORT_SYMBOL(__netlink_kernel_create);

void
netlink_kernel_release(struct sock *sk)
{
	if (sk == NULL || sk->sk_socket == NULL)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return;
}

	sock_release(sk->sk_socket);
}
EXPORT_SYMBOL(netlink_kernel_release);

int __netlink_change_ngroups(struct sock *sk, unsigned int groups)
{
	struct listeners *new, *old;
	struct netlink_table *tbl = &nl_table[sk->sk_protocol];

	if (groups < 32)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		groups = 32;
}

	if (NLGRPSZ(tbl->groups) < NLGRPSZ(groups)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		new = kzalloc(sizeof(*new) + NLGRPSZ(groups), GFP_ATOMIC);
		if (!new)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return -ENOMEM;
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		old = nl_deref_protected(tbl->listeners);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		memcpy(new->masks, old->masks, NLGRPSZ(tbl->groups));
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		rcu_assign_pointer(tbl->listeners, new);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		kfree_rcu(old, rcu);
	}
	tbl->groups = groups;

	return 0;
}

/**
 * netlink_change_ngroups - change number of multicast groups
 *
 * This changes the number of multicast groups that are available
 * on a certain netlink family. Note that it is not possible to
 * change the number of groups to below 32. Also note that it does
 * not implicitly call netlink_clear_multicast_users() when the
 * number of groups is reduced.
 *
 * @sk: The kernel netlink socket, as returned by netlink_kernel_create().
 * @groups: The new number of groups.
 */
int netlink_change_ngroups(struct sock *sk, unsigned int groups)
{
	int err;

	netlink_table_grab();
	err = __netlink_change_ngroups(sk, groups);
	netlink_table_ungrab();

	return err;
}

void __netlink_clear_multicast_users(struct sock *ksk, unsigned int group)
{
	struct sock *sk;
	struct netlink_table *tbl = &nl_table[ksk->sk_protocol];

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	sk_for_each_bound(sk, &tbl->mc_list)
		netlink_update_socket_mc(nlk_sk(sk), group, 0);
}

struct nlmsghdr *
__nlmsg_put(struct sk_buff *skb, u32 portid, u32 seq, int type, int len, int flags)
{
	struct nlmsghdr *nlh;
	int size = nlmsg_msg_size(len);

	nlh = skb_put(skb, NLMSG_ALIGN(size));
	nlh->nlmsg_type = type;
	nlh->nlmsg_len = size;
	nlh->nlmsg_flags = flags;
	nlh->nlmsg_pid = portid;
	nlh->nlmsg_seq = seq;
	if (!__builtin_constant_p(size) || NLMSG_ALIGN(size) - size != 0)
		memset(nlmsg_data(nlh) + len, 0, NLMSG_ALIGN(size) - size);
	return nlh;
}
EXPORT_SYMBOL(__nlmsg_put);

/*
 * It looks a bit ugly.
 * It would be better to create kernel thread.
 */

static int netlink_dump(struct sock *sk)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct netlink_sock *nlk = nlk_sk(sk);
	struct netlink_callback *cb;
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlh;
	struct module *module;
	int err = -ENOBUFS;
	int alloc_min_size;
	int alloc_size;

	mutex_lock(nlk->cb_mutex);
	if (!nlk->cb_running) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = -EINVAL;
		goto errout_skb;
	}

	if (atomic_read(&sk->sk_rmem_alloc) >= sk->sk_rcvbuf)
		goto errout_skb;

	/* NLMSG_GOODSIZE is small to avoid high order allocations being
	 * required, but it makes sense to _attempt_ a 16K bytes allocation
	 * to reduce number of system calls on dump operations, if user
	 * ever provided a big enough buffer.
	 */
	cb = &nlk->cb;
	alloc_min_size = max_t(int, cb->min_dump_alloc, NLMSG_GOODSIZE);

	if (alloc_min_size < nlk->max_recvmsg_len) {
		alloc_size = nlk->max_recvmsg_len;
		skb = alloc_skb(alloc_size,
				(GFP_KERNEL & ~__GFP_DIRECT_RECLAIM) |
				__GFP_NOWARN | __GFP_NORETRY);
	}
	if (!skb) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		alloc_size = alloc_min_size;
		skb = alloc_skb(alloc_size, GFP_KERNEL);
	}
	if (!skb)
		goto errout_skb;

	/* Trim skb to allocated size. User is expected to provide buffer as
	 * large as max(min_dump_alloc, 16KiB (mac_recvmsg_len capped at
	 * netlink_recvmsg())). dump will pack as many smaller messages as
	 * could fit within the allocated skb. skb is typically allocated
	 * with larger space than required (could be as much as near 2x the
	 * requested size with align to next power of 2 approach). Allowing
	 * dump to use the excess space makes it difficult for a user to have a
	 * reasonable static buffer based on the expected largest dump of a
	 * single netdev. The outcome is MSG_TRUNC error.
	 */
	skb_reserve(skb, skb_tailroom(skb) - alloc_size);
	netlink_skb_set_owner_r(skb, sk);

	if (nlk->dump_done_errno > 0)
		nlk->dump_done_errno = cb->dump(skb, cb);

	if (nlk->dump_done_errno > 0 ||
	    skb_tailroom(skb) < nlmsg_total_size(sizeof(nlk->dump_done_errno))) {
		mutex_unlock(nlk->cb_mutex);

		if (sk_filter(sk, skb))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			kfree_skb(skb);
}
		else
			__netlink_sendskb(sk, skb);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return 0;
	}

	nlh = nlmsg_put_answer(skb, cb, NLMSG_DONE,
			       sizeof(nlk->dump_done_errno), NLM_F_MULTI);
	if (WARN_ON(!nlh))
		goto errout_skb;

	nl_dump_check_consistent(cb, nlh);

	memcpy(nlmsg_data(nlh), &nlk->dump_done_errno,
	       sizeof(nlk->dump_done_errno));

	if (sk_filter(sk, skb))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		kfree_skb(skb);
}
	else
		__netlink_sendskb(sk, skb);

	if (cb->done)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		cb->done(cb);
}

	nlk->cb_running = false;
	module = cb->module;
	skb = cb->skb;
	mutex_unlock(nlk->cb_mutex);
	module_put(module);
	consume_skb(skb);
	return 0;

errout_skb:
	mutex_unlock(nlk->cb_mutex);
	kfree_skb(skb);
	return err;
}

int __netlink_dump_start(struct sock *ssk, struct sk_buff *skb,
			 const struct nlmsghdr *nlh,
			 struct netlink_dump_control *control)
{
	struct netlink_callback *cb;
	struct sock *sk;
	struct netlink_sock *nlk;
	int ret;

	refcount_inc(&skb->users);

	sk = netlink_lookup(sock_net(ssk), ssk->sk_protocol, NETLINK_CB(skb).portid);
	if (sk == NULL) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		ret = -ECONNREFUSED;
		goto error_free;
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	nlk = nlk_sk(sk);
	mutex_lock(nlk->cb_mutex);
	/* A dump is in progress... */
	if (nlk->cb_running) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		ret = -EBUSY;
		goto error_unlock;
	}
	/* add reference of module which cb->dump belongs to */
	if (!try_module_get(control->module)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		ret = -EPROTONOSUPPORT;
		goto error_unlock;
	}

	cb = &nlk->cb;
	memset(cb, 0, sizeof(*cb));
	cb->start = control->start;
	cb->dump = control->dump;
	cb->done = control->done;
	cb->nlh = nlh;
	cb->data = control->data;
	cb->module = control->module;
	cb->min_dump_alloc = control->min_dump_alloc;
	cb->skb = skb;

	if (cb->start) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		ret = cb->start(cb);
		if (ret)
			goto error_unlock;
	}

	nlk->cb_running = true;
	nlk->dump_done_errno = INT_MAX;

	mutex_unlock(nlk->cb_mutex);

	ret = netlink_dump(sk);

	sock_put(sk);

	if (ret)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ret;
}

	/* We successfully started a dump, by returning -EINTR we
	 * signal not to send ACK even if it was requested.
	 */
	return -EINTR;

error_unlock:
	sock_put(sk);
	mutex_unlock(nlk->cb_mutex);
error_free:
	kfree_skb(skb);
	return ret;
}
EXPORT_SYMBOL(__netlink_dump_start);

void netlink_ack(struct sk_buff *in_skb, struct nlmsghdr *nlh, int err,
		 const struct netlink_ext_ack *extack)
{
	struct sk_buff *skb;
	struct nlmsghdr *rep;
	struct nlmsgerr *errmsg;
	size_t payload = sizeof(*errmsg);
	size_t tlvlen = 0;
	struct netlink_sock *nlk = nlk_sk(NETLINK_CB(in_skb).sk);
	unsigned int flags = 0;
	bool nlk_has_extack = nlk->flags & NETLINK_F_EXT_ACK;

	/* Error messages get the original request appened, unless the user
	 * requests to cap the error message, and get extra error data if
	 * requested.
	 */
	if (err) {
		if (!(nlk->flags & NETLINK_F_CAP_ACK))
			payload += nlmsg_len(nlh);
		else
			flags |= NLM_F_CAPPED;
		if (nlk_has_extack && extack) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			if (extack->_msg)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				tlvlen += nla_total_size(strlen(extack->_msg) + 1);
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			if (extack->bad_attr)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				tlvlen += nla_total_size(sizeof(u32));
}
		}
	} else {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		flags |= NLM_F_CAPPED;

		if (nlk_has_extack && extack && extack->cookie_len)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			tlvlen += nla_total_size(extack->cookie_len);
}
	}

	if (tlvlen)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		flags |= NLM_F_ACK_TLVS;
}

	skb = nlmsg_new(payload + tlvlen, GFP_KERNEL);
	if (!skb) {
		struct sock *sk;

		sk = netlink_lookup(sock_net(in_skb->sk),
				    in_skb->sk->sk_protocol,
				    NETLINK_CB(in_skb).portid);
		if (sk) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			sk->sk_err = ENOBUFS;
			sk->sk_error_report(sk);
			sock_put(sk);
		}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return;
	}

	rep = __nlmsg_put(skb, NETLINK_CB(in_skb).portid, nlh->nlmsg_seq,
			  NLMSG_ERROR, payload, flags);
	errmsg = nlmsg_data(rep);
	errmsg->error = err;
	memcpy(&errmsg->msg, nlh, payload > sizeof(*errmsg) ? nlh->nlmsg_len : sizeof(*nlh));

	if (nlk_has_extack && extack) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (err) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			if (extack->_msg)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				WARN_ON(nla_put_string(skb, NLMSGERR_ATTR_MSG,
						       extack->_msg));
}
			if (extack->bad_attr &&
			    !WARN_ON((u8 *)extack->bad_attr < in_skb->data ||
				     (u8 *)extack->bad_attr >= in_skb->data +
							       in_skb->len))
				WARN_ON(nla_put_u32(skb, NLMSGERR_ATTR_OFFS,
						    (u8 *)extack->bad_attr -
						    in_skb->data));
		} else {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			if (extack->cookie_len)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				WARN_ON(nla_put(skb, NLMSGERR_ATTR_COOKIE,
						extack->cookie_len,
						extack->cookie));
}
		}
	}

	nlmsg_end(skb, rep);

	netlink_unicast(in_skb->sk, skb, NETLINK_CB(in_skb).portid, MSG_DONTWAIT);
}
EXPORT_SYMBOL(netlink_ack);

int netlink_rcv_skb(struct sk_buff *skb, int (*cb)(struct sk_buff *,
						   struct nlmsghdr *,
						   struct netlink_ext_ack *))
{
	struct netlink_ext_ack extack;
	struct nlmsghdr *nlh;
	int err;

	while (skb->len >= nlmsg_total_size(0)) {
		int msglen;

		memset(&extack, 0, sizeof(extack));
		nlh = nlmsg_hdr(skb);
		err = 0;

		if (nlh->nlmsg_len < NLMSG_HDRLEN || skb->len < nlh->nlmsg_len)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return 0;
}

		/* Only requests are handled by the kernel */
		if (!(nlh->nlmsg_flags & NLM_F_REQUEST))
			goto ack;

		/* Skip control messages */
		if (nlh->nlmsg_type < NLMSG_MIN_TYPE)
			goto ack;

		err = cb(skb, nlh, &extack);
		if (err == -EINTR)
			goto skip;

ack:
		if (nlh->nlmsg_flags & NLM_F_ACK || err)
			netlink_ack(skb, nlh, err, &extack);

skip:
		msglen = NLMSG_ALIGN(nlh->nlmsg_len);
		if (msglen > skb->len)
			msglen = skb->len;
		skb_pull(skb, msglen);
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return 0;
}
EXPORT_SYMBOL(netlink_rcv_skb);

/**
 * nlmsg_notify - send a notification netlink message
 * @sk: netlink socket to use
 * @skb: notification message
 * @portid: destination netlink portid for reports or 0
 * @group: destination multicast group or 0
 * @report: 1 to report back, 0 to disable
 * @flags: allocation flags
 */
int nlmsg_notify(struct sock *sk, struct sk_buff *skb, u32 portid,
		 unsigned int group, int report, gfp_t flags)
{
	int err = 0;

	if (group) {
		int exclude_portid = 0;

		if (report) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			refcount_inc(&skb->users);
			exclude_portid = portid;
		}

		/* errors reported via destination sk->sk_err, but propagate
		 * delivery errors if NETLINK_BROADCAST_ERROR flag is set */
		err = nlmsg_multicast(sk, skb, exclude_portid, group, flags);
	}

	if (report) {
		int err2;

		err2 = nlmsg_unicast(sk, skb, portid);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (!err || err == -ESRCH)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			err = err2;
}
	}

	return err;
}
EXPORT_SYMBOL(nlmsg_notify);

#ifdef CONFIG_PROC_FS
struct nl_seq_iter {
	struct seq_net_private p;
	struct rhashtable_iter hti;
	int link;
};

static int netlink_walk_start(struct nl_seq_iter *iter)
{
	int err;

	err = rhashtable_walk_init(&nl_table[iter->link].hash, &iter->hti,
				   GFP_KERNEL);
	if (err) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		iter->link = MAX_LINKS;
		return err;
	}

	err = rhashtable_walk_start(&iter->hti);
	return err == -EAGAIN ? 0 : err;
}

static void netlink_walk_stop(struct nl_seq_iter *iter)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	rhashtable_walk_stop(&iter->hti);
	rhashtable_walk_exit(&iter->hti);
}

static void *__netlink_seq_next(struct seq_file *seq)
{
	struct nl_seq_iter *iter = seq->private;
	struct netlink_sock *nlk;

	do {
		for (;;) {
			int err;

			nlk = rhashtable_walk_next(&iter->hti);

			if (IS_ERR(nlk)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				if (PTR_ERR(nlk) == -EAGAIN)
					continue;

				return nlk;
			}

			if (nlk)
				break;

			netlink_walk_stop(iter);
			if (++iter->link >= MAX_LINKS)
				return NULL;

			err = netlink_walk_start(iter);
			if (err)
				return ERR_PTR(err);
		}
	} while (sock_net(&nlk->sk) != seq_file_net(seq));

	return nlk;
}

static void *netlink_seq_start(struct seq_file *seq, loff_t *posp)
{
	struct nl_seq_iter *iter = seq->private;
	void *obj = SEQ_START_TOKEN;
	loff_t pos;
	int err;

	iter->link = 0;

	err = netlink_walk_start(iter);
	if (err)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_PTR(err);
}

	for (pos = *posp; pos && obj && !IS_ERR(obj); pos--)
		obj = __netlink_seq_next(seq);

	return obj;
}

static void *netlink_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	++*pos;
	return __netlink_seq_next(seq);
}

static void netlink_seq_stop(struct seq_file *seq, void *v)
{
	struct nl_seq_iter *iter = seq->private;

	if (iter->link >= MAX_LINKS)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return;
}

	netlink_walk_stop(iter);
}


static int netlink_seq_show(struct seq_file *seq, void *v)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (v == SEQ_START_TOKEN) {
		seq_puts(seq,
			 "sk       Eth Pid    Groups   "
			 "Rmem     Wmem     Dump     Locks     Drops     Inode\n");
	} else {
		struct sock *s = v;
		struct netlink_sock *nlk = nlk_sk(s);

		seq_printf(seq, "%pK %-3d %-6u %08x %-8d %-8d %d %-8d %-8d %-8lu\n",
			   s,
			   s->sk_protocol,
			   nlk->portid,
			   nlk->groups ? (u32)nlk->groups[0] : 0,
			   sk_rmem_alloc_get(s),
			   sk_wmem_alloc_get(s),
			   nlk->cb_running,
			   refcount_read(&s->sk_refcnt),
			   atomic_read(&s->sk_drops),
			   sock_i_ino(s)
			);

	}
	return 0;
}

static const struct seq_operations netlink_seq_ops = {
	.start  = netlink_seq_start,
	.next   = netlink_seq_next,
	.stop   = netlink_seq_stop,
	.show   = netlink_seq_show,
};


static int netlink_seq_open(struct inode *inode, struct file *file)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return seq_open_net(inode, file, &netlink_seq_ops,
				sizeof(struct nl_seq_iter));
}

static const struct file_operations netlink_seq_fops = {
	.owner		= THIS_MODULE,
	.open		= netlink_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release_net,
};

#endif

int netlink_register_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&netlink_chain, nb);
}
EXPORT_SYMBOL(netlink_register_notifier);

int netlink_unregister_notifier(struct notifier_block *nb)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return blocking_notifier_chain_unregister(&netlink_chain, nb);
}
EXPORT_SYMBOL(netlink_unregister_notifier);

static const struct proto_ops netlink_ops = {
	.family =	PF_NETLINK,
	.owner =	THIS_MODULE,
	.release =	netlink_release,
	.bind =		netlink_bind,
	.connect =	netlink_connect,
	.socketpair =	sock_no_socketpair,
	.accept =	sock_no_accept,
	.getname =	netlink_getname,
	.poll =		datagram_poll,
	.ioctl =	netlink_ioctl,
	.listen =	sock_no_listen,
	.shutdown =	sock_no_shutdown,
	.setsockopt =	netlink_setsockopt,
	.getsockopt =	netlink_getsockopt,
	.sendmsg =	netlink_sendmsg,
	.recvmsg =	netlink_recvmsg,
	.mmap =		sock_no_mmap,
	.sendpage =	sock_no_sendpage,
};

static const struct net_proto_family netlink_family_ops = {
	.family = PF_NETLINK,
	.create = netlink_create,
	.owner	= THIS_MODULE,	/* for consistency 8) */
};

static int __net_init netlink_net_init(struct net *net)
{
#ifdef CONFIG_PROC_FS
	if (!proc_create("netlink", 0, net->proc_net, &netlink_seq_fops))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENOMEM;
}
#endif
	return 0;
}

static void __net_exit netlink_net_exit(struct net *net)
{
#ifdef CONFIG_PROC_FS
	remove_proc_entry("netlink", net->proc_net);
#endif
}

static void __init netlink_add_usersock_entry(void)
{
	struct listeners *listeners;
	int groups = 32;

	listeners = kzalloc(sizeof(*listeners) + NLGRPSZ(groups), GFP_KERNEL);
	if (!listeners)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		panic("netlink_add_usersock_entry: Cannot allocate listeners\n");
}

	netlink_table_grab();

	nl_table[NETLINK_USERSOCK].groups = groups;
	rcu_assign_pointer(nl_table[NETLINK_USERSOCK].listeners, listeners);
	nl_table[NETLINK_USERSOCK].module = THIS_MODULE;
	nl_table[NETLINK_USERSOCK].registered = 1;
	nl_table[NETLINK_USERSOCK].flags = NL_CFG_F_NONROOT_SEND;

	netlink_table_ungrab();
}

static struct pernet_operations __net_initdata netlink_net_ops = {
	.init = netlink_net_init,
	.exit = netlink_net_exit,
};

static inline u32 netlink_hash(const void *data, u32 len, u32 seed)
{
	const struct netlink_sock *nlk = data;
	struct netlink_compare_arg arg;

	netlink_compare_arg_init(&arg, sock_net(&nlk->sk), nlk->portid);
	return jhash2((u32 *)&arg, netlink_compare_arg_len / sizeof(u32), seed);
}

static const struct rhashtable_params netlink_rhashtable_params = {
	.head_offset = offsetof(struct netlink_sock, node),
	.key_len = netlink_compare_arg_len,
	.obj_hashfn = netlink_hash,
	.obj_cmpfn = netlink_compare,
	.automatic_shrinking = true,
};

static int __init netlink_proto_init(void)
{
	int i;
	int err = proto_register(&netlink_proto, 0);

	if (err != 0)
		goto out;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	BUILD_BUG_ON(sizeof(struct netlink_skb_parms) > FIELD_SIZEOF(struct sk_buff, cb));

	nl_table = kcalloc(MAX_LINKS, sizeof(*nl_table), GFP_KERNEL);
	if (!nl_table)
		goto panic;

	for (i = 0; i < MAX_LINKS; i++) {
		if (rhashtable_init(&nl_table[i].hash,
				    &netlink_rhashtable_params) < 0) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			while (--i > 0)
				rhashtable_destroy(&nl_table[i].hash);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			kfree(nl_table);
			goto panic;
		}
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	INIT_LIST_HEAD(&netlink_tap_all);

	netlink_add_usersock_entry();

	sock_register(&netlink_family_ops);
	register_pernet_subsys(&netlink_net_ops);
	/* The netlink device handler may be needed early. */
	rtnetlink_init();
out:
	return err;
panic:
	panic("netlink_init: Cannot allocate nl_table\n");
}

core_initcall(netlink_proto_init);
