#include "MagSwipe.h"
#include <msrDevice.h>
#include <fcntl.h>
#include <svc.h>
#include <stdio.h>
#include <pthread.h>
#include <types.h>
#include <beepAPI.h>
#include <string.h>
#include <unistd.h>
#include "TCPIP.h"

#include "guimgr.h"


#if defined(DEBUG)
	//#include "tcpipdbg.h"
	#define debug_sprintf(x) sprintf x
	//static void initDebugger(void);
	extern void _TRACE(char *pszMessage);
	//static char gchDebug = NO_DEBUG;
#else
    #define _TRACE(s) ;
    #define debug_sprintf(x) ;
#endif

char gszDebugMsg[1024+1];
#define FS	 28
#define MAX_BYTES_TRACK 150
static char machTrack1Data[MAX_BYTES_TRACK];
static char machTrack2Data[MAX_BYTES_TRACK];
static char machTrack3Data[MAX_BYTES_TRACK];
static MSR_TRACK_DATA stMSR[3];

static void parse_msr_data(unsigned char *auchCombinedTrackData, int iLenTrackData);
extern int SendResponseToAutomn(char *pszResult, char *pszDescription);

#define GOOD_MSR "180t 5v; s (CEGAR)"
#define BAD_MSR "90t 5v s BRBRBR"
#define GoodMsr() beep(GOOD_MSR)
#define BadMsr() beep(BAD_MSR)

static int msr_handle = 0;
static int msr_enable = 0;
static pthread_t ptMSRTrapThread;
static short bMSRTrapThreadRunning = FALSE;

static char szLed[] = "ledTimer";
static int ledTimer;
static char e=0;

extern int giGetCardData;

unsigned char gszParsedTrackData[512];

static void *MSRReadThread(void *vptr);

static MSR_DATA CardTrack;

int InitMagSwipe(void)
{
	int msr_handle;
	_TRACE("In InitMagSwipe ");
	
	stMSR[0].cpData = machTrack1Data;
    stMSR[1].cpData = machTrack2Data;
    stMSR[2].cpData = machTrack3Data;
	
	bMSRTrapThreadRunning = TRUE;
	
	memset(gszParsedTrackData, 0x00, sizeof(gszParsedTrackData));
	
	if((msr_handle = msrOpen(O_RDONLY, NULL)) <= 0)
    {
    	debug_sprintf((gszDebugMsg, "msrDevice open failed: retval=%d", msr_handle));
        _TRACE(gszDebugMsg);
    }
	else
	{
		debug_sprintf((gszDebugMsg, "msrDevice open OK: retval=%d", msr_handle));
        _TRACE(gszDebugMsg);
        
	}
	if (pthread_create(&ptMSRTrapThread, NULL, MSRReadThread, NULL))
	{
		debug_sprintf((gszDebugMsg, "%s - failed to create MSRReadThread!", __FUNCTION__));
	    _TRACE(gszDebugMsg);
		return(0);
	}
	_TRACE("MSRRead thread Created");
	//bMSRTrapThreadRunning = TRUE;
	CardTrack.stTrack1.cpData=NULL;
	CardTrack.stTrack2.cpData=NULL;
	CardTrack.stTrack3.cpData=NULL;
	ledOff(7);
	return 1;
}

static void onMSR(unsigned char *pszMSRData, int iNumBytes)
{		

	
  
}


