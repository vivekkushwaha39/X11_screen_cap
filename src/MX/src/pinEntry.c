/*******************************************************
  File Name : pinEntry.c
  Author    : Gururaj_Kheni
  Company   : VeriFone, Inc.
********************************************************/

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <fp.h>
#include <fp_image.h>
#include <time.h>		// For struct tm
#include <sys/timeb.h>	// For struct timeb
#include <gui.h>
#include <guimgr.h>
#include "ctrlids.h"
#include <fcntl.h>
#include <fp_media.h>
#include <fp_media_enum.h>
#include <stdlib.h>
#include <string.h>
#include "svc.h"
#include <unistd.h>

#define read_ticks svcGetSysMillisec

#define STX 2
#define ETX 3
#define EOT	4
#define ACK	6
#define NAK	21
#define SI	15
#define SO	14
#define FS	28

#define STX_ETX 1
#define SI_SO	2


int bRetVal;
volatile BOOL bAppAlive;
//int timer=0;
int gidCtrl=99;
BOOL gbTimeout=FALSE;
BOOL gbGameOver;
struct fp * gfp;

void ipp_pinEntryTest(char *, char *, char *);


#if defined(DEBUG)
	
	#define NO_DEBUG 		0
	#define CONSOLE_DEBUG	1
	#define ETHERNET_DEBUG	2

	#include "tcpipdbg.h"
	#define debug_sprintf(x) sprintf x	
	
	char gchDebug;
	char gszDebugMessage[1024+1];
	
	void initDebugger(void)
	{
		char *pszDebugFlag;	
		gchDebug = NO_DEBUG;
	
		// Even though the code may be built with debug info, we still check the environment
		// for the DEBUG var, in case the user wants to run a debug program without debug messages
		// (so that it runs slightly faster).
		if ((pszDebugFlag = getenv("DEBUG")) == NULL)
		{
        	gchDebug = NO_DEBUG;
    	}
    	else
    	{
        	if (pszDebugFlag[0] == '1')			// Console debug
        	{
            	gchDebug = CONSOLE_DEBUG;
        	}
        	else if (pszDebugFlag[0] == '2')	// Ethernet debug
        	{
            	gchDebug = ETHERNET_DEBUG;
        	}
    	}

    	if (gchDebug == ETHERNET_DEBUG)
    	{
        	iOpenDebugSocket();
    	}
}

void TRACE(char *pszMessage)
{
	char 		 szTimeStampPrefix[40+1]; /* MM/DD/YYYY HH:MM:SS:sss */
	char 		 szTempMsg[1024+1];
	char 		 *pszMsgPrefix = "MAGSWIPE:";

//	struct timeb the_time;
//	struct tm 	 *tm_ptr;
	//pid_t 		 processID = getpid();		// Get current process ID
	//pthread_t 	 threadID = pthread_self();	// Get current thread's ID
	
/*	ftime(&the_time);
	tm_ptr = gmtime(&the_time.time);
	if (the_time.millitm >= 1000)
	{
		the_time.millitm = 999;	// Keep within 3 digits, since sprintf's width.precision specification does not truncate large values
	}
	sprintf(szTimeStampPrefix, "%02d/%02d/%04d %02d:%02d:%02d:%3.03d", 
						tm_ptr->tm_mon+1, tm_ptr->tm_mday, 1900+tm_ptr->tm_year, 
						tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, the_time.millitm);*/ 
	
	if (gchDebug == ETHERNET_DEBUG)
    {
		sprintf(szTempMsg, "%s: %s\n", pszMsgPrefix, pszMessage);
		DebugMsg(szTempMsg);
	}
    else if (gchDebug == CONSOLE_DEBUG)
	{
		sprintf(szTempMsg, "%s: %s\n", pszMsgPrefix, pszMessage);
		printf(szTempMsg);
    }
}
#else
    #define TRACE(s) ;
    #define debug_sprintf(x) ;
#endif

Event_Callback_Function cb_Functions[] = 
{
   NULL,
};

int waitForChar(char aChar, int aTimeout)
{
	char ch=0;
	int timeout;
	
	debug_sprintf((gszDebugMessage, "waiting for %c ==> %d", aChar,aChar));
	TRACE(gszDebugMessage);
	
	timeout = read_ticks() + (aTimeout*1000);			
	while(ch != aChar && (read_ticks() < timeout))
	{
		ippRead(&ch,1);				
		debug_sprintf((gszDebugMessage, "ippRead ch = %c ==> %d", ch,ch));
		TRACE(gszDebugMessage);
		svcWait(100);
	}
	
	if(ch==aChar)
		return 1;
	return 0;
}

