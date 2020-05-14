# Board_Driver
## 新建仓库流程
echo "# Board_Driver" >> README.md<br>
git ini<br>
git add README.md<br>
git commit -m "first commit"<br>
git remote add origin https://github.com/ErisedF/Board_Driver.git<br>
git push -u origin master<br>

## c/c++:ED ；打开scode编辑器 ，添加头文件路经

## 3_ledc_stm32_style
添加了bss段清除程序；bss段为定义为使用的变量区<br>

##4_ledc_mdk
使用NXP提供的mdk文件移植


## 5_ledc_bsp
* 'Makefile改成了通用形式'，BSP规范进行文档管理，改TARGET名字即可<br>
* 注：start.o main.o 在链接脚本里需改成对应文件路径，否则找不到<br>

## 6_beep
* 注：烧文件写要方对路径  /dev/sdb

## 7_key
* 1.设置UART1_CTS复用为GPIO1_IO18
* 2.设置UART1_CTS的电气属性。
* 3.配置GPIO1_IO08为输入模式。
* 4.读取按键值，也就是GPIO1_IO08的高低电平
### 注：启动文件把bss区放到了 _start后可以运行，而教材中是直接删除

## 8_clk 
主频为528MHz<br>
### 1.配置主频
* 1.判断当前始终设置状态,并设置为step_clk，step_clk为修改主频时的临时时钟
* 2.设置主屏时钟PLL1=1056MHz
* 3.设置 PLL1分频 系数为2
* 4.切换主频到 pll1_main_clk*
### 2.设置其他PLL的PFD PLL2 PLL3 PLLx_PFDx：12～35
* PLL2_PFDn
		* PLL2_PFD3=528*18/ <32> =297Mhz 
		* PLL2_PFD2=528*18/ <24> =396Mhz
		* PLL2_PFD1=528*18/ <16> =594Mhz 
		* PLL2_PFD0=528*18/ <27> =352Mhz
* PLL3_PFDn
		* PLL3_PFD3=480*18/ <19> =454.74Mhz
		* PLL3_PFD2=480*18/ <17> =508.24Mhz
		* PLL3_PFD1=480*18/ <16> =540Mhz
		* PLL3_PFD0=480*18/ <12> =720Mhz
	
### 3.设置其他其他外设时钟源
* 寄存器名称[需要设置位]->值
* 1.AHB_CLK_ROOT    	132MHz
		*CBCMR[PRE_PERIPH_CLK_SEL]  -> 01 derive clock from PLL2 PFD2
		*CBCDR[PERIPH_CLK_SEL]  -> 0 PLL2 (pll2_main_clk)
		*CBCDR[AHB_PODF] -> CBCDR[AHB_PODF]
* 2.IPG_CLK_ROOT   		66MHz
		*CBCDR[IPG_PODF] -> 01 divide by 2
* 3.PERCLK_CLK_ROOT		66MHz
		*CSCMR1[PERCLK_PODF] -> 000000 divide by 1
		*CSCMR1[PERCLK_CLK_SEL] ->0 derive clock from ipg clk root
### 注：配置时看是否需要`忙等待握手信号`

		
		
		
