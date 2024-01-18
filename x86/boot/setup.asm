[ORG  0x500]

[SECTION .data]
KERNEL_ADDRESS equ 0x1200   ;内核在磁盘中的地址
x64_KERNEL_ADDRESS equ 0x100000
x64_KERNEL_ADDRESS_ESI_MAX equ 50
X64_KERNEL_SECTOR_START equ 41  ;此参数需要与makefile中的数据保持一致
REAL_MODE_STACK_BASE equ 0x7c00

MEMORY_TEST_TIME equ 0
MEMORY_TEST_TIME_ADDRESS equ 0x7e00
MEMORY_TEST_BUFFER_ADDRESS equ 0x7e02

[SECTION .gdt]
SEG_BASE equ 0
SEG_LIMIT equ 0xfffff

CODE_SELECTOR equ  (1 << 3)   ;系统规定0不能使用，这里是假设1是代码段选择子，2是数据段选择子
DATA_SELECTOR equ  (2 << 3)

;下面开始GDT表的定义
gdt_base:
    dd 0, 0
gdt_code:   ;设置代码段
    dw  SEG_LIMIT & 0xffff
    dw  SEG_BASE & 0xffff
    db  SEG_BASE >> 16 & 0xff
    ;     P_DPL_S_TYPE
    db  0b1_00_1_1000
    ;     G_DB_AVL_LIMIT
    db  0b0_1_00_0000 | (SEG_LIMIT >> 16 & 0xf)
    db  SEG_BASE >> 24 & 0xff
gdt_data:   ;设置数据段
    dw  SEG_LIMIT & 0xffff
    dw  SEG_BASE & 0xffff
    db  SEG_BASE >> 16 & 0xff
    ;     P_DPL_S_TYPE
    db  0b1_00_1_0010
    ;     G_DB_AVL_LIMIT
    db  0b1_1_00_0000 | (SEG_LIMIT >> 16 & 0xf)
    db  SEG_BASE >> 24 & 0xff
gdt_ptr:
    dw  $ - gdt_base ; 这是GDT表的描述符，包含GDT表的大小（16位）和基地址（32位）。这个描述符在加载GDT时会用到。
    dd  gdt_base

[SECTION .text]
[BITS 16]
global setup_start
setup_start:
    mov     ax, 0
    mov     ss, ax
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     si, ax

    mov     si, prepare_enter_protected_mode_msg
    call    print

;做内存检测
;Basic Usage:
;For the first call to the function, point ES:DI at the destination buffer for the list. Clear EBX. Set EDX to the magic number 0x534D4150. Set EAX to 0xE820 (note that the upper 16-bits of EAX should be set to 0). Set ECX to 24. Do an INT 0x15.
;If the first call to the function is successful, EAX will be set to 0x534D4150, and the Carry flag will be clear. EBX will be set to some non-zero value, which must be preserved for the next call to the function. CL will contain the number of bytes actually stored at ES:DI (probably 20).
;For the subsequent calls to the function: increment DI by your list entry size, reset EAX to 0xE820, and ECX to 24. When you reach the end of the list, EBX may reset to 0. If you call the function again with EBX = 0, the list will start over. If EBX does not reset to 0, the function will return with Carry set when you try to access the entry after the last valid entry.

do_e820:
xchg  bx, bx
	xor ebx, ebx
    mov di, MEMORY_TEST_BUFFER_ADDRESS

.memory_test_loop:

	mov edx, 0x0534D4150
	mov eax, 0xe820
	mov ecx, 20
	int 0x15

    jc .memory_check_error   ; 如果出错

	add di, cx
	inc dword [MEMORY_TEST_TIME]

	cmp ebx, 0
	jne .memory_test_loop

	mov ax, [MEMORY_TEST_TIME]
	mov [MEMORY_TEST_TIME_ADDRESS], ax

.memory_check_success:
    mov si, memory_check_success_msg
    call print

    jmp enter_protected_mode

.memory_check_error:
    mov     si, memory_check_error_msg
    call    print

    jmp $

enter_protected_mode:  ;上面的准备工作做完，开始正式的进入保护模式的流程

    xchg    bx, bx

    cli
    lgdt [gdt_ptr]

    in    al,  92h
    or    al,  00000010b
    out   92h, al

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    xchg  bx, bx
    jmp CODE_SELECTOR:protected_mode

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

[BITS 32]
protected_mode:
    mov ax, DATA_SELECTOR
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, REAL_MODE_STACK_BASE

    mov edi, KERNEL_ADDRESS
    mov ecx, 3  ;这个应该是和makefile中需要读取的seek一致
    mov bl, 30  ;这个应该是和makefile中需要读取的count一致
    call read_hd

    xchg  bx, bx
    xor esi, esi    ;将esi置为0,这里esi的作用就是记录前边读硬盘次数，从而在内存中跳过之前已经使用的空间

.load_x64_kernel:

    mov eax, 0x20000
    mul esi
    lea edi, [x64_KERNEL_ADDRESS + eax]     ; 从硬盘中读取数据后放在内存的哪个位置

    mov eax, 256
    mul esi
    lea ecx, [X64_KERNEL_SECTOR_START + eax]    ;从哪个扇区开始读
    mov bl, 0xff ;每次读多少个扇区

    push esi
    call read_hd
    pop  esi

    inc esi
    cmp esi, x64_KERNEL_ADDRESS_ESI_MAX
    jne .load_x64_kernel

    jmp CODE_SELECTOR:KERNEL_ADDRESS

read_hd:
    ; 0x1f2 8bit 指定读取或写入的扇区数
    mov dx, 0x1f2
    mov al, bl
    out dx, al

    ; 0x1f3 8bit iba地址的第八位 0-7
    inc dx
    mov al, cl
    out dx, al

    ; 0x1f4 8bit iba地址的中八位 8-15
    inc dx
    mov al, ch
    out dx, al

    ; 0x1f5 8bit iba地址的高八位 16-23
    inc dx
    shr ecx, 16
    mov al, cl
    out dx, al

    ; 0x1f6 8bit
    ; 0-3 位iba地址的24-27
    ; 4 0表示主盘 1表示从盘
    ; 5、7位固定为1
    ; 6 0表示CHS模式，1表示LAB模式
    inc dx
    shr ecx, 8
    and cl, 0b1111
    mov al, 0b1110_0000     ; LBA模式
    or al, cl
    out dx, al

    ; 0x1f7 8bit  命令或状态端口
    inc dx
    mov al, 0x20
    out dx, al

    ; 设置loop次数，读多少个扇区要loop多少次
    mov cl, bl
.start_read:
    push cx     ; 保存loop次数，防止被下面的代码修改破坏

    call .wait_hd_prepare
    call read_hd_data

    pop cx      ; 恢复loop次数

    loop .start_read

.return:
    ret

; 一直等待，直到硬盘的状态是：不繁忙，数据已准备好
; 即第7位为0，第3位为1，第0位为0
.wait_hd_prepare:
    mov dx, 0x1f7

.check:
    in al, dx
    and al, 0b1000_1000
    cmp al, 0b0000_1000
    jnz .check

    ret

; 读硬盘，一次读两个字节，读256次，刚好读一个扇区
read_hd_data:
    mov dx, 0x1f0
    mov cx, 256

.read_word:
    in ax, dx
    mov [edi], ax
    add edi, 2
    loop .read_word

    ret


prepare_enter_protected_mode_msg:
    db "let's go to the protected mode... ",10, 13, 0

memory_check_error_msg:
    db "memory check fail...", 10, 13, 0

memory_check_success_msg:
    db "memory check success...", 10, 13, 0