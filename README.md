# Board_Driver
## 新建仓库流程
echo "# Board_Driver" >> README.md<br>
git ini<br>
git add README.md<br>
git commit -m "first commit"<br>
git remote add origin https://github.com/ErisedF/Board_Driver.git<br>
git push -u origin master<br>

## vscode 添加头文件路经
* vscode中输入：`c/c++:ED`；打开scode编辑器 ，添加头文件路经

## 3_ledc_stm32_style
添加了bss段清除程序；bss段为定义为使用的变量区<br>

##4_ledc_mdk
使用NXP提供的mdk文件移植


## 5_ledc_bsp
`Makefile改成了通用形式`，BSP规范进行文档管理，改TARGET名字即可<br>
* 注：start.o main.o 在链接脚本里需改成对应文件路径，否则找不到<br>

## 6_beep
* 注：烧文件写要方对路径  `/dev/sdb`

## 7_key
* 1、设置UART1_CTS复用为GPIO1_IO18
* 2、设置UART1_CTS的电气属性。
* 3、配置GPIO1_IO08为输入模式。
* 4、读取按键值，也就是GPIO1_IO08的高低电平
* 注： 启动文件把_bss区放到了_start后可以运行，而教材中是直接删除
## 7_key_v1
* 在分支7_key_v1上，增加控制`GPIO的通用函数`，led，beep，key相应改为用库控制