void SelectDukptEngine(int iEngine)
{
	char   ippWBuffer[200];
	char 	ippBuffer[200];
	memset(ippWBuffer,0,sizeof(ippWBuffer));
	ippWBuffer[0]=SI;	
	ippWBuffer[1]='1';
	ippWBuffer[2]='9';
	ippWBuffer[3]=iEngine+0x30;
	ippWBuffer[4]=SO;
	ippWBuffer[5]=svcCrcCalc(0, &ippWBuffer[1], strlen(ippWBuffer)-1);
	
	debug_sprintf((gszDebugMessage, "ippWBuffer = %s", ippWBuffer));
	TRACE(gszDebugMessage);
		
	bRetVal=ippWrite(ippWBuffer,strlen(ippWBuffer));
	
	debug_sprintf((gszDebugMessage, "ippWrite rslt = %d", bRetVal));
	TRACE(gszDebugMessage);
		
	waitForChar(ACK,7);
	waitForPacket(SI_SO,ippBuffer,60);
	waitForChar(EOT,7);
	
	
}
/*	Function Name:	ipp_pinEntryTest
 * 	Functionality:	ipp_pinEntryTest takes 'pan' "Personal Account Number", 'amnt' "Amount" for the 
 * 					transaction and 'Buffer' Empty buffer for the response packet.
 * 					It sends the '76' packet to the IPP and wait for the response packet '71'
 * 					with encrypted PIN
 * 	Arguments:		pan (Personal Account Number)
 * 					amnt (amount) and Buffer (empty character buffer).
 * 	Return Type:	Void
 */
void ipp_pinEntryTest(char *pan, char *amnt, char *Buffer)
{
	char   ippWBuffer[200];
	char 	*ippBuffer  = Buffer;
	int size = 0;
		
	memset(ippWBuffer,0,sizeof(ippWBuffer));
	
	printf("ipp_pinEntryTest: \n");
	strcpy(ippWBuffer, "\00276");
	strcat(ippWBuffer, pan);
	printf("ipp_pinEntryTest: pan/ippWBuffer is %s\n", pan);
	strcat(ippWBuffer, "\034");
	strcat(ippWBuffer, amnt);
	strcat(ippWBuffer, "\003");
	
	printf("ipp_pinEntryTest: ippWBuffer is %s\n", ippWBuffer);
	size = strlen(ippWBuffer);
	ippWBuffer[size] = svcCrcCalc(0, &ippWBuffer[1], (size - 1));
	    
    debug_sprintf((gszDebugMessage, "ippWBuffer = %s", ippWBuffer));
	TRACE(gszDebugMessage);
	printf("ipp_pinEntryTest: ippWBuffer is %s\n", ippWBuffer);	
	bRetVal=ippWrite(ippWBuffer,strlen(ippWBuffer));
	
	debug_sprintf((gszDebugMessage, "ippWrite rslt = %d", bRetVal));
	TRACE(gszDebugMessage);
	printf("ipp_pinEntryTest: ippWrite rslt = %d\n", bRetVal);
	
	waitForChar(ACK,7);
	waitForPacket(STX_ETX,ippBuffer,60);
	
	
	//waitForChar(EOT,7);
	
}
int waitForPacket(int protocal,char* pcPacket, int timeout)
{
	char cStart, cEnd,ch;	
	int iResult;
	int i=0,iLrc;
	char *ippBuffer=pcPacket;
	
			
	switch(protocal)
	{
		case SI_SO:
			TRACE("Waiting for SI_SO");
			cStart=SI;
			cEnd=SO;
			break;
		case STX_ETX:
			TRACE("Waiting for STX_ETX");
			cStart=STX;
			cEnd=ETX;
			break;
		default:
			return 0;			
	}
	
	if(!waitForChar(cStart,7))
		return 0;
	
	timeout = read_ticks() + (timeout*1000);
	
	ippBuffer[i++]=cStart;
		
	while(ch != cEnd &&(read_ticks() < timeout))
	{
		ippRead(&ch,1);				
		debug_sprintf((gszDebugMessage, "ippRead ch = %c ==> %d", ch,ch));
		TRACE(gszDebugMessage);
		
			ippBuffer[i++]=ch;	
		svcWait(100);			
	}
	if(ch!=cEnd)
		return 0;
		
	ippRead(&ch,1);				
	TRACE("Reading LRC");
	ippBuffer[i++]=ch;
	svcWait(100);	
	
	ippBuffer[i]=0;
	debug_sprintf((gszDebugMessage, "ippBuffer %s", ippBuffer));
	TRACE(gszDebugMessage);
	
	iLrc=svcCrcCalc(0, &ippBuffer[1], strlen(ippBuffer)-2);
	
	debug_sprintf((gszDebugMessage, "iLRC = %c ==> %d", iLrc,iLrc));
	TRACE(gszDebugMessage);
		
	if(ippBuffer[i-1]!=iLrc)
	{		
		debug_sprintf((gszDebugMessage, "LRC mismatch, iLrc=%c==>%d, ippBuffer[%d-1]=%c==>%d", iLrc,iLrc,i,ippBuffer[i-1],ippBuffer[i-1]));
		TRACE(gszDebugMessage);
		TRACE("LRC mismatch");
		
		return 0;
	}
	
	
	TRACE("LRC OK");
	
	ippWrite("\006", 1);
	TRACE("Writing ACK");
		
}

