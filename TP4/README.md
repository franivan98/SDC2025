# Marco Teórico

## ¿Qué es el Kernel?

El kernel, también conocido como núcleo, es el corazón del sistema operativo. Es esa capa intermedia que se encarga de que el software pueda usar el hardware sin tener que lidiar con sus complejidades. Está presente en cualquier sistema operativo moderno, desde Windows hasta Linux, siendo este último uno de los más conocidos por ser de código abierto.

### Función del Kernel

El kernel es como un "organizador" que reparte los recursos del hardware (como CPU, memoria o dispositivos) entre los programas que los necesitan. Se encarga de decidir quién usa qué, cuánto y cuándo. También es el guardián que evita que un programa interfiera con otro o acceda a partes del sistema que no debería. Esto lo hace crucial para la seguridad y estabilidad general.

### ¿Que son los modulos del kernel?

En el caso de Linux, el kernel es modular. Esto significa que se pueden agregar o quitar funcionalidades sin tener que reiniciar el sistema ni recompilar todo el núcleo. Por ejemplo:

* Agregar drivers para nuevas tarjetas gráficas.
* Habilitar sensores de temperatura del sistema.
* Activar soporte para una nueva placa de red.

Estos módulos se cargan o descargan según se necesiten, lo que permite adaptar el sistema al hardware disponible de forma flexible y eficiente.

---

# Desarrollo

Para trabajar con el kernel y sus módulos, vamos a usar un entorno Linux. Es importante tener instalado el código fuente del kernel o los encabezados (headers). 

## Primeros Pasos

1. **Instalar las herramientas básicas:**

```bash
$ sudo apt-get install build-essential 
```

Esto instala lo necesario para compilar programas en Linux, como `gcc`, `make` y herramientas para trabajar específicamente con el kernel. 

2. **Instalar el código fuente del kernel:**

```bash
$ sudo apt-get install linux-source
```

Con esto se tiene lo necesario para compilar neustros propios módulos del kernel.


## Comandos Clave para Manejo de Módulos


El trabajo tiene una carpeta llamada "module", donde hay dos archivos inicialmente, un Makefile que se encarga de compilar el módulo y un archivo .c que es el código del módulo.

1. **`make clean`**

   * Limpia los archivos de compilaciones anteriores.

2. **`make all`**

   * Compila el módulo, generando un archivo `.ko`.

3. **`ls -lha`**

   * Muestra información sobre los archivos generados.


[![](./images/1.png)]()




Una vez compilado el módulo, se puede ver el archivo .ko en la carpeta "module".


4. **`modinfo mimodulo.ko`**

   * Muestra detalles del módulo: autor, licencia, dependencias, etc.

5. **`sudo insmod mimodulo.ko`**

   * Carga el módulo en el kernel.

6. **`sudo rmmod mimodulo.ko`**

   * Descarga el módulo del kernel.

7. **`lsmod`**

   * Muestra todos los módulos actualmente cargados.

 


 [![](./images/2.png)]()



Finalmente, se puede ver el mensaje de que el módulo se cargó correctamente cuando lo agregamos al kernel. Y cuando lo eliminamos, se ve el mensaje de que se descargó correctamente.


8. **`dmesg`**

   * Visualiza los mensajes generados por el kernel.

 [![](./images/3.png)]()


> **Advertencia: Problemas con Secure Boot**
>
> Algunos módulos pueden fallar al cargarse si Secure Boot está activado. Para solucionarlo:
>
> 1. Entra al BIOS/UEFI.
> 2. Busca y desactiva "Secure Boot".
> 3. Guarda y reinicia.
> 4. Prueba de nuevo a cargar el módulo.

### Nuestro Módulo de Ejemplo

Este módulo solo imprime un mensaje con `printk` cuando se carga. Aunque simple, es muy útil para practicar cómo compilar, insertar y depurar módulos.

## ¿Qué módulos están disponibles?

Para ver los módulos del kernel instalados:

