extern int kernel_init_done;
int printk(const char *fmt, ...);
/*
 * Cryptographic API for algorithms (i.e., low-level API).
 *
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <linux/err.h>
#include <linux/errno.h>
#include <linux/fips.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/rtnetlink.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "internal.h"

static LIST_HEAD(crypto_template_list);

static inline int crypto_set_driver_name(struct crypto_alg *alg)
{
	static const char suffix[] = "-generic";
	char *driver_name = alg->cra_driver_name;
	int len;

	if (*driver_name)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return 0;
}

	len = strlcpy(driver_name, alg->cra_name, CRYPTO_MAX_ALG_NAME);
	if (len + sizeof(suffix) > CRYPTO_MAX_ALG_NAME)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENAMETOOLONG;
}

	memcpy(driver_name + len, suffix, sizeof(suffix));
	return 0;
}

static inline void crypto_check_module_sig(struct module *mod)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (fips_enabled && mod && !module_sig_ok(mod))
		panic("Module %s signature verification failed in FIPS mode\n",
		      module_name(mod));
}

static int crypto_check_alg(struct crypto_alg *alg)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	crypto_check_module_sig(alg->cra_module);

	if (alg->cra_alignmask & (alg->cra_alignmask + 1))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EINVAL;
}

	if (alg->cra_blocksize > PAGE_SIZE / 8)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EINVAL;
}

	if (alg->cra_priority < 0)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EINVAL;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	atomic_set(&alg->cra_refcnt, 1);

	return crypto_set_driver_name(alg);
}

static void crypto_free_instance(struct crypto_instance *inst)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (!inst->alg.cra_type->free) {
		inst->tmpl->free(inst);
		return;
	}

	inst->alg.cra_type->free(inst);
}

static void crypto_destroy_instance(struct crypto_alg *alg)
{
	struct crypto_instance *inst = (void *)alg;
	struct crypto_template *tmpl = inst->tmpl;

	crypto_free_instance(inst);
	crypto_tmpl_put(tmpl);
}

static struct list_head *crypto_more_spawns(struct crypto_alg *alg,
					    struct list_head *stack,
					    struct list_head *top,
					    struct list_head *secondary_spawns)
{
	struct crypto_spawn *spawn, *n;

	spawn = list_first_entry_or_null(stack, struct crypto_spawn, list);
	if (!spawn)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return NULL;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	n = list_next_entry(spawn, list);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (spawn->alg && &n->list != stack && !n->alg)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		n->alg = (n->list.next == stack) ? alg :
			 &list_next_entry(n, list)->inst->alg;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	list_move(&spawn->list, secondary_spawns);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return &n->list == stack ? top : &n->inst->alg.cra_users;
}

static void crypto_remove_instance(struct crypto_instance *inst,
				   struct list_head *list)
{
	struct crypto_template *tmpl = inst->tmpl;

	if (crypto_is_dead(&inst->alg))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return;
}

	inst->alg.cra_flags |= CRYPTO_ALG_DEAD;
	if (hlist_unhashed(&inst->list))
		return;

	if (!tmpl || !crypto_tmpl_get(tmpl))
		return;

	crypto_notify(CRYPTO_MSG_ALG_UNREGISTER, &inst->alg);
	list_move(&inst->alg.cra_list, list);
	hlist_del(&inst->list);
	inst->alg.cra_destroy = crypto_destroy_instance;

	BUG_ON(!list_empty(&inst->alg.cra_users));
}

void crypto_remove_spawns(struct crypto_alg *alg, struct list_head *list,
			  struct crypto_alg *nalg)
{
	u32 new_type = (nalg ?: alg)->cra_flags;
	struct crypto_spawn *spawn, *n;
	LIST_HEAD(secondary_spawns);
	struct list_head *spawns;
	LIST_HEAD(stack);
	LIST_HEAD(top);

	spawns = &alg->cra_users;
	list_for_each_entry_safe(spawn, n, spawns, list) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if ((spawn->alg->cra_flags ^ new_type) & spawn->mask)
			continue;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		list_move(&spawn->list, &top);
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	spawns = &top;
	do {
		while (!list_empty(spawns)) {
			struct crypto_instance *inst;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			spawn = list_first_entry(spawns, struct crypto_spawn,
						 list);
			inst = spawn->inst;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			BUG_ON(&inst->alg == alg);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			list_move(&spawn->list, &stack);

			if (&inst->alg == nalg)
				break;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			spawn->alg = NULL;
			spawns = &inst->alg.cra_users;

			/*
			 * We may encounter an unregistered instance here, since
			 * an instance's spawns are set up prior to the instance
			 * being registered.  An unregistered instance will have
			 * NULL ->cra_users.next, since ->cra_users isn't
			 * properly initialized until registration.  But an
			 * unregistered instance cannot have any users, so treat
			 * it the same as ->cra_users being empty.
			 */
			if (spawns->next == NULL)
				break;
		}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	} while ((spawns = crypto_more_spawns(alg, &stack, &top,
					      &secondary_spawns)));

	list_for_each_entry_safe(spawn, n, &secondary_spawns, list) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (spawn->alg)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			list_move(&spawn->list, &spawn->alg->cra_users);
}
		else
			crypto_remove_instance(spawn->inst, list);
	}
}
EXPORT_SYMBOL_GPL(crypto_remove_spawns);

