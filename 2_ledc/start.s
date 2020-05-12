.global _start

_start:
    /*第一步 设置处理器进入SVC模式 */
    mrs r0 , cpsr   /*读取cpsr到r0 */
    bic r0 , r0 , #0x1f /* 按位清除cpsr bit0-4*/
    orr r0 , r0 ,#0x13  /*按位或 赋值10010 SVC模式 */
    msr cpsr , r0   /*赋值到cpsr */

    /*第二步 设置sp指针 
    *外部ddr地址范围0x8000 0000 - 0xa000 0000
    *堆栈大小2mbit 0x8020 0000
    *堆栈向下生长
    */
    ldr sp , = 0x80200000

    /*第三步 跳转到main函数 */
    b main

