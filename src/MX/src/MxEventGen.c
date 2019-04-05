
#include <pthread.h>
#include <svc.h>
//#include <COMReceiver.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "main.h"
#include "TCPIP.h"
#include "MESimulator.h"
#include "debug.h"
#include "EventQueue.h"
#include "General.h"
#include <svcsec.h>

#include "guimgr.h"

#define FS	 28
#define STX  2
#define ETX  3

#define MAX_FUEL_PUMPS 36

int giGetCardData = 0;

pthread_t event_thread,sp_thread;
pthread_t multicast_thread;
static Evas *e;
Evas_Event_Mouse_Up mouse_up;
Evas_Event_Mouse_Down mouse_down;
Evas_Event_Mouse_In mouse_in;
Evas_Event_Mouse_Out mouse_out;
static void OnConnectCallbackTCPIP(char *szIPAddress); 
static void OnTCPIPDataAvailable(unsigned char *pchData, int iDataSize, int flags);
static void OnSocketDisconnect(void);
static int SendSuccessResponse();
int OnSerialDataAvailable(void *,int);
void *InitSerialPortThread(void *pData);
//static pthread_t ptMSRTrapThread;
void *MSRReadThread(void *vptr);
void *MulticastProcThread(void *pData);
//static unsigned char m_bMSRTrapThreadRunning = FALSE;
TCPIP_INIT_PARAMS tcpip_params;
int data_rcvd_tcp = FALSE,data_rcvd_sp = FALSE,HandleDownEvent = FALSE;
int MSRReadEvent=FALSE;
int x=0,y=0;
int card_handle;
unsigned int card_swiped = 0;   
int led_cnt = 3;
char client_ip[25];
int gbAutoMsr=1; 
unsigned char gcAutoMsrData[256]=""; 
char gszCommandType[128]="";
char gcPinAccount[32]="0123456789";
int (*pinCallback)(IPP_CALLBACK_DATA *) ;
void (*getDecimalCallback)(char *,int ,void *);
KEYPADEVENTDATA *pstEventData = NULL;
IPP_CALLBACK_DATA *pIPPData = NULL;

extern void ipp_pinEntryTest(char *, char *, char *);
extern unsigned char gszParsedTrackData[512];

#define MULTICAST_IP   "225.0.0.1"
#define MULTICAST_PORT 4446

float gfFuelStopAmount[MAX_FUEL_PUMPS];
int giFuelAmountCommand[MAX_FUEL_PUMPS];

char gszServerPortNumber[5+1];	

char gszCurrentAction[10];
char gszActionData[256];

char getDec[11][6]={"157196","078078","160075","240077","078117","160116","240118","078157","160157","240157","240194"};
	
int trackLen = 0;

void formatPINBlock(char *);

extern void SelectDukptEngine(int iEngine);
extern int InitMagSwipe(void);

static int ParseIncomingPacket(char *pszData, int iDataSize);
static void PrepareMulticastMessage(char *pszMessage);
static int IsMessageFromAutomn(char *pszMessage);

double HandleDownAmount=0;
//double HandleDownAmount[37]; 
//static int PumpNo;

#define TRUE 1
#define FALSE 0

int InitMouseEventSimulator(void *pData)
{
	char szSection[10];
	char szData[10];
	int iRetVal;
	int bRetVal = 0;
	
	memset(szSection ,0x00, sizeof(szSection));		
	memset(szData, 0x00, sizeof(szData));
	memcpy(szSection, "perm", 4);
	
	#ifdef DEBUG
		initEventGenDebugger();
	#endif
	
	debug_sprintf((gszDebugMessage, " %s - InitMouseEventSimulator called", __FUNCTION__));
    _TRACE(gszDebugMessage);
	
	/* Gururaj: For Input Pin Pad operation */
	bRetVal = ippOpen();
    debug_sprintf((gszDebugMessage, "ippOpen rslt = %d", bRetVal));
	_TRACE(gszDebugMessage);
	//printf("ippOpen rslt = %d\n", bRetVal);
	SelectDukptEngine(0);
	/* End: Gururaj */
	e = (Evas *)pData;
	initialiseMxEventQueue();
	
	iRetVal = pthread_create(&event_thread,NULL,EventProcThread,NULL);
	
	debug_sprintf((gszDebugMessage, " %s - Thread created for TCPIP", __FUNCTION__));
    _TRACE(gszDebugMessage);
    
	if(iRetVal == -1)
	{
		e = NULL;
		return -1;
	}
	
	iRetVal = pthread_create(&multicast_thread, NULL, MulticastProcThread, NULL);
	
	if(iRetVal == -1)
	{
		debug_sprintf((gszDebugMessage, " %s - Error while creating Thread created for Multicast Processing", __FUNCTION__));
    	_TRACE(gszDebugMessage);
		e = NULL;
		return -1;
	}
	debug_sprintf((gszDebugMessage, " %s - Thread created for Multicast Processing", __FUNCTION__));
    _TRACE(gszDebugMessage);
    
	debug_sprintf((gszDebugMessage, "%s - Evas pointer = %p",__FUNCTION__,e));
	_TRACE(gszDebugMessage);		
	return 0;
}