```bash
ls -l /lib/modules/$(uname -r)/kernel
```

[![](./images/4.png)]()
 


# DESAFIO #1


## ¿Qué es CheckInstall?

Cuando instalás software en Linux desde el código fuente, normalmente usás comandos como `./configure`, `make`, y `make install`. El problema de este enfoque es que el sistema no tiene forma de saber qué archivos se instalaron ni cómo desinstalarlos fácilmente después.

Ahí entra **CheckInstall**, una herramienta muy útil que resuelve este problema: permite registrar esa instalación como si fuera un paquete `.deb` o `.rpm`, dependiendo de la distribución. De esta manera, podés gestionar ese software con tu gestor de paquetes habitual (como `dpkg` en Debian/Ubuntu), hacer actualizaciones más limpias y desinstalaciones más seguras.

Es especialmente útil si estás desarrollando software propio o compilando programas que todavía no están en los repositorios oficiales.

## Crear un paquete con CheckInstall: Ejemplo con Hello World

1. **Se instala CheckInstall:**

```bash
sudo apt-get update
sudo apt-get install checkinstall
```

2. **Se escribe el código fuente:**
   Archivo `hello.c`:

```c
#include <stdio.h>
int main() {
    printf("HELLO WORLD!\n");
    return 0;
}
```

3. **Se compila el código fuente:**

```bash
gcc -o hello hello.c
```

4. **Se crea un script de instalación:**
   Archivo `install.sh`:

```bash
#!/bin/bash
cp hello /usr/local/bin/
```

5. **Se hace ejecutable el script e instalá con CheckInstall:**

```bash
chmod +x install.sh
sudo checkinstall --pkgname=hello-world --pkgversion=1.0 --backup=no --deldoc=yes --default ./install.sh
```

La salida de este comando es la siguiente:

[![](./images/5.png)]()




Esto generará un archivo `.deb` que se instalará en tu sistema y que luego podés remover con:

```bash
sudo dpkg -r hello-world
```
 
[![](./images/6.png)]()


## Seguridad en los Módulos del Kernel

Cuando trabajamos con módulos del kernel, la seguridad es un aspecto crítico. Cargar un módulo es darle acceso directo al corazón del sistema, por eso es fundamental garantizar su integridad y autenticidad. Una forma de hacerlo es mediante la firma digital de módulos.

### ¿Por qué firmar un módulo?

Firmar un módulo permite verificar que no ha sido alterado desde que fue creado. Es especialmente importante si tenés Secure Boot activado, ya que éste bloquea la carga de módulos no firmados como medida de protección ante software malicioso.

### Firmado de Módulos: Pasos

1. **Crear un certificado SSL:**

Usá OpenSSL y un archivo `.cnf` que describe los atributos del certificado. Ejemplo básico:

```ini
[ req ]
distinguished_name = req_distinguished_name
x509_extensions = v3
prompt = no

[ req_distinguished_name ]
countryName = AR
stateOrProvinceName = Cordoba
localityName = Cordoba
organizationName = UNC
commonName = FirmaDeModulo

[ v3 ]
basicConstraints = CA:FALSE
keyUsage = digitalSignature
extendedKeyUsage = codeSigning
```

Generá las claves:

```bash
openssl req -config openssl.cnf -new -x509 -newkey rsa:2048 -nodes -days 36500 -outform DER \
-keyout MOK.priv -out MOK.der
```

2. **Registrar la clave en el sistema (enroll):**

```bash
sudo mokutil --import MOK.der
```

Esto activa un proceso de confirmación al reiniciar desde el entorno UEFI.

3. **Firmar el módulo compilado:**

```bash
sudo /usr/src/linux-headers-$(uname -r)/scripts/sign-file sha256 MOK.priv MOK.der mimodulo.ko
```

4. **Verificar la firma:**

```bash
modinfo mimodulo.ko
```

### Medidas adicionales de seguridad

