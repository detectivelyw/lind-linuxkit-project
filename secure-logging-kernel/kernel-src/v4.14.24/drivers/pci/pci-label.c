extern int kernel_init_done;
int printk(const char *fmt, ...);
// SPDX-License-Identifier: GPL-2.0
/*
 * Purpose: Export the firmware instance and label associated with
 * a pci device to sysfs
 * Copyright (C) 2010 Dell Inc.
 * by Narendra K <Narendra_K@dell.com>,
 * Jordan Hargrave <Jordan_Hargrave@dell.com>
 *
 * PCI Firmware Specification Revision 3.1 section 4.6.7 (DSM for Naming a
 * PCI or PCI Express Device Under Operating Systems) defines an instance
 * number and string name. This code retrieves them and exports them to sysfs.
 * If the system firmware does not provide the ACPI _DSM (Device Specific
 * Method), then the SMBIOS type 41 instance number and string is exported to
 * sysfs.
 *
 * SMBIOS defines type 41 for onboard pci devices. This code retrieves
 * the instance number and string from the type 41 record and exports
 * it to sysfs.
 *
 * Please see http://linux.dell.com/files/biosdevname/ for more
 * information.
 */

#include <linux/dmi.h>
#include <linux/sysfs.h>
#include <linux/pci.h>
#include <linux/pci_ids.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/nls.h>
#include <linux/acpi.h>
#include <linux/pci-acpi.h>
#include "pci.h"

#ifdef CONFIG_DMI
enum smbios_attr_enum {
	SMBIOS_ATTR_NONE = 0,
	SMBIOS_ATTR_LABEL_SHOW,
	SMBIOS_ATTR_INSTANCE_SHOW,
};

static size_t find_smbios_instance_string(struct pci_dev *pdev, char *buf,
					  enum smbios_attr_enum attribute)
{
	const struct dmi_device *dmi;
	struct dmi_dev_onboard *donboard;
	int domain_nr;
	int bus;
	int devfn;

	domain_nr = pci_domain_nr(pdev->bus);
	bus = pdev->bus->number;
	devfn = pdev->devfn;

	dmi = NULL;
	while ((dmi = dmi_find_device(DMI_DEV_TYPE_DEV_ONBOARD,
				      NULL, dmi)) != NULL) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		donboard = dmi->device_data;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		if (donboard && donboard->segment == domain_nr &&
				donboard->bus == bus &&
				donboard->devfn == devfn) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			if (buf) {
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
				if (attribute == SMBIOS_ATTR_INSTANCE_SHOW)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
					return scnprintf(buf, PAGE_SIZE,
							 "%d\n",
							 donboard->instance);
}
				else if (attribute == SMBIOS_ATTR_LABEL_SHOW)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
					return scnprintf(buf, PAGE_SIZE,
							 "%s\n",
							 dmi->name);
}
			}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
			return strlen(dmi->name);
		}
	}
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return 0;
}

static umode_t smbios_instance_string_exist(struct kobject *kobj,
					    struct attribute *attr, int n)
{
	struct device *dev;
	struct pci_dev *pdev;

	dev = kobj_to_dev(kobj);
	pdev = to_pci_dev(dev);

	return find_smbios_instance_string(pdev, NULL, SMBIOS_ATTR_NONE) ?
					   S_IRUGO : 0;
}

static ssize_t smbioslabel_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct pci_dev *pdev;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	pdev = to_pci_dev(dev);

	return find_smbios_instance_string(pdev, buf,
					   SMBIOS_ATTR_LABEL_SHOW);
}

static ssize_t smbiosinstance_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct pci_dev *pdev;
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	pdev = to_pci_dev(dev);

	return find_smbios_instance_string(pdev, buf,
					   SMBIOS_ATTR_INSTANCE_SHOW);
}

static struct device_attribute smbios_attr_label = {
	.attr = {.name = "label", .mode = 0444},
	.show = smbioslabel_show,
};

static struct device_attribute smbios_attr_instance = {
	.attr = {.name = "index", .mode = 0444},
	.show = smbiosinstance_show,
};

static struct attribute *smbios_attributes[] = {
	&smbios_attr_label.attr,
	&smbios_attr_instance.attr,
	NULL,
};

static const struct attribute_group smbios_attr_group = {
	.attrs = smbios_attributes,
	.is_visible = smbios_instance_string_exist,
};

static int pci_create_smbiosname_file(struct pci_dev *pdev)
{
	return sysfs_create_group(&pdev->dev.kobj, &smbios_attr_group);
}

static void pci_remove_smbiosname_file(struct pci_dev *pdev)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	sysfs_remove_group(&pdev->dev.kobj, &smbios_attr_group);
}
#else
static inline int pci_create_smbiosname_file(struct pci_dev *pdev)
{
	return -1;
}

static inline void pci_remove_smbiosname_file(struct pci_dev *pdev)
{
}
#endif

