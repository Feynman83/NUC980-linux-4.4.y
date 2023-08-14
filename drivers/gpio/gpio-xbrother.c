


#include "asm-generic/errno-base.h"
#include "linux/delay.h"
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

#define  ADC_SAMPLE_NUM (128)
#define NUC980_ADC_REG_BEGINE (0xB0043000)

#define ADC_CTRL (0x00)
#define ADC_CONF (0x04)
#define ADC_IER (0x08)
#define ADC_ISR (0x0c)
#define ADC_DATA (0x28)



#define GPIO_ADC_SEL0 (0x02)
#define GPIO_ADC_SEL1 (0x03)
#define GPIO_ADC_SEL2 (0x04)

struct iio_xbro{
    int gpio;
    unsigned long flags;
    const char * name;
    unsigned int channel; 
    unsigned int mode; //0:ai 1:di 
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
        .gpio=5,
        .flags=GPIOF_OUT_INIT_LOW,
        .name="do_buzz",
    },
    {
        .gpio=196,
        .flags=GPIOF_OUT_INIT_HIGH,
        .name="do_pwr_4g",
    },
    {
        .gpio=194,
        .flags=GPIOF_OUT_INIT_HIGH,
        .name="do_pwr_usb",
    },
    {
        .gpio=193,
        .flags=GPIOF_OUT_INIT_HIGH,
        .name="do_pwr_hub",
    },
    {
        .gpio=204,
        .flags=GPIOF_IN|GPIOF_ACTIVE_LOW,
        .name="di_ext1",
    },
    {
        .gpio=64,
        .flags=GPIOF_IN|GPIOF_ACTIVE_LOW,
        .name="di_ext2",
    },
    {
        .gpio=39,
        .flags=GPIOF_IN|GPIOF_ACTIVE_LOW,
        .name="di_ext3",
    },
    {
        .gpio=-1,
        .name="ai01",
        .channel=0,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai02",
        .channel=1,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai03",
        .channel=2,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai04",
        .channel=3,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai05",
        .channel=4,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai06",
        .channel=5,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai07",
        .channel=6,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai08",
        .channel=7,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai09",
        .channel=8,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai10",
        .channel=9,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai11",
        .channel=10,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai12",
        .channel=11,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai13",
        .channel=12,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai14",
        .channel=13,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai15",
        .channel=14,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai16",
        .channel=15,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai17",
        .channel=16,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai18",
        .channel=17,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai19",
        .channel=18,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai20",
        .channel=19,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai21",
        .channel=20,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai22",
        .channel=21,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai23",
        .channel=22,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="ai24",
        .channel=23,
        .mode=0,
    },
    {
        .gpio=-1,
        .name="di01",
        .channel=0,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di02",
        .channel=1,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di03",
        .channel=2,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di04",
        .channel=3,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di05",
        .channel=4,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di06",
        .channel=5,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di07",
        .channel=6,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di08",
        .channel=7,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di09",
        .channel=8,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di10",
        .channel=9,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di11",
        .channel=10,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di12",
        .channel=11,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di13",
        .channel=12,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di14",
        .channel=13,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di15",
        .channel=14,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di16",
        .channel=15,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di17",
        .channel=16,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di18",
        .channel=17,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di19",
        .channel=18,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di20",
        .channel=19,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di21",
        .channel=20,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di22",
        .channel=21,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di23",
        .channel=22,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="di24",
        .channel=23,
        .mode=1,
    },
    {
        .gpio=-1,
        .name="w1",
        .channel=24,
    },
    {
        .gpio=-1,
        .name="w2",
        .channel=25,
    },
    {
        .gpio=-1,
        .name="w3",
        .channel=26,
    },
    {
        .gpio=-1,
        .name="w4",
        .channel=27,
    },
    {
        .gpio=-1,
        .name="w5",
        .channel=28,
    },
    {
        .gpio=-1,
        .name="w6",
        .channel=29,
    },
    {
        .gpio=-1,
        .name="w7",
        .channel=30,
    },
    {
        .gpio=-1,
        .name="w8",
        .channel=31,
    },
};
static struct kobject *kobj_xbro;





#define MAX_XBR_ATTR_NUM (128)