static struct crypto_larval *__crypto_register_alg(struct crypto_alg *alg)
{
	struct crypto_alg *q;
	struct crypto_larval *larval;
	int ret = -EAGAIN;

	if (crypto_is_dead(alg))
		goto err;

	INIT_LIST_HEAD(&alg->cra_users);

	/* No cheating! */
	alg->cra_flags &= ~CRYPTO_ALG_TESTED;

	ret = -EEXIST;

	list_for_each_entry(q, &crypto_alg_list, cra_list) {
		if (q == alg)
			goto err;

		if (crypto_is_moribund(q))
			continue;

		if (crypto_is_larval(q)) {
			if (!strcmp(alg->cra_driver_name, q->cra_driver_name))
				goto err;
			continue;
		}

		if (!strcmp(q->cra_driver_name, alg->cra_name) ||
		    !strcmp(q->cra_name, alg->cra_driver_name))
			goto err;
	}

	larval = crypto_larval_alloc(alg->cra_name,
				     alg->cra_flags | CRYPTO_ALG_TESTED, 0);
	if (IS_ERR(larval))
		goto out;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	ret = -ENOENT;
	larval->adult = crypto_mod_get(alg);
	if (!larval->adult)
		goto free_larval;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	atomic_set(&larval->alg.cra_refcnt, 1);
	memcpy(larval->alg.cra_driver_name, alg->cra_driver_name,
	       CRYPTO_MAX_ALG_NAME);
	larval->alg.cra_priority = alg->cra_priority;

	list_add(&alg->cra_list, &crypto_alg_list);
	list_add(&larval->alg.cra_list, &crypto_alg_list);

out:
	return larval;

free_larval:
	kfree(larval);
err:
	larval = ERR_PTR(ret);
	goto out;
}

void crypto_alg_tested(const char *name, int err)
{
	struct crypto_larval *test;
	struct crypto_alg *alg;
	struct crypto_alg *q;
	LIST_HEAD(list);

	down_write(&crypto_alg_sem);
	list_for_each_entry(q, &crypto_alg_list, cra_list) {
		if (crypto_is_moribund(q) || !crypto_is_larval(q))
			continue;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		test = (struct crypto_larval *)q;

		if (!strcmp(q->cra_driver_name, name))
			goto found;
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	pr_err("alg: Unexpected test result for %s: %d\n", name, err);
	goto unlock;

found:
	q->cra_flags |= CRYPTO_ALG_DEAD;
	alg = test->adult;
	if (err || list_empty(&alg->cra_list))
		goto complete;

	alg->cra_flags |= CRYPTO_ALG_TESTED;

	list_for_each_entry(q, &crypto_alg_list, cra_list) {
		if (q == alg)
			continue;

		if (crypto_is_moribund(q))
			continue;

		if (crypto_is_larval(q)) {
			struct crypto_larval *larval = (void *)q;

			/*
			 * Check to see if either our generic name or
			 * specific name can satisfy the name requested
			 * by the larval entry q.
			 */
			if (strcmp(alg->cra_name, q->cra_name) &&
			    strcmp(alg->cra_driver_name, q->cra_name))
				continue;

			if (larval->adult)
				continue;
			if ((q->cra_flags ^ alg->cra_flags) & larval->mask)
				continue;
			if (!crypto_mod_get(alg))
				continue;

			larval->adult = alg;
			continue;
		}

		if (strcmp(alg->cra_name, q->cra_name))
			continue;

		if (strcmp(alg->cra_driver_name, q->cra_driver_name) &&
		    q->cra_priority > alg->cra_priority)
			continue;

		crypto_remove_spawns(q, &list, alg);
	}

complete:
	complete_all(&test->completion);

unlock:
	up_write(&crypto_alg_sem);

	crypto_remove_final(&list);
}
EXPORT_SYMBOL_GPL(crypto_alg_tested);

void crypto_remove_final(struct list_head *list)
{
	struct crypto_alg *alg;
	struct crypto_alg *n;

	list_for_each_entry_safe(alg, n, list, cra_list) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		list_del_init(&alg->cra_list);
		crypto_alg_put(alg);
	}
}
EXPORT_SYMBOL_GPL(crypto_remove_final);

