section .text
global _start

_start:
; Define a data segment named "data_segment"
data_segment:
    db "This is some data"

; Change the access bits of the data segment to read-only
mov eax, 0x0f            ; Set EAX to the control register number
mov ecx, data_segment    ; Set ECX to the address of the data segment
mov ebx, 0x02            ; Set EBX to the read-only bitmask
mov [ecx], ebx           ; Write the bitmask to the control register

; Attempt to write to the data segment
mov eax, 0x61            ; Set EAX to the value to write
mov ebx, data_segment    ; Set EBX to the address of the data segment
mov [ebx], eax           ; Attempt to write the value to the data segment

; Print a message indicating that the data has been modified
mov eax, 4           ; Set EAX to the system call number for writing to stdout
mov ebx, data_segment ; Set EBX to the address of the data
mov ecx, 16            ; Set ECX to the number of bytes to write
mov edx, 0           ; Set EDX to the output file descriptor (1 for stdout)
int 0x80              ; Make the system call

; Exit the program
mov eax, 1           ; Set EAX to the system call number for exiting
int 0x80              ; Make the system call



;nasm -f elf32 -g test_modify_with_read.S -o test_modify_with_read.o
;ld -m elf_i386 test_modify_with_read.o -o programa
