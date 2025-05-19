.code16
        mov $msg, %si
        mov $0x0e, %ah
loop:
        lodsb
        or %al, %al
        jz halt
        int $0x10
        jmp loop
halt:
        jmp halt

msg:
        .ascii "hello world"
        .byte 0