static void crypto_wait_for_test(struct crypto_larval *larval)
{
	int err;

	err = crypto_probing_notify(CRYPTO_MSG_ALG_REGISTER, larval->adult);
	if (err != NOTIFY_STOP) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (WARN_ON(err != NOTIFY_DONE))
			goto out;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		crypto_alg_tested(larval->alg.cra_driver_name, 0);
	}

	err = wait_for_completion_killable(&larval->completion);
	WARN_ON(err);

out:
	crypto_larval_kill(&larval->alg);
}

int crypto_register_alg(struct crypto_alg *alg)
{
	struct crypto_larval *larval;
	int err;

	alg->cra_flags &= ~CRYPTO_ALG_DEAD;
	err = crypto_check_alg(alg);
	if (err)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return err;
}

	down_write(&crypto_alg_sem);
	larval = __crypto_register_alg(alg);
	up_write(&crypto_alg_sem);

	if (IS_ERR(larval))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return PTR_ERR(larval);
}

	crypto_wait_for_test(larval);
	return 0;
}
EXPORT_SYMBOL_GPL(crypto_register_alg);

static int crypto_remove_alg(struct crypto_alg *alg, struct list_head *list)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (unlikely(list_empty(&alg->cra_list)))
		return -ENOENT;

	alg->cra_flags |= CRYPTO_ALG_DEAD;

	crypto_notify(CRYPTO_MSG_ALG_UNREGISTER, alg);
	list_del_init(&alg->cra_list);
	crypto_remove_spawns(alg, list, NULL);

	return 0;
}

int crypto_unregister_alg(struct crypto_alg *alg)
{
	int ret;
	LIST_HEAD(list);

	down_write(&crypto_alg_sem);
	ret = crypto_remove_alg(alg, &list);
	up_write(&crypto_alg_sem);

	if (ret)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ret;
}

	BUG_ON(atomic_read(&alg->cra_refcnt) != 1);
	if (alg->cra_destroy)
		alg->cra_destroy(alg);

	crypto_remove_final(&list);
	return 0;
}
EXPORT_SYMBOL_GPL(crypto_unregister_alg);

