[SECTION .text]
[bits 32]

global  in_byte
in_byte:
    push ebp
    mov ebp, esp

    mov dl, [ebp + 8]
    in al, dl

    leave
    ret

global out_byte
out_byte:
    push ebp
    mov ebp, esp

    mov edx, [ebp + 8]   ;port
    mov eax, [ebp + 12]    ;value

    out dx, al

    leave
    ret
