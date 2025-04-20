global float_to_int
section .text

; int float_to_int(float f);
float_to_int:
    push ebp
    mov ebp, esp

    sub esp, 4            ; reservar espacio en stack temporal

    fld dword [ebp + 8]   ; cargar el float desde los argumentos
    fistp dword [esp]     ; convertir a int y guardar en stack
    mov eax, [esp]        ; pasar el valor a eax

    add eax, 1            ; sumar 1 como indica el TP

    mov esp, ebp
    pop ebp
    ret
section .note.GNU-stack noalloc noexec nowrite progbits