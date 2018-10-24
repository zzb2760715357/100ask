
#ifndef _ENCODING_MANAGER_H
#define _ENCODING_MANAGER_H

#include <fonts_manager.h>
#include <disp_manager.h>

typedef struct EncodingOpr {
	char *name;
	int iHeadLen;
	PT_FontOpr aptFontOprSupported[4];
	int (*isSupport)(unsigned char *pucBufHead);
	int (*GetCodeFrmBuf)(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);
}T_EncodingOpr, *PT_EncodingOpr;

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr);
void ShowEncodingOpr(void);
int SelectFontOpr(char *pcName);
PT_DispOpr GetDispOpr(char *pcName);
PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead);
int EncodingInit(void);
int AsciiEncodingInit(void);
int  Utf16beEncodingInit(void);
int  Utf16leEncodingInit(void);
int  Utf8EncodingInit(void);

#endif /* _ENCODING_MANAGER_H */

