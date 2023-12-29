BUILD:=./build

CFLAGS:= -m32 # 32 位的程序
CFLAGS+= -masm=intel
CFLAGS+= -fno-builtin	# 不需要 gcc 内置函数
CFLAGS+= -nostdinc		# 不需要标准头文件
CFLAGS+= -fno-pic		# 不需要位置无关的代码  position independent code
CFLAGS+= -fno-pie		# 不需要位置无关的可执行程序 position independent executable
CFLAGS+= -nostdlib		# 不需要标准库
CFLAGS+= -fno-stack-protector	# 不需要栈保护
CFLAGS:=$(strip ${CFLAGS})

CFLAGS64 := -m64
CFLAGS64 += -mcmodel=large
CFLAGS64 += -fno-builtin	# 不需要 gcc 内置函数
CFLAGS64 += -nostdinc		# 不需要标准头文件
CFLAGS64 += -fno-pic		# 不需要位置无关的代码  position independent code
CFLAGS64 += -fno-pie		# 不需要位置无关的可执行程序 position independent executable
CFLAGS64 += -nostdlib		# 不需要标准库
CFLAGS64 += -fno-stack-protector	# 不需要栈保护

DEBUG:= -g

HD_IMG_NAME:= "hd.img"

# 磁盘制作
all: ${BUILD}/boot/boot.o ${BUILD}/boot/setup.o ${BUILD}/system.bin
#		${BUILD}/x64/system.bin
	$(shell rm -rf $(BUILD)/$(HD_IMG_NAME))
	bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $(BUILD)/$(HD_IMG_NAME)	#通过bochs模拟一块硬盘
	dd if=${BUILD}/boot/boot.o of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=0 count=1 conv=notrunc	#通过dd命令，将.o文件写入到硬盘当中
	dd if=${BUILD}/boot/setup.o of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=1 count=2 conv=notrunc
	dd if=${BUILD}/system.bin of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=3 count=60 conv=notrunc
	#dd if=${BUILD}/x64/system.bin of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=71 count=5000 conv=notrunc

# x86内核使用
${BUILD}/system.bin: ${BUILD}/kernel.bin
	objcopy -O binary ${BUILD}/kernel.bin ${BUILD}/system.bin
	nm ${BUILD}/kernel.bin | sort > ${BUILD}/system.map

${BUILD}/kernel.bin: ${BUILD}/boot/head.o ${BUILD}/init/main.o ${BUILD}/init/cpu.o ${BUILD}/lib/string.o ${BUILD}/kernel/chr_drv/console.o \
	${BUILD}/kernel/asm/myio.o  ${BUILD}/kernel/asm/cpu_correlation.o
	ld -m elf_i386 $^ -o $@ -Ttext 0x1200

${BUILD}/init/%.o: x86/init/%.c
	$(shell mkdir -p ${BUILD}/init)
	gcc ${CFLAGS} ${DEBUG} -c $< -o $@

${BUILD}/boot/head.o: x86/boot/head.asm
	nasm -f elf32 -g $< -o $@

${BUILD}/lib/%.o: x86/lib/%.c
	$(shell mkdir -p ${BUILD}/lib)
	gcc ${CFLAGS} ${DEBUG} -c $< -o $@

${BUILD}/kernel/chr_drv/%.o: x86/kernel/chr_drv/%.c
	$(shell mkdir -p ${BUILD}/kernel/chr_drv)
	gcc ${CFLAGS} ${DEBUG} -c $< -o $@

${BUILD}/kernel/asm/%.o: x86/kernel/asm/%.asm
	$(shell mkdir -p ${BUILD}/kernel/asm)
	nasm -f elf32 -g $< -o $@

#通过asm文件生成.o文件
${BUILD}/boot/%.o: x86/boot/%.asm
	$(shell mkdir -p ${BUILD}/boot)
	nasm $< -o $@

clean:
	$(shell rm -rf ${BUILD})

bochs: all
	bochs -q -f bochsrc

qemu: all
	qemu-system-x86_64 -hda ./build/hd.img

qemug: all
	qemu-system-x86_64 -hda ./build/hd.img 	-s -S