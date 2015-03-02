#include <linux/module.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include<linux/slab.h>
#define SUCCESS 0
#define DEVICE_NAME "TickTock"   

static int Tick_init(void);
static void Tock_exit(void);
static int opendev(struct inode *, struct file *);
static int closedev(struct inode *, struct file *);
static ssize_t readme(struct file *, char *, size_t, loff_t *);
static ssize_t writeme(struct file *, const char *, size_t, loff_t *);
static int Major;            
        
static struct file_operations fops = {
  .read = readme, 
  .write = writeme,
  .open = opendev,
  .release = closedev
};

static int Tick_init(void){
  	Major = register_chrdev(0, DEVICE_NAME, &fops);
 		
		if(Major>0){
 			printk("TickTock is Ready For Requests!:%d\n",Major);
 			return 0;
 		}
		
		else{
 			printk("Something Bad happened OhOhhh: %d",Major);
 		}
}

module_init(Tick_init);

static void Tock_exit(void){
 	unregister_chrdev(Major, DEVICE_NAME);
 	printk("Goodbye Crewl World!\n");
}

module_exit(Tock_exit);
MODULE_LICENSE("GPL");

static int opendev(struct inode * a, struct file * b){
printk("Someone opened me!\n");
//nothing to do!!!
return SUCCESS;
}

static int closedev(struct inode * a, struct file * b){
//nothing to do!!
printk("Someone Closed me!\n");
return SUCCESS;
}

static ssize_t readme(struct file *filp,char *buffer,size_t length,loff_t *offset)  {
   
   int len;
   char *b;

   struct timeval time;
   do_gettimeofday(&time);
   
   b=kmalloc(sizeof(char)*40,GFP_KERNEL);
   sprintf(b,"%.2lu",time.tv_sec);
   len=strlen(b);
   copy_to_user(buffer,b,len);
   printk("Return val:%s\n",b);
   return 0;
}

static ssize_t writeme(struct file *filp,const char *buff,size_t len, loff_t *off){
  int *i;
 struct timespec ab; 
 
  i=kmalloc(sizeof(int),GFP_KERNEL);
  kstrtoint(buff,10,i);
 
  ab.tv_sec=*i;
  ab.tv_nsec=0;
  do_settimeofday(&ab);
  return 0;
}

