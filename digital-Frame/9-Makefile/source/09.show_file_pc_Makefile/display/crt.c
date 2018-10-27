#include <config.h>
#include <disp_manager.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <string.h>
#include <vga.h>
#include <vgagl.h>

static int CRTDeviceInit(void);
static int CRTShowPixel(int iX, int iY, unsigned int dwColor);
static int CRTCleanScreen(unsigned int dwBackColor);
static int CRTDeviceExit(void);

static T_DispOpr g_tCRTOpr = {
	.name        = "crt",
	.DeviceInit  = CRTDeviceInit,
	.DeviceExit  = CRTDeviceExit,
	.ShowPixel   = CRTShowPixel,
	.CleanScreen = CRTCleanScreen,
};

static int CRTDeviceInit(void)
{
	vga_init();
	vga_setmode(G320x200x256);
	g_tCRTOpr.iXres = 320;
	g_tCRTOpr.iYres = 200;
	g_tCRTOpr.iBpp  = 8; 
	return 0;
}

static int CRTDeviceExit(void)
{
	vga_setmode(TEXT);
	return 0;
}

static int CRTShowPixel(int iX, int iY, unsigned int dwColor)
{
	int iRed, iGreen, iBlue;
	
	iRed   = (dwColor >> 16) & 0xff;
	iGreen = (dwColor >> 8) & 0xff;
	iBlue  = (dwColor >> 0) & 0xff;
	gl_setpalettecolor(5, iRed>>2, iGreen>>2, iBlue>>2);   /* 0xE7DBB5	*/ /* ·º»ÆµÄÖ½ */
	vga_setcolor(5);

	vga_drawpixel(iX, iY);
	return 0;
}


static int CRTCleanScreen(unsigned int dwBackColor)
{
	int iX;
	int iY;
	int iRed, iGreen, iBlue;
	
	iRed   = (dwBackColor >> 16) & 0xff;
	iGreen = (dwBackColor >> 8) & 0xff;
	iBlue  = (dwBackColor >> 0) & 0xff;
	gl_setpalettecolor(4, iRed>>2, iGreen>>2, iBlue>>2);   /* 0xE7DBB5  */ /* ·º»ÆµÄÖ½ */
	vga_setcolor(4);

	for (iX = 0; iX < 320; iX++)
		for (iY = 0; iY < 200; iY++)
			vga_drawpixel(iX, iY);
	return 0;		
}

int CRTInit(void)
{
	return RegisterDispOpr(&g_tCRTOpr);
}

