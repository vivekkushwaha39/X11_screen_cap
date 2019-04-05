#ifndef DEBUG_H_
#define DEBUG_H_

#if defined(DEBUG)
	#include "tcpipdbg.h"
	#define debug_sprintf(x) sprintf x
	void initEventGenDebugger(void);
	void _TRACE(char *pszMessage);
	
#else
    #define _TRACE(s) ;
    #define debug_sprintf(x) ;
#endif

char gszDebugMessage[1024+1];

#endif /*DEBUG_H_*/
