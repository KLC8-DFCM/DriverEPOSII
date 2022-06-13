#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>

/* Meta information for alllowing correct loading of modules through distros*/
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Diego Fleury");
MODULE_DESCRIPTION("Driver for Maxon Epos II 50/5 motor driver, for use in Ubuntu server 20.04, inside Raspberry Pi 4");

/* Function called when entering the Kernel */

static int __init ModuleInit(void){

		printk("Epos II 50/5 module loaded\n");

		/* Inicialização vem aqui TODO */

		// Sucess		
		return 0; 
}


static void __exit ModuleExit(void){

		printk("Epos II 50/5 module unloaded\n");

		/* Finalização vem aqui TODO */
}

module_init(ModuleInit);
module_exit(ModuleExit);