#ifdef CONFIG_ACPI
enum acpi_attr_enum {
	ACPI_ATTR_LABEL_SHOW,
	ACPI_ATTR_INDEX_SHOW,
};

static void dsm_label_utf16s_to_utf8s(union acpi_object *obj, char *buf)
{
	int len;
	len = utf16s_to_utf8s((const wchar_t *)obj->buffer.pointer,
			      obj->buffer.length,
			      UTF16_LITTLE_ENDIAN,
			      buf, PAGE_SIZE);
	buf[len] = '\n';
}

static int dsm_get_label(struct device *dev, char *buf,
			 enum acpi_attr_enum attr)
{
	acpi_handle handle;
	union acpi_object *obj, *tmp;
	int len = -1;

if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	handle = ACPI_HANDLE(dev);
	if (!handle)
		return -1;

	obj = acpi_evaluate_dsm(handle, &pci_acpi_dsm_guid, 0x2,
				DEVICE_LABEL_DSM, NULL);
	if (!obj)
		return -1;

	tmp = obj->package.elements;
	if (obj->type == ACPI_TYPE_PACKAGE && obj->package.count == 2 &&
	    tmp[0].type == ACPI_TYPE_INTEGER &&
	    (tmp[1].type == ACPI_TYPE_STRING ||
	     tmp[1].type == ACPI_TYPE_BUFFER)) {
		/*
		 * The second string element is optional even when
		 * this _DSM is implemented; when not implemented,
		 * this entry must return a null string.
		 */
		if (attr == ACPI_ATTR_INDEX_SHOW) {
			scnprintf(buf, PAGE_SIZE, "%llu\n", tmp->integer.value);
		} else if (attr == ACPI_ATTR_LABEL_SHOW) {
			if (tmp[1].type == ACPI_TYPE_STRING)
				scnprintf(buf, PAGE_SIZE, "%s\n",
					  tmp[1].string.pointer);
			else if (tmp[1].type == ACPI_TYPE_BUFFER)
				dsm_label_utf16s_to_utf8s(tmp + 1, buf);
		}
		len = strlen(buf) > 0 ? strlen(buf) : -1;
	}

	ACPI_FREE(obj);

	return len;
}

static bool device_has_dsm(struct device *dev)
{
	acpi_handle handle;

	handle = ACPI_HANDLE(dev);
	if (!handle)
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return false;
}

	return !!acpi_check_dsm(handle, &pci_acpi_dsm_guid, 0x2,
				1 << DEVICE_LABEL_DSM);
}

static umode_t acpi_index_string_exist(struct kobject *kobj,
				       struct attribute *attr, int n)
{
	struct device *dev;

	dev = kobj_to_dev(kobj);

	if (device_has_dsm(dev))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		return S_IRUGO;
}

	return 0;
}

static ssize_t acpilabel_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return dsm_get_label(dev, buf, ACPI_ATTR_LABEL_SHOW);
}

static ssize_t acpiindex_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return dsm_get_label(dev, buf, ACPI_ATTR_INDEX_SHOW);
}

static struct device_attribute acpi_attr_label = {
	.attr = {.name = "label", .mode = 0444},
	.show = acpilabel_show,
};

static struct device_attribute acpi_attr_index = {
	.attr = {.name = "acpi_index", .mode = 0444},
	.show = acpiindex_show,
};

static struct attribute *acpi_attributes[] = {
	&acpi_attr_label.attr,
	&acpi_attr_index.attr,
	NULL,
};

static const struct attribute_group acpi_attr_group = {
	.attrs = acpi_attributes,
	.is_visible = acpi_index_string_exist,
};

static int pci_create_acpi_index_label_files(struct pci_dev *pdev)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	return sysfs_create_group(&pdev->dev.kobj, &acpi_attr_group);
}

static int pci_remove_acpi_index_label_files(struct pci_dev *pdev)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	sysfs_remove_group(&pdev->dev.kobj, &acpi_attr_group);
	return 0;
}
#else
static inline int pci_create_acpi_index_label_files(struct pci_dev *pdev)
{
	return -1;
}

static inline int pci_remove_acpi_index_label_files(struct pci_dev *pdev)
{
	return -1;
}

static inline bool device_has_dsm(struct device *dev)
{
	return false;
}
#endif

void pci_create_firmware_label_files(struct pci_dev *pdev)
{
	if (device_has_dsm(&pdev->dev))
{ if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
		pci_create_acpi_index_label_files(pdev);
}
	else
		pci_create_smbiosname_file(pdev);
}

void pci_remove_firmware_label_files(struct pci_dev *pdev)
{
if (kernel_init_done) printk("We reached unpopular paths: %s:%i\n", __FILE__, __LINE__);
	if (device_has_dsm(&pdev->dev))
		pci_remove_acpi_index_label_files(pdev);
	else
		pci_remove_smbiosname_file(pdev);
}
