#ifndef MESIMULATOR_H_
#define MESIMULATOR_H_
#include"guimgr.h"


extern int gbAutoMsr; /*Flag that shows the availability of track data*/
extern unsigned char gcAutoMsrData[256]; /*Buffer that stores the track data*/
extern char gszCommandType[128];
extern char gcPinAccount[32];
extern int (*pinCallback)(IPP_CALLBACK_DATA *);
extern void (*getDecimalCallback)(char *,int ,void *);
extern int trackLen;

int InitMouseEventSimulator(void *pData);
void ExitMouseEventSimulator();
int SendResponseToAutomn(char *pszResult, char *pszDescription);
#endif /*MESIMULATOR_H_*/
