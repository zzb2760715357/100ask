### 按键驱动---查询方式获取按键值
![按键驱动模型](https://i.imgur.com/rz4kNj7.png)

### 按键驱动---中断方式获取按键值

![](https://i.imgur.com/o3Jq3J9.png)
	

	memcpy((void *)vectors, __vectors_start, __vectors_end - __vectors_start);
	memcpy((void *)vectors + 0x200, __stubs_start, __stubs_end - __stubs_start);

----
	.equ	stubs_offset, __vectors_start + 0x200 - __stubs_start

	.globl	__vectors_start
	__vectors_start:
		swi	SYS_ERROR0
		b	vector_und + stubs_offset
		ldr	pc, .LCvswi + stubs_offset
		b	vector_pabt + stubs_offset
		b	vector_dabt + stubs_offset
		b	vector_addrexcptn + stubs_offset
		b	vector_irq + stubs_offset
		b	vector_fiq + stubs_offset
	
		.globl	__vectors_end
	__vectors_end:

----

	vector_stub	und, UND_MODE

	.long	__und_usr			@  0 (USR_26 / USR_32)
	.long	__und_invalid			@  1 (FIQ_26 / FIQ_32)
	.long	__und_invalid			@  2 (IRQ_26 / IRQ_32)
	.long	__und_svc			@  3 (SVC_26 / SVC_32)
	.long	__und_invalid			@  4
	.long	__und_invalid			@  5
	.long	__und_invalid			@  6
	.long	__und_invalid			@  7
	.long	__und_invalid			@  8
	.long	__und_invalid			@  9
	.long	__und_invalid			@  a
	.long	__und_invalid			@  b
	.long	__und_invalid			@  c
	.long	__und_invalid			@  d
	.long	__und_invalid			@  e
	.long	__und_invalid			@  f

	.align	5

---

	.macro	vector_stub, name, mode, correction=0
	.align	5

	vector_\name:
	.if \correction
	sub	lr, lr, #\correction
	.endif

	@
	@ Save r0, lr_<exception> (parent PC) and spsr_<exception>
	@ (parent CPSR)
	@
	stmia	sp, {r0, lr}		@ save r0, lr
	mrs	lr, spsr
	str	lr, [sp, #8]		@ save spsr

	@
	@ Prepare for SVC32 mode.  IRQs remain disabled.
	@
	mrs	r0, cpsr
	eor	r0, r0, #(\mode ^ SVC_MODE)
	msr	spsr_cxsf, r0

	@
	@ the branch table must immediately follow this code
	@
	and	lr, lr, #0x0f
	mov	r0, sp
	ldr	lr, [pc, lr, lsl #2]
	movs	pc, lr			@ branch to handler in SVC mode
	.endm

---

### 设置中断的异常向量表
	__vectors_start 0xffff0000

	memcpy((void *)vectors, __vectors_start, __vectors_end - __vectors_start);
	memcpy((void *)vectors + 0x200, __stubs_start, __stubs_end - __stubs_start);
	memcpy((void *)vectors + 0x1000 - kuser_sz, __kuser_helper_start, kuser_sz);

	.equ	stubs_offset, __vectors_start + 0x200 - __stubs_start

	.globl	__vectors_start
	__vectors_start:
		swi	SYS_ERROR0
		b	vector_und + stubs_offset
		ldr	pc, .LCvswi + stubs_offset
		b	vector_pabt + stubs_offset
		b	vector_dabt + stubs_offset
		b	vector_addrexcptn + stubs_offset
		b	vector_irq + stubs_offset
		b	vector_fiq + stubs_offset
	
		.globl	__vectors_end
	__vectors_end:

		.globl	__stubs_start
	__stubs_start:
	/*
	 * Interrupt dispatcher
	 */
	vector_stub	irq, IRQ_MODE, 4

	.long	__irq_usr			@  0  (USR_26 / USR_32)
	.long	__irq_invalid			@  1  (FIQ_26 / FIQ_32)
	.long	__irq_invalid			@  2  (IRQ_26 / IRQ_32)
	.long	__irq_svc			@  3  (SVC_26 / SVC_32)
	.long	__irq_invalid			@  4
	.long	__irq_invalid			@  5
	.long	__irq_invalid			@  6
	.long	__irq_invalid			@  7
	.long	__irq_invalid			@  8
	.long	__irq_invalid			@  9
	.long	__irq_invalid			@  a
	.long	__irq_invalid			@  b
	.long	__irq_invalid			@  c
	.long	__irq_invalid			@  d
	.long	__irq_invalid			@  e
	.long	__irq_invalid			@  f
	
	/*
	 * Data abort dispatcher
	 * Enter in ABT mode, spsr = USR CPSR, lr = USR PC
	 */
	vector_stub	dabt, ABT_MODE, 8

	.long	__dabt_usr			@  0  (USR_26 / USR_32)
	.long	__dabt_invalid			@  1  (FIQ_26 / FIQ_32)
	.long	__dabt_invalid			@  2  (IRQ_26 / IRQ_32)
	.long	__dabt_svc			@  3  (SVC_26 / SVC_32)
	.long	__dabt_invalid			@  4
	.long	__dabt_invalid			@  5
	.long	__dabt_invalid			@  6
	.long	__dabt_invalid			@  7
	.long	__dabt_invalid			@  8
	.long	__dabt_invalid			@  9
	.long	__dabt_invalid			@  a
	.long	__dabt_invalid			@  b
	.long	__dabt_invalid			@  c
	.long	__dabt_invalid			@  d
	.long	__dabt_invalid			@  e
	.long	__dabt_invalid			@  f

	/*
	 * Prefetch abort dispatcher
	 * Enter in ABT mode, spsr = USR CPSR, lr = USR PC
	 */
	vector_stub	pabt, ABT_MODE, 4

	.long	__pabt_usr			@  0 (USR_26 / USR_32)
	.long	__pabt_invalid			@  1 (FIQ_26 / FIQ_32)
	.long	__pabt_invalid			@  2 (IRQ_26 / IRQ_32)
	.long	__pabt_svc			@  3 (SVC_26 / SVC_32)
	.long	__pabt_invalid			@  4
	.long	__pabt_invalid			@  5
	.long	__pabt_invalid			@  6
	.long	__pabt_invalid			@  7
	.long	__pabt_invalid			@  8
	.long	__pabt_invalid			@  9
	.long	__pabt_invalid			@  a
	.long	__pabt_invalid			@  b
	.long	__pabt_invalid			@  c
	.long	__pabt_invalid			@  d
	.long	__pabt_invalid			@  e
	.long	__pabt_invalid			@  f
	
	/*
	 * Undef instr entry dispatcher
	 * Enter in UND mode, spsr = SVC/USR CPSR, lr = SVC/USR PC
	 */
	vector_stub	und, UND_MODE

	.long	__und_usr			@  0 (USR_26 / USR_32)
	.long	__und_invalid			@  1 (FIQ_26 / FIQ_32)
	.long	__und_invalid			@  2 (IRQ_26 / IRQ_32)
	.long	__und_svc			@  3 (SVC_26 / SVC_32)
	.long	__und_invalid			@  4
	.long	__und_invalid			@  5
	.long	__und_invalid			@  6
	.long	__und_invalid			@  7
	.long	__und_invalid			@  8
	.long	__und_invalid			@  9
	.long	__und_invalid			@  a
	.long	__und_invalid			@  b
	.long	__und_invalid			@  c
	.long	__und_invalid			@  d
	.long	__und_invalid			@  e
	.long	__und_invalid			@  f

	.align	5


### 中断异常向量的展开
	b	vector_irq + stubs_offset	

	vector_stub	irq, IRQ_MODE, 4

	.macro	vector_stub, name, mode, correction=0
	.align	5

	vector_irq:
	sub	lr, lr, #4   @ 计算返回地址

	@
	@ Save r0, lr_<exception> (parent PC) and spsr_<exception>
	@ (parent CPSR)
	@
	stmia	sp, {r0, lr}		@ save r0, lr
	mrs	lr, spsr
	str	lr, [sp, #8]		@ save spsr

	@
	@ Prepare for SVC32 mode.  IRQs remain disabled.
	@
	mrs	r0, cpsr
	eor	r0, r0, #(\mode ^ SVC_MODE)
	msr	spsr_cxsf, r0

	@
	@ the branch table must immediately follow this code
	@
	and	lr, lr, #0x0f
	mov	r0, sp
	ldr	lr, [pc, lr, lsl #2]
	movs	pc, lr			@ branch to handler in SVC mode
	.endm


	.long	__irq_usr			@  0  (USR_26 / USR_32)
	.long	__irq_invalid			@  1  (FIQ_26 / FIQ_32)
	.long	__irq_invalid			@  2  (IRQ_26 / IRQ_32)
	.long	__irq_svc			@  3  (SVC_26 / SVC_32)
	.long	__irq_invalid			@  4
	.long	__irq_invalid			@  5
	.long	__irq_invalid			@  6
	.long	__irq_invalid			@  7
	.long	__irq_invalid			@  8
	.long	__irq_invalid			@  9
	.long	__irq_invalid			@  a
	.long	__irq_invalid			@  b
	.long	__irq_invalid			@  c
	.long	__irq_invalid			@  d
	.long	__irq_invalid			@  e
	.long	__irq_invalid			@  f



	__irq_usr:
	usr_entry    @ 保存现场

	#ifdef CONFIG_TRACE_IRQFLAGS
		bl	trace_hardirqs_off
	#endif
		get_thread_info tsk
	#ifdef CONFIG_PREEMPT
		ldr	r8, [tsk, #TI_PREEMPT]		@ get preempt count
		add	r7, r8, #1			@ increment it
		str	r7, [tsk, #TI_PREEMPT]
	#endif
	
		irq_handler


	.macro	irq_handler
	get_irqnr_preamble r5, lr
	1:	get_irqnr_and_base r0, r6, r5, lr
		movne	r1, sp
		@
		@ routine called with r0 = irq number, r1 = struct pt_regs *
		@
		adrne	lr, 1b
		bne	asm_do_IRQ

	调用C处理中断
	asmlinkage void __exception asm_do_IRQ(unsigned int irq, struct pt_regs *regs)


### 中断处理过程
	asm_do_IRQ
		desc_handle_irq
			set_irq_handler
				__set_irq_handler
					s3c24xx_init_irq

		
	handle_edge_irq
		desc->chip->mask(irq);      //清除中断
			handle_IRQ_event        //

![](https://i.imgur.com/z9bXFgt.png)
![](https://i.imgur.com/vwHIIlI.png)


### request_irq函数
	request_irq
		setup_irq


![](https://i.imgur.com/cznBjku.png)
![](https://i.imgur.com/OltY182.png)

----

## 中断方式的按键驱动程序
	cat /proc/interrupts
	exec 6 < /dev/buttons
	exec 6<&-

### poll驱动机制分析
### poll机制的加入在read的时候不会一直等待，在没有poll到的时候会返回。
	app:poll
	kernel:sys_poll
				do_sys_poll(ufds, nfds, &timeout_jiffies);
					poll_initwait(&table);
						init_poll_funcptr(&pwq->pt, __pollwait); > pt->qproc = qproc;
					do_poll
					for (;;) {
                                                  //把当前进程放到队列去
													__pollwait(filp, &wait_address, p);
						if (do_pollfd(pfd, pt)) {   file->f_op && file->f_op->poll
							count++;     //如果驱动poll返回值为非0值，那么count++
							pt = NULL;
						}

						if (count || !*timeout || signal_pending(current))
							break;

						schedule_timeout(__timeout);

					}



### 字符设备之异步通知
### 实验的目标按下按键时，驱动通知应用程序
#### 1.应用程序要注册信号处理函数
#### 2.驱动要在file_operations中实现.fasync接口，
#### 3.应用程序要进行如下的设置，将应用的pid设置到驱动程序
	fcntl(fd, F_SETOWN, getpid());  // 告诉内核，发给谁
	Oflags = fcntl(fd, F_GETFL);   
	fcntl(fd, F_SETFL, Oflags | FASYNC);  // 改变fasync标记，最终会调用到驱动的faync > fasync_helper：初始化/释放fasync_struct	

#### 为了使设备支持异步通知机制，驱动程序中涉及以下3项工作：
#### 1. 支持F_SETOWN命令，能在这个控制命令处理中设置filp->f_owner为对应进程ID。
####   不过此项工作已由内核完成，设备驱动无须处理。
#### 2. 支持F_SETFL命令的处理，每当FASYNC标志改变时，驱动程序中的fasync()函数将得以执行。
####   驱动中应该实现fasync()函数。
#### 3. 在设备资源可获得时，调用kill_fasync()函数激发相应的信号

![](https://i.imgur.com/Oks9H79.png)
![](https://i.imgur.com/dTQB4xZ.png)




### 同步互斥阻塞驱动
### 目的同一个时刻只能打开一次驱动

### 使用简单的变量作为标记，限定只有一次打开驱动存在问题，不是原子操作
