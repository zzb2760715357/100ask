#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "oledfont.h"

/* oled_test init
 * oled_test clear
 * oled_test clear <page>
 * oled_test <page> <col> <string>
 */

#define OLED_CMD_INIT       0x100001
#define OLED_CMD_CLEAR_ALL  0x100002
#define OLED_CMD_CLEAR_PAGE 0x100003
#define OLED_CMD_SET_POS    0x100004



/* page: 0-7
 * col : 0-127
 * 字符: 8x16象素
 */
void OLEDPutChar(int fd, int page, int col, char c)
{
    int i = 0;
    /* 得到字模 */
    const unsigned char *dots = oled_asc2_8x16[c - ' '];

    /* 发给OLED */
    //OLEDSetPos(page, col);
    //ioctl(fd, OLED_CMD_CLEAR_PAGE, page);
    ioctl(fd, OLED_CMD_SET_POS, page | (col << 8));
    /* 发出8字节数据 */
    //for (i = 0; i < 8; i++)
    //    OLEDWriteDat(dots[i]);
    write(fd, &dots[0], 8);

    //OLEDSetPos(page+1, col);
    //ioctl(fd, OLED_CMD_CLEAR_PAGE, page+1);
    ioctl(fd, OLED_CMD_SET_POS, (page+1) | (col << 8));
    /* 发出8字节数据 */
    //for (i = 0; i < 8; i++)
    //    OLEDWriteDat(dots[i+8]);
    write(fd, &dots[8], 8);
}



/* page: 0-7
 * col : 0-127
 * 字符: 8x16象素
 */
void OLEDPrint(int fd, int page, int col, char *str)
{
    int i = 0;

    ioctl(fd, OLED_CMD_CLEAR_PAGE, page);
    ioctl(fd, OLED_CMD_CLEAR_PAGE, page+1);
    while (str[i])
    {
        OLEDPutChar(fd, page, col, str[i]);
        col += 8;
        if (col > 127)
        {
            col = 0;
            page += 2;
            ioctl(fd, OLED_CMD_CLEAR_PAGE, page);
            ioctl(fd, OLED_CMD_CLEAR_PAGE, page+1);
        }
        i++;
    }
}


void print_usage(char *cmd)
{
    printf("Usage:\n");
    printf("%s init\n", cmd);
    printf("%s clear\n", cmd);
    printf("%s clear <page>\n", cmd);
    printf("%s <page> <col> <string>\n", cmd);
    printf("eg:\n");
    printf("%s 2 0 100ask.taobao.com\n", cmd);
    printf("page is 0,1,...,7\n");
    printf("col is 0,1,...,127\n");
}

int main(int argc, char **argv)
{
    int do_init  = 0;
    int do_clear = 0;
    int do_show  = 0;
    int page = -1;
    int col;

    int fd;

    if (argc == 2 && !strcmp(argv[1], "init"))
        do_init = 1;
    if ((argc == 2) && !strcmp(argv[1], "clear"))
    {
        do_clear = 1;
    }
    if ((argc == 3) && !strcmp(argv[1], "clear"))
    {
        do_clear = 1;
        page = strtoul(argv[2], NULL, 0);
    }
    if (argc == 4)
    {
        do_show = 1;
        page = strtoul(argv[1], NULL, 0);
        col = strtoul(argv[2], NULL, 0);
    }

    if (!do_init && !do_clear && !do_show)
    {
        print_usage(argv[0]);
        return -1;
    }

    fd = open("/dev/oled", O_RDWR);
    if (fd < 0)
    {
        printf("can't open /dev/oled\n");
        return -1;
    }

    if (do_init)
        ioctl(fd, OLED_CMD_INIT);
    else if (do_clear)
    {
        if (page == -1)
            ioctl(fd, OLED_CMD_CLEAR_ALL);
        else
        {
            if (page < 0 || page > 7)
            {
                printf("page is 0,1,...,7\n");
                return -1;
            }
            ioctl(fd, OLED_CMD_CLEAR_PAGE, page);
        }
    }
    else if (do_show)
    {
        if (page < 0 || page > 7)
        {
            printf("page is 0,1,...,7\n");
            return -1;
        }
        if (col < 0 || col > 127)
        {
            printf("col is 0,1,...,127\n");
            return -1;
        }

        OLEDPrint(fd, page, col, argv[3]);
    }
    return 0;
}