Firmar módulos es sólo una parte. Algunas estrategias extra para fortalecer la seguridad del kernel incluyen:

* **Prevención de desbordamientos de búfer (buffer overflow):** mitigando vulnerabilidades comunes.
* **Protección de memoria crítica:** evitando escrituras no autorizadas en estructuras internas del kernel.
* **Uso de herramientas como LKRG:** para detectar modificaciones del kernel en tiempo real.
* **Políticas de acceso:** como SELinux o AppArmor para restringir lo que pueden hacer usuarios y procesos.
* **Randomización de memoria (ASLR):** moviendo aleatoriamente datos sensibles en la RAM para prevenir ataques.

---

# DESAFIO #2

Vamos a comparar un modulo de kernel con nuestro modulo y vamos a identificar las diferencias.

## ¿Qué diferencias se pueden observar entre los dos modinfo ? 

```bash
modinfo mimodulo.ko > modInfo/mimoduloInfo.txt
modinfo /lib/modules/$(uname -r)/kernel/crypto/des_generic.ko.zst > modInfo/genericInfo.txt
```
Los archivos los guardamos en `module/modInfo`.

[![](./images/modinfoComp.png)]()

Se puede ver que `mimodulo.ko` es un ejemplo sencillo sin funcionalidades reales, sin dependencias ni firma digital. No esta comprimido ni forma parte de un arbol oficial de kernel.

En cambio `des_generic.ko.zst`es un modulo oficial del kernel ubicado en `lib/modules/`, implementa cifrado DES/3DES, tiene dependencias, esta comprimido, firmado digitalmente y marcado como parte del arbol del kernel.

## ¿Qué divers/modulos estan cargados en sus propias pc?

vamos a trabajar en el directorio `TP4/modulos`.
Aqui van a estar cargados 3 archivos con los modulos cargados en cada pc.

# (FALTA COMPLETAR. CARGUEN SUS MODULOS CON ESTO: `lsmod > PC_nombre_modulos.txt`)

## ¿cuales no están cargados pero están disponibles? que pasa cuando el driver de un dispositivo no está disponible. 

Para poder ver todos los modulos disponibles de nuestro sistema, vamos a usar el siguiente comando:

```bash
find /lib/modules/$(uname -r)/kernel -type f -name "*.ko*" > PC_Franco_modulos_disponibles.txt
```
si abrimos el archivo, vamos a ver que tenemos disponibles un total de 6589 modulos, como drivers para camaras, controladores de red, sonido, etc. Cargados actualmente tenemos 158, como pudimos ver en el item anterior.

[![](./images/modDisponibles.png)]()

Si un driver o modulo no esta disponible en el sistema (ni cargado ni instalado), el hardware correspondiente no funcionará. En estos casos, el sistema puede mostrar errores en `dmesg`, `journalctl`, o el hardware directamente no aparecera en herramientas como `lspci`, `lsusb`o `ifconfig`.

Ejemplos de consecuencias:
* Una tarjeta wifi no funcionara si falta su modulo.
* UN puerto USB 3.0 puede no tener la velocidad maxima si falta el modulo `xhci_hcd`.
* UN sistema virtualizado no podra acceder al disco si falta el driver de almacenamiento virtual.

## Correr hwinfo en una pc real con hw real y agregar la url de la información de hw en el reporte. 

hwinfo es una herramienta que permite detectar y mostrar informacion detallada del hardware de una computadora. Escanea todos los componentes del sistema: CPU, memoria, discos, tarjetas, graficas, red, sonido, etc.

para usarlo, primero lo vamos a instalar:
```bash
sudo apt install hwinfo
```

Para ver un resumen rapido del sistema podemos usar el siguiente comando:

```bash
hwinfo --short
```
[![](./images/hwinfo.png)]()

y por ultimo, para guardar toda nuestra informacion usaremos lo siguiente (seguimos en el directorio modulos)

```bash
hwinfo > hwinfo_Franco.txt
```



