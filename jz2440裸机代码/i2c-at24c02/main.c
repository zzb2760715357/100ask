#include <stdio.h>
#include "serial.h"
#include "i2c.h"
#include "m41t11.h"


int main()
{
    char c;
    char str[200];
    int i;
    struct rtc_time dt;
    
    uart0_init();   // 波特率115200，8N1(8个数据位，无校验位，1个停止位)
    
    i2c_init();
    
    while (1)
    {
        printf("\r\n##### RTC Menu #####\r\n");
        printf("Data format: 'year.month.day w hour:min:sec', 'w' is week day\n\r");
        printf("eg: 2007.08.30 4 01:16:57\n\r");
        printf("[S] Set the RTC\n\r");
        printf("[R] Read the RTC\n\r");
        printf("Enter your selection: ");

        c = getc();
        printf("%c\n\r", c);
        switch (c)
        {
            case 's':
            case 'S':
            {
                printf("Enter date&time: ");
                i = 0;
                do
                {
                    c = getc();
                    str[i++] = c;
                    putc(c);
                } while(c != '\n' && c != '\r');
                str[i] = '\0';

                while(--i >= 0)
                {
                    if (str[i] < '0' || str[i] > '9')
                        str[i] = ' ';
                }

                sscanf(str, "%d %d %d %d %d %d %d",         \
                    &dt.tm_year, &dt.tm_mon, &dt.tm_mday,   \
                    &dt.tm_wday,                            \
                    &dt.tm_hour, &dt.tm_min, &dt.tm_sec);

                printf("\n\r*** Date set as: %04d.%02d.%02d %d %02d:%02d:%02d ***\n\r", \
                    dt.tm_year, dt.tm_mon, dt.tm_mday,     \
                    dt.tm_wday,                            \
                    dt.tm_hour, dt.tm_min, dt.tm_sec);

                if (m41t11_set_datetime(&dt))
                    printf("Set RTC failed!\n\r");
                    
                break;
            }
            
            case 'r':
            case 'R':
            {
                if (m41t11_get_datetime(&dt))
                    printf("Read RTC failed!\n\r");

                printf("\n\r*** Now is: %04d.%02d.%02d %d %02d:%02d:%02d ***\n\r",  \
                    dt.tm_year, dt.tm_mon, dt.tm_mday,                  \
                    dt.tm_wday,                                         \
                    dt.tm_hour, dt.tm_min, dt.tm_sec);

                break;
            }
        }
        
    }
    
    return 0;
}
