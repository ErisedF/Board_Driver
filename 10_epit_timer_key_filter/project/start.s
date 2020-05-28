.global _start


_start:

/*
 * 中断向量表创建
 * 参考文档:ARM Cortex-A(armV7)编程手册V4.0.pdf P42，3 ARM Processor Modes and Registers（ARM处理器模型和寄存器）
 * 		 	ARM Cortex-A(armV7)编程手册V4.0.pdf P165 11.1.1 Exception priorities(异常)
 */
	ldr pc, =Reset_Handler		/* 复位中断 				*/	
	ldr pc, =Undefined_Handler	/* 未定义指令中断 				*/
	ldr pc, =SVC_Handler		/* SVC(Supervisor)中断 	   */
	ldr pc, =PrefAbort_Handler	/* 预取终止中断 			 */
	ldr pc, =DataAbort_Handler	/* 数据终止中断 			 */
	ldr	pc, =NotUsed_Handler	/* 未使用中断			   	 */
	ldr pc, =IRQ_Handler		/* IRQ中断 	                */
	ldr pc, =FIQ_Handler		/* FIQ(快速中断)未定义中断     */




/*复位中断服务函数 */
Reset_Handler:
    
    cpsid i/*关闭全局中断 禁止IRQ */

    /*  1  关闭I Cache ，DCache 、MMU
    读改写CP15协处理器
    格式：MCR{cond} p15, <opc1>, <Rt>, <CRn>, <CRm>, <opc2>
    位清除：BIC Rd, Rn, #immed      Rd = Rn & (~#immed)
    */
	mrc     p15, 0, r0, c1, c0, 0     // 读取CP15的C1寄存器到R0中
    bic     r0,  r0, #(0x1 << 12)     // 清除C1寄存器的bit12位(I位)，关闭I Cache
    bic     r0,  r0, #(0x1 << 11)     // 清除C1寄存器的bit11(Z位)，关闭分支预测
    bic     r0,  r0, #(0x1 <<  2)     // 清除C1寄存器的bit2(C位)，关闭D Cache
    bic     r0,  r0, #(0x1 <<  1)     // 清除C1寄存器的bit1(A位)，关闭对齐
    bic     r0,  r0, #(0x1 <<  0)     // 清除C1寄存器的bit0(M位)，关闭MMU	
    mcr     p15, 0, r0, c1, c0, 0     // 将r0寄存器中的值写入到CP15的C1寄存器中   


    /*   2     设置中断向量偏移量 CP15 C12
    DMB\DSB\ISB:https://blog.csdn.net/guojing3625/article/details/16877639
    DMB:数据存储器隔离。DMB 指令保证： 仅当所有在它前面的存储器访问操作
        都执行完毕后，才提交(commit)在它后面的存储器访问操作。
    DSB:数据同步隔离。比 DMB 严格： 仅当所有在它前面的存储器访问操作
        都执行完毕后，才执行在它后面的指令（亦即任何指令都要等待存储器访 问操作——译者注）
    ISB:指令同步隔离。最严格：它会清洗流水线，以保证所有它前面的指令都执
        行完毕之后，才执行它后面的指令。
     */
@ #if 0
@     ldr     r0 , =0X87800000
@     dsb     //数据同步指令
@     isb     //指令同步指令 
@     mcr     p15, 0, r0, c12, c0, 0     // 将r0寄存器中的值写入到CP15的C12寄存器中      VBAR, Vector Base Address Register †
@     dsb 
@     isb
@ #endif

    /* IRQ模式 */
    /*第一步 设置处理器进入IRQ模式 */
    mrs r0 , cpsr   /*读取cpsr到r0 */
    bic r0 , r0 , #0x1f /* 按位清除cpsr bit0-4*/
    orr r0 , r0 ,#0x12  /*按位或 赋值10010 IRQ模式 */
    msr cpsr , r0   /*赋值到cpsr */
    /*第二步 设置sp指针 
    *外部ddr地址范围0x8000 0000 - 0xa000 0000
    *堆栈大小2mbit 0x8060 0000
    *堆栈向下生长
    */
    ldr sp , = 0X80600000

    /* SYS模式 */
    /*第一步 设置处理器进入SYS模式 */
    mrs r0 , cpsr   /*读取cpsr到r0 */
    bic r0 , r0 , #0x1f /* 按位清除cpsr bit0-4*/
    orr r0 , r0 ,#0x1f  /*按位或 赋值10010 SYS模式 */
    msr cpsr , r0   /*赋值到cpsr */
    /*第二步 设置sp指针 
    *外部ddr地址范围0x8000 0000 - 0xa000 0000
    *堆栈大小2mbit 0x8040 0000
    *堆栈向下生长
    */
    ldr sp , = 0X80400000



    /* SVC模式 */
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
    ldr sp , = 0X80200000

    /*使能IRQ */
    cpsie i
    b   main //进入main函数


