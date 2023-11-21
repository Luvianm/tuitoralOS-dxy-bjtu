GPPPARAMS = -m32 -I include -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASMPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/kernel.o \
		  obj/loader.o \
		  obj/gdt.o \
		  obj/port.o \
		  obj/interrupts.o \
		  obj/interruptstubs.o \
		  obj/keyboard.o \
		  obj/driver.o \
		  obj/mouse.o \
		  obj/multitasking.o

obj/%.o: src/%.cpp
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	as $(ASMPARAMS) -o $@ $<

mykernel.bin: link.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '    boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: mykernel.iso
	virtualboxvm --startvm "mykernel" &

.phony: clean
clean: 
	rm -rf $(objects) mykernel.bin mykernel.iso
	rm -rf iso
