#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/init.h>

static int __init mi_mod_init(void) {
    struct new_utsname *u = utsname(); // Obtener la estructura utsname
    printk(KERN_INFO "MiModulo: Se está cargando en la máquina: %s\n", u->nodename); // Imprimir el nombre de la máquina
    return 0;
}

static void __exit mi_mod_exit(void) {
    printk(KERN_INFO "MiModulo: Se está descargando de la máquina: %s\n", utsname()->nodename); // Imprimir el nombre de la máquina al descargar el módulo
}

module_init(mi_mod_init);
module_exit(mi_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lenox");
MODULE_DESCRIPTION("Módulo que imprime el nombre de la máquina");
