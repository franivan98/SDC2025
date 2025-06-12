#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>


static int __init cdd_init(void){
    printk(KERN_INFO "CDD: Modulo Cargado.\n");
    return 0; // Retorna 0 si la carga fue exitosa
}
static void __exit cdd_exit(void){
    printk(KERN_INFO "CDD: Modulo Descargado.\n");
}

module_init(cdd_init);
module_exit(cdd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Graham Lenox, Mamani Franco, Vega Guadalupe");
MODULE_DESCRIPTION("Driver de caracteres para sensar señales en gpio para raspberry pi");