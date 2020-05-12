.global _start      @全局标号

_start:
    /*使能所有时钟 */
    ldr r0, = 0x020c4068        @CCM-CCGR0
    ldr r1, = 0xffffffff        @向CCM-CCGR0写入的值
    str r1, [r0]                @把0xffffffff写如r0中

    ldr r0, = 0x020c406c        @CCM-CCGR1
    str r1, [r0]                @把0xffffffff写如r0中

    ldr r0, = 0x020c4070        @CCM-CCGR2
    str r1, [r0]                @把0xffffffff写如r0中

    ldr r0, = 0x020c4074        @CCM-CCGR3
    str r1, [r0]                @把0xffffffff写如r0中

    ldr r0, = 0x020c4078        @CCM-CCGR4
    str r1, [r0]                @把0xffffffff写如r0中

    ldr r0, = 0x020c407c        @CCM-CCGR5
    str r1, [r0]                @把0xffffffff写如r0中

    ldr r0, = 0x020c4080        @CCM-CCGR6
    str r1, [r0]                @把0xffffffff写如r0中


    /*设置IO复用功能 GPIO_03*/
    ldr r0, = 0x020e0068        @SW_MUX_CTL_Register
    ldr r1, = 0x5               @向CCM-CCGR0写入的值
    str r1, [r0]                @把0x5写如r0中

    /*配置电气属性
    *bit    0      ： 0    低压摆率
    *bit    5-3    ： 110  驱动能力 r0/6
    *bit    7-6    ： 10   100mhz
    *bit    11     ： 0    不是开漏输出
    *bit    12     ： 1     是能上拉保持
    *bit    13     ： 0     保持器  
    *bit    14-15  ：10     100k上拉电阻
    *bit    16     ：0      不使用迟滞比较器
    */
    ldr r0, = 0x020e02f4        @IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03
    ldr r1, = 0x10b0
    str r1, [r0]                @把0x10b0写如r0中

    /*设置gpio输出模式  GPIO1_GDIR bit3 为 1*/
    ldr r0, = 0x0209c004           @GPIO1_GDIR
    ldr r1, = 0x08
    str r1, [r0] 

    /*LED0  @GPIO1_GD*/
    ldr r0, = 0x0209c000
    ldr r1, = 0x0               @低电平
    str r1, [r0]   


    /*死循环 */             
loop:
    b loop      @b 跳转


