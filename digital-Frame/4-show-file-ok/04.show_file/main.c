#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <config.h>
#include <draw.h>
#include <encoding_manager.h>
#include <fonts_manager.h>
#include <disp_manager.h>
#include <string.h>


/* ./show_file [-s Size] [-f freetype_font_file] [-h HZK] <text_file> */
int main(int argc, char **argv)
{
	int iError;
	unsigned int dwFontSize = 16;
	char acHzkFile[128];
	char acFreetypeFile[128];
	char acTextFile[128];

	char cOpr;

	acHzkFile[0]  = '\0';
	acFreetypeFile[0] = '\0';
	acTextFile[0] = '\0';
	
	while ((iError = getopt(argc, argv, "s:f:h:")) != -1)
	{
		switch(iError)
		{
			case 's':
			{
				  dwFontSize = strtoul(optarg, NULL, 0);
				  break;
			}
			case 'f':
			{
				  strncpy(acFreetypeFile, optarg, 128);
				  acFreetypeFile[127] = '\0';
				  break;
			}
			case 'h':
			{
					strncpy(acHzkFile, optarg, 128);
					acHzkFile[127] = '\0';
					break;
			}
			default:
			{
				  printf("Usage: %s [-s Size] [-f font_file] [-h HZK] <text_file>\n", argv[0]);
				  return -1;
				  break;
			}
		}
	}

	if (optind >= argc)
	{
		printf("Usage: %s [-s Size] [-f font_file] [-h HZK] <text_file>\n", argv[0]);
		return -1;
	}

	strncpy(acTextFile, argv[optind], 128);
	acTextFile[127] = '\0';
		
	iError = DisplayInit();
	if (iError)
	{
		DBG_PRINTF("DisplayInit error!\n");
		return -1;
	}

	iError = FontsInit();
	if (iError)
	{
		DBG_PRINTF("FontsInit error!\n");
		return -1;
	}

	iError = EncodingInit();
	if (iError)
	{
		DBG_PRINTF("EncodingInit error!\n");
		return -1;
	}
		
	iError = OpenTextFile(acTextFile);
	if (iError)
	{
		DBG_PRINTF("OpenTextFile error!\n");
		return -1;
	}

	iError = SetTextDetail(acHzkFile, acFreetypeFile, dwFontSize);
	if (iError)
	{
		DBG_PRINTF("SetTextDetail error!\n");
		return -1;
	}

	iError = SelectAndInitDisplay("fb");
	if (iError)
	{
		DBG_PRINTF("SelectAndInitDisplay error!\n");
		return -1;
	}
	
	iError = ShowNextPage();
	if (iError)
	{
		DBG_PRINTF("Error to show first page\n");
		return -1;
	}

	while (1)
	{
		printf("Enter 'n' to show next page, 'u' to show previous page, 'q' to exit: ");

		do {
			cOpr = getchar();			
		} while ((cOpr != 'n') && (cOpr != 'u') && (cOpr != 'q'));

		if (cOpr == 'n')
		{
			ShowNextPage();
		}
		else if (cOpr == 'u')
		{
			ShowPrePage();			
		}
		else 
		{
			return 0;
		}			
	}
	return 0;
}

