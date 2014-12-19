/*
 * translate.h
 *
 *  Created on: 18.12.2014
 *      Author: louisa
 */

#ifndef TRANSLATE_H_
#define TRANSLATE_H_

/* Includes */
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>  	/* char device aufsetzten, device Nummer bekommen */
#include <linux/cdev.h>
#include <linux/slab.h> /* für kzalloc */
#include <linux/wait.h>

/* Defines */
#define DEVICE_NODE "trans"
#define FIRSTMINOR 0
#define COUNT 1
#define MAX 40

/* Variables */
dev_t dev_num;

//int major;
int is_open = 0;



struct translate_dev{
	struct cdev chardevice;
	char buffer[MAX];
	char *p_in;		/* Zeiger auf Stelle in Buffer, wo reingeschrieben werden kann */
	char *p_out; 	/* Zeiger auf Stelle in Buffer, wo herausgelesen weerden kann */
	int count; 		/* Anzahl der elemente im buffer */
};

/* Functions */
static int __init init_translate(void);

static void cleanup_translate(void);


/* open, wenn Programm auf Device zugreifen möchte */
static int open(struct inode *devicefile, struct file *instance);

/* close, wenn Programm Driver space verlässt */
static int close(struct inode *devicefile, struct file *instance);

/* read, wenn Programm lesen möchte */
ssize_t read(struct file *filp, char __user *buff, size_t count, loff_t *offp);

/* write, wenn Programm schreiben möchte */
ssize_t write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

/* Strukturen */
/* Strukt, welches zeigt welche Funktion welche ist */
struct file_operations fops = {
	.open = open,
	.release = close,
	.read = read,
	.write = write,
};

#endif /* TRANSLATE_H_ */