static unsigned int adc_sample(unsigned int channel);

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
            .name="do_buzz",
            .mode=VERIFY_OCTAL_PERMISSIONS(0664),
        },
        .show=attr_show,
        .store=attr_store,
    },
    {
        .attr={
            .name="do_pwr_4g",
            .mode=VERIFY_OCTAL_PERMISSIONS(0664),
        },
        .show=attr_show,
        .store=attr_store,
    },
    {
        .attr={
            .name="do_pwr_usb",
            .mode=VERIFY_OCTAL_PERMISSIONS(0664),
        },
        .show=attr_show,
        .store=attr_store,
    },
    {
        .attr={
            .name="do_pwr_hub",
            .mode=VERIFY_OCTAL_PERMISSIONS(0664),
        },
        .show=attr_show,
        .store=attr_store,
    },
    {
        .attr={
            .name="di_ext1",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di_ext2",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di_ext3",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
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
    {
        .attr={
            .name="ai09",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai10",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai11",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai12",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai13",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai14",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai15",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai16",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai17",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai18",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai19",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai20",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai21",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai22",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai23",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="ai24",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di01",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di02",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di03",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di04",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di05",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di06",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di07",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di08",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di09",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di10",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di11",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di12",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di13",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di14",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di15",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di16",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di17",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di18",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di19",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di20",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di21",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di22",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di23",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="di24",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="w1",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="w2",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="w3",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="w4",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="w5",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="w6",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="w7",
            .mode=VERIFY_OCTAL_PERMISSIONS(0444),
        },
        .show=attr_show,
        .store=NULL,
    },
    {
        .attr={
            .name="w8",
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


void volatile __iomem * NUC980_ADC_REG_BASE=(void volatile __iomem *)(0xF0043000);

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
    unsigned int adc_raw;
    index=   attr_kobj-attr_all;
    if(index <0 || index >=sizeof(attr_all)/(sizeof(struct kobj_attribute))){
        return -EINVAL;
    }
    if(iio_xbro_list[index].gpio>=0){
        return sprintf(buf, "%d\n", gpio_get_value_cansleep(iio_xbro_list[index].gpio));
    }else{  //do AD sample
        adc_raw=adc_sample(iio_xbro_list[index].channel);
        adc_raw=adc_sample(iio_xbro_list[index].channel);
        
        if(iio_xbro_list[index].mode==0){
              adc_raw= adc_raw*1018*5/4096; //ideal =adc_raw*5*1000/4096 ,,1018 for calibration
        }else{
              adc_raw= adc_raw*1018*12/4096; //ideal =adc_raw*12*1000/4096 ,,1018 for calibration
              if(adc_raw >=2000){
                adc_raw=1;
              }else{
                adc_raw=0;
              }
        }
        
        return snprintf(buf, PAGE_SIZE, "%u\n", adc_raw);
    }
    return -EAGAIN;
}

static void adc_channel_switch(unsigned int channel){

    gpio_set_value_cansleep(GPIO_ADC_SEL0,!!(channel&0x01));
    gpio_set_value_cansleep(GPIO_ADC_SEL1,!!(channel&0x02));
    gpio_set_value_cansleep(GPIO_ADC_SEL2,!!(channel&0x04));
    msleep(10);
}


static unsigned int adc_sample(unsigned int channel){

    unsigned int chip=channel/8;
    unsigned int ad_channel=channel%8;

    volatile  int adc_wait;
    unsigned int adc_min,adc_max,adc_sum,adc_raw;
    int i;
    
    adc_channel_switch(ad_channel);

   
    writel((readl(NUC980_ADC_REG_BASE+ADC_CONF)&0xFFFF0FFF)|((chip)<<12), NUC980_ADC_REG_BASE+ADC_CONF); //select chip
    

    adc_min=0xffffffff;
    adc_max=0;
    adc_sum=0;
    for(i=0;i<ADC_SAMPLE_NUM+2;i++){
        writel(readl(NUC980_ADC_REG_BASE+ADC_IER)|0x01, NUC980_ADC_REG_BASE+ADC_IER); 
        writel(readl(NUC980_ADC_REG_BASE+ADC_CTRL)|((1)<<8), NUC980_ADC_REG_BASE+ADC_CTRL); //start adc convert

        adc_wait=1000;
        while((readl(NUC980_ADC_REG_BASE+ADC_ISR)&0x400)==0){
            adc_wait--;
            if(adc_wait<=0) break;
        }
        adc_raw=readl(NUC980_ADC_REG_BASE+ADC_DATA)&0xfff;

        if(adc_raw>adc_max){
            adc_max=adc_raw;
        }
        if(adc_raw<adc_min){
            adc_min=adc_raw;
        }
        adc_sum+=adc_raw;
    }

    return (adc_sum-adc_max-adc_min)/ADC_SAMPLE_NUM;
}
static int __init nuc980nadc_init(void){
    struct clk * clk_eclk;
    struct clk * clk_adc;
    


    //select adc0-adc3
    nuc980_mfp_set_port_b(0,8);
    nuc980_mfp_set_port_b(1,8);
    nuc980_mfp_set_port_b(2,8);
    nuc980_mfp_set_port_b(3,8);

    //PA2,3,4 for ADC channel select 
    nuc980_mfp_set_port_a(GPIO_ADC_SEL0,0);
    nuc980_mfp_set_port_a(GPIO_ADC_SEL1,0);
    nuc980_mfp_set_port_a(GPIO_ADC_SEL2,0);
    


    if(gpio_request_one(GPIO_ADC_SEL0,GPIOF_OUT_INIT_LOW,"adc_sel0")){
        pr_err("gpio %d request failed\n",GPIO_ADC_SEL0);
        return -EINVAL;
    }
    if(gpio_request_one(GPIO_ADC_SEL1,GPIOF_OUT_INIT_LOW,"adc_sel1")){
        pr_err("gpio %d request failed\n",GPIO_ADC_SEL1);
        return -EINVAL;
    }
    if(gpio_request_one(GPIO_ADC_SEL2,GPIOF_OUT_INIT_LOW,"adc_sel2")){
        pr_err("gpio %d request failed\n",GPIO_ADC_SEL2);
        return -EINVAL;
    }
   

    clk_eclk=clk_get(NULL, "adc_eclk");
    clk_prepare(clk_eclk);
    clk_enable(clk_eclk);
    clk_set_rate(clk_eclk, 4000000);

    clk_adc= clk_get(NULL, "adc");
    clk_prepare(clk_adc);
    clk_enable(clk_adc);

    NUC980_ADC_REG_BASE = ioremap_nocache(NUC980_ADC_REG_BEGINE, 0xff);
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