#include <config.h>
#include <fonts_manager.h>
#include <string.h>

static PT_FontOpr g_aptFontOprs[FONT_OPR_NUM];
static int g_iFontOprUsing;

int RegisterFontOpr(PT_FontOpr ptFontOpr)
{
	int i;

	for (i = 0; i < FONT_OPR_NUM; i++)
	{
		if (NULL == g_aptFontOprs[i])
		{
			break;
		}
	}

	if (i == FONT_OPR_NUM)
	{
		DBG_PRINTF("can't RegisterFontOpr, it is full\n");
		return -1;
	}
	else
	{
		g_aptFontOprs[i] = ptFontOpr;
	}
	
	return 0;
}


void ShowFontOpr(void)
{
	int i;
	
	for (i = 0; i < FONT_OPR_NUM; i++)
	{
		if (g_aptFontOprs[i])
		{
			printf("%02d %s\n", i, g_aptFontOprs[i]->name);
		}
	}
}

int SelectFontOpr(char *pcName)
{
	int i;

	g_iFontOprUsing = -1;
	for (i = 0; i < FONT_OPR_NUM; i++)
	{
		if (g_aptFontOprs[i] && (strcmp(g_aptFontOprs[i]->name, pcName) == 0))
		{
			g_iFontOprUsing = i;
			return 0;
		}
	}
	return -1;
}

PT_FontOpr GetFontOpr(char *pcName)
{
	int i;

	for (i = 0; i < FONT_OPR_NUM; i++)
	{
		if (g_aptFontOprs[i] && (strcmp(g_aptFontOprs[i]->name, pcName) == 0))
		{
			return g_aptFontOprs[i];
		}
	}
	return NULL;
}


int FontsInit(void)
{
	int iError;
	
	iError = ASCIIInit();
	if (iError)
	{
		DBG_PRINTF("ASCIIInit error!\n");
		return -1;
	}

	iError = GBKInit();
	if (iError)
	{
		DBG_PRINTF("GBKInit error!\n");
		return -1;
	}
	
	iError = FreeTypeInit();
	if (iError)
	{
		DBG_PRINTF("FreeTypeInit error!\n");
		return -1;
	}

	return 0;
}

