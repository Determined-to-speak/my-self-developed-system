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
all: ${BUILD}/boot/boot.o ${BUILD}/boot/setup.o ${BUILD}/system.bin	${BUILD}/x86_64/system.bin
	$(shell rm -rf $(BUILD)/$(HD_IMG_NAME))
	bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $(BUILD)/$(HD_IMG_NAME)	#通过bochs模拟一块硬盘
	dd if=${BUILD}/boot/boot.o of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=0 count=1 conv=notrunc	#通过dd命令，将.o文件写入到硬盘当中
	dd if=${BUILD}/boot/setup.o of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=1 count=2 conv=notrunc
	dd if=${BUILD}/system.bin of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=3 count=30 conv=notrunc
	dd if=${BUILD}/x86_64/system.bin of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=41 count=5000 conv=notrunc

# x86_64内核使用
${BUILD}/x86_64/system.bin: ${BUILD}/x86_64/kernel.bin
	objcopy -O binary ${BUILD}/x86_64/kernel.bin ${BUILD}/x86_64/system.bin
	nm ${BUILD}/x86_64/kernel.bin | sort > ${BUILD}/x86_64/system.map

${BUILD}/x86_64/kernel.bin: ${BUILD}/x86_64/boot/head.o \
	 ${BUILD}/x86_64/init/main.o \
	 ${BUILD}/x86_64/kernel/console.o ${BUILD}/x86_64/kernel/printk.o ${BUILD}/x86_64/kernel/vsprintf.o ${BUILD}/x86_64/kernel/idt.o \
	 ${BUILD}/x86_64/kernel/keyboard.o \
	 ${BUILD}/x86_64/kernel/asm/myio.o  ${BUILD}/x86_64/kernel/asm/interrupt_handler.o \
	 ${BUILD}/x86_64/kernel/mm/memory.o ${BUILD}/x86_64/kernel/mm/bitmap.o ${BUILD}/x86_64/kernel/mm/malloc.o \
 	 ${BUILD}/x86_64/lib/string.o
	ld -b elf64-x86-64 -o $@ $^ -Ttext 0x100000

${BUILD}/x86_64/boot/%.o: x86_64/boot/%.asm
	$(shell mkdir -p ${BUILD}/x86_64/boot)
	nasm -f elf64 ${DEBUG} $< -o $@

${BUILD}/x86_64/init/%.o: x86_64/init/%.c
	$(shell mkdir -p ${BUILD}/x86_64/init)
	gcc ${DEBUG} ${CFLAGS64} -c $< -o $@

${BUILD}/x86_64/lib/%.o: x86_64/lib/%.c
	$(shell mkdir -p ${BUILD}/x86_64/lib)
	gcc ${DEBUG} ${CFLAGS64} -c $< -o $@

${BUILD}/x86_64/kernel/asm/%.o: x86_64/kernel/asm/%.asm
	$(shell mkdir -p ${BUILD}/x86_64/kernel/asm)
	nasm -f elf64 ${DEBUG} $< -o $@

${BUILD}/x86_64/kernel/mm/%.o: x86_64/kernel/mm/%.c
	$(shell mkdir -p ${BUILD}/x86_64/kernel/mm)
	gcc ${DEBUG} ${CFLAGS64} -c $< -o $@

${BUILD}/x86_64/kernel/%.o: x86_64/kernel/%.c
	$(shell mkdir -p ${BUILD}/x86_64/kernel)
	gcc ${DEBUG} ${CFLAGS64} -c $< -o $@

# x86内核使用
${BUILD}/system.bin: ${BUILD}/kernel.bin
	objcopy -O binary ${BUILD}/kernel.bin ${BUILD}/system.bin
	nm ${BUILD}/kernel.bin | sort > ${BUILD}/system.map

${BUILD}/kernel.bin: ${BUILD}/boot/head.o ${BUILD}/init/main.o ${BUILD}/init/cpu.o ${BUILD}/lib/string.o ${BUILD}/kernel/chr_drv/console.o \
	${BUILD}/kernel/asm/myio.o  ${BUILD}/kernel/asm/cpu_correlation.o
	ld -m elf_i386 $^ -o $@ -Ttext 0x1200	#这里暂时理解为将 x86 kernel.bin 文件生成在0x1200的位置

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

qemug: all
	qemu-system-x86_64 \
	-m 4G \
	-boot c \
	-cpu Haswell -smp cores=1,threads=2 \
	-hda ./build/hd.img \
	-s -S

qemu: all
	qemu-system-x86_64 \
	-m 4G \
	-boot c \
	-cpu Haswell -smp cores=1,threads=2 \
	-hda ./build/hd.img

# 生成的内核镜像给VBox、VMware用
vmdk: $(BUILD)/master.vmdk

$(BUILD)/master.vmdk: ./build/hd.img
	qemu-img convert -O vmdk $< $@