void eventHandler(char *pszFormName, int iControlType, int iControlId, void *pData, int iCBIndex)
{
	char ch=0;
    char   ippBuffer[200];
    char   ippWBuffer[200];
     unsigned long long timeout;
     int i=0;
     //char szAccNo[]="01234566789";
     //char szAmt[]	= "12345";
     
    
    if(!strcmp(pszFormName,"WELCOME"))
    {
    	TRACE("Event on WELCOME Form");
    	if(iControlId==	WELCOME_BTN_1)
		{
			TRACE("Event on WELCOME Form");
			//InitForm("SWIPE");
   			//ShowForm(PM_NORMAL);
   			//<STX>761234567891<FS>D12345<ETX>k   			
   			//bRetVal=ippWrite("\x02\x37\x36\x01\x02\x03\x04\x05\x06\x07\x08\x09\x01\x1C\x44\x01\x02\x03\x04\x05\x03\x6B", 22);
   			//bRetVal=ippWrite("\x0F\x31\x38\x0E\x07", 5);
   			
   			memset(ippWBuffer,0,sizeof(ippWBuffer));
   			ippWBuffer[0]=STX;   			
   			strcat(ippWBuffer,"76");
   			strcat(ippWBuffer,"01234566789");
   			ippWBuffer[strlen(ippWBuffer)]=FS;
   			ippWBuffer[strlen(ippWBuffer)]='D';
   			strcat(ippWBuffer,"12345");
   			ippWBuffer[strlen(ippWBuffer)]=ETX;
   			ippWBuffer[strlen(ippWBuffer)] = svcCrcCalc(0, &ippWBuffer[1], strlen(ippWBuffer)-1);
   			
   			debug_sprintf((gszDebugMessage, "ippWBuffer = %s", ippWBuffer));
			TRACE(gszDebugMessage);
   			
   			bRetVal=ippWrite(ippWBuffer,strlen(ippWBuffer));
   			
   			debug_sprintf((gszDebugMessage, "ippWrite rslt = %d", bRetVal));
			TRACE(gszDebugMessage);
			
			waitForChar(0x06,7);			
			
			waitForPacket(STX_ETX,ippBuffer,60);
			debug_sprintf((gszDebugMessage, "MAIN:ippBuffer %s", ippBuffer));
			TRACE(gszDebugMessage);			
			
		}
		else
		{
			debug_sprintf((gszDebugMessage, "-TrapHook- Form=WELCOME CtrlType[%d] CtrlId[%d] iCBIndex[%d] ",iControlType, iControlId, iCBIndex ));
			TRACE(gszDebugMessage);
		}
    }
    else 
    {
    	debug_sprintf((gszDebugMessage, "-TrapHook- callback invoked FormName [%s] CtrlType[%d] CtrlId[%d] iCBIndex[%d] ", pszFormName, iControlType, iControlId, iCBIndex ));
		TRACE(gszDebugMessage);
    }     
	       
}
