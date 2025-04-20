# Analisis sobre el comportamiento de la pila y el stackframe en la función `float_to_int`

Analizaremos el comportamiento del stack durante la ejecución de `float_int.asm` usando GDB. Para ello, escribimos un código en `test_stack.c` que le pasa un `float` como parámetro a nuestra función (parámetro = 3.14).

Para compilar usaremos lo siguiente: 
```bash
nasm -f elf32 float_int.asm -o float_to_int.o
gcc -m32 test_stack.c float_to_int.o -o test_stack
```

Utilizamos un script de GDB llamado `debug_float_to_int.gdb` para automatizar la depuración. Lo ejecutamos con el siguiente comando:

```bash
gdb -x debug_float_to_int.gdb ./test_stack
```

A continuación, explicaremos la salida obtenida en `gdb_output.txt`.

---

 ## Primer Fragmento:
 ```plaintext
 ===========================
INICIO DE DEPURACIÓN
===========================
Punto de interrupción 1 at 0x1210: file float_to_int.asm, line 6.

This GDB supports auto-downloading debuginfo from the following URLs:
  <https://debuginfod.ubuntu.com>
Enable debuginfod for this session? (y or [n]) [answered N; input not from terminal]
Debuginfod has been disabled.
To make this setting permanent, add 'set debuginfod enabled off' to .gdbinit.
[Depuración de hilo usando libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, float_to_int () at float_to_int.asm:6
6	    push ebp

>>> Corriendo hasta entrar a float_to_int

>>> [1] Estado de registros al entrar a float_to_int:
eax            0x29                41
ecx            0x0                 0
edx            0x0                 0
ebx            0x56558fd8          1448447960
esp            0xffffcd1c          0xffffcd1c
ebp            0xffffcd48          0xffffcd48
esi            0xffffce1c          -12772
edi            0xf7ffcb60          -134231200
eip            0x56556210          0x56556210 <float_to_int>
eflags         0x296               [ PF AF SF IF ]
cs             0x23                35
ss             0x2b                43
ds             0x2b                43
es             0x2b                43
fs             0x0                 0
gs             0x63                99
 ```
Estamos justo en la línea 6 de `float_to_int.asm`, donde estamos a punto de preparar el stack frame con `push ebp`.

Estado de los registros antes de la ejecución:

| Registro | Valor        | Significado                                |
|----------|--------------|--------------------------------------------|
| eax      | 0x29 (41)    | Registro general, suele devolver resultados|
| ecx, edx | 0            | Libres por ahora                           |
| esp      | 0xffffcd1c   | Stack Pointer (puntero al tope de la pila) |
| ebp      | 0xffffcd48   | Base Pointer del stack frame anterior      |
| eip      | 0x56556210   | Dirección de la instrucción actual         |

---
## Segundo Fragmento: 
 ```plaintext
>>> [2] Stack inicial (16 palabras desde ESP):
0xffffcd1c:	0x565561da	0x4048f5c3	0x60000000	0x40091eb8
0xffffcd2c:	0x565561a4	0x00000000	0x00000000	0x4048f5c3
0xffffcd3c:	0x00000000	0xffffcd60	0xf7fa1e34	0x00000000
0xffffcd4c:	0xf7d95cb9	0x00000000	0xffffce14	0xf7daf13d
7	    mov ebp, esp
9	    sub esp, 4            ; reservar espacio en stack temporal
 ```

 En este fragmento, podemos observar las 16 primeras palabras del stack, partiendo desde la posicion esp. Estos valores pertenecen a la funcion que llamo a `float_to_int.asm`. El stack es compartido entre funciones. cuando nuestra funcion empieza a ejecutarse la funcion anterior ya dejo estas cosas en la pila.
 El stack frame de una funcion tiene la siguiente estructura: 
 | Elemento               | Descripción                                                  |
|------------------------|--------------------------------------------------------------|
| Dirección de retorno   | <- la dirección a la que se regresa cuando la función termina. |
| Valor anterior de EBP  | <- el valor de `ebp` del stack frame anterior.               |
| Parámetro 1            | <- el primer parámetro pasado a la función.                  |
| Parámetro 2            | <- el segundo parámetro pasado a la función.                 |
| ...                    |                                                              |
| Variables locales      | <- espacio reservado por la función para variables internas. |


 Ahora se esta ejecutando lo siguiente:
```
mov ebp, esp
```
Ahora, el puntero base (ebp) se va a mover al valor actual del esp. es aqui donde se crea el stack frame en donde posteriormente reservamos 4 bytes de memoria con `sub esp,4`

## Tercer Fragmento:
```plaintext
>>> [3] Float original (en [ebp+8]):
0xffffcd20:	0x4048f5c3
$1 = 3.1400001
11	    fld dword [ebp + 8]   ; cargar el float desde los argumentos
12	    fistp dword [esp]     ; convertir a int y guardar en stack
13	    mov eax, [esp]        ; pasar el valor a eax
```

El float en la posicion ebp+8 es el parametro guardado en la pila que nos esta pasando la funcion anterior. En este caso, nos esta pasando el flotante 3.14. 
Con `fld` lo que hacemos es pasar el flotante que nos envian y pasarlo al coprocesador. posteriormente, convertimos ese valor a int con `fistp` y lo guardamos en el stack (esp). por ultimo, pasamos el valor del stack al registro eax para seguir con el proximo fragmento.

## Cuarto Fragmento:
```plaintext
>>> [4] Valor entero guardado en stack:
0xffffcd14:	0x00000003
15	    add eax, 1            ; sumar 1
```

En este fragmento, podemos ver que el valor guardado en el stack es 3, el cual es lo que nosotros queremos. como ese valor esta en el registro eax, le sumamos 1, obteniendo como resultado 4, que es lo que vamos a retornar en la funcion.

## Ultimo Fragmento
```plaintext
>>> [7] Stack restaurado (16 palabras desde ESP):
0xffffcd1c:	0x565561da	0x4048f5c3	0x60000000	0x40091eb8
0xffffcd2c:	0x565561a4	0x00000000	0x00000000	0x4048f5c3
0xffffcd3c:	0x00000000	0xffffcd60	0xf7fa1e34	0x00000000
0xffffcd4c:	0xf7d95cb9	0x00000000	0xffffce14	0xf7daf13d
0x565561da in main () at test_stack.c:10
10	    resultado=float_to_int(valor);

=============================
FIN DE LA DEPURACIÓN
=============================
```

En este fragmento, restauramos el stack con las instrucciones `mov esp,ebp`(que restaura el puntero del stack a su valor original) y con `pop ebp`. se puede ver que el stack es el mismo que el que teniamos antes de ejecutar nuestra funcion. 
usando la instruccion `ret` retornamos a la direccion de retorno que figura en el stack.(0x565561da).
