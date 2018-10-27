
#include <config.h>
#include <fonts_manager.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

static int FreeTypeFontInit(char *pcFontFile, unsigned int dwFontSize);
static int FreeTypeGetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap);

static T_FontOpr g_tFreeTypeFontOpr = {
	.name          = "freetype",
	.FontInit      = FreeTypeFontInit,
	.GetFontBitmap = FreeTypeGetFontBitmap,
};

static FT_Library g_tLibrary;
static FT_Face g_tFace;
static FT_GlyphSlot g_tSlot;

static int FreeTypeFontInit(char *pcFontFile, unsigned int dwFontSize)
{
	int iError;

	/* ÏÔÊ¾Ê¸Á¿×ÖÌå */
	iError = FT_Init_FreeType(&g_tLibrary );			   /* initialize library */
	/* error handling omitted */
	if (iError)
	{
		DBG_PRINTF("FT_Init_FreeType failed\n");
		return -1;
	}
	
	iError = FT_New_Face(g_tLibrary, pcFontFile, 0, &g_tFace); /* create face object */
	/* error handling omitted */
	if (iError)
	{
		DBG_PRINTF("FT_Init_FreeType failed\n");		
		return -1;
	}
	
	g_tSlot = g_tFace->glyph;

	iError = FT_Set_Pixel_Sizes(g_tFace, dwFontSize, 0);
	if (iError)
	{
		DBG_PRINTF("FT_Set_Pixel_Sizes failed : %d\n", dwFontSize);
		return -1;
	}
	
	
	return 0;
}

static int FreeTypeGetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap)
{
	int iError;
	int iPenX = ptFontBitMap->iCurOriginX;
	int iPenY = ptFontBitMap->iCurOriginY;
#if 0
	FT_Vector tPen;

	tPen.x = 0;
	tPen.y = 0;
	
	/* set transformation */
	FT_Set_Transform(g_tFace, 0, &tPen);
#endif

	/* load glyph image into the slot (erase previous one) */
	//iError = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER );
	iError = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
	if (iError)
	{
		DBG_PRINTF("FT_Load_Char error for code : 0x%x\n", dwCode);
		return -1;
	}

	//DBG_PRINTF("iPenX = %d, iPenY = %d, bitmap_left = %d, bitmap_top = %d, width = %d, rows = %d\n", iPenX, iPenY, g_tSlot->bitmap_left, g_tSlot->bitmap_top, g_tSlot->bitmap.width, g_tSlot->bitmap.rows);
	ptFontBitMap->iXLeft    = iPenX + g_tSlot->bitmap_left;
	ptFontBitMap->iYTop     = iPenY - g_tSlot->bitmap_top;
	ptFontBitMap->iXMax     = ptFontBitMap->iXLeft + g_tSlot->bitmap.width;
	ptFontBitMap->iYMax     = ptFontBitMap->iYTop  + g_tSlot->bitmap.rows;
	ptFontBitMap->iBpp      = 1;
	ptFontBitMap->iPitch    = g_tSlot->bitmap.pitch;
	ptFontBitMap->pucBuffer = g_tSlot->bitmap.buffer;
	
	ptFontBitMap->iNextOriginX = iPenX + g_tSlot->advance.x / 64;
	ptFontBitMap->iNextOriginY = iPenY;

	//DBG_PRINTF("iXLeft = %d, iYTop = %d, iXMax = %d, iYMax = %d, iNextOriginX = %d, iNextOriginY = %d\n", ptFontBitMap->iXLeft, ptFontBitMap->iYTop, ptFontBitMap->iXMax, ptFontBitMap->iYMax, ptFontBitMap->iNextOriginX, ptFontBitMap->iNextOriginY);

	return 0;
}

int FreeTypeInit(void)
{
	return RegisterFontOpr(&g_tFreeTypeFontOpr);
}

