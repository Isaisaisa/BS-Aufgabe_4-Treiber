/*
 * example.c
 *
 *  Created on: 18.12.2014
 *      Author: louisa
 */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>  	/* char device aufsetzten, device Nummer bekommen */
#include <linux/cdev.h>

#include <translate.h>

MODULE_LICENSE("GPL");

#define DEVICE_NODE "trans"
#define FIRSTMINOR 0
#define COUNT 1

dev_t dev;
//int major;
int is_open = 0;

static int __init init_translate(void){

	/*int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);
	 * return value if success then 0, else negative value */
	if (alloc_chrdev_region(&dev, FIRSTMINOR, COUNT, DEVICE_NODE) != 0){
		printk(KERN_ALERT "init failed");
		return -1;
	}
	//major = MAJOR(dev);
	return 0;
}

static void cleanup_translate(void){
	/*unregestrieren der Devices*/
	unregister_chrdev_region(dev, COUNT);

}

/* open, wenn Programm auf Device zugreifen möchte */
static int open(struct inode *devicefile, struct file *instance){
	/* wenn Device bereits geöffnet ist, darf Prozess nicht darauf zugreifen */
	if(is_open == 1){
		printk(KERN_ALERT "already open");
		return -EBUSY; /* Device ist beschäftigt */
	}
	/* Dvice war nicht offen wird aber jetzt als offen definiert */
	is_open = 1;

	if(iminor(devicefile)==0){
		printk(KERN_INFO "Minor Number 0");
	}else if(iminor(devicefile)==1) {
		printk(KERN_INFO "Minor Number 1");
	}else{
		printk(KERN_INFO "Minor Number is higher than 1");
	}

	/* try_module_get(THIS_MODULE) */
	return 0;
}

/* close, wenn Programm Driver space verlässt */
static int close(struct inode *devicefile, struct file *instance){

	is_open = 0;

	/* module_put(THIS_MODULE) */

	return 0;
}

/* read, wenn Programm lesen möchte */
ssize_t read(struct file *filp, char __user *buff, size_t count, loff_t *offp);

/* write, wenn Programm schreiben möchte */
ssize_t write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);






module_init(init_example);
module_exit(cleanup_example);




/* Fpr cleanup funktion
 * void unregister_chrdev_region(dev_t first, unsigned int count);*/
