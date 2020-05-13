CROSS_COMPILE ?= arm-linux-gnueabihf-
TARGET		  ?= key_bsp

CC			  := $(CROSS_COMPILE)gcc
LD			  := $(CROSS_COMPILE)ld 
OBJCOPY 	  := $(CROSS_COMPILE)objcopy
OBJDUMP		  := $(CROSS_COMPILE)objdump 

#头文件路径 %.h
INCUDIRS 	  := imx6ull \
				 bsp/led \
				 bsp/clk \
				 bsp/delay \
				 bsp/beep \
				 bsp/key

				 

#源码路径 %.c %.s
SRCDIRS 	   := project \
				  bsp/led \
				  bsp/clk \
				  bsp/delay \
				  bsp/beep \
				  bsp/key

#Makefile 指定头文件时需要 -I
#$(patsubst <pattern>,<replacement>,<text>) ,搜索<text>，若符合<pattern>，则用<replacement>替换
#在 路径前加 -I
INCLUDE 	   := $(patsubst %, -I %, $(INCUDIRS))



#$(foreach <var>,<list>,<text>) 把参数<list>中的单词逐一取出放到参数<var>所指定的变量中，然后再执行<text>所包含的表达式
#objects := $(wildcard *.o) 通配符在变量中展开 是让 objects 的值是所有[.o]的文件名的集合
#输出是带路径的
SFILES 	   	   := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.s)) 		#把dir下所有的s文件展开 赋值SFIE中
CFILES 	   	   := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c)) 		#把dir下所有的c文件展开 赋值CFIE中

#$(notdir <names...>)  ：从文件名序列<names>中取出非目录部分。非目录部分是指后一个反斜杠（“ /”） 之后的部分
SFILENDIR	   := $(notdir $(SFILES))		#去掉路径
CFILENDIR	   := $(notdir $(CFILES))		#去掉路径

#把%.c %.s 的文名字改为%.o 原文件名字不变 
SOBJS 	   	   := $(patsubst %, obj/%, $(SFILENDIR:.s=.o))
COBJS 	   	   := $(patsubst %, obj/%, $(CFILENDIR:.c=.o))
OBJS 		   := $(SOBJS) $(COBJS) #%.o 文件

#VPATH = src:../headers  文件搜索 如果定义了这个变量，那么，make 就会在当当前目录找不到的情况下，到所指定的目录中去找寻文件了
VPATH		   := $(SRCDIRS)

$(TARGET).bin  : $(OBJS)
	$(LD) -Timx6ull.lds $^ -o $(TARGET).elf
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

#静态模式
#<targets ...>: <target-pattern>: <prereq-patterns ...> 
#<commands>  ....   
#<target-pattern>指定targets的模式 <prereq-patterns ...> 对<target-pattern>的模式再进行一次依赖目标定义
$(SOBJS)       : obj/%.o: %.s  #把%，s 编译为%.o文件 并在生成的%.o文件前加路径 obj/ 模式修饰
	$(CC) -Wall -nostdlib -c $(INCLUDE) $< -o $@
$(COBJS)       : obj/%.o: %.c  #把%，c 编译为%.o文件 并在生成的%.o文件前加路径 obj/ 模式修饰
	$(CC) -Wall -nostdlib -c $(INCLUDE) $< -o $@


print:
	@echo INCLUDE 	 = $(INCLUDE)
	@echo SFILES  	 = $(SFILES)
	@echo CFILES  	 = $(CFILES)
	@echo SFILENDIR  = $(SFILENDIR)
	@echo CFILENDIR  = $(CFILENDIR)	
	@echo SOBJS  	 = $(SOBJS)
	@echo COBJS  	 = $(COBJS)
	@echo OBJS  	 = $(OBJS)

#.PHONY 声明伪目标
.PHONY:clean
clean:
	rm -rf $(TARGET).bin $(TARGET).elf $(TARGET).dis $(OBJS)