static void *MSRReadThread(void *vptr)
{
	unsigned char msr_buf[512];
	unsigned char crd_buff[512];
    int bytesRead,i;
    int iRetVal = 0;
    
    memset(gszParsedTrackData, 0x00, sizeof(gszParsedTrackData));
    memset(msr_buf, 0x00, sizeof(msr_buf));
    _TRACE("Inside thread");
   	enable_msr();
	while (bMSRTrapThreadRunning) 
	{
		// Do a blocked read on the MSR port
    	bytesRead = msrRead((char *)msr_buf, sizeof(msr_buf));
    	
    	if(bytesRead>6) 
    	{			
			debug_sprintf((gszDebugMsg, "bytesRead=%d",bytesRead));
    		_TRACE(gszDebugMsg);   
			_TRACE("poll_msr - MSR read OK");
			if(msr_enable) 
			{
				GoodSwipe_tone();	
				/*			  
				for(i=0;i<bytesRead;i++)
				{
					debug_sprintf((gszDebugMsg, "msr_buf[%d]=%d,%c",i,msr_buf[i],msr_buf[i]));
    				_TRACE(gszDebugMsg);  
				}
				*/
				ledOff(7);
				
				parse_msr_data(msr_buf, bytesRead); //Parsing the MSR data
				
				sprintf(gszParsedTrackData, "%s%c%s%c%s", stMSR[0].cpData, FS, stMSR[1].cpData, FS, stMSR[2].cpData);
				
				debug_sprintf((gszDebugMsg, "%s - gszParsedTrackData = [%s]", __FUNCTION__, gszParsedTrackData));
    			_TRACE(gszDebugMsg);
				
				iRetVal = SendResponseToAutomn("SUCCESS", gszParsedTrackData);
						
				debug_sprintf((gszDebugMsg, "%s - Return value of SendResponseToAutomn is %d", __FUNCTION__, iRetVal));
    			_TRACE(gszDebugMsg);
    			
    			memset(gszParsedTrackData, 0x00, sizeof(gszParsedTrackData));
				giGetCardData = 0;
				
				debug_sprintf((gszDebugMsg, "%s - Disabling MSR....", __FUNCTION__));
    			_TRACE(gszDebugMsg);
				
				disable_msr();			
			}
		}
		
	}
	return NULL;
}

static void parse_msr_data(unsigned char *auchCombinedTrackData, int iLenTrackData)
{
    int i, iTrack;
    char *psDest;
    unsigned int uiIndex = 0;
#ifdef DEBUG
    char szDebugMsg[1024+1];
#endif
    debug_sprintf((szDebugMsg, "%s - entered", __FUNCTION__));
    _TRACE(szDebugMsg);

    // Initialize stMSR structure
    for (iTrack = 0; iTrack <= 2; iTrack++)
    {
    	debug_sprintf((szDebugMsg, "%s - Initializing for Track%d", __FUNCTION__, iTrack+1));
    	_TRACE(szDebugMsg);
        stMSR[iTrack].ucStatus = 1;
        stMSR[iTrack].ucCount = 0;
        memset(stMSR[iTrack].cpData, 0x00, MAX_BYTES_TRACK);
    }

    for (iTrack = 0; iTrack <= 2; iTrack++)
    {
    	debug_sprintf((szDebugMsg, "%s - For the Track%d", __FUNCTION__, iTrack+1));
    	_TRACE(szDebugMsg);
    
        // Set up track related variables
        if (iTrack == 0)
        {
            psDest = machTrack1Data;
        }
        else if (iTrack == 1)
        {
            psDest = machTrack2Data;
        }
        else
        {
            psDest = machTrack3Data;
        }

        // Parse it!
        if (uiIndex < iLenTrackData) // was m_iMSRBytesRead
        {
            stMSR[iTrack].ucCount = auchCombinedTrackData[uiIndex++] - 2; // Note: -2 to subtract count and status bytes themselves
            stMSR[iTrack].ucStatus = auchCombinedTrackData[uiIndex++];
            if (stMSR[iTrack].ucCount == 0)
            {
                // No Data
                *psDest = 0;
                debug_sprintf((szDebugMsg, "%s - No data for the Track%d", __FUNCTION__, iTrack+1));
    			_TRACE(szDebugMsg);
            }
            else
            {
            	debug_sprintf((szDebugMsg, "%s - Storing data for the Track%d", __FUNCTION__, iTrack+1));
    			_TRACE(szDebugMsg);
                // Store Track Data
                for (i = 0; i < stMSR[iTrack].ucCount; i++)
                {
                    stMSR[iTrack].cpData[i] = auchCombinedTrackData[uiIndex++];
                }
                stMSR[iTrack].cpData[i] = 0; // NULL terminate
            }
        }


        debug_sprintf((szDebugMsg, "%s - Track %d status=%d, count=%d, data=[%s]",
                      __FUNCTION__, (iTrack+1), stMSR[iTrack].ucStatus, stMSR[iTrack].ucCount, stMSR[iTrack].cpData));
        _TRACE(szDebugMsg);


        if ((stMSR[iTrack].ucStatus == 0) && (stMSR[iTrack].ucCount >= 3))
        {
            // Strip End-sentinel and LRC
            //memmove(&stMSR[iTrack].cpData[0], &stMSR[iTrack].cpData[0], stMSR[iTrack].ucCount-2); // -2 to exclude ES and LRC
            stMSR[iTrack].ucCount -= 2; // Update count to reflect stripped ES and LRC
            stMSR[iTrack].cpData[stMSR[iTrack].ucCount] = 0;  // Null-terminate

           // Do not print full mag data in debug as it is a security risk - Sadiq_m1 07-Mar-07
            debug_sprintf((szDebugMsg, "%s - - after ES & LRC: Track %d status=%d, count=%d, data=[%s]",
                          __FUNCTION__, (iTrack+1), stMSR[iTrack].ucStatus, stMSR[iTrack].ucCount, stMSR[iTrack].cpData));
            _TRACE(szDebugMsg);

            i = stMSR[iTrack].ucCount;
            
            debug_sprintf((szDebugMsg, "%s - after ES & LRC: Track %d status=%d, count=%d, data=[%c%c%c%c.... %c%c%c%c]",
                          __FUNCTION__, (iTrack+1), stMSR[iTrack].ucStatus, stMSR[iTrack].ucCount, stMSR[iTrack].cpData[0],
                          stMSR[iTrack].cpData[1],stMSR[iTrack].cpData[2],stMSR[iTrack].cpData[3],
                          stMSR[iTrack].cpData[i-4],stMSR[iTrack].cpData[i-3],stMSR[iTrack].cpData[i-2],stMSR[iTrack].cpData[i-1]));
            _TRACE(szDebugMsg);
        }
    }
    debug_sprintf((szDebugMsg, "%s - returned", __FUNCTION__));
    _TRACE(szDebugMsg);
}

