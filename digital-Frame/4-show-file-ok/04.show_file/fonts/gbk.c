#include <config.h>
#include <fonts_manager.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>


static int GBKFontInit(char *pcFontFile, unsigned int dwFontSize);
static int GBKGetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap);

static T_FontOpr g_tGBKFontOpr = {
	.name          = "gbk",
	.FontInit      = GBKFontInit,
	.GetFontBitmap = GBKGetFontBitmap,
};

static int g_iFdHZK;
static unsigned char *g_pucHZKMem;

static int GBKFontInit(char *pcFontFile, unsigned int dwFontSize)
{
	struct stat tStat;

	if (16 != dwFontSize)
	{
		DBG_PRINTF("GBK can't support %d fontsize\n", dwFontSize);
		return -1;
	}
	
	g_iFdHZK = open(pcFontFile, O_RDONLY);
	if (g_iFdHZK < 0)
	{
		DBG_PRINTF("can't open %s\n", pcFontFile);
		return -1;
	}
	if(fstat(g_iFdHZK, &tStat))
	{
		DBG_PRINTF("can't get fstat\n");
		return -1;
	}
	g_pucHZKMem = (unsigned char *)mmap(NULL , tStat.st_size, PROT_READ, MAP_SHARED, g_iFdHZK, 0);
	if (g_pucHZKMem == (unsigned char *)-1)
	{
		DBG_PRINTF("can't mmap for hzk16\n");
		return -1;
	}
	
	return 0;
}

static int GBKGetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap)
{
	unsigned int dwArea;
	unsigned int dwWhere;

	int iPenX = ptFontBitMap->iCurOriginX;
	int iPenY = ptFontBitMap->iCurOriginY;

	if (NULL == g_pucHZKMem)
	{
		DBG_PRINTF("has not init GBK\n");
		return -1;
	}

	if (dwCode & 0xffff0000)
	{
		DBG_PRINTF("don't support this code : 0x%x\n", dwCode);
		return -1;
	}	

	dwArea  = (dwCode & 0xff) - 0xA1;
	dwWhere = ((dwCode >> 8) & 0xff) - 0xA1;
	
	ptFontBitMap->iXLeft    = iPenX;
	ptFontBitMap->iYTop     = iPenY - 16;
	ptFontBitMap->iXMax     = iPenX + 16;
	ptFontBitMap->iYMax     = iPenY;
	ptFontBitMap->iBpp      = 1;
	ptFontBitMap->pucBuffer = g_pucHZKMem + (dwArea * 94 + dwWhere)*32;;	

	ptFontBitMap->iNextOriginX = iPenX + 16;
	ptFontBitMap->iNextOriginY = iPenY;
	
	return 0;
}

int GBKInit(void)
{
	return RegisterFontOpr(&g_tGBKFontOpr);
}

