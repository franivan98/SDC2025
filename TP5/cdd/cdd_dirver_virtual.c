#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
// #include <linux/gpio.h> // VirtualBox no tiene hardware GPIO, así que no se usa esta librería

#define DEVICE_NAME "cdd_signal"
#define CLASS_NAME "cdd_class"

// En Raspberry usaríamos GPIO 23 y 24 reales, pero en VirtualBox no existen.
// #define GPIO_SIG0 23 // Definición del GPIO para la señal 0
// #define GPIO_SIG1 24 // Definición del GPIO para la señal 1
#define GPIO_SIG0 0
#define GPIO_SIG1 1

#define MESSAGE_SIZE 32 // Tamaño del mensaje para lectura/escritura

static dev_t dev_num; // Número de dispositivo
static struct cdev cdd_dev; // Estructura del dispositivo de caracteres
static struct class *cdd_class = NULL; // Clase del dispositivo

static struct timer_list signal_timer; // Temporizador para manejar las señales
static int current_signal = 0; // Variable para almacenar la señal actual
static int signal_value[2]={0,0}; // Array para almacenar los valores de las señales

//Timer Callback Function
// static void timer_callback(struct timer_list *t){
//     // Leer el valor de los GPIO
//     signal_value[0] = gpio_get_value(GPIO_SIG0);
//     signal_value[1] = gpio_get_value(GPIO_SIG1);

//     // Imprimir los valores leídos
//     printk(KERN_INFO "CDD: Signal 0: %d, Signal 1: %d\n", signal_value[0], signal_value[1]);

//     // Reiniciar el temporizador
//     mod_timer(&signal_timer, jiffies + msecs_to_jiffies(1000)); // 1000 ms = 1 segundo
// }
// Simulación del timer: aquí se simulan las señales de GPIO.
static void timer_callback(struct timer_list *t){
    // Simulamos valores alternantes en lugar de leer GPIO reales
    signal_value[0] = jiffies % 2; 
    signal_value[1] = (jiffies / HZ) % 2;

    printk(KERN_INFO "CDD: Signal 0: %d, Signal 1: %d\n", signal_value[0], signal_value[1]);

    mod_timer(&signal_timer, jiffies + msecs_to_jiffies(1000));
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
    static char msg[MESSAGE_SIZE];
    int value = signal_value[current_signal];
    int msg_len;

    if (*offset != 0)
        return 0;  // Fin del archivo

    msg_len = snprintf(msg, MESSAGE_SIZE, "Signal %d: %d\n", current_signal, value);

    if (copy_to_user(buffer, msg, msg_len))
        return -EFAULT;

    *offset = msg_len;  // Avanzamos el offset para que cat no entre en bucle
    return msg_len;
}


static ssize_t cdd_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset){
    char kbuf[2];
    if(len<1 || copy_from_user(kbuf, buffer, 1)) {
        return -EFAULT; // Error al copiar del usuario
    }
    if(kbuf[0] == '0' || kbuf[0] == '1') {
        current_signal = kbuf[0] - '0'; // Actualizar la señal actual
        printk(KERN_INFO "CDD: Cambiando a señal %d\n", current_signal);
    } else {
        printk(KERN_WARNING "CDD: Señal invalida.\n");
    }
    return len; // Retornar el número de bytes escritos
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = cdd_opeen,
    .release = cdd_release,
    .read = cdd_read,
    .write = cdd_write,
};

static int __init cdd_init(void){
    int ret;

    // ❌ Se eliminó el request de GPIOs porque VirtualBox no tiene hardware GPIO
    // ret=gpio_request(GPIO_SIG0,"sig0");
    // if(ret) {
    //     printk(KERN_ERR "CDD: Error al solicitar GPIO %d.\n", GPIO_SIG0);
    //     return ret; // Retorna error si no se pudo solicitar el GPIO
    // }
    // ret=gpio_request(GPIO_SIG1,"sig1");
    // if(ret) {
    //     printk(KERN_ERR "CDD: Error al solicitar GPIO %d.\n", GPIO_SIG1);
    //     gpio_free(GPIO_SIG0); // Liberar el GPIO anterior
    //     return ret; // Retorna error si no se pudo solicitar el GPIO
    // }

    // // Configurar los GPIO como entradas
    // gpio_direction_input(GPIO_SIG0);
    // gpio_direction_input(GPIO_SIG1);

    //Registrar el dispositivo de caracteres
    ret=alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if(ret < 0) {
        printk(KERN_ERR "CDD: No se pudo registrar el dispositivo.\n");
        // ❌ Se eliminó el gpio_free porque VirtualBox no tiene hardware GPIO
        // gpio_free(GPIO_SIG0);
        // gpio_free(GPIO_SIG1);

        return ret; // Retorna error si no se pudo registrar el dispositivo
    }
    cdev_init(&cdd_dev, &fops);
    cdd_dev.owner = THIS_MODULE;
    ret = cdev_add(&cdd_dev, dev_num, 1);
    if(ret < 0) {
        printk(KERN_ERR "CDD: No se pudo agregar el dispositivo de caracteres.\n");
        unregister_chrdev_region(dev_num, 1);
        // ❌ Se eliminó el gpio_free porque VirtualBox no tiene hardware GPIO
        // gpio_free(GPIO_SIG0);
        // gpio_free(GPIO_SIG1);
        return ret; // Retorna error si no se pudo agregar el dispositivo
    }
    cdd_class = class_create(THIS_MODULE, CLASS_NAME);
    if(IS_ERR(cdd_class)) {
        printk(KERN_ERR "CDD: No se pudo crear la clase del dispositivo.\n");
        cdev_del(&cdd_dev);
        unregister_chrdev_region(dev_num, 1);
        // ❌ Se eliminó el gpio_free porque VirtualBox no tiene hardware GPIO
        // gpio_free(GPIO_SIG0);
        // gpio_free(GPIO_SIG1);
        return PTR_ERR(cdd_class); // Retorna error si no se pudo crear la clase
    }
    device_create(cdd_class, NULL, dev_num, NULL, DEVICE_NAME);

    // Inicializar el temporizador
    timer_setup(&signal_timer, timer_callback, 0);
    mod_timer(&signal_timer, jiffies + msecs_to_jiffies(1000)); // Iniciar el temporizador para que se ejecute cada segundo
    
    printk(KERN_INFO "CDD: Modulo Cargado.\n");
    return 0; // Retorna 0 si la carga fue exitosa
}
static void __exit cdd_exit(void){

    del_timer_sync(&signal_timer); // Eliminar el temporizador
    device_destroy(cdd_class, dev_num); // Destruir el dispositivo
    class_destroy(cdd_class); // Destruir la clase del dispositivo
    cdev_del(&cdd_dev); // Eliminar el dispositivo de caracteres
    unregister_chrdev_region(dev_num, 1); // Desregistrar el dispositivo de caracteres

    // ❌ GPIOs no se usan en este entorno
    // gpio_free(GPIO_SIG0); // Liberar el GPIO 0
    // gpio_free(GPIO_SIG1); // Liberar el GPIO 1

    printk(KERN_INFO "CDD: Modulo Descargado.\n");
}

module_init(cdd_init);
module_exit(cdd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Graham Lenox, Mamani Franco, Vega Guadalupe");
MODULE_DESCRIPTION("Driver de caracteres para sensar señales en gpio para raspberry pi");