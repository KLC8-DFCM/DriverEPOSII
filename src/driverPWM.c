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

// Name of the device 
#define DRIVER_NAME "PWM driver" 
#define DRIVER_CLASS "MyModuleClass"

// buffer for read and write operations
static char buffer[BUFF_SiZE];
static int buffer_pointer;

// Variables for interfacing with the devices and their class
static dev_t my_device_nr;
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

        not_copied = copy_from_user(buffer, user_buffer, to_copy);
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
        if(alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0){
                printk("Error in allocation of device file number\n");
                return -1;
        }

        // Read write operations 
        printk("Device number M = %d, m = %d was registered\n", my_device_nr >> 20, my_device_nr && 0xfffff);


        // Device class creation 
        if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL){
                printk("Device class cannot be created\n");
                
                goto ClassError;
        }

        // Device file creation
        if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL){
                printk("Cannot create device file\n");
                
                goto FileError;
        }

        // Device file initialization, addition of one instance to /dev folder
        cdev_init(&my_device, &fops);
        if(cdev_add(&my_device, my_device_nr, 1) == -1){
                printk("Cannot initialize device file\n");

                goto AddError;
        }

        // Sucess
        return 0;

        // Gotos are introduced in the tutorial as justifiable due to the fact 
        // that it's easy to remove initialized variables, if organized in this manner,
        // via the goto construct 
        AddError:
                device_destroy(my_class, my_device_nr);

        FileError:
                class_destroy(my_class);

        ClassError:
                unregister_chrdev(my_device_nr,DRIVER_NAME);

                return -1;
}

/* Function called when exiting the Kernel */
static void ModuleExit(void)
{
        /* Finalização vem aqui TODO */

        cdev_del(&my_device);
        device_destroy(my_class, my_device_nr);
        class_destroy(my_class);
        unregister_chrdev(my_device_nr,DRIVER_NAME);

        printk("PWM module unloaded\n");
}

/* Meta information for alllowing correct loading of modules through distros*/
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Diego Fleury");
MODULE_DESCRIPTION("Driver for PWM, for use in Ubuntu server 20.04, inside Raspberry Pi 3 B+");

// Linking the initialization and exit of the module to
// it's respective functions
module_init(ModuleInit);
module_exit(ModuleExit);
