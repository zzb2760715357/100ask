#include <config.h>
#include <encoding_manager.h>
#include <string.h>

static int isUtf16leCoding(unsigned char *pucBufHead);
static int Utf16leGetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);

static T_EncodingOpr g_tUtf16leEncodingOpr = {
	.name          = "utf-16le",
	.iHeadLen	   = 2,
	.isSupport     = isUtf16leCoding,
	.GetCodeFrmBuf = Utf16leGetCodeFrmBuf,
};

static int isUtf16leCoding(unsigned char *pucBufHead)
{
	const char aStrUtf16le[] = {0xFF, 0xFE, 0};
	if (strncmp((const char *)pucBufHead, aStrUtf16le, 2) == 0)
	{
		/* UTF-16 little endian */
		return 1;
	}
	else
	{
		return 0;
	}
}

static int Utf16leGetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode)
{
	if (pucBufStart + 1 < pucBufEnd)
	{
		*pdwCode = (((unsigned int)pucBufStart[1])<<8) + pucBufStart[0];
		return 2;
	}
	else
	{
		/* ÎÄ¼þ½áÊø */
		return 0;
	}
}

int  Utf16leEncodingInit(void)
{
	AddFontOprForEncoding(&g_tUtf16leEncodingOpr, GetFontOpr("freetype"));
	AddFontOprForEncoding(&g_tUtf16leEncodingOpr, GetFontOpr("ascii"));
	return RegisterEncodingOpr(&g_tUtf16leEncodingOpr);
}


