#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/gpio.h>

#define DEVICE_NAME "cdd_signal"
#define CLASS_NAME "cdd_class"

#define GPIO_SIG0 23 // Definición del GPIO para la señal 0
#define GPIO_SIG1 24 // Definición del GPIO para la señal 1

static dev_t dev_num; // Número de dispositivo
static struct cdev c_dev; // Estructura del dispositivo de caracteres
static struct class *cdd_class = NULL; // Clase del dispositivo

static struct timer_list signal_timer; // Temporizador para manejar las señales
static int current_signal = 0; // Variable para almacenar la señal actual
static int signal_value[2]={0,0}; // Array para almacenar los valores de las señales

//Timer Callback Function
static void timer_callback(struct timer_list *t){
    // Leer el valor de los GPIO
    signal_value[0] = gpio_get_value(GPIO_SIG0);
    signal_value[1] = gpio_get_value(GPIO_SIG1);

    // Imprimir los valores leídos
    printk(KERN_INFO "CDD: Signal 0: %d, Signal 1: %d\n", signal_value[0], signal_value[1]);

    // Reiniciar el temporizador
    mod_timer(&signal_timer, jiffies + msecs_to_jiffies(1000)); // 1000 ms = 1 segundo
}

//operaciones de archivo
static int cdd_opeen(struct inode *inode, struct file *file){
    printk(KERN_INFO "CDD: Dispositivo abierto.\n");
    return 0; // Retorna 0 si la apertura fue exitosa
}
static int cdd_release(struct inode *inode, struct file *file){
    printk(KERN_INFO "CDD: Dispositivo cerrado.\n");
    return 0; // Retorna 0 si el cierre fue exitoso
}

static ssize_t cdd_read(struct file *file, char __user *buffer, size_t len, loff_t *offset){
    
    //TODO: Implementar la lectura de las señales GPIO

    ssize_t ret = 0;
    return ret; // Retornar el número de bytes leídos
}

static ssize_t cdd_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset){
    // TODO: Implementar la escritura de las señales GPIO
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = cdd_opeen,
    .release = cdd_release,
    .read = cdd_read,
    .write = cdd_write,
};

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