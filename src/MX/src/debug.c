#include "debug.h"
#include <svc.h>
#include <time.h>	
#include <sys/timeb.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define NO_DEBUG 		'0'
#define CONSOLE_DEBUG	'1'
#define ETHERNET_DEBUG	'2'


static char gchDebug = NO_DEBUG;


#ifdef DEBUG
void initEventGenDebugger(void)
{
	char *pszDebugFlag;
	
	gchDebug = NO_DEBUG;
	
	// Even though the code may be built with debug info, we still check the environment
	// for the DEBUG var, in case the user wants to run a debug program without debug messages
	// (so that it runs slightly faster).
	if ((pszDebugFlag = getenv("DEBUG_MXEVENTGEN")) == NULL)
	{
        gchDebug = NO_DEBUG;
        printf("No debug is enabled \n");
    }
    else
    {
    	//printf("pszDebugFlag is %c \n", pszDebugFlag);
    	
        if (pszDebugFlag[0] == '1')			// Console debug
        {
            gchDebug = CONSOLE_DEBUG;
            printf("Console debug is enabled \n");
        }
        else if (pszDebugFlag[0] == '2')	// Ethernet debug
        {
            gchDebug = ETHERNET_DEBUG;
             printf("Ethernet debug is enabled \n");
        }
    }
	//gchDebug = CONSOLE_DEBUG;
    if (gchDebug == ETHERNET_DEBUG)
    {
        iOpenDebugSocket();
         printf("Opened Debug Socket \n");
    }
}
#endif

#if defined(DEBUG)
void _TRACE(char *pszMessage)
{
	char 		 szTimeStampPrefix[40+1]; /* MM/DD/YYYY HH:MM:SS:sss */
	char 		 szTempMsg[1024+1];
	char 		 *pszMsgPrefix = "MXEVENTGENT";
	struct timeb the_time;
	struct tm 	 *tm_ptr;
	pid_t 		 processID = getpid();		// Get current process ID
	pthread_t 	 threadID = pthread_self();	// Get current thread's ID
	
	ftime(&the_time);
	tm_ptr = gmtime(&the_time.time);
	if (the_time.millitm >= 1000)
	{
		the_time.millitm = 999;	// Keep within 3 digits, since sprintf's width.precision specification does not truncate large values
	}
	sprintf(szTimeStampPrefix, "%02d/%02d/%04d %02d:%02d:%02d:%3.03d", 
						tm_ptr->tm_mon+1, tm_ptr->tm_mday, 1900+tm_ptr->tm_year, 
						tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, the_time.millitm); 
	
	if (gchDebug == ETHERNET_DEBUG)
    {
		sprintf(szTempMsg, "%s %04d %04d %s: %s", szTimeStampPrefix, processID, (int)threadID, pszMsgPrefix, pszMessage);
		DebugMsg(szTempMsg);
	}
    else if (gchDebug == CONSOLE_DEBUG)
	{
		sprintf(szTempMsg, "%s %04d %04d %s: %s\n", szTimeStampPrefix, processID, (int)threadID, pszMsgPrefix, pszMessage);
		printf(szTempMsg);
    }
}
#endif
