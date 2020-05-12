CROSS_COMPILE ?= arm-linux-gnueabihf-
NAME		  ?= ledc

OBJS 		  := start.o main.o

CC			  := $(CROSS_COMPILE)gcc
LD			  := $(CROSS_COMPILE)ld 
OBJCOPY 	  := $(CROSS_COMPILE)objcopy
OBJDUMP		  := $(CROSS_COMPILE)objdump 

$(NAME).bin   : $(OBJS)
	$(LD) -Timx6ull.lds $^ -o $(NAME).elf	#链接
	$(OBJCOPY) -O binary -S $(NAME).elf $@ #生成bin文件
	$(OBJDUMP) -D -m arm $(NAME).elf > $(NAME).dis #反汇编

%.o:%.c
	$(CC) -Wall -nostdlib -c $< -o $@
%.o:%.s
	$(CC) -Wall -nostdlib -c $< -o $@

clean:
	rm -rf *.o $(NAME).bin $(NAME).elf $(NAME).dis