#include "s3c24xx.h"

void Timer0_Handle(void)
{
    /*
     * 每次中断令4个LED改变状态
     */
    if(INTOFFSET == 10)
    {
        GPFDAT = ~(GPFDAT & (0x7 << 4));
    }
    //清中断
    SRCPND = 1 << INTOFFSET;
    INTPND = 1 << INTOFFSET;     
}

