extern int kernel_init_done;
int printk(const char *fmt, ...);
// SPDX-License-Identifier: GPL-2.0
/*
 * IA-32 Huge TLB Page Support for Kernel.
 *
 * Copyright (C) 2002, Rohit Seth <rohit.seth@intel.com>
 */

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/hugetlb.h>
#include <linux/pagemap.h>
#include <linux/err.h>
#include <linux/sysctl.h>
#include <linux/compat.h>
#include <asm/mman.h>
#include <asm/tlb.h>
#include <asm/tlbflush.h>
#include <asm/pgalloc.h>
#include <asm/elf.h>
#include <asm/mpx.h>

#if 0	/* This is just for testing */
struct page *
follow_huge_addr(struct mm_struct *mm, unsigned long address, int write)
{
	unsigned long start = address;
	int length = 1;
	int nr;
	struct page *page;
	struct vm_area_struct *vma;

	vma = find_vma(mm, addr);
	if (!vma || !is_vm_hugetlb_page(vma))
		return ERR_PTR(-EINVAL);

	pte = huge_pte_offset(mm, address, vma_mmu_pagesize(vma));

	/* hugetlb should be locked, and hence, prefaulted */
	WARN_ON(!pte || pte_none(*pte));

	page = &pte_page(*pte)[vpfn % (HPAGE_SIZE/PAGE_SIZE)];

	WARN_ON(!PageHead(page));

	return page;
}

int pmd_huge(pmd_t pmd)
{
	return 0;
}

int pud_huge(pud_t pud)
{
	return 0;
}

#else

/*
 * pmd_huge() returns 1 if @pmd is hugetlb related entry, that is normal
 * hugetlb entry or non-present (migration or hwpoisoned) hugetlb entry.
 * Otherwise, returns 0.
 */
int pmd_huge(pmd_t pmd)
{
	return !pmd_none(pmd) &&
		(pmd_val(pmd) & (_PAGE_PRESENT|_PAGE_PSE)) != _PAGE_PRESENT;
}

int pud_huge(pud_t pud)
{
	return !!(pud_val(pud) & _PAGE_PSE);
}
#endif

#ifdef CONFIG_HUGETLB_PAGE
static unsigned long hugetlb_get_unmapped_area_bottomup(struct file *file,
		unsigned long addr, unsigned long len,
		unsigned long pgoff, unsigned long flags)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	struct hstate *h = hstate_file(file);
	struct vm_unmapped_area_info info;

	info.flags = 0;
	info.length = len;
	info.low_limit = get_mmap_base(1);

	/*
	 * If hint address is above DEFAULT_MAP_WINDOW, look for unmapped area
	 * in the full address space.
	 */
	info.high_limit = in_compat_syscall() ?
		task_size_32bit() : task_size_64bit(addr > DEFAULT_MAP_WINDOW);

	info.align_mask = PAGE_MASK & ~huge_page_mask(h);
	info.align_offset = 0;
	return vm_unmapped_area(&info);
}

static unsigned long hugetlb_get_unmapped_area_topdown(struct file *file,
		unsigned long addr, unsigned long len,
		unsigned long pgoff, unsigned long flags)
{
	struct hstate *h = hstate_file(file);
	struct vm_unmapped_area_info info;

	info.flags = VM_UNMAPPED_AREA_TOPDOWN;
	info.length = len;
	info.low_limit = PAGE_SIZE;
	info.high_limit = get_mmap_base(0);

	/*
	 * If hint address is above DEFAULT_MAP_WINDOW, look for unmapped area
	 * in the full address space.
	 */
	if (addr > DEFAULT_MAP_WINDOW && !in_compat_syscall())
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		info.high_limit += TASK_SIZE_MAX - DEFAULT_MAP_WINDOW;
}

	info.align_mask = PAGE_MASK & ~huge_page_mask(h);
	info.align_offset = 0;
	addr = vm_unmapped_area(&info);

	/*
	 * A failed mmap() very likely causes application failure,
	 * so fall back to the bottom-up function here. This scenario
	 * can happen with large stack limits and large mmap()
	 * allocations.
	 */
	if (addr & ~PAGE_MASK) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		VM_BUG_ON(addr != -ENOMEM);
		info.flags = 0;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		info.low_limit = TASK_UNMAPPED_BASE;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		info.high_limit = TASK_SIZE_LOW;
		addr = vm_unmapped_area(&info);
	}

	return addr;
}

unsigned long
hugetlb_get_unmapped_area(struct file *file, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct hstate *h = hstate_file(file);
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;

	if (len & ~huge_page_mask(h))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -EINVAL;
}

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	addr = mpx_unmapped_area_check(addr, len, flags);
	if (IS_ERR_VALUE(addr))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return addr;
}

	if (len > TASK_SIZE)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return -ENOMEM;
}

	if (flags & MAP_FIXED) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (prepare_hugepage_range(file, addr, len))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return -EINVAL;
}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return addr;
	}

	if (addr) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		addr = ALIGN(addr, huge_page_size(h));
		vma = find_vma(mm, addr);
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (TASK_SIZE - len >= addr &&
		    (!vma || addr + len <= vm_start_gap(vma)))
			return addr;
	}
	if (mm->get_unmapped_area == arch_get_unmapped_area)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return hugetlb_get_unmapped_area_bottomup(file, addr, len,
				pgoff, flags);
}
	else
		return hugetlb_get_unmapped_area_topdown(file, addr, len,
				pgoff, flags);
}
#endif /* CONFIG_HUGETLB_PAGE */

#ifdef CONFIG_X86_64
static __init int setup_hugepagesz(char *opt)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	unsigned long ps = memparse(opt, &opt);
	if (ps == PMD_SIZE) {
		hugetlb_add_hstate(PMD_SHIFT - PAGE_SHIFT);
	} else if (ps == PUD_SIZE && boot_cpu_has(X86_FEATURE_GBPAGES)) {
		hugetlb_add_hstate(PUD_SHIFT - PAGE_SHIFT);
	} else {
		hugetlb_bad_size();
		printk(KERN_ERR "hugepagesz: Unsupported page size %lu M\n",
			ps >> 20);
		return 0;
	}
	return 1;
}
__setup("hugepagesz=", setup_hugepagesz);

#if (defined(CONFIG_MEMORY_ISOLATION) && defined(CONFIG_COMPACTION)) || defined(CONFIG_CMA)
static __init int gigantic_pages_init(void)
{
	/* With compaction or CMA we can allocate gigantic pages at runtime */
	if (boot_cpu_has(X86_FEATURE_GBPAGES) && !size_to_hstate(1UL << PUD_SHIFT))
		hugetlb_add_hstate(PUD_SHIFT - PAGE_SHIFT);
	return 0;
}
arch_initcall(gigantic_pages_init);
#endif
#endif
