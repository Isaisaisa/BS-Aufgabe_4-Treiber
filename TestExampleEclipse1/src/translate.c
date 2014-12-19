/*
 * example.c
 *
 *  Created on: 18.12.2014
 *      Author: louisa
 */


#include "translate.h"

MODULE_LICENSE("GPL");

struct translate_dev *ptranslate_devices;

void setup_devices(struct translate_dev *dev, int index){
	/* Funktionen werden an das eine Device der Devices gebunden
	 * Struct chardevice wird belegt */
	cdev_init(&dev->chardevice, &fops);
	dev->chardevice.owner = THIS_MODULE;
	int dev_no_of_curr_minor = MKDEV(MAJOR(dev_num), index);

	/*  Fehlerfall Device konnte nicht erstellt werden */
	if(cdev_add(&dev->chardevice, dev_no_of_curr_minor, 1)){
		printk(KERN_ALERT "unable to add Device: major %d minor %d", MAJOR(dev_num), index);
	}

	/* Initialiesierung des Ringbufers
	 * Schreib und leseadresse sind anfangs identisch */
	dev->p_in = dev->buffer;
	dev->p_out = dev->buffer;
	dev->count = 0;

}

static int __init init_translate(void){
	printk(KERN_INFO "Sie befinden sich in Funktion init");

	/*int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);
	 * return value if success then 0, else negative value */
	if (alloc_chrdev_region(&dev_num, FIRSTMINOR, COUNT, DEVICE_NODE) != 0){
		printk(KERN_ALERT "init failed");
		return -1;
	}

	/* Mit kmalloc wird der Speicherplatz der Devices reserviert
	 * kzalloc belegt den Speicher mit 0 vor */
	ptranslate_devices = kzalloc(sizeof(struct translate_dev) * 2, GFP_KERNEL);
	if(!ptranslate_devices){
		printk(KERN_ALERT "unable to allocate memory");
		cleanup_translate();
		return -ENOMEM;
	}

	int i;
	for(i = 0; i<2; i++){
		setup_devices(&ptranslate_devices[i], i);
		}

	//major = MAJOR(dev);
	return 0;
}

static void cleanup_translate(void){
	printk(KERN_INFO "Sie befinden sich in Funktion cleanup");

	/* Nur wenn Translate devices gesetzt waren */
	if(ptranslate_devices){

		/* Devices entfernen */
		int i;
		for(i=0; i<2; i++){
			cdev_del(&ptranslate_devices[i].chardevice);
		}

		/* Speicher befreien */
		kfree(ptranslate_devices);
	}

	/* unregestrieren der Devices */
	unregister_chrdev_region(dev_num, COUNT);


}





/* open, wenn Programm auf Device zugreifen möchte */
static int open(struct inode *devicefile, struct file *instance){
	printk(KERN_INFO "Sie befinden sich in Funktion open");

	/* zeiger auf die Device Struktur */
	struct translate_dev *dev;
	/*TODO strukt initialisieren*/

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


	try_module_get(THIS_MODULE);
	return 0;
}







/* close, wenn Programm Driver space verlässt */
static int close(struct inode *devicefile, struct file *instance){

	is_open = 0;

	module_put(THIS_MODULE);

	return 0;
}






/* read, wenn Programm lesen möchte */
ssize_t read(struct file *filp, char __user *buff, size_t count, loff_t *offp){

	return count;
}






/* write, wenn Programm schreiben möchte */
ssize_t write(struct file *filp, const char __user *buff, size_t count, loff_t *offp){


	return count;
}






module_init(init_translate);
module_exit(cleanup_translate);




/* Fpr cleanup funktion
 * void unregister_chrdev_region(dev_t first, unsigned int count);*/