int crypto_register_algs(struct crypto_alg *algs, int count)
{
	int i, ret;

	for (i = 0; i < count; i++) {
		ret = crypto_register_alg(&algs[i]);
		if (ret)
			goto err;
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return 0;

err:
	for (--i; i >= 0; --i)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		crypto_unregister_alg(&algs[i]);
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return ret;
}
EXPORT_SYMBOL_GPL(crypto_register_algs);

int crypto_unregister_algs(struct crypto_alg *algs, int count)
{
	int i, ret;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	for (i = 0; i < count; i++) {
		ret = crypto_unregister_alg(&algs[i]);
		if (ret)
			pr_err("Failed to unregister %s %s: %d\n",
			       algs[i].cra_driver_name, algs[i].cra_name, ret);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(crypto_unregister_algs);

int crypto_register_template(struct crypto_template *tmpl)
{
	struct crypto_template *q;
	int err = -EEXIST;

	down_write(&crypto_alg_sem);

	crypto_check_module_sig(tmpl->module);

	list_for_each_entry(q, &crypto_template_list, list) {
		if (q == tmpl)
			goto out;
	}

	list_add(&tmpl->list, &crypto_template_list);
	crypto_notify(CRYPTO_MSG_TMPL_REGISTER, tmpl);
	err = 0;
out:
	up_write(&crypto_alg_sem);
	return err;
}
EXPORT_SYMBOL_GPL(crypto_register_template);

void crypto_unregister_template(struct crypto_template *tmpl)
{
	struct crypto_instance *inst;
	struct hlist_node *n;
	struct hlist_head *list;
	LIST_HEAD(users);

	down_write(&crypto_alg_sem);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	BUG_ON(list_empty(&tmpl->list));
	list_del_init(&tmpl->list);

	list = &tmpl->instances;
	hlist_for_each_entry(inst, list, list) {
		int err = crypto_remove_alg(&inst->alg, &users);

		BUG_ON(err);
	}

	crypto_notify(CRYPTO_MSG_TMPL_UNREGISTER, tmpl);

	up_write(&crypto_alg_sem);

	hlist_for_each_entry_safe(inst, n, list, list) {
		BUG_ON(atomic_read(&inst->alg.cra_refcnt) != 1);
		crypto_free_instance(inst);
	}
	crypto_remove_final(&users);
}
EXPORT_SYMBOL_GPL(crypto_unregister_template);

static struct crypto_template *__crypto_lookup_template(const char *name)
{
	struct crypto_template *q, *tmpl = NULL;

	down_read(&crypto_alg_sem);
	list_for_each_entry(q, &crypto_template_list, list) {
		if (strcmp(q->name, name))
			continue;
		if (unlikely(!crypto_tmpl_get(q)))
			continue;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		tmpl = q;
		break;
	}
	up_read(&crypto_alg_sem);

	return tmpl;
}

struct crypto_template *crypto_lookup_template(const char *name)
{
	return try_then_request_module(__crypto_lookup_template(name),
				       "crypto-%s", name);
}
EXPORT_SYMBOL_GPL(crypto_lookup_template);

int crypto_register_instance(struct crypto_template *tmpl,
			     struct crypto_instance *inst)
{
	struct crypto_larval *larval;
	int err;

	err = crypto_check_alg(&inst->alg);
	if (err)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return err;
}

	inst->alg.cra_module = tmpl->module;
	inst->alg.cra_flags |= CRYPTO_ALG_INSTANCE;

	if (unlikely(!crypto_mod_get(&inst->alg)))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EAGAIN;
}

	down_write(&crypto_alg_sem);

	larval = __crypto_register_alg(&inst->alg);
	if (IS_ERR(larval))
		goto unlock;

	hlist_add_head(&inst->list, &tmpl->instances);
	inst->tmpl = tmpl;

unlock:
	up_write(&crypto_alg_sem);

	err = PTR_ERR(larval);
	if (IS_ERR(larval))
		goto err;

	crypto_wait_for_test(larval);

	/* Remove instance if test failed */
	if (!(inst->alg.cra_flags & CRYPTO_ALG_TESTED))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		crypto_unregister_instance(inst);
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	err = 0;

err:
	crypto_mod_put(&inst->alg);
	return err;
}
EXPORT_SYMBOL_GPL(crypto_register_instance);

int crypto_unregister_instance(struct crypto_instance *inst)
{
	LIST_HEAD(list);

	down_write(&crypto_alg_sem);

	crypto_remove_spawns(&inst->alg, &list, NULL);
	crypto_remove_instance(inst, &list);

	up_write(&crypto_alg_sem);

	crypto_remove_final(&list);

	return 0;
}
EXPORT_SYMBOL_GPL(crypto_unregister_instance);

int crypto_init_spawn(struct crypto_spawn *spawn, struct crypto_alg *alg,
		      struct crypto_instance *inst, u32 mask)
{
	int err = -EAGAIN;

	spawn->inst = inst;
	spawn->mask = mask;

	down_write(&crypto_alg_sem);
	if (!crypto_is_moribund(alg)) {
		list_add(&spawn->list, &alg->cra_users);
		spawn->alg = alg;
		err = 0;
	}
	up_write(&crypto_alg_sem);

	return err;
}
EXPORT_SYMBOL_GPL(crypto_init_spawn);

int crypto_init_spawn2(struct crypto_spawn *spawn, struct crypto_alg *alg,
		       struct crypto_instance *inst,
		       const struct crypto_type *frontend)
{
	int err = -EINVAL;

	if ((alg->cra_flags ^ frontend->type) & frontend->maskset)
		goto out;

	spawn->frontend = frontend;
	err = crypto_init_spawn(spawn, alg, inst, frontend->maskset);

out:
	return err;
}
EXPORT_SYMBOL_GPL(crypto_init_spawn2);

int crypto_grab_spawn(struct crypto_spawn *spawn, const char *name,
		      u32 type, u32 mask)
{
	struct crypto_alg *alg;
	int err;

	alg = crypto_find_alg(name, spawn->frontend, type, mask);
	if (IS_ERR(alg))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return PTR_ERR(alg);
}

	err = crypto_init_spawn(spawn, alg, spawn->inst, mask);
	crypto_mod_put(alg);
	return err;
}
EXPORT_SYMBOL_GPL(crypto_grab_spawn);

