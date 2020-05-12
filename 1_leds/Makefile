led.bin:led.s
	arm-linux-gnueabihf-gcc -g -c led.s -o led.o				#中间文件
	arm-linux-gnueabihf-ld -Ttext 0x87800000 led.o -o led.elf	#汇编文件
	arm-linux-gnueabihf-objcopy -O binary -g -S led.elf led.bin	#二进制文件
	arm-linux-gnueabihf-objdump -D led.elf > led.dis			#反汇编文件
clean:
	rm -rf *.o led.bin led.elf led.dis