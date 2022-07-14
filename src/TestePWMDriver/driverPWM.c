/*
        Diego Fleury Correa de Moraes
        Nusp: 11800584
*/

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
#include <linux/uaccess.h>

// For pwm control
#include <linux/pwm.h>

// Name of the device 
#define DRIVER_NAME "PWM_DRIVER" 
#define DRIVER_CLASS "MyModuleClass"

// Period for pwm data (1 second)
#define PWM_PERIOD 1000000000

// Initial period for duty cycle (5%)
#define PWM_DUTY_INITIAL 50000000

// Struct that defines the pwm control abstraction
struct pwm_device *pwm_0 = NULL;

// Initial on period, that corresponds to 100 ms 
u32 pwm_periodo_ligado = PWM_DUTY_INITIAL;

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

// Write data to driver, it's usual for linux's native write/read functions to return the amount 
// of *something* that it was able to write/read
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs){

        int to_copy, not_copied, delta;
        char input_value;

        to_copy = min(count, sizeof(input_value));

        not_copied = copy_from_user(&input_value, user_buffer, to_copy);

        // Set pwm frequency
        // Rules: as it's a character device, let's define that sending 
        // the 0 corresponds to a duty cycle of 5%, through 9 corresponding to one of 95%  
        if(input_value < '0' || input_value > '9'){
                printk("Invalid input value, try again\n");
        }else{
                pwm_config(pwm_0, PWM_DUTY_INITIAL + 10 * (input_value - '0'), PWM_PERIOD);
        }

        delta = to_copy - not_copied;

        return delta;
}
 
// Assuming pwm function is, for now, a char dev
// We could instantiate even more operations here (like read)
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .write = driver_write
};

/* Function called when entering the Kernel */
static int ModuleInit(void)
{

        printk("PWM module loaded\n");

        /* Inicialização vem aqui TODO */

        // Instead of using a static device number, like in many tutorials, 
        // let's try to used a system allocated one by the means of interfacing 
        // with the alloc_chrdev_region function

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

        // Initiate pwm interface
        pwm_0 = pwm_request(0,"Pwm pin 1");

        // Kernel log if not possible to initiate pwm pinnage
        if(pwm_0 == NULL){
                printk("Could not initiate Pwm pin 1\n");
                goto AddError;
        }

        // Configure pwm pin to have a period of 1 second
        // Remember thatt the duty cycle period is defined 
        // in the beggining, with this ratio of 1 ms / 1 s corresponding
        // to a 0,1 % duty cycle percentage
        pwm_config(pwm_0, pwm_periodo_ligado, PWM_PERIOD);

        // Enable toggling of pwm output
        pwm_enable(pwm_0);

        // Sucess
        return 0;

        // Gotos are introduced in the tutorial as justifiable due to the fact 
        // that it's easy to remove initialized variables, if organized in this manner,
        // via the goto construct. I personally think that there might be a similar way 
        // to achieve this via a switch-case clause. 
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

        // Clean up pwm structure
        pwm_disable(pwm_0);
        pwm_free(pwm_0);

        // Delete device file stuff
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
