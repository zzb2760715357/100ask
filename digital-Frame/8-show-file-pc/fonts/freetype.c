
#include <config.h>
#include <fonts_manager.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

static int FreeTypeFontInit(char *pcFontFile, unsigned int dwFontSize);
static int FreeTypeGetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap);

static FT_Library	g_tLibrary;
static FT_Face 	    g_tFace;
static FT_GlyphSlot g_tSlot;


/* 分配、设置、注册T_FontOpr */
static T_FontOpr g_tFreeTypeFontOpr = {
	.name          = "freetype",
	.FontInit      = FreeTypeFontInit,
	.GetFontBitmap = FreeTypeGetFontBitmap,
};

static int FreeTypeFontInit(char *pcFontFile, unsigned int dwFontSize)
{
	int iError;
	
	/* 显示矢量字体 */
	iError = FT_Init_FreeType( &g_tLibrary );			   /* initialize library */
	/* error handling omitted */
	if (iError)
	{
		DBG_PRINTF("FT_Init_FreeType error!\n");
		return -1;
	}
	
	iError = FT_New_Face( g_tLibrary, pcFontFile, 0, &g_tFace ); /* create face object */
	/* error handling omitted */	
	if (iError)
	{
		DBG_PRINTF("FT_New_Face error!\n");
		return -1;
	}
	
	g_tSlot = g_tFace->glyph;

	iError = FT_Set_Pixel_Sizes(g_tFace, dwFontSize, 0);
	if (iError)
	{
		DBG_PRINTF("FT_Set_Pixel_Sizes error!\n");
		return -1;
	}

	return 0;
}

static int FreeTypeGetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap)
{
	int iError;
	int iPenX = ptFontBitMap->iCurOriginX;
	int iPenY = ptFontBitMap->iCurOriginY;
	
	/* load glyph image into the slot (erase previous one) */
	iError = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER | FT_LOAD_MONOCHROME );
	if (iError)
	{
		DBG_PRINTF("FT_Load_Char error\n");
		return -1;
	}
	
	ptFontBitMap->iXLeft       = iPenX + g_tSlot->bitmap_left;
	ptFontBitMap->iYTop        = iPenY - g_tSlot->bitmap_top;
	ptFontBitMap->iXMax        = ptFontBitMap->iXLeft + g_tSlot->bitmap.width;
	ptFontBitMap->iYMax        = ptFontBitMap->iYTop  + g_tSlot->bitmap.rows;
	ptFontBitMap->iBpp         = 1;
	ptFontBitMap->iPitch       = g_tSlot->bitmap.pitch;
	ptFontBitMap->iNextOriginX = iPenX + g_tSlot->advance.x / 64;
	ptFontBitMap->iNextOriginY = iPenY;
	ptFontBitMap->pucBuffer    = g_tSlot->bitmap.buffer;
	return 0;
}

int FreeTypeInit(void)
{
	return RegisterFontOpr(&g_tFreeTypeFontOpr);
}

