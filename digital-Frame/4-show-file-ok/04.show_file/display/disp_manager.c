
#include <config.h>
#include <disp_manager.h>
#include <string.h>

static PT_DispOpr g_aptDispOprs[DISP_OPR_NUM];
static int g_iDispOprUsing;

int RegisterDispOpr(PT_DispOpr ptDispOpr)
{
	int i;

	for (i = 0; i < DISP_OPR_NUM; i++)
	{
		if (NULL == g_aptDispOprs[i])
		{
			break;
		}
	}

	if (i == DISP_OPR_NUM)
	{
		DBG_PRINTF("can't RegisterDispOpr, it is full\n");
		return -1;
	}
	else
	{
		g_aptDispOprs[i] = ptDispOpr;
	}
	
	return 0;
}

void ShowDispOpr(void)
{
	int i;
	
	for (i = 0; i < DISP_OPR_NUM; i++)
	{
		if (g_aptDispOprs[i])
		{
			printf("%02d %s\n", i, g_aptDispOprs[i]->name);
		}
	}
}

int SelectDispOpr(char *pcName)
{
	int i;

	g_iDispOprUsing = -1;
	for (i = 0; i < DISP_OPR_NUM; i++)
	{
		if (g_aptDispOprs[i] && (strcmp(g_aptDispOprs[i]->name, pcName) == 0))
		{
			g_iDispOprUsing = i;
			return 0;
		}
	}
	return -1;
}

PT_DispOpr GetDispOpr(char *pcName)
{
	int i;

	for (i = 0; i < DISP_OPR_NUM; i++)
	{
		if (g_aptDispOprs[i] && (strcmp(g_aptDispOprs[i]->name, pcName) == 0))
		{
			return g_aptDispOprs[i];
		}
	}
	return NULL;
}

int DisplayInit(void)
{
	int iError;
	
	iError = FBInit();

	return iError;
}

