

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NAME{
	char *name;
	struct NAME *pre;
	struct NAME *next;
}T_Name, *PT_Name;

static PT_Name g_ptNameHead;


void add_name(PT_Name ptNew)
{
	PT_Name ptCur;
	
	if (g_ptNameHead == NULL)
	{
		g_ptNameHead = ptNew;
	}
	else
	{
		ptCur = g_ptNameHead;
		while (ptCur->next)
		{
			ptCur = ptCur->next;
		}
		ptCur->next = ptNew;
		ptNew->pre  = ptCur;
	}
}

void del_name(PT_Name ptDel)
{
	PT_Name ptCur;	
	PT_Name ptPre;	
	PT_Name ptNext;	
	
	if (g_ptNameHead == ptDel)
	{
		g_ptNameHead = ptDel->next;
		/* 释放 */
		return;
	}
	else
	{
		ptCur = g_ptNameHead->next;
		while (ptCur)
		{
			if (ptCur == ptDel)
			{
				/* 从链表中删除 */
				ptPre  = ptCur->pre;
				ptNext = ptCur->next;
				ptPre->next = ptNext;
				if (ptNext)
				{
					ptNext->pre = ptPre;
				}
				break;
			}
			else
			{
				ptCur = ptCur->next;
			}
		}
	}

	free(ptDel->name);
	free(ptDel);
}

void add_one_name()
{
	PT_Name ptNew;
	char *str;
	char name[128];
	
	printf("enter the name:");
	scanf("%s", name);

	str  = malloc(strlen(name) + 1);
	strcpy(str, name);
	
	ptNew = malloc(sizeof(T_Name));
	ptNew->name = str;
	ptNew->pre  = NULL;
	ptNew->next = NULL;

	add_name(ptNew);
}

PT_Name get_name(char *name)
{
	PT_Name ptCur;
	if (g_ptNameHead == NULL)
	{
		return NULL;
	}
	else
	{
		ptCur = g_ptNameHead;
		do {
			if (strcmp(ptCur->name, name) == 0)
				return ptCur;
			else
				ptCur = ptCur->next;
		}while (ptCur);
	}
	return NULL;
}

void del_one_name()
{	
	PT_Name ptFind;
	char name[128];
	
	printf("enter the name:");
	scanf("%s", name);

	ptFind = get_name(name);
	if (ptFind == NULL)
	{
		printf("do not have this name\n");
		return ;
	}
	
	del_name(ptFind);
	
}

void list_all_name(void)
{
	PT_Name ptCur;
	int i = 0;
	ptCur = g_ptNameHead;
	while (ptCur)
	{
		printf("%02d : %s\n", i++, ptCur->name);
		ptCur = ptCur->next;
	}
}
	
int main(int argc, char **argv)
{
	char c;

	while (1)
	{
		printf("<l> List all the names\n");
		printf("<a> add one name\n");
		printf("<d> del one name\n");
		printf("<x> exit\n");
		

		printf("Enter the choise: ");

		c = getchar();
		switch (c)
		{
			case 'l':
			{
				list_all_name();
				break;
			}
			case 'a':
			{
				add_one_name();
				break;
			}
			case 'd':
			{
				del_one_name();
				break;
			}
			case 'x':
			{
				return 0;
				break;
			}
			default:
			{
				break;
			}
		}
	}

	return 0;
}

