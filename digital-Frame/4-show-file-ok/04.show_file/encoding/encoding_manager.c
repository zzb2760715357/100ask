#include <config.h>
#include <encoding_manager.h>
#include <string.h>

static PT_EncodingOpr g_aptEncodingOpr[ENCODING_OPR_NUM];
static int g_iEncodingOprUsing;

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr)
{
	int i;

	for (i = 0; i < ENCODING_OPR_NUM; i++)
	{
		if (NULL == g_aptEncodingOpr[i])
		{
			break;
		}
	}

	if (i == ENCODING_OPR_NUM)
	{
		DBG_PRINTF("can't RegisterEncodingOpr, it is full\n");
		return -1;
	}
	else
	{
		g_aptEncodingOpr[i] = ptEncodingOpr;
	}
	
	return 0;
}


void ShowEncodingOpr(void)
{
	int i;
	
	for (i = 0; i < ENCODING_OPR_NUM; i++)
	{
		if (g_aptEncodingOpr[i])
		{
			printf("%02d %s\n", i, g_aptEncodingOpr[i]->name);
		}
	}
}

int SelectEncodingOpr(char *name)
{
	int i;

	g_iEncodingOprUsing = -1;
	for (i = 0; i < ENCODING_OPR_NUM; i++)
	{
		if (g_aptEncodingOpr[i] && (strcmp(g_aptEncodingOpr[i]->name, name) == 0))
		{
			g_iEncodingOprUsing = i;
			return 0;
		}
	}
	return -1;
}

PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead)
{
	int i;
	
	for (i = 0; i < ENCODING_OPR_NUM; i++)
	{
		if (g_aptEncodingOpr[i] && g_aptEncodingOpr[i]->isSupport(pucFileBufHead))
			return g_aptEncodingOpr[i];
	}
	return NULL;
}

int EncodingInit(void)
{
	int iError;

	iError = AsciiEncodingInit();
	if (iError)
	{
		DBG_PRINTF("AsciiEncodingInit error!\n");
		return -1;
	}

	iError = Utf16leEncodingInit();
	if (iError)
	{
		DBG_PRINTF("Utf16leEncodingInit error!\n");
		return -1;
	}
	
	iError = Utf16beEncodingInit();
	if (iError)
	{
		DBG_PRINTF("Utf16beEncodingInit error!\n");
		return -1;
	}
	
	iError = Utf8EncodingInit();
	if (iError)
	{
		DBG_PRINTF("Utf8EncodingInit error!\n");
		return -1;
	}

	return 0;
}

