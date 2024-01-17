[SECTION .text]
[bits 32]

global  my_in_byte
my_in_byte:
    push ebp
    mov ebp, esp

    mov edx, [ebp + 8]
    in al, dx

    leave
    ret

global my_out_byte
my_out_byte:
    push ebp
    mov ebp, esp

    mov edx, [ebp + 8]   ;port
    mov eax, [ebp + 12]    ;value

    out dx, al

    leave
    ret

global my_in_word
my_in_word:
    push ebp
    mov ebp, esp

    xor eax, eax

    mov edx, [ebp + 8]      ; port
    in ax, dx

    leave
    ret

global my_out_word
my_out_word:
    push ebp
    mov ebp, esp

    mov edx, [ebp + 8]      ; port
    mov eax, [ebp + 12]     ; value
    out dx, ax

    leave
    ret