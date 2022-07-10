// Linux modules interfaces defined here
#include <linux/module.h>

// Initialization functions
#include <linux/init.h>

// Just for compatibility issues
#include <linux/version.h>

// Needed for definition, and creation, of file
// system user mode interface with the driver
// via device files
#include <linux/fs.h>

// For the addition of the feature of creation of device file INSIDE 
// the kernel module, we include the following kernel header files
#include <linux/cdev.h>
#include <linux/uacess.h>

// Buffer size for RW ops 
#define BUFF_SIZE 255

// buffer for read and write operations
static char buffer[BUFF_SiZE];
static int buffer_pointer;

// Variables for interfacing with the devices and their class
static struct class *my_class;
static struct cdev my_device;

// Opening function, lower layer abstraction to the kernel's
// open() function. This can be defined in another way, with
// permissions and options as well
static int driver_open(struct inode *device_file, struct file *instance)
{
        printk("PWM Driver - open was called\n");

        // Sucess
        return 0;
}


// Same thing, but for close
static int driver_close(struct inode *device_file, struct file *instance)
{
        printk("PWM Driver - close was called\n");

        // Sucess
        return 0;
}

// Read data from driver
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){

        int to_copy, not_copied, delta;

        to_copy = min(count, buffer_pointer);
        not_copied = copy_to_user(user_buffer, buffer, to_copy);
        delta = to_copy - not_copied;

        return delta;
}

// Write data to driver
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *o>

        int to_copy, not_copied, delta;

        to_copy = min(count, sizeof(buffer));

        not_copied = copy_to_user(buffer, user_buffer, to_copy);
        delta = to_copy - not_copied;

        return delta;
}
 
// Assuming pwm function is, for now, a char dev
// We could instantiate even more operations here
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
    .write = driver_write
};

/* Function called when entering the Kernel */
static int ModuleInit(void)
{

        printk("PWM module loaded\n");

        /* Inicialização vem aqui TODO */

        // Instead of using a static device number, let's try to used a system allocated one
        // by the means of interfacing with the alloc_chrdev_region function


        // Allocated new device number
        if(alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME){


        // Sucess
        return 0;
}

/* Function called when exiting the Kernel */
static void ModuleExit(void)
{

        printk("PWM module unloaded\n");

        /* Finalização vem aqui TODO */
}

/* Meta information for alllowing correct loading of modules through distros*/
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Diego Fleury");
MODULE_DESCRIPTION("Driver for PWM, for use in Ubuntu server 20.04, inside Raspberry Pi 3 B+");

// Linking the initialization and exit of the module to
// it's respective functions
module_init(ModuleInit);
module_exit(ModuleExit);
