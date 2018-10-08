#include <stdio.h>
#include "serial.h"
#include "i2c.h"
#include "m41t11.h"


int main()
{
    char c;
    char str[200];
    int i;
	int address;
	int data;
    struct rtc_time dt;
    
    uart0_init();   // 波特率115200，8N1(8个数据位，无校验位，1个停止位)
    
    i2c_init();
    
    while (1)
    {
        printf("\r\n##### AT24C02 Menu #####\r\n");
        printf("[W] Write the AT24C02\n\r");
        printf("[R] Read the AT24C02\n\r");
        printf("Enter your selection: ");

        c = getc();
        printf("%c\n\r", c);
        switch (c)
        {
            case 'r':
            case 'R':
            {
                printf("Enter addres: ");
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

                sscanf(str, "%d", &address);
				
				printf("\r\nRead Address = %d\r\n",address);
				//data = at24cxx_read(address);
				printf("data = %d\r\n",data);
					
                break;
            }
            
            case 'w':
            case 'W':
            {
				printf("Enter addres: ");
                i = 0;
                do
                {
                    c = getc();
                    str[i++] = c;
                    putc(c);
                } while(c != '\n' && c != '\r');
                str[i] = '\0';
				printf("\r\n");

                while(--i >= 0)
                {
                    if (str[i] < '0' || str[i] > '9')
                        str[i] = ' ';
                }

                sscanf(str, "%d", &address);

				printf("Enter data: ");
                i = 0;
                do
                {
                    c = getc();
                    str[i++] = c;
                    putc(c);
                } while(c != '\n' && c != '\r');
                str[i] = '\0';
				printf("\r\n");
				//printf("get str %s\r\n", str);

                while(--i >= 0)
                {
                    if (str[i] < '0' || str[i] > '9')
                        str[i] = ' ';
                }

                sscanf(str, "%d", &data);

				printf("Write address %d with data %d\r\n",address,data);
				//at24cxx_write(address,data);
               
                break;
            }
        }
        
    }
    
    return 0;
}