void crypto_drop_spawn(struct crypto_spawn *spawn)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (!spawn->alg)
		return;

	down_write(&crypto_alg_sem);
	list_del(&spawn->list);
	up_write(&crypto_alg_sem);
}
EXPORT_SYMBOL_GPL(crypto_drop_spawn);

static struct crypto_alg *crypto_spawn_alg(struct crypto_spawn *spawn)
{
	struct crypto_alg *alg;
	struct crypto_alg *alg2;

	down_read(&crypto_alg_sem);
	alg = spawn->alg;
	alg2 = alg;
	if (alg2)
		alg2 = crypto_mod_get(alg2);
	up_read(&crypto_alg_sem);

	if (!alg2) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (alg)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			crypto_shoot_alg(alg);
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_PTR(-EAGAIN);
	}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return alg;
}

struct crypto_tfm *crypto_spawn_tfm(struct crypto_spawn *spawn, u32 type,
				    u32 mask)
{
	struct crypto_alg *alg;
	struct crypto_tfm *tfm;

	alg = crypto_spawn_alg(spawn);
	if (IS_ERR(alg))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_CAST(alg);
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	tfm = ERR_PTR(-EINVAL);
	if (unlikely((alg->cra_flags ^ type) & mask))
		goto out_put_alg;

	tfm = __crypto_alloc_tfm(alg, type, mask);
	if (IS_ERR(tfm))
		goto out_put_alg;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return tfm;

out_put_alg:
	crypto_mod_put(alg);
	return tfm;
}
EXPORT_SYMBOL_GPL(crypto_spawn_tfm);

void *crypto_spawn_tfm2(struct crypto_spawn *spawn)
{
	struct crypto_alg *alg;
	struct crypto_tfm *tfm;

	alg = crypto_spawn_alg(spawn);
	if (IS_ERR(alg))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_CAST(alg);
}

	tfm = crypto_create_tfm(alg, spawn->frontend);
	if (IS_ERR(tfm))
		goto out_put_alg;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return tfm;

out_put_alg:
	crypto_mod_put(alg);
	return tfm;
}
EXPORT_SYMBOL_GPL(crypto_spawn_tfm2);

int crypto_register_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&crypto_chain, nb);
}
EXPORT_SYMBOL_GPL(crypto_register_notifier);

int crypto_unregister_notifier(struct notifier_block *nb)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return blocking_notifier_chain_unregister(&crypto_chain, nb);
}
EXPORT_SYMBOL_GPL(crypto_unregister_notifier);

struct crypto_attr_type *crypto_get_attr_type(struct rtattr **tb)
{
	struct rtattr *rta = tb[0];
	struct crypto_attr_type *algt;

	if (!rta)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_PTR(-ENOENT);
}
	if (RTA_PAYLOAD(rta) < sizeof(*algt))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_PTR(-EINVAL);
}
	if (rta->rta_type != CRYPTOA_TYPE)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_PTR(-EINVAL);
}

	algt = RTA_DATA(rta);

	return algt;
}
EXPORT_SYMBOL_GPL(crypto_get_attr_type);

int crypto_check_attr_type(struct rtattr **tb, u32 type)
{
	struct crypto_attr_type *algt;

	algt = crypto_get_attr_type(tb);
	if (IS_ERR(algt))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return PTR_ERR(algt);
}

	if ((algt->type ^ type) & algt->mask)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EINVAL;
}

	return 0;
}
EXPORT_SYMBOL_GPL(crypto_check_attr_type);

