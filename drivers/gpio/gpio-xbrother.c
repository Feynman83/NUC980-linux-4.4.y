


#include "linux/printk.h"
#include "linux/stddef.h"
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/slab.h>
#include <linux/platform_device.h>


static struct kobject *kobj_xbro;


#define MAX_XBR_ATTR_NUM (128)





static ssize_t attr_show(struct kobject *kobj, struct kobj_attribute *attr_kobj,
			char *buf);

static  struct kobj_attribute attr_all []={
    {
        .attr={
            .name="do01",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="do02",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="do03",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="do04",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="do05",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="do06",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="do07",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
};

static struct attribute* attrs_xbro[MAX_XBR_ATTR_NUM];
static struct attribute_group attr_group_xbro = {
	.attrs = attrs_xbro,
};



static ssize_t attr_show(struct kobject *kobj, struct kobj_attribute *attr_kobj,
			char *buf){
    int i=   attr_kobj-attr_all;
    pr_info("%d",i);
    return 0;
}

static int __init nuc980iio_init(void)
{
    int err=0;
    int attr_num=sizeof(attr_all)/(sizeof(struct kobj_attribute));
    int i=0;
    for(i=0;i<attr_num;i++){
        attrs_xbro[i]=&attr_all[i].attr;
    }
    attrs_xbro[i]=NULL;
	kobj_xbro = kobject_create_and_add("xbrother", NULL);
    if(!kobj_xbro){
        return -ENOMEM;
    }

    
    /* Create the files associated with this kobject */
	err = sysfs_create_group(kobj_xbro, &attr_group_xbro);
	if (err)
		kobject_put(kobj_xbro);


	pr_info("fmt, ...");
    return 0;
}

static void __exit nuc980iio_exit(void)
{
	
}

module_init(nuc980iio_init);
module_exit(nuc980iio_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("NUC980 xbrother driver");