void GenerateMouseEvents(short x,short y)
{	
	
	debug_sprintf((gszDebugMessage, "%s - Entered",__FUNCTION__));
	 _TRACE(gszDebugMessage);	
	 
	debug_sprintf((gszDebugMessage, "%s - Coordinates : x = %d y = %d",__FUNCTION__, x,y));
	 _TRACE(gszDebugMessage);	
	 
	e->pointer.x = x;
	e->pointer.y = y;
	
	e->pointer.canvas_x = x;
	e->pointer.canvas_y = y;
	debug_sprintf((gszDebugMessage, "%s - Mouse event is going to start",__FUNCTION__));
	_TRACE(gszDebugMessage);
		
	evas_event_feed_mouse_out(e,&mouse_out); 
	debug_sprintf((gszDebugMessage, "%s - Mouse event-- mouse_out",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	evas_event_feed_mouse_in(e,&mouse_in);
	debug_sprintf((gszDebugMessage, "%s - Mouse event-- mouse_in",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	evas_event_feed_mouse_down(e,1,EVAS_BUTTON_NONE,&mouse_down);
	debug_sprintf((gszDebugMessage, "%s - Mouse event-- mouse_down",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	evas_event_feed_mouse_up(e,1,EVAS_BUTTON_NONE,&mouse_up);
	debug_sprintf((gszDebugMessage, "%s - Mouse event ended",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	//HandleDownAmount = 0;
}


int SendTCPIPData(char * buf, int len)
{	
	char data[255];
	char cLrc=0;
	int i=0;	
	int iRetVal = 0;
	
	debug_sprintf((gszDebugMessage, "%s - Entered",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	memset(data,0,sizeof(data));
	data[0]=2;
	memcpy(&data[1],buf,len);
	data[len+1]=3;
	for(i=1;i<=len+1;i++)
		cLrc=cLrc^data[i];
	data[len+2]=cLrc;
	
	debug_sprintf((gszDebugMessage, "%s - Sending Message %s",__FUNCTION__,data));
	_TRACE(gszDebugMessage);
	
	//sendMessage(client_ip, "Hello", 5,1,3);  
	iRetVal = sendMessage(client_ip, data, len+3,1,3);
	
	debug_sprintf((gszDebugMessage, "%s - The return value of sendMessage is  %d",__FUNCTION__,iRetVal));
	_TRACE(gszDebugMessage);
	
	return iRetVal;
}

void *MulticastProcThread(void *pData)
{
	struct sockaddr_in addr;
    int fd, iRetVal;
    int iBytesRead = 0;
    struct ip_mreq mreq;
    char szSendMessage[256];
    char szReceiveMessage[256];
    u_int yes=1;  
    int iAddrLength = 0;
    
	debug_sprintf((gszDebugMessage, "%s - Multicast Processor thread started",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	memset(szSendMessage, 0x00, sizeof(szSendMessage));
	
	/* create what looks like an ordinary UDP socket */
	fd = socket(AF_INET,SOCK_DGRAM,0);
	if (fd < 0) 
	{
		debug_sprintf((gszDebugMessage, "%s - Error while creating the socket",__FUNCTION__));
		_TRACE(gszDebugMessage);
	  	
	  	perror("socket");
	  	
	  	return 0;
	}
	
	/* set up destination address */
    memset(&addr,0x00,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(MULTICAST_IP);
    addr.sin_port=htons(MULTICAST_PORT);
	
	PrepareMulticastMessage(szSendMessage);
	
	debug_sprintf((gszDebugMessage, "%s - Message to be sent is [%s]",__FUNCTION__, szSendMessage));
	_TRACE(gszDebugMessage);
	
	/* Sending message */
	iRetVal=sendto(fd, szSendMessage, strlen(szSendMessage), 0, (struct sockaddr *) &addr, sizeof(addr));
	
	if(iRetVal < 0)
	{
		debug_sprintf((gszDebugMessage, "%s - Failure while sending message",__FUNCTION__));
		_TRACE(gszDebugMessage);
		perror("sendto");
		return 0;
	}
	
	close(fd); //Close the socket..we are done with the writing to the socket
	
	//Opening the socket for the listening purpose
	
	fd = socket(AF_INET,SOCK_DGRAM,0);
	
	if (fd < 0) 
	{
		debug_sprintf((gszDebugMessage, "%s - Error while creating the socket",__FUNCTION__));
		_TRACE(gszDebugMessage);
	  	perror("socket");
	  	return 0;
	}
	
	/* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) 
    {
    	debug_sprintf((gszDebugMessage, "%s - Set Socket options failed while setting for ADDR",__FUNCTION__));
		_TRACE(gszDebugMessage);
       	perror("Reusing ADDR failed");
       	return 0;
    }
	/* set up destination address */
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
    addr.sin_port=htons(MULTICAST_PORT);
    
    /* bind to receive address */
    if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) 
    {
    	debug_sprintf((gszDebugMessage, "%s - Error while binding",__FUNCTION__));
		_TRACE(gszDebugMessage);
		perror("bind");
	  	return 0;
    }
    
    /* use setsockopt() to request that the kernel join a multicast group */
    mreq.imr_multiaddr.s_addr=inet_addr(MULTICAST_IP);
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if (setsockopt(fd,IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) 
    {
     	debug_sprintf((gszDebugMessage, "%s - Set Socket options failed while adding for multicast group",__FUNCTION__));
		_TRACE(gszDebugMessage);
	 	perror("setsockopt");
	 	return 0;
    }
     
	while(1)
	{
		iAddrLength = sizeof(addr);
		
		memset(szReceiveMessage, 0x00, sizeof(szReceiveMessage));
		iBytesRead = recvfrom(fd, szReceiveMessage, sizeof(szReceiveMessage), 0, (struct sockaddr *) &addr, &iAddrLength);
		
	  	if(iBytesRead < 0) 
	  	{
	  		debug_sprintf((gszDebugMessage, "%s - Error while listening",__FUNCTION__));
			_TRACE(gszDebugMessage);
	        perror("recvfrom");
	       	return 0;
	  	}
	  	else if(iBytesRead == 0)
	  	{
	  		debug_sprintf((gszDebugMessage, "%s - Connection closed other half!!!!",__FUNCTION__));
			_TRACE(gszDebugMessage);
	  	}
	  	else
	  	{
	  		debug_sprintf((gszDebugMessage, "%s - Received Message is %s",__FUNCTION__, szReceiveMessage));
			_TRACE(gszDebugMessage);
			iRetVal = IsMessageFromAutomn(szReceiveMessage);
			if(iRetVal == 1)
			{
				debug_sprintf((gszDebugMessage, "%s - Received Multicast message from Automn",__FUNCTION__));
				_TRACE(gszDebugMessage);
				//Need to respond with our message
				memset(szSendMessage, 0x00, sizeof(szSendMessage));
				PrepareMulticastMessage(szSendMessage);
				
				debug_sprintf((gszDebugMessage, "%s - Prepared Message is %s",__FUNCTION__, szSendMessage));
				_TRACE(gszDebugMessage);
				
				/* Sending message */
				iRetVal=sendto(fd, szSendMessage, strlen(szSendMessage), 0, (struct sockaddr *) &addr, sizeof(addr));
	
				if(iRetVal < 0)
				{
					debug_sprintf((gszDebugMessage, "%s - Failure while sending message",__FUNCTION__));
					_TRACE(gszDebugMessage);
					perror("sendto");
					return 0;
				}
				else
				{
					debug_sprintf((gszDebugMessage, "%s - Send the message succesfully", __FUNCTION__));
					_TRACE(gszDebugMessage);
				}
			}
			else
			{
				debug_sprintf((gszDebugMessage, "%s - Received Multicast message from some other device (Not Automn)",__FUNCTION__));
				_TRACE(gszDebugMessage);
			}
	  	}
	  	svcWait(1000);
	}
	return 0;
}

void *EventProcThread(void *pData)
{
	char szData[256];	
	int iLength = 0;
	int iRetVal;
	char szTemp[50];
	char szBuffer[100];
	char szDbgIP[56+1];    // IP address in nnn.nnn.nnn.nnn format -or- DomainName
    char szDbgPort[5+1];
    //char *pchTemp;
    char buffer[200];
    char szTempBuffer[10];
    //int iBufferCount;
    //int iIndex;
    char *pchTemp = NULL;
    int iPumpNumber;
    float fFillFuelAmount;
    
	memset(szDbgIP, 0, sizeof(szDbgIP));
    memset(szDbgPort, 0, sizeof(szDbgPort));
    
	net_conf_t stNetConf;
	
	
	debug_sprintf((gszDebugMessage, "%s - Event Gen Application started",__FUNCTION__));
	_TRACE(gszDebugMessage);	

	iRetVal = netGetConfig(&stNetConf);
	
	debug_sprintf((gszDebugMessage, "%s - netGetConfig returns iRetVal=%d",__FUNCTION__, iRetVal));
    _TRACE(gszDebugMessage);
	
	strcpy(gszServerPortNumber, "9000");
	
	tcpip_params.OnDataAvailable = OnTCPIPDataAvailable;     /* Callback function to call when Host sends us something */
    tcpip_params.OnSocketDisconnect = OnSocketDisconnect;    /* Callback function to call when Host disconnects */
    tcpip_params.nMaxSeverConnections = 1;  /* default to  one 1 connection FROM the Host */
    strncpy(tcpip_params.deviceIP, stNetConf.ipaddr, 16);
    strncpy(tcpip_params.netMask, stNetConf.netmask, 16);
    strncpy(tcpip_params.gatewayIP, stNetConf.gateway, 16);
    strncpy(tcpip_params.dns1IP, stNetConf.dns1, 16);
    strncpy(tcpip_params.dns2IP, stNetConf.dns2, 16);
	
	//Praveen_P1 : Taking debugging IP from the env variable
	pchTemp = getenv("DBGIP");
	if(pchTemp != NULL)
	{
		strcpy(szDbgIP, pchTemp);
	}
	pchTemp = getenv("DGBPRT");
	if(pchTemp != NULL)
	{
		strcpy(szDbgPort, pchTemp);
	}
    
	if ((szDbgIP[0] != 0) && (szDbgPort[0] != 0))
    {
    	sprintf((char*)tcpip_params.dbgIPAddress, "%s:%s", szDbgIP, szDbgPort);

        debug_sprintf((gszDebugMessage, "%s - dbgIPAddress=%s", __FUNCTION__, tcpip_params.dbgIPAddress));
        _TRACE(gszDebugMessage);
    }
    else
    {
    	strcpy(tcpip_params.dbgIPAddress,"010.064.122.85:5678"); 
    }
    
    tcpip_params.rxProtocolOFF = FALSE;         /* We DO WANT link-level ACK/NAK protocol */
    tcpip_params.enableHeartBeat=0;
	
    tcpip_params.nMaxClientConnections=5;   
    
    iRetVal = initTCPIP(&tcpip_params, 0);
    debug_sprintf((gszDebugMessage, "%s - initTCPIP returns iRetVal=%d",__FUNCTION__, iRetVal));
    _TRACE(gszDebugMessage);
    
    debug_sprintf((gszDebugMessage, "%s - After initTCPIP(),  *NETIP=[%s] *NETMASK=[%s] *NETGTW=[%s] *NETDNS1=[%s] *NETDNS2=[%s] ", 
       __FUNCTION__, tcpip_params.deviceIP, tcpip_params.netMask, tcpip_params.gatewayIP, tcpip_params.dns1IP, tcpip_params.dns2IP));
    _TRACE(gszDebugMessage);
    if (iRetVal <= 0)
    {
    	debug_sprintf((gszDebugMessage, "%s - initTCPIP failed",__FUNCTION__));
    	_TRACE(gszDebugMessage);
		//printf("initTCPIP failed\n");
        
    }
    iRetVal = startServer(gszServerPortNumber, OnConnectCallbackTCPIP);  
    debug_sprintf((gszDebugMessage, "%s - StartServer returns iRetVal=%d",__FUNCTION__, iRetVal));
    _TRACE(gszDebugMessage);    
   
   while(1)
    {
    	switch(getMxEvent(szData, &iLength))
    	{    		
			/*Automation related events*/
			case MOUSE_EVENT :
				debug_sprintf((gszDebugMessage, "%s - [E]MOUSE_EVENT D[%s] L[%d]", __FUNCTION__, szData , iLength));
			    _TRACE(gszDebugMessage);
			    strncpy(szTemp,&szData[0],3);
			    szTemp[3]=0;
			    x=atoi(szTemp);
			    
			    strncpy(szTemp,&szData[3],3);
			    szTemp[3]=0;
			    y=atoi(szTemp);
				GenerateMouseEvents(x,y);
				//SendResponse(gszCurrentAction, "SUCCESS", "");			
				break;
			case PIN_ENTRY_EVENT:
				debug_sprintf((gszDebugMessage, "%s - [E] PIN_ENTRY_EVENT D[%s] L[%d]", __FUNCTION__, szData , iLength));
			    _TRACE(gszDebugMessage);
			    char amnt[5] = "D1234";/*= "D1234";*/
		
				memset(buffer, 0x00, sizeof(buffer));
				memset(gcPinAccount, 00, sizeof(gcPinAccount));
				
				memcpy(gcPinAccount, szData, iLength);
				
				debug_sprintf((gszDebugMessage, "%s - Account Number is [%s] ", __FUNCTION__, gcPinAccount));
			    _TRACE(gszDebugMessage);
				
				ipp_pinEntryTest(gcPinAccount, amnt, buffer);
				
				debug_sprintf((gszDebugMessage, "%s - Buffer after ipp_pinEntryTest [%s] ", __FUNCTION__, buffer));
			    _TRACE(gszDebugMessage);
		
				formatPINBlock(buffer);
				
				debug_sprintf((gszDebugMessage, "%s - Buffer after formatPINBlock [%s] ", __FUNCTION__, buffer));
			    _TRACE(gszDebugMessage);
				
				if(pIPPData == NULL)
				{
					pIPPData = (IPP_CALLBACK_DATA *)malloc(sizeof(IPP_CALLBACK_DATA));
				}
				
				memset(pIPPData,0x00,sizeof(IPP_CALLBACK_DATA));
				pIPPData->iStatus=10;//strlen(buffer);
				
				memset(pIPPData->szResponsePkt,0x00,sizeof(pIPPData->szResponsePkt));
				memcpy(pIPPData->szResponsePkt,buffer,sizeof(buffer));
		
				pinCallback(pIPPData);
		
				debug_sprintf((gszDebugMessage, "%s - Sending TCP data which is  76 response %s",__FUNCTION__,buffer));
				_TRACE(gszDebugMessage);
				
				//SendResponseToAutomn("SUCCESS", buffer);
					
				//SendTCPIPData(buffer,strlen(buffer));
				break;
			case FUEL_STOP_EVENT :
				debug_sprintf((gszDebugMessage, "%s - [E] FUEL_STOP_EVENT D[%s] L[%d]", __FUNCTION__, szData , iLength));
			    _TRACE(gszDebugMessage);
			    
			    memset(szTempBuffer, 0x00, sizeof(szTempBuffer));
			    
			    pchTemp = strchr(szData, FS);
			    if(pchTemp != NULL)
			    {
			    	debug_sprintf((gszDebugMessage, "%s - Packet contains FS", __FUNCTION__));
			    	_TRACE(gszDebugMessage);
			    	memcpy(szTempBuffer, szData, (pchTemp - szData));
			    }
			    else
			    {
			    	debug_sprintf((gszDebugMessage, "%s - Incoming packet does not get contain FS", __FUNCTION__));
			    	_TRACE(gszDebugMessage);
			    }
			    
			     debug_sprintf((gszDebugMessage, "%s - szTempBuffer is %s", __FUNCTION__, szTempBuffer));
			    _TRACE(gszDebugMessage);
			    
			    iPumpNumber = atoi(szTempBuffer);
			    
			    debug_sprintf((gszDebugMessage, "%s - iPumpNumber = %d", __FUNCTION__, iPumpNumber));
			    _TRACE(gszDebugMessage);
			    
			    pchTemp++;
			    
			     debug_sprintf((gszDebugMessage, "%s - pchTemp after incrementing %s", __FUNCTION__, pchTemp));
			    _TRACE(gszDebugMessage);
			    
			    fFillFuelAmount = atof(pchTemp);
			    
			    debug_sprintf((gszDebugMessage, "%s - fFillFuelAmount = %f", __FUNCTION__, fFillFuelAmount));
			    _TRACE(gszDebugMessage);
			    
			    /*iIndex = 0;
			    while(szData[iIndex] != FS)
			    {
			    	szTempBuffer[iIndex] = szData[iIndex];
			    	iIndex++;
			    } 
			    debug_sprintf((gszDebugMessage, "%s - szTempBuffer is %s", __FUNCTION__, szTempBuffer));
			    _TRACE(gszDebugMessage);
			    iPumpNumber = atoi(szTempBuffer);
			    iIndex++; //At the end of above loop, szData will be pointing to '|'.... ++ will be pointing to starting of the amount
			    fFillFuelAmount = atof(&szData[iIndex]);*/
			    
			    debug_sprintf((gszDebugMessage, "%s - iPumpNumber = %d, fFillFuelAmount = %f", __FUNCTION__, iPumpNumber, fFillFuelAmount));
			    _TRACE(gszDebugMessage);
			    
			    //Copying amount to the global variable so that it is accesible from the application
			    gfFuelStopAmount[iPumpNumber] = fFillFuelAmount;
			    
			    debug_sprintf((gszDebugMessage, "%s - The fuel stop amount is %f", __FUNCTION__, gfFuelStopAmount[iPumpNumber]));
			    _TRACE(gszDebugMessage);
			    
			    //Setting giFuelAmountCommand to 1 so that response for the flow on and flow off is not sent 
			    giFuelAmountCommand[iPumpNumber] = 1;
				//Sending the FLOW_ON event
				GenerateMouseEvents(140,115);	
				break;
			case MAGSWIPE_INIT_EVENT :
				debug_sprintf((gszDebugMessage, "%s - [E]MAGSWIPE_INIT_EVENT D[%s] L[%d]", __FUNCTION__, szData , iLength));
			    _TRACE(gszDebugMessage);
			    if(giGetCardData != 1) //Command is not in progress..so can send
			    {
			    	InitMagSwipe();
					giGetCardData = 1;		
			    }
			    else //Command is in progress...cant process it now
			    {
			    	iRetVal = SendResponseToAutomn("ERROR", "DEVICE IS BUSY");
						
					debug_sprintf((gszDebugMessage, "%s - Return value of SendResponseToAutomn is %d", __FUNCTION__, iRetVal));
    				_TRACE(gszDebugMessage);
			    }
					
				break;
			case AUTOMN_TEST_COMMAND:
				debug_sprintf((gszDebugMessage, "%s - [E]AUTOMN_TEST_COMMAND D[%s] L[%d]", __FUNCTION__, szData , iLength));
			    _TRACE(gszDebugMessage);
				
				iRetVal = SendSuccessResponse();
				
				if(iRetVal == 0)
				{
					debug_sprintf((gszDebugMessage, "%s - Successful in sending response", __FUNCTION__));
			    	_TRACE(gszDebugMessage);
				}
				else
				{
					debug_sprintf((gszDebugMessage, "%s - Failure in sending response", __FUNCTION__));
			    	_TRACE(gszDebugMessage);
				}
					
				break;
			case MSR_DATA_EVENT:
				debug_sprintf((gszDebugMessage, "%s - [E] MSR_DATA_EVENT D[%s] L[%d]", __FUNCTION__, szData , iLength));
			    _TRACE(gszDebugMessage);
			    //printf("MSR_DATA_EVENT,gbAutoMsr=%d ",gbAutoMsr);				
				gbAutoMsr=1;
				//memset(gcAutoMsrData,0x00,sizeof(gcAutoMsrData));
				//memcpy(gcAutoMsrData,szData,sizeof(szData));
				//printf("******* MSR lag = %d and MSR Data %s",gbAutoMsr,gcAutoMsrData);
				break;
				
			case CURRENT_COMMAND:
				debug_sprintf((gszDebugMessage, "%s - [E] CURRENT_COMMAND D[%s] L[%d]", __FUNCTION__, szData , iLength));
			    _TRACE(gszDebugMessage);
			    //printf("gcCommandType %s\n",gcCommandType);
			   	SendResponseToAutomn("SUCCESS", gszCommandType);
			   	//SendTCPIPData(gcCommandType,strlen(gcCommandType));	
				break;			
			case INVALID_COMMAND:
				debug_sprintf((gszDebugMessage, "%s - Received Invalid command", __FUNCTION__));
			    _TRACE(gszDebugMessage);
			   	memset(szBuffer, 0x00, sizeof(szBuffer));
			   	sprintf(szBuffer, "%s%c%s%c%s", gszCurrentAction, FS, "NOT_SUPPORTED", FS, "INVALID COMMAND");
				SendResponseToAutomn("FAILURE", "INVALID COMMAND");			  
				//SendTCPIPData(szBuffer, strlen(szBuffer));	
				break;	
    		default:
    			//_TRACE("In default case of main Event gen thread");
			    svcWait(250);
			    ;
    	}
    }
   
}

static void OnTCPIPDataAvailable(unsigned char *pchData, int iDataSize, int flags)
{
	char szTemp[10];
	int iRetVal = 0;
	int modelNumber = 860;	
	int i=0,x,y,key;
	char tempKeyData[32];

	//char strFormName[] = "NUMERICINPUT860";
	
	debug_sprintf((gszDebugMessage, "%s - Entered",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	debug_sprintf((gszDebugMessage, "%s - Received data is:%s",__FUNCTION__, pchData));
	_TRACE(gszDebugMessage);
	
	memset(gszCurrentAction, 0x00, sizeof(gszCurrentAction));
	memset(gszActionData, 0x00, sizeof(gszActionData));
	
	iRetVal = ParseIncomingPacket(pchData, iDataSize);
	
	if(!strcmp(gszCurrentAction, "AFFM")) //Fill Fuel Amount command
	{
		//HandleDownEvent = TRUE;
		if( setMxEvent(FUEL_STOP_EVENT, gszActionData, strlen(gszActionData)) == FAILURE)
		{
			debug_sprintf((gszDebugMessage, "%s - Unable to set the FUEL_STOP_EVENT",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
		else
		{
			debug_sprintf((gszDebugMessage, "%s - Able to set FUEL_STOP_EVENT",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
	    
	}
	else if(!strcmp(gszCurrentAction, "AGCD")) //Get Card Data
	//else if(pchData[1] == '?')
	{
		debug_sprintf((gszDebugMessage, "Calling InitMagSwipe()"));
	    _TRACE(gszDebugMessage);		
		if( setMxEvent(MAGSWIPE_INIT_EVENT,"" , 0) == FAILURE)
		{
			debug_sprintf((gszDebugMessage, "%s - Unable to set MAGSWIPE_INIT_EVENT",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
		else
		{
			debug_sprintf((gszDebugMessage, "%s - Able to set MAGSWIPE_INIT_EVENT",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
	}
	else if(!strcmp(gszCurrentAction, "ACRE")) //Card Read event
	{
		trackLen = 0;
	   	
	   	trackLen = iDataSize - 8; //Praveen_P1 : 4 for ACRE, 1 for FS and 3 for STX/ETX/LRC
		
		memset(gcAutoMsrData,0x00,sizeof(gcAutoMsrData));
	    memcpy(gcAutoMsrData,gszActionData,sizeof(gcAutoMsrData));
	    
	    gbAutoMsr=1;
	    
		if( setMxEvent(MSR_DATA_EVENT,"" , 0) == FAILURE)
		{
			debug_sprintf((gszDebugMessage, "%s - Unable to set MSR_DATA_EVENT",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
		else
		{
			debug_sprintf((gszDebugMessage, "%s - Able to set MSR_DATA_EVENT",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
	}
	else if(!strcmp(gszCurrentAction, "AGCS")) //Get Current State
	{		
		if(setMxEvent(CURRENT_COMMAND,"" , 0) == FAILURE)
		{
			debug_sprintf((gszDebugMessage, "%s - Able to set CURRENT_COMMAND",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
		debug_sprintf((gszDebugMessage, "%s - Current state is %s",__FUNCTION__,gszCommandType));
		_TRACE(gszDebugMessage);
	}
	else if(!strcmp(gszCurrentAction, "APEE")) //Pin Entry Event
	{
		
		if( setMxEvent(PIN_ENTRY_EVENT,gszActionData , strlen(gszActionData)) == FAILURE)
		{
			debug_sprintf((gszDebugMessage, "%s - Unable to set PIN_ENTRY_EVENT",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
		else
		{
			debug_sprintf((gszDebugMessage, "%s - Able to set PIN_ENTRY_EVENT",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
	}
	else if(!strcmp(gszCurrentAction, "ATCE")) //This command is only for the testing
	{
		
		if( setMxEvent(AUTOMN_TEST_COMMAND,gszActionData , strlen(gszActionData)) == FAILURE)
		{
			debug_sprintf((gszDebugMessage, "%s - Unable to set AUTOMN_TEST_COMMAND",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
		else
		{
			debug_sprintf((gszDebugMessage, "%s - Able to set AUTOMN_TEST_COMMAND",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
	}
	else if(!strcmp(gszCurrentAction, "AMEG")) //Mouse Event Generation command
	{			
		if(strcmp(gszCommandType,"GetDecimals")==0)
		{	
			modelNumber = svcInfoPlatform(FEATURE_MODEL);			
			if(modelNumber == 860)
			{						
				if(pstEventData == NULL)
				pstEventData = (KEYPADEVENTDATA *)malloc(sizeof(KEYPADEVENTDATA));						
				memset(pstEventData->val.pszValue,0x00,sizeof(pstEventData->val.pszValue));
				//printf(" iDataSize = %d",iDataSize);
				pstEventData->iEventType = 2;			
				memcpy(pstEventData->val.pszValue,gszActionData,strlen(gszActionData));
				getDecimalCallback("NUMERICINPUT860",1,pstEventData);
			}
			else if(modelNumber == 870)
			{
				memset(tempKeyData,0x00,sizeof(tempKeyData));
				memcpy(tempKeyData,gszActionData,strlen(gszActionData));
				memset(szTemp,0x00,sizeof(szTemp));				
				while(i<strlen(tempKeyData))
				{					
					key = (int)tempKeyData[i++];
					key = key - 48;					
					strncpy(szTemp,&getDec[key][0],3);
			    	szTemp[3]=0;			    
			    	x=atoi(szTemp);
			    
			    	strncpy(szTemp,&getDec[key][3],3);
			    	szTemp[3]=0;
			    	y=atoi(szTemp);
			    			  
			    	GenerateMouseEvents(x,y);			    	
	    	   		svcWait(250);
				}
				GenerateMouseEvents(240,196);
			}	
		}
		else if( setMxEvent(MOUSE_EVENT,gszActionData, 6) == FAILURE)
		{
			_TRACE("Unable to set the MOUSE_EVENT");
		}
		_TRACE("MOUSE_EVENT");	
	}
	else
	{
		_TRACE("Invalid command");
		if(setMxEvent(INVALID_COMMAND,"" , 0) == FAILURE)
		{
			debug_sprintf((gszDebugMessage, "%s - Unable to set INVALID_COMMAND",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
		else
		{
			debug_sprintf((gszDebugMessage, "%s - Able to set INVALID_COMMAND",__FUNCTION__));
			_TRACE(gszDebugMessage);
		}
	}  
}

static int IsMessageFromAutomn(char *pszMessage)
{
	int  iRetVal  = 0;
	char *pchTemp = NULL;
	
	debug_sprintf((gszDebugMessage, "%s - Entered",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	debug_sprintf((gszDebugMessage, "%s - Message Received is %s",__FUNCTION__, pszMessage));
	_TRACE(gszDebugMessage);
	
	pchTemp = strstr(pszMessage, "AUTOMN");
	if(pchTemp == NULL)
	{
		debug_sprintf((gszDebugMessage, "%s - Message does not contain AUTOMN",__FUNCTION__));
		_TRACE(gszDebugMessage);
		iRetVal = 0;
	}
	else
	{
		debug_sprintf((gszDebugMessage, "%s - Message contains AUTOMN",__FUNCTION__));
		_TRACE(gszDebugMessage);
		iRetVal = 1;
	}
	
	debug_sprintf((gszDebugMessage, "%s - Returning with %d",__FUNCTION__, iRetVal));
	_TRACE(gszDebugMessage);
	
	return iRetVal;
}

static void PrepareMulticastMessage(char *pszMessage)
{
	char szAppName[20];
	char szTempBuffer[50];
	char szIpAddress[20];
	char *pszTemp = NULL;
	int iRetVal = 0;
	memset(szAppName, 0x00, sizeof(szAppName));
	net_conf_t stNetConf;
	
	debug_sprintf((gszDebugMessage, "%s - Entered",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	memset(szAppName, 0x00, sizeof(szAppName));
	memset(szTempBuffer, 0x00, sizeof(szTempBuffer));
	memset(szIpAddress, 0x00, sizeof(szIpAddress));
	/*----Step1 : Need to get the Application Name -----*/
	
	//Getting the go variable name
	iRetVal = getEnvFile("perm", "go", szTempBuffer, sizeof(szTempBuffer));
	
	debug_sprintf((gszDebugMessage, "%s - go variable set to %s",__FUNCTION__, szTempBuffer));
	_TRACE(gszDebugMessage);
	
	pszTemp = strstr(szTempBuffer, "."); //Checking whether .exe presents in the app name
	if(pszTemp != NULL)
	{
		debug_sprintf((gszDebugMessage, "%s - App name contains .exe, need to remove",__FUNCTION__));
		_TRACE(gszDebugMessage);
		
		debug_sprintf((gszDebugMessage, "%s - Pointer is pointing to %s",__FUNCTION__, pszTemp));
		_TRACE(gszDebugMessage);
		
		debug_sprintf((gszDebugMessage, "%s - Length to be copied is %d",__FUNCTION__, (pszTemp - szTempBuffer)));
		_TRACE(gszDebugMessage);
		
		memcpy(szAppName, szTempBuffer, (pszTemp - szTempBuffer));
	}
	else
	{
		debug_sprintf((gszDebugMessage, "%s - App name does not contain .exe",__FUNCTION__));
		_TRACE(gszDebugMessage);
		memcpy(szAppName, szTempBuffer, strlen(szTempBuffer));
	}
	
	debug_sprintf((gszDebugMessage, "%s - App Name is %s",__FUNCTION__, szAppName));
	_TRACE(gszDebugMessage);
	
	/*----Step2 : Need to get IP address -----*/
	
	iRetVal = netGetConfig(&stNetConf); //calling netGetConfig() to get our true IP settings directly from the device.
	
	strncpy(szIpAddress, stNetConf.ipaddr, 16);
	
	debug_sprintf((gszDebugMessage, "%s - Ip Address is %s",__FUNCTION__, szIpAddress));
	_TRACE(gszDebugMessage);
	
	/*-----Step3: Port Number -------*/
	
	debug_sprintf((gszDebugMessage, "%s - Terminal(server) Port number is %s",__FUNCTION__, gszServerPortNumber));
	_TRACE(gszDebugMessage);
	
	/* ---- Message Format : APP Name<FS>IP Address<FS>Port Number -----*/
	sprintf(pszMessage, "%s%c%s%c%s", szAppName, FS, szIpAddress, FS, gszServerPortNumber);
	
	debug_sprintf((gszDebugMessage, "%s - Returning...Message %s",__FUNCTION__, pszMessage));
	_TRACE(gszDebugMessage);
}

static int SendSuccessResponse()
{
	int iRetVal = -1;
	
	debug_sprintf((gszDebugMessage, "%s - Entered",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	iRetVal = SendResponseToAutomn("SUCCESS", "TEST COMMAND SUCCESSFUL");
	
	debug_sprintf((gszDebugMessage, "%s - Return value of SendResponseToAutomn is %d",__FUNCTION__, iRetVal));
	_TRACE(gszDebugMessage);
	
	return iRetVal;
}

static int ParseIncomingPacket(char *pszData, int iDataSize)
{
	int iRetVal = 0;
	char *pch;
	char szDelimiter[5];
	
	debug_sprintf((gszDebugMessage, "%s - Entered",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	debug_sprintf((gszDebugMessage, "%s - Going to Parse:[%s]",__FUNCTION__, pszData));
	_TRACE(gszDebugMessage);
	
	memset(szDelimiter, 0x00, sizeof(szDelimiter));
	szDelimiter[0] = FS;
	
	pch = strtok(pszData, szDelimiter);
	if(pch == NULL)
	{
		debug_sprintf((gszDebugMessage, "%s - Incorrect Packet",__FUNCTION__));
		_TRACE(gszDebugMessage);
		return iRetVal;
	}
	strcpy(gszCurrentAction, &pch[1]);
	memset(szDelimiter, 0x00, sizeof(szDelimiter));
	szDelimiter[0] = ETX;
	
	pch = strtok(NULL, szDelimiter);
	if(pch == NULL)
	{
		debug_sprintf((gszDebugMessage, "%s - Incorrect Packet",__FUNCTION__));
		_TRACE(gszDebugMessage);
		return iRetVal;
	}
	memcpy(gszActionData, pch, sizeof(gszActionData));
	iRetVal = 1;
	
	debug_sprintf((gszDebugMessage, "%s - Action is [%s], Data is [%s]",__FUNCTION__, gszCurrentAction, gszActionData));
	_TRACE(gszDebugMessage);
	
	return iRetVal;
}

int SendResponseToAutomn(char *pszResult, char *pszDescription)
{
	char szPacket[512];
	int iRetVal = -1;
	
	debug_sprintf((gszDebugMessage, "%s - Entered",__FUNCTION__));
	_TRACE(gszDebugMessage);
	
	if(!strlen(pszResult))
	{
		debug_sprintf((gszDebugMessage, "%s - Action/Result is not mentioned",__FUNCTION__));
		_TRACE(gszDebugMessage);
		return 0;
	}
	memset(szPacket, 0x00, sizeof(szPacket));
	sprintf(szPacket, "%s%c%s%c%s", gszCurrentAction, FS, pszResult, FS, pszDescription);
	debug_sprintf((gszDebugMessage, "%s - Response Packet is %s",__FUNCTION__, szPacket));
	_TRACE(gszDebugMessage);
	debug_sprintf((gszDebugMessage, "%s - Length of the packet is %d",__FUNCTION__, strlen(szPacket)));
	_TRACE(gszDebugMessage);
	
	iRetVal = SendTCPIPData(szPacket, strlen(szPacket));
	
	debug_sprintf((gszDebugMessage, "%s - The RetVal for SendTCPIPData is %d",__FUNCTION__, iRetVal));
	_TRACE(gszDebugMessage);
	
	if(iRetVal != -1)
	{
		memset(gszCurrentAction, 0x00, sizeof(gszCurrentAction));
	}
	
	debug_sprintf((gszDebugMessage, "%s - Returning with %d",__FUNCTION__, iRetVal));
	_TRACE(gszDebugMessage);
	
	return iRetVal;
}

void formatPINBlock(char *pinData)
{
	char temp[200];
	int i = 0,j=0;
	
	debug_sprintf((gszDebugMessage, "%s - PinData is %s",__FUNCTION__, pinData));
	_TRACE(gszDebugMessage);
	
	memset(temp,0x00,sizeof(temp));
	temp[i++]=pinData[0];
	temp[i++]='7';
	temp[i++]='3';
	temp[i++]='.';
	temp[i++]='0';
	temp[i++]='0';
	temp[i++]='0';
	temp[i++]='0';
	temp[i++]='0';
	j=4;
	while(j<strlen(pinData)){
		temp[i++]=pinData[j++];		
	}
	
	debug_sprintf((gszDebugMessage, "%s - Temp is %s",__FUNCTION__, temp));
	_TRACE(gszDebugMessage);
	
	memset(pinData,0x00,sizeof(pinData));
	
	strcpy(pinData,temp);
	
	debug_sprintf((gszDebugMessage, "%s - PinData is %s",__FUNCTION__, pinData));
	_TRACE(gszDebugMessage);
	//printf("Pin Data = %s\n",pinData);
}

static void OnSocketDisconnect(void)
{           
 
  debug_sprintf((gszDebugMessage, "Socket Disconnected"));
   _TRACE(gszDebugMessage);
}

static void OnConnectCallbackTCPIP(char *szIPAddress)
{	debug_sprintf((gszDebugMessage, "Accepted Connection from %s",szIPAddress));
    _TRACE(gszDebugMessage);  
    memset(client_ip,0,sizeof(client_ip));
    strcpy(client_ip,szIPAddress); 
}



void LogData(const char *pchData, int iDataLen, char aDirection)
{
}
void HandleDown()
{
	
}