/*未定义指令中断服务函数 */
Undefined_Handler:
    ldr r0, =Undefined_Handler      
    bx  r0    /*间接跳转 */

/*SVC中断服务函数 */
SVC_Handler:
    ldr r0, =SVC_Handler      
    bx  r0    /*间接跳转 */

/*预取终止中断服务函数 */
PrefAbort_Handler:
    ldr r0, =PrefAbort_Handler      
    bx  r0    /*间接跳转 */

/*数据终止中断服务函数 */
DataAbort_Handler:
    ldr r0, =DataAbort_Handler      
    bx  r0    /*间接跳转 */

/*未使用中断服务函数 */
NotUsed_Handler:
    ldr r0, =NotUsed_Handler      
    bx  r0    /*间接跳转 */




/*IRQ中断服务函数  所有中断函数进这里*/
IRQ_Handler:
    /*保护现场 
    *r0：保存GIC的CPU接口端基地址加0X0C  GICC_IAR寄存器
    *r1：保存GIC的CPU接口端基地址
    *r2：system_irqhandler函数地址
    */
    push    {lr}                   /*保存当前程序运行地址 R14(LR)来存放当前子程序的返回地址*/
    push    {r0-r3, r12}           /*保存r0-r3，r13寄存器的数据数据 */

    mrs     r0, spsr                /*读取spsr寄存器 程序状态保存寄存器 SPSR用于度保存CPSR的状态*/
    push    {r0}                    /*保存 */

    /*访问GIC控制器 */
	mrc p15, 4, r1, c15, c0, 0 /* 从CP15的C0寄存器内的值到R1寄存器中 GIC控制器基地址到r1
                                *读取CBAR寄存器 Cortex-A7 Technical ReferenceManua.pdf P68 P138  Configuration Base Address Register on page 4-83
                                *   保存了GIC控制器的基地址
								*   参考文档ARM Cortex-A(armV7)编程手册V4.0.pdf P49
                                *0x1000-0x1FFF Distributor
                                *0x2000-0x3FFF CPU interface
								*/	
    /*0x2000-0x3FFF CPU interface */ 
    /*0x000C        GICC_IAR */                          
	add r1, r1, #0X2000			/* GIC基地址加0X2000，也就是GIC的CPU接口端基地址 */
	ldr r0, [r1, #0XC]			/* GIC的CPU接口端基地址加0X0C就是GICC_IAR寄存器，
								 * GICC_IAR寄存器保存这当前发生中断的中断号，我们要根据
								 * 这个中断号来绝对调用哪个中断服务函数
								 */
	push {r0, r1}				/* 保存r0,r1 */

	cps #0x13					/* 进入SVC模式，三允许其他中断再次进去 */    

	push {lr}					/* 保存SVC模式的lr寄存器 */
	ldr r2, =system_irqhandler	/* 加载C语言中断处理函数到r2寄存器中*/
	blx r2						/* 运行C语言中断处理函数，带有一个参数，为返回地址保存在R0寄存器中 自动返回r0 r1 r2 */                                     
                            			
	pop {lr}					/* 执行完C语言中断服务函数，lr出栈 */
	cps #0x12					/* 进入IRQ模式 */
	pop {r0, r1}				
	str r0, [r1, #0X10]			/* 中断执行完成，写EOIR r0写如r1+0x10  0x0010 GICC_EOIR*/

    pop {r0}						
	msr spsr_cxsf, r0			/* 恢复spsr */

	pop {r0-r3, r12}			/* r0-r3,r12出栈 */
	pop {lr}					/* lr出栈 */
	subs pc, lr, #4				/* 将lr-4赋给pc
                                 * 三级流水线 取址 译址 执行
                                 */


/*FIQ(快速中断)未定义中断 服务函数 */
FIQ_Handler:
    ldr r0, =FIQ_Handler      
    bx  r0    /*间接跳转 */



.global _bss_start
.global _bss_end

_bss_start:
    .word __bss_start        //.word放置一个变量 _bss_start 变量名 bss开始地址 _bss_start = __bss_start
_bss_end:
    .word __bss_end         //bss结束地址

     /*清除BSS段代码 */
    ldr r0 , = _bss_start
    ldr r1 , = _bss_end
    mov r2 , #0
    bss_loop:
    stmia r0!, {r2}      //往r0写r2 !加1
    cmp r0 , r1     //比较r0 r1
    ble bss_loop    //b跳转 le小于等于 如果r0<=r1 跳转到bss_loop
