.global _start
.global _bss_start
.global _bss_end


_bss_start:
    .word __bss_start            //.word放置一个变量 _bss_start 变量名 bss开始地址 _bss_start = __bss_start
_bss_end:
    .word __bss_end         //bss结束地址

_start:
    /*第一步 设置处理器进入SVC模式 */
    mrs r0 , cpsr   /*读取cpsr到r0 */
    bic r0 , r0 , #0x1f /* 按位清除cpsr bit0-4*/
    orr r0 , r0 ,#0x13  /*按位或 赋值10010 SVC模式 */
    msr cpsr , r0   /*赋值到cpsr */

    /*清除BSS段 */
    ldr r0 , = _bss_start
    ldr r1 , = _bss_end
    mov r2 , #0

bss_loop:
    stmia r0!, {r2}      //往r0写r2 !加1
    cmp r0 , r1     //比较r0 r1
    ble bss_loop    //b跳转 le小于等于 如果r0<=r1 跳转到bss_loop


    /*第二步 设置sp指针 
    *外部ddr地址范围0x8000 0000 - 0xa000 0000
    *堆栈大小2mbit 0x8020 0000
    *堆栈向下生长
    */
    ldr sp , = 0x80200000

    /*第三步 跳转到main函数 */
    b main
    