objs=start.o main.o	

ledc.bin:$(objs)
	arm-linux-gnueabihf-ld -Timx6ull.lds $^ -o ledc.elf 			#链接
	arm-linux-gnueabihf-objcopy -O binary -S ledc.elf $@		#二进制文件
	arm-linux-gnueabihf-objdump -D -m arm ledc.elf > ledc.dis	#反汇编


%.o:%.c
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -o $@ $<

%.o:%.s
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -o $@ $<

clean: 
	rm -rf *.o ledc.bin ledc.elf ledc.dis