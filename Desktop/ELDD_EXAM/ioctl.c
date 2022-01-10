/*IOCTL UART EXAMPLE*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/types.h> 
#include <linux/fs.h> 
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include "ioctl.h"
#define Name IOCTL

int Name_open(struct inode *inode,struct file *filp);
int Name_release(struct inode *inode,struct file *filp);
ssize_t Name_write(struct file *filp,const char __user *Ubuff, size_t count, loff_t *offp);
ssize_t Name_read(struct file *filp,char __user *Ubuff, size_t count, loff_t *offp);
long Name_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

struct file_operations fops=
{
 .owner  	  = THIS_MODULE,
 .open   	  = Name_open,
 .read   	  = Name_read,
 .write   	  = Name_write,
 .unlocked_ioctl  = Name_ioctl, //ioctl structure
 .release	  = Name_release,
 };
 
 struct cdev *my_cdev;
 struct Data{
	int Baudrate;
	int StopBits;
	int Parity;
};

 dev_t Mydev=0;
 
 static int __init chardevice_init(void)
{

 int result;
 int MAJOR,MINOR;
 result=alloc_chrdev_region(&Mydev,0,1,"IOCTL");
 MAJOR=MAJOR(Mydev);
 MINOR=MINOR(Mydev);
 printk(KERN_INFO "\nThis is init function\n");
 printk(KERN_ALERT "\nThe Major no.= %d and Minor no.=%d\n",MAJOR,MINOR);
 if(result<0)
 {
 printk(KERN_ALERT "\n The region is not obtain \n");
 return(-1);
 }
 my_cdev = cdev_alloc();
 my_cdev ->ops = &fops;
  
 result= cdev_add(my_cdev,Mydev,1);
 if(result<0)
 {
 printk(KERN_ALERT "\n The char driver as not been created \n");
 unregister_chrdev_region(Mydev,1);
 }
 return 0;
 }
 
 
 void __exit chardevice_exit(void)
{
int MAJOR,MINOR;
MAJOR=MAJOR(Mydev);
MINOR=MINOR(Mydev);
printk(KERN_ALERT "The Major no.= %d and Minor no.=%d",MAJOR,MINOR);
unregister_chrdev_region(Mydev,1);
cdev_del(my_cdev);
printk(KERN_ALERT "\n Deallocated stuff GOODBYE!! \n");
}
//open fuction
int Name_open(struct inode *inode,struct file *filp)
{
printk(KERN_ALERT "\nThis is kernel open call\n");
return 0;
}

//write function
ssize_t Name_write(struct file *filp,const char __user *Ubuff, size_t count, loff_t *offp)
{
 char kbuff[100];
 unsigned long result;
 ssize_t retvalue;
 result=copy_from_user((char *)kbuff,(char *)Ubuff,count);
 if(result==0)
 {
 printk(KERN_ALERT "\nMessage from user to kernel:\n %s \n",kbuff);
 retvalue=count;
 return retvalue;
 }
 else 
 {
 printk(KERN_ALERT "\n CANNOT OPEN FILE IN KERNEL\n");
 retvalue =-EFAULT;
 return retvalue;
 }
 }
 
 //read function
ssize_t Name_read(struct file *filp,char __user *Ubuff, size_t count, loff_t *offp)
{
char kbuff[]="Read from kernel to User\n";
unsigned long result;
ssize_t retvalue;
result=copy_to_user((char *)Ubuff,(char*)kbuff,sizeof(kbuff));
if(result==0)
 {
 printk(KERN_ALERT "\n Data Read Success\n");
 retvalue =count;
 return retvalue;
 }
 else 
 {
 printk(KERN_ALERT "\n CANNOT OPEN FILE IN KERNEL\n");
 retvalue =-EFAULT;
 return retvalue;
 }
}
 

//release function
int Name_release(struct inode *inode,struct file *filp)
{
printk(KERN_ALERT "\nThis is kernel release call\n");
return 0;
}

//ioctl function
long Name_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
unsigned int value, STOP_BITS;
char Ubuff[20];
struct default_Data *ToSend;
printk(KERN_INFO "\n IOCTL FUNCTION" );
switch(cmd) {
 	case SET_BAUDRATE:
		get_user (value,(int __user *)arg);
		printk("\n BAUDRATE SET = %d",value);
		value=10;
		put_user(value,(int __user *)arg);
		break;
	case SET_DIRECTION_WRITE:
		printk("\n The direction is set to be write\n");
		break;
	case SET_NON_STOP_BITS:
		get_user(STOP_BITS,(int __user *)arg);
		printk("\n STOP_BITS = %d",STOP_BITS);
		break;

	case DATA:
		ToSend =(struct Default_Data *)Ubuff;
		copy_from_user (Ubuff ,(struct Data *)arg,sizeof(struct Data));
		printk("\n Default Data = %d , %d , %d",ToSend->Baudrate,ToSend->StopBits,ToSend->Parity);
		break;
	default :
		printk("\n Command not found");
		return (-EINVAL);
}

return 0;
}
module_init(chardevice_init);
module_exit(chardevice_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("RAJU");