const char *crypto_attr_alg_name(struct rtattr *rta)
{
	struct crypto_attr_alg *alga;

	if (!rta)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_PTR(-ENOENT);
}
	if (RTA_PAYLOAD(rta) < sizeof(*alga))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_PTR(-EINVAL);
}
	if (rta->rta_type != CRYPTOA_ALG)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_PTR(-EINVAL);
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	alga = RTA_DATA(rta);
	alga->name[CRYPTO_MAX_ALG_NAME - 1] = 0;

	return alga->name;
}
EXPORT_SYMBOL_GPL(crypto_attr_alg_name);

struct crypto_alg *crypto_attr_alg2(struct rtattr *rta,
				    const struct crypto_type *frontend,
				    u32 type, u32 mask)
{
	const char *name;

	name = crypto_attr_alg_name(rta);
	if (IS_ERR(name))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_CAST(name);
}

	return crypto_find_alg(name, frontend, type, mask);
}
EXPORT_SYMBOL_GPL(crypto_attr_alg2);

int crypto_attr_u32(struct rtattr *rta, u32 *num)
{
	struct crypto_attr_u32 *nu32;

	if (!rta)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENOENT;
}
	if (RTA_PAYLOAD(rta) < sizeof(*nu32))
		return -EINVAL;
	if (rta->rta_type != CRYPTOA_U32)
		return -EINVAL;

	nu32 = RTA_DATA(rta);
	*num = nu32->num;

	return 0;
}
EXPORT_SYMBOL_GPL(crypto_attr_u32);

int crypto_inst_setname(struct crypto_instance *inst, const char *name,
			struct crypto_alg *alg)
{
	if (snprintf(inst->alg.cra_name, CRYPTO_MAX_ALG_NAME, "%s(%s)", name,
		     alg->cra_name) >= CRYPTO_MAX_ALG_NAME)
		return -ENAMETOOLONG;

	if (snprintf(inst->alg.cra_driver_name, CRYPTO_MAX_ALG_NAME, "%s(%s)",
		     name, alg->cra_driver_name) >= CRYPTO_MAX_ALG_NAME)
		return -ENAMETOOLONG;

	return 0;
}
EXPORT_SYMBOL_GPL(crypto_inst_setname);

void *crypto_alloc_instance2(const char *name, struct crypto_alg *alg,
			     unsigned int head)
{
	struct crypto_instance *inst;
	char *p;
	int err;

	p = kzalloc(head + sizeof(*inst) + sizeof(struct crypto_spawn),
		    GFP_KERNEL);
	if (!p)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return ERR_PTR(-ENOMEM);
}

	inst = (void *)(p + head);

	err = crypto_inst_setname(inst, name, alg);
	if (err)
		goto err_free_inst;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return p;

err_free_inst:
	kfree(p);
	return ERR_PTR(err);
}
EXPORT_SYMBOL_GPL(crypto_alloc_instance2);

struct crypto_instance *crypto_alloc_instance(const char *name,
					      struct crypto_alg *alg)
{
	struct crypto_instance *inst;
	struct crypto_spawn *spawn;
	int err;

	inst = crypto_alloc_instance2(name, alg, 0);
	if (IS_ERR(inst))
		goto out;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	spawn = crypto_instance_ctx(inst);
	err = crypto_init_spawn(spawn, alg, inst,
				CRYPTO_ALG_TYPE_MASK | CRYPTO_ALG_ASYNC);

	if (err)
		goto err_free_inst;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return inst;

err_free_inst:
	kfree(inst);
	inst = ERR_PTR(err);

out:
	return inst;
}
EXPORT_SYMBOL_GPL(crypto_alloc_instance);

void crypto_init_queue(struct crypto_queue *queue, unsigned int max_qlen)
{
	INIT_LIST_HEAD(&queue->list);
	queue->backlog = &queue->list;
	queue->qlen = 0;
	queue->max_qlen = max_qlen;
}
EXPORT_SYMBOL_GPL(crypto_init_queue);

int crypto_enqueue_request(struct crypto_queue *queue,
			   struct crypto_async_request *request)
{
	int err = -EINPROGRESS;

	if (unlikely(queue->qlen >= queue->max_qlen)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		err = -EBUSY;
		if (!(request->flags & CRYPTO_TFM_REQ_MAY_BACKLOG))
			goto out;
		if (queue->backlog == &queue->list)
			queue->backlog = &request->list;
	}

