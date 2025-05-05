;-----------------------------
; Ultimo Desafio
;-----------------------------

[global _start]
[extern _start]

; Dirección del inicio del código en memoria
ORG 0x7C00

_start:
    ; Preparar la GDT (Global Descriptor Table)
    ; En un sistema real, los descriptores de segmentos se configuran aquí.

    ; Cargar el Descriptor de Código (32 bits)
    lgdt [gdt_desc]           ; Cargar la GDT
    
    ; Entrar al modo protegido activando PE en CR0
    mov eax, cr0
    or eax, 1                 ; Establecer el bit PE (modo protegido)
    mov cr0, eax              ; Guardar en CR0 para activar el modo protegido

    ; Saltar a un segmento de 32 bits
    ljmp 0x08, protected_mode

;-----------------------------
; Definición de la GDT (Global Descriptor Table)
;-----------------------------
gdt_desc:
    dw 0xFFFF                ; Limite del GDT
    dw 0x0000                ; Limite superior (el GDT)
    dd gdt_entry_0           ; Dirección de la primera entrada

gdt_entry_0:
    dw 0x0000                ; Base baja
    dw 0xFFFF                ; Limite bajo
    db 0x00                  ; Base media
    db 0x9A                  ; Tipo de descriptor (código de lectura, en ejecución)
    db 0xCF                  ; Propiedades (presente, 32 bits)
    db 0x00                  ; Limitado a 4GB
    db 0x00                  ; Base alta

gdt_entry_1:
    dw 0x0000                ; Base baja
    dw 0xFFFF                ; Limite bajo
    db 0x00                  ; Base media
    db 0x92                  ; Tipo de descriptor (lectura, escritura)
    db 0xCF                  ; Propiedades (presente, 32 bits)
    db 0x00                  ; Limitado a 4GB
    db 0x00                  ; Base alta

;-----------------------------
; Segmentos en modo protegido
;-----------------------------

protected_mode:
    ; Aquí podemos colocar nuestro código que corre en modo protegido

    ; Definir un segmento de datos solo lectura
    ; Iniciar con datos
    mov eax, 0x1234
    mov [data_segment], eax  ; Guardamos un valor en el segmento de datos

    ; Intentamos escribir en un segmento de solo lectura (esto debería fallar)
    mov eax, 0x5678
    mov [data_segment], eax  ; Esto debe provocar una excepción (porque el segmento es solo lectura)

    ; Fin del programa
    hlt

;-----------------------------
; Segmento de datos
;-----------------------------
section .data
    data_segment: dd 0x00000000  ; Esta es una variable en el segmento de datos

