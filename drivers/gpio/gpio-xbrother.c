


#include "asm-generic/errno-base.h"
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
#include <linux/clk.h>
#include <linux/io.h>


#include <mach/mfp.h>

#define ADC_CTRL (0x00)
#define ADC_CONF (0x04)
#define ADC_IER (0x08)
#define ADC_ISR (0x0c)
#define ADC_DATA (0x28)
struct iio_xbro{
    int gpio;
    unsigned long flags;
    const char * name;
    unsigned int chip;  
    unsigned int channel; //greater than 8 means ad for DI
};


static struct iio_xbro iio_xbro_list[]={
    {
        .gpio=100,
        .flags=GPIOF_OUT_INIT_LOW,
        .name="do01",
    },
    {
        .gpio=101,
        .flags=GPIOF_OUT_INIT_LOW,
        .name="do02",
    },
    {
        .gpio=104,
        .flags=GPIOF_OUT_INIT_LOW,
        .name="do03",
    },
    {
        .gpio=105,
        .flags=GPIOF_OUT_INIT_LOW,
        .name="do04",
    },
    {
        .gpio=-1,
        .name="ai01",
        .chip=0,
        .channel=0,
    },
    {
        .gpio=-1,
        .name="ai02",
        .chip=0,
        .channel=1,
    },
    {
        .gpio=-1,
        .name="ai03",
        .chip=0,
        .channel=2,
    },
    {
        .gpio=-1,
        .name="ai04",
        .chip=0,
        .channel=3,
    },
    {
        .gpio=-1,
        .name="ai05",
        .chip=0,
        .channel=4,
    },
    {
        .gpio=-1,
        .name="ai06",
        .chip=0,
        .channel=5,
    },
    {
        .gpio=-1,
        .name="ai07",
        .chip=0,
        .channel=6,
    },
    {
        .gpio=-1,
        .name="ai08",
        .chip=0,
        .channel=7,
    },
};
static struct kobject *kobj_xbro;





#define MAX_XBR_ATTR_NUM (128)





static ssize_t attr_show(struct kobject *kobj, struct kobj_attribute *attr_kobj,
			char *buf);
static ssize_t attr_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count);
static  struct kobj_attribute attr_all []={
    {
        .attr={
            .name="do01",
            .mode=VERIFY_OCTAL_PERMISSIONS(0664),
        },
        .show=attr_show,
        .store=attr_store,
    },
    {
        .attr={
            .name="do02",
            .mode=VERIFY_OCTAL_PERMISSIONS(0664),
        },
        .show=attr_show,
        .store=attr_store,
    },
    {
        .attr={
            .name="do03",
            .mode=VERIFY_OCTAL_PERMISSIONS(0664),
        },
        .show=attr_show,
        .store=attr_store,
    },
    {
        .attr={
            .name="do04",
            .mode=VERIFY_OCTAL_PERMISSIONS(0664),
        },
        .show=attr_show,
        .store=attr_store,
    },
    {
        .attr={
            .name="ai01",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai02",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai03",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai04",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai05",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai06",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai07",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai08",
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


void __iomem * NUC980_ADC_REG_BASE;
static ssize_t attr_store(struct kobject *kobj, struct kobj_attribute *attr_kobj,
			 const char *buf, size_t count)
{

    
	int index,number, ret;

    index=   attr_kobj-attr_all;
    if(index <0 || index >=sizeof(attr_all)/(sizeof(struct kobj_attribute))){
        return -EINVAL;
    }
	ret = kstrtoint(buf, 10, &number);
	if (ret < 0)
		return ret;
    if(iio_xbro_list[index].gpio>=0){
        gpio_set_value_cansleep(iio_xbro_list[index].gpio,!!number);
    }
    return count;
}

static ssize_t attr_show(struct kobject *kobj, struct kobj_attribute *attr_kobj,
			char *buf){
    int index;
    index=   attr_kobj-attr_all;
    if(index <0 || index >=sizeof(attr_all)/(sizeof(struct kobj_attribute))){
        return -EINVAL;
    }
    if(iio_xbro_list[index].gpio>=0){
        return sprintf(buf, "%d\n", gpio_get_value_cansleep(iio_xbro_list[index].gpio));
    }else{  //do AD sample
        return -EINVAL;
    }
    return -EAGAIN;
}

static void adc_channel_switch(unsigned int channel){

    gpio_set_value_cansleep(2,!!(channel&0x01));
    gpio_set_value_cansleep(3,!!(channel&0x02));
    gpio_set_value_cansleep(4,!!(channel&0x04));
}


static unsigned int adc_sample(unsigned int channel){

    unsigned int chip=channel/8;
    unsigned int ad_channel=channel%8;

    writel(readl(NUC980_ADC_REG_BASE+ADC_CONF)|((chip)<<12), NUC980_ADC_REG_BASE+ADC_CONF); //select chip
    
    writel(readl(NUC980_ADC_REG_BASE+ADC_IER)|0x01, NUC980_ADC_REG_BASE+ADC_IER); 
    writel(readl(NUC980_ADC_REG_BASE+ADC_CTRL)|((1)<<8), NUC980_ADC_REG_BASE+ADC_CTRL); //start adc convert


    
    return 0;
}
static int __init nuc980nadc_init(void){
    struct clk * clk_eclk;
    struct clk * clk_adc;
 

    //PA2,3,4 for ADC channel select 
    nuc980_mfp_set_port_a(2,0);
    nuc980_mfp_set_port_a(3,0);
    nuc980_mfp_set_port_a(4,0);
    

    gpio_request_one(2,GPIOF_OUT_INIT_LOW,"adc_sel0");
    gpio_request_one(3,GPIOF_OUT_INIT_LOW,"adc_sel1");
    gpio_request_one(4,GPIOF_OUT_INIT_LOW,"adc_sel2");


    clk_eclk=clk_get(NULL, "adc_eclk");
    clk_prepare(clk_eclk);
    clk_enable(clk_eclk);
    clk_set_rate(clk_eclk, 4000000);

    clk_adc= clk_get(NULL, "adc");
    clk_prepare(clk_adc);
    clk_enable(clk_adc);

    NUC980_ADC_REG_BASE = ioremap_nocache(0xB0043000, 0xff);
    writel(3, NUC980_ADC_REG_BASE+ADC_CTRL);
    writel(readl(NUC980_ADC_REG_BASE+ADC_CONF)|0x400000, NUC980_ADC_REG_BASE+ADC_CONF);
    writel(readl(NUC980_ADC_REG_BASE+ADC_CONF)|0x4, NUC980_ADC_REG_BASE+ADC_CONF);
    return 0;
}

static int __init nuc980iio_init(void)
{
    int err=0;
    int attr_num=sizeof(attr_all)/(sizeof(struct kobj_attribute));
    int iio_num=sizeof(iio_xbro_list)/sizeof(struct iio_xbro);
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

    for(i=0;i<iio_num;i++){
        if(iio_xbro_list[i].gpio<0){
            continue;
        }
       if(gpio_request_one(iio_xbro_list[i].gpio,iio_xbro_list[i].flags,iio_xbro_list[i].name)){
            pr_err("gpio%d request failed",iio_xbro_list[i].gpio);
       }
    }
    nuc980nadc_init();
	pr_info("xbrother iio init\n");
    return 0;
}

static void __exit nuc980iio_exit(void)
{
	
}


late_initcall(nuc980iio_init);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("NUC980 xbrother driver");