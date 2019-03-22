# Project1: Introduction to Linux Kernel Modules

##Preparation:

#### Install Ubuntu:

I install ubuntu 14.04 (download from website and its kernel is 4.4)on the Vmware Workstation 15. Not the double system as it maybe destory the guidance and the system cannot use. 

What's more, do not install the ubuntu 16.04 because this index will cause the failure when compiling *.c files. And I cann't solve it after I look up this question in Google and Baidu.

##Procedure:

1. Commands: lsmod, this command will list all the modules in the kernel. Just like the figures below:

![lsmod](D:\课程\大三上\计算机系统工程\project\1\shootcut\lsmod.png)

2. Then I create the simple.c and filled up with the example code. And create the Makefile file.

![create_simple_file](D:\课程\大三上\计算机系统工程\project\1\shootcut\create_simple_file.png)

![create_makefile](D:\课程\大三上\计算机系统工程\project\1\shootcut\simple_makefile.png)

After make command, it produce many files include *.ko file, the result.

![compile_done](D:\课程\大三上\计算机系统工程\project\1\shootcut\compile_done.png)

And I found the module in the lsmod.![lsmod_found_simple](D:\课程\大三上\计算机系统工程\project\1\shootcut\lsmod_found_simple.png)

And I check the message by using dmesg command. Just as following:

![simple_ko_result](D:\课程\大三上\计算机系统工程\project\1\shootcut\simple_ko_result.png)

3. /Proc FIle System.

   Just like the code in the sample, I create the hello.c file, then compile it. After compile, I inserted it into the kernel module. Then cat /proc/hello. The result is as following:

   ![create_hello](D:\课程\大三上\计算机系统工程\project\1\shootcut\create_hello.png)


## Practise:

1. Print out the value of the GOLDEN_RATIO_PRIME in the init function and print out the greatest commaon divisor of 3300 and 24 in the exit function.

![printk_number](D:\课程\大三上\计算机系统工程\project\1\shootcut\printk_number.png)

2. print out the values of jiffies and HZ in the init function and print out the value of jiffies in the exit function.

   ![printk_HZ_jiffies](D:\课程\大三上\计算机系统工程\project\1\shootcut\printk_HZ_jiffies.png)


3. print the jiffies every time I cat it.

   ![print_jiffies](D:\课程\大三上\计算机系统工程\project\1\shootcut\print_jiffies.png)



   4. print the seconds after I insert the module.

      In the code, I use global var to record the start jiffies when inserting the modules. Then everytime I cat it, it will compute the difference between start jiffies and the current jiffies. Then divide by HZ. I can get the seconds then print it.

      ![print_seconds](D:\课程\大三上\计算机系统工程\project\1\shootcut\print_seconds.png)

## Codes:

**I will add the notation to the codes where I have changed.**

1. Here is the code for 1、2 practise.

   ```c
   #include <linux/init.h>
   #include <linux/kernel.h>
   #include <linux/module.h>
   #include <linux/hash.h>
   #include <linux/gcd.h>
   #include <asm/param.h>
   #include <linux/jiffies.h>
   
   
   /* This function is called when the module is loaded. */
   int simple_init(void){
   printk(KERN_INFO "Loading Kernel Module\n");
   // This code is print Golden ratio prime, when init the simple module.
   printk(KERN_INFO "%lu\n", GOLDEN_RATIO_PRIME);
   // This code is print HZ, when init the simple module.
   printk(KERN_INFO "%lu\n", HZ);
   // This code is print jiffies, when init the simple module.
   printk(KERN_INFO "%lu\n", jiffies);
   return 0;
   }
   
   /* This function is called when the module is removed. */
   void simple_exit(void){
   printk(KERN_INFO "Removing Kernel Module\n");
   // This code is print the gcd of 3300 and 24, when exit the simple module.
   printk(KERN_INFO "%lu\n", gcd(3300,24));
   // This code is print jiffies, when exit the simple module.
   printk(KERN_INFO "%lu\n", jiffies);
   }
   
   /* Macros for registering module entry and exit points. */
   module_init(simple_init);
   module_exit(simple_exit);
   
   /* The Module description. */
   MODULE_LICENSE("GPL");
   MODULE_DESCRIPTION("Simple Module");
   MODULE_AUTHOR("Bing Han");
   ```

