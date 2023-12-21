[section .data]
global charge_char
charge_char db 'l'

[section .text]
[BITS 16]
global head_start
head_start:
    ; 设置屏幕模式为文本模式，清除屏幕
    mov ax, 3
    int 0x10

    ; 跳过去
    mov     si, print_str
    call    print

    jmp     $

print:
    mov ah, 0x0e
    mov bh, 0
    mov bl, 0x01
.loop:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10

    inc si
    jmp .loop
.done:
    ret

print_str:
    db "success", 10, 13, 0