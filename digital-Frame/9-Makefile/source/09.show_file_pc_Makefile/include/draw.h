
#ifndef _DRAW_H
#define _DRAW_H
int OpenTextFile(char *pcFileName);
int SetFontsDetail(char *pcHZKFile, char *pcFileFreetype, unsigned int dwFontSize);
int SelectAndInitDisplay(char *pcName);
int ShowNextPage(void);
int ShowPrePage(void);
int DrawInit(void);
void ExitDisplay(void);


#endif /* _DRAW_H */

