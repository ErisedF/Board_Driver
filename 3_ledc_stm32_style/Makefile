objs 	:= start.o main.o           
gcc 	:= arm-linux-gnueabihf-gcc			#编译
ld 		:= arm-linux-gnueabihf-ld 			#链接
objcopy := arm-linux-gnueabihf-objcopy		#格式转换为bin
objdump := arm-linux-gnueabihf-objdump		#bin反汇编

ledc.bin : $(objs)
	$(ld) -Timx6ull.lds $^ -o ledc.elf
	$(objcopy) -O binary -S ledc.elf $@
	$(objdump) -D -m arm ledc.elf > ledc.dis	#反汇编

%.o:%.c 
	$(gcc) -Wall -nostdlib -c $< -o $@
%.o:%.s
	$(gcc) -Wall -nostdlib -c $< -o $@

clean:
	rm -rf *.o ledc.bin ledc.elf ledc.dis