	queue->qlen++;
	list_add_tail(&request->list, &queue->list);

out:
	return err;
}
EXPORT_SYMBOL_GPL(crypto_enqueue_request);

struct crypto_async_request *crypto_dequeue_request(struct crypto_queue *queue)
{
	struct list_head *request;

	if (unlikely(!queue->qlen))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return NULL;
}

	queue->qlen--;

	if (queue->backlog != &queue->list)
		queue->backlog = queue->backlog->next;

	request = queue->list.next;
	list_del(request);

	return list_entry(request, struct crypto_async_request, list);
}
EXPORT_SYMBOL_GPL(crypto_dequeue_request);

int crypto_tfm_in_queue(struct crypto_queue *queue, struct crypto_tfm *tfm)
{
	struct crypto_async_request *req;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	list_for_each_entry(req, &queue->list, list) {
		if (req->tfm == tfm)
			return 1;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(crypto_tfm_in_queue);

static inline void crypto_inc_byte(u8 *a, unsigned int size)
{
	u8 *b = (a + size);
	u8 c;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	for (; size; size--) {
		c = *--b + 1;
		*b = c;
		if (c)
			break;
	}
}

void crypto_inc(u8 *a, unsigned int size)
{
	__be32 *b = (__be32 *)(a + size);
	u32 c;

	if (IS_ENABLED(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) ||
	    IS_ALIGNED((unsigned long)b, __alignof__(*b)))
		for (; size >= 4; size -= 4) {
			c = be32_to_cpu(*--b) + 1;
			*b = cpu_to_be32(c);
			if (likely(c))
				return;
		}

	crypto_inc_byte(a, size);
}
EXPORT_SYMBOL_GPL(crypto_inc);

void __crypto_xor(u8 *dst, const u8 *src1, const u8 *src2, unsigned int len)
{
	int relalign = 0;

	if (!IS_ENABLED(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)) {
		int size = sizeof(unsigned long);
		int d = (((unsigned long)dst ^ (unsigned long)src1) |
			 ((unsigned long)dst ^ (unsigned long)src2)) &
			(size - 1);

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		relalign = d ? 1 << __ffs(d) : size;

		/*
		 * If we care about alignment, process as many bytes as
		 * needed to advance dst and src to values whose alignments
		 * equal their relative alignment. This will allow us to
		 * process the remainder of the input using optimal strides.
		 */
		while (((unsigned long)dst & (relalign - 1)) && len > 0) {
			*dst++ = *src1++ ^ *src2++;
			len--;
		}
	}

	while (IS_ENABLED(CONFIG_64BIT) && len >= 8 && !(relalign & 7)) {
		*(u64 *)dst = *(u64 *)src1 ^  *(u64 *)src2;
		dst += 8;
		src1 += 8;
		src2 += 8;
		len -= 8;
	}

	while (len >= 4 && !(relalign & 3)) {
		*(u32 *)dst = *(u32 *)src1 ^ *(u32 *)src2;
		dst += 4;
		src1 += 4;
		src2 += 4;
		len -= 4;
	}

	while (len >= 2 && !(relalign & 1)) {
		*(u16 *)dst = *(u16 *)src1 ^ *(u16 *)src2;
		dst += 2;
		src1 += 2;
		src2 += 2;
		len -= 2;
	}

	while (len--)
		*dst++ = *src1++ ^ *src2++;
}
EXPORT_SYMBOL_GPL(__crypto_xor);

unsigned int crypto_alg_extsize(struct crypto_alg *alg)
{
	return alg->cra_ctxsize +
	       (alg->cra_alignmask & ~(crypto_tfm_ctx_alignment() - 1));
}
EXPORT_SYMBOL_GPL(crypto_alg_extsize);

int crypto_type_has_alg(const char *name, const struct crypto_type *frontend,
			u32 type, u32 mask)
{
	int ret = 0;
	struct crypto_alg *alg = crypto_find_alg(name, frontend, type, mask);

	if (!IS_ERR(alg)) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		crypto_mod_put(alg);
		ret = 1;
	}

	return ret;
}
EXPORT_SYMBOL_GPL(crypto_type_has_alg);

static int __init crypto_algapi_init(void)
{
	crypto_init_proc();
	return 0;
}

static void __exit crypto_algapi_exit(void)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	crypto_exit_proc();
}

module_init(crypto_algapi_init);
module_exit(crypto_algapi_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Cryptographic algorithms API");
