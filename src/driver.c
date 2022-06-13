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

// As the device file system (devfs) is not used here, 
// we need to assign a new major number directly, as 
// so, we start, after inspection of /proc/devices, 
// picking a major number still not being used, let's 
// say 600

#define MAJOR_NUMBER 600

// We need to pass to the register_chrdev (assuming for
// now that Epos II maxon is a char device) a struct that
// defines the file operations, as so, let's define a simple
// one for now
static struct file_operations fops = {
        .owner = THIS_MODULE,
        .open = driver_open,
        .release = driver_close
};

// Opening function, lower layer abstraction to the kernel's 
// open() function. This can be defined in another way, with
// permissions and options as well

static int driver_open(struct inode *device_file, struct file *instance){
        printk("Epos II Driver - open was called\n");

        // Sucess
        return 0;
}

// Same thing, but for close
static int driver_close(struct inode *device_file, struct file *instance){
        printk("Epos II Driver - close was called\n");

        // Sucess
        return 0;
}

/* Function called when entering the Kernel */
static int ModuleInit(void){

                printk("Epos II 50/5 module loaded\n");
                                     
                // Return Value for registration of char device, with my 
                // major number, name and given fops
                int retval = register_chrdev(MAJOR_NUMBER, "MaxonEposII_50/5", &fops);

				if(retval == 0){

						printk("Sucessfully loaded Epos II 50/5 device, with major number %d and minor number %d\n");

				}else{
						if(retval > 0){

							printk("Sucessfully loaded Epos II 50/5 device, but with major number %d and minor number %d\n", retval>>20, retval&0xfffff);

						}else{
								printk("An error has occured, cannot initialize device file\n");
								return -1;
						}
				}
                /* Inicialização vem aqui TODO */

                // Sucess               
                return 0;
}


/* Function called when exiting the Kernel */
static void ModuleExit(void){

				unregister_chrdev(MAJOR_NUMBER,"MaxonEposII_50/5");
                printk("Epos II 50/5 module unloaded\n");

                /* Finalização vem aqui TODO */
}


/* Meta information for alllowing correct loading of modules through distros*/
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Diego Fleury");
MODULE_DESCRIPTION("Driver for Maxon Epos II 50/5 motor driver, for use in Ubuntu server 20.04, inside Raspberry Pi 4");

// Linking the initialization and exit of the module to
// it's respective functions
module_init(ModuleInit);
module_exit(ModuleExit);

