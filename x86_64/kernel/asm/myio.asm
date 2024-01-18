[SECTION .text]
[bits 32]

global  my_in_byte
my_in_byte:
    push ebp
    mov ebp, esp

    mov edx, edi      ; port
    in al, dx

    leave
    ret

global my_out_byte
my_out_byte:
    push ebp
    mov ebp, esp

    mov edx, edi      ; port
    mov eax, esi      ; value
    out dx, al

    leave
    ret

global my_in_word
my_in_word:
    push ebp
    mov ebp, esp

    xor eax, eax

    mov edx, edi      ; port
    in ax, dx

    leave
    ret

global my_out_word
my_out_word:
    push ebp
    mov ebp, esp

    mov edx, edi      ; port
    mov eax, esi      ; value
    out dx, ax

    leave
    ret