2. Here is the code for 3 practise.

   ```c
   #include <linux/init.h>
   #include <linux/module.h>
   #include <linux/kernel.h>
   #include <linux/proc_fs.h>
   #include <linux/jiffies.h>
   #include <linux/hash.h>
   #include <asm/uaccess.h>
   
   #define BUFFER_SIZE 128
   #define PROC_NAME "jiffies"
   #define MESSAGE "Hello World\n"
   /**
    * Function prototypes
    */
   ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
   static struct file_operations proc_ops = {
           .owner = THIS_MODULE,
           .read = proc_read,
   };
   /* This function is called when the module is loaded. */
   int proc_init(void)
   {
           // creates the /proc/hello entry
           // the following function call is a wrapper for
           // proc_create_data() passing NULL as the last argument
           proc_create(PROC_NAME, 0, NULL, &proc_ops);
           printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
   	return 0;
   }
   /* This function is called when the module is removed. */
   void proc_exit(void) {
           // removes the /proc/hello entry
           remove_proc_entry(PROC_NAME, NULL);
           printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
   }
   /**
    * This function is called each time the /proc/hello is read.
    * 
    * This function is called repeatedly until it returns 0, so
    * there must be logic that ensures it ultimately returns 0
    * once it has collected the data that is to go into the 
    * corresponding /proc file.
    *
    * params:
    *
    * file:
    * buf: buffer in user space
    * count:
    * pos:
    */
   ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
   {
           int rv = 0;
           char buffer[BUFFER_SIZE];
           static int completed = 0;
           if (completed) {
                   completed = 0;
                   return 0;
           }
           completed = 1;
       // I just add this line, to print the jiffies when cat the proc file.
           rv = sprintf(buffer, "%lu\n", jiffies);
           // copies the contents of buffer to userspace usr_buf
           copy_to_user(usr_buf, buffer, rv);
           return rv;
   }
   /* Macros for registering module entry and exit points. */
   module_init(proc_init);
   module_exit(proc_exit);
   /* The Module description. */
   MODULE_LICENSE("GPL");
   MODULE_DESCRIPTION("jiffies Module");
   MODULE_AUTHOR("Bing Han");
   ```

3.  Here is the code for 4 practice: 

   ```c
   #include <linux/init.h>
   #include <linux/module.h>
   #include <linux/kernel.h>
   #include <linux/proc_fs.h>
   #include <linux/jiffies.h>
   #include <linux/hash.h>
   #include <asm/uaccess.h>
   
   #define BUFFER_SIZE 128
   #define PROC_NAME "seconds"
   #define MESSAGE "Hello World\n"
   
   /**
    * Function prototypes
    */
   // I create a global var to store the start_jiffies when create the file.
   unsigned long start_time = 0;
   
   ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
   
   static struct file_operations proc_ops = {
           .owner = THIS_MODULE,
           .read = proc_read,
   };
   
   /* This function is called when the module is loaded. */
   int proc_init(void)
   {
           // creates the /proc/hello entry
           // the following function call is a wrapper for
           // proc_create_data() passing NULL as the last argument
           proc_create(PROC_NAME, 0, NULL, &proc_ops);
       // When initing the file, store the start_jiffies.
           start_time = jiffies;
           printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
   	return 0;
   }
   /* This function is called when the module is removed. */
   void proc_exit(void) {
           // removes the /proc/hello entry
           remove_proc_entry(PROC_NAME, NULL);
           printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
   }
   /**
    * This function is called each time the /proc/hello is read.
    * 
    * This function is called repeatedly until it returns 0, so
    * there must be logic that ensures it ultimately returns 0
    * once it has collected the data that is to go into the 
    * corresponding /proc file.
    *
    * params:
    *
    * file:
    * buf: buffer in user space
    * count:
    * pos:
    */
   ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
   {
           int rv = 0;
           char buffer[BUFFER_SIZE];
           static int completed = 0;
   
           if (completed) {
                   completed = 0;
                   return 0;
           }
           completed = 1;
       // everytime cat the file, to get the difference between the current_jiffies and start_jiffies, then divide by HZ to get the time past.
           int pass_time = (jiffies-start_time)/HZ;
       //print the time past.
           rv = sprintf(buffer, "%d\n", pass_time);
           // copies the contents of buffer to userspace usr_buf
           copy_to_user(usr_buf, buffer, rv);
           return rv;
   }
   /* Macros for registering module entry and exit points. */
   module_init(proc_init);
   module_exit(proc_exit);
   /* The Module description. */
   MODULE_LICENSE("GPL");
   MODULE_DESCRIPTION("seconds Module");
   MODULE_AUTHOR("Bing Han");
   ```