int enable_msr(void)
{
	char szDebugMsg[256+1];

    debug_sprintf((szDebugMsg, "%s - m_cardHandle=%d", __FUNCTION__, msr_handle));
    _TRACE(szDebugMsg);
    e=0;
    ledTimer=CreateTimer(0.25,cycle_led, szLed);
     
    msr_enable = 1;
    return msr_enable;
}

int disable_msr(void)
{
	char szDebugMsg[256+1];
    
    debug_sprintf((szDebugMsg, "%s - m_cardHandle=%d", __FUNCTION__, msr_handle));
    _TRACE(szDebugMsg);   
      
    msr_enable = 0;  
    if(ledTimer > 0)
    {
    	CancelTimer(ledTimer); 
    }
    ledOff(7); 
    
    msrClose(); //Closing the MSR thread
    
    bMSRTrapThreadRunning = FALSE;
    return msr_enable;
}

void GoodSwipe_tone()
{
	PlayAudio("Goodswipe.wav", 0, NULL);   
	GoodMsr(); 
    /*if (m_iPlatFormModelNum == Model_Mx870) 
    {
    	PlayAudio("Goodswipe.wav", 0, NULL);    
    }
    else
    {
    	GoodMsr();
    }*/
}     

void BadSwipe_tone()
{
	PlayAudio("msrBadRead.wav", 0, NULL);   
	BadMsr();
   /* if (m_iPlatFormModelNum == Model_Mx870)
    {
//    	PlayAudio("msrBadRead.wav", 0, NULL);    
    	iErrorBeep();
    }
    else
    {
    	BadMsr();
    }*/
}     

	
int cycle_led(void *pData)
{
	
	static char flag;
		
	if(e==0)
	{
		flag=0x8;
	}
	e=e|flag;	
	e>>=1;
	e=e&0x7;	
	ledOff(7);
	ledOn(e);	
	if(e==7)
	{
		flag=0;
	}
	return 1;  	
}

