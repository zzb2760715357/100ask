.text
.global _start
_start:
		LDR R0,=0x56000050	@ 设置R0为GPFCON寄存器
                            @ 用于配置端口的输入、输出状态

		mov R1,#0x00000100   
        str R1,[R0]         @ 设置GPF4端口为输出口
        
		ldr R0,=0x56000054  @ 设置R0为GPFDAT寄存器
							@ 用于配置端口输出的高低电平
	
		mov R1,#0x00000100
		str R1,[R0]         @ 输出低电平
        
MAIN_LOOP:
			B MAIN_LOOP
