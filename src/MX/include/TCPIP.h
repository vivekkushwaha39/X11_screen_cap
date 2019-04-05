#ifndef __TCPIP_H
#define __TCPIP_H

/******************************************************************
*                       TCPIP.H                                   *
*******************************************************************
* Application: Core library for TCPIP communication.              *  
* Platform:    Mx870                                              *
* Language:    Linux                                              *
* Lib used:    none                                               *
* Purpose:     Defines and funtion prototypes used throughout     * 
*              the TCPIPLib project.                              *
*                                                                 *
* History:                                                        *
* Date     Ver   Developer     Description                        *
* -------- ----  ------------  -----------------------------      *
* 01/30/03 001   Sadiq_M1         Initial release                   *
*                                                                 *
*                                                                 *
* ================================================================*
*                   Copyright, 1995 - 2005 VeriFone, Inc.         *
*                   2455 Augustine Drive                          *
*                   Santa Clara, CA 95054                         *
*                                                                 *
*                   All Rights Reserved.                          *
* ================================================================*/

typedef struct TAG_TCPIP_INIT_PARAMS
{
    void (*OnDataAvailable)(unsigned char *pchData, int iDataSize, int flags); /* Call back function to indicate when data is available.
                                            This MUST be set to NULL (0) if not using traps. */
    void (*OnSocketDisconnect)(void);    /* Call back function to indicate when socket has been disconnected.
                                            This MUST be set to NULL (0) if not using traps. */                                    
    unsigned char deviceIP[16];          /* The static IP  address of the OMNI7K, NULL if using DHCP. */
    unsigned char netMask[16];           /* The network mask for LAN. */
    unsigned char gatewayIP[16];         /* The IP address of the LAN, NULL if using DHCP. */ 
    unsigned char dns1IP[16];            /* The IP address of the primary DNS server, NULL if using DHCP. */
    unsigned char dns2IP[16];            /* The IP address of the secondary DNS server, NULL if using DHCP. */
    unsigned char dbgIPAddress[21];      /* The IP address and port number of the debug listener, NULL if not debugging. */
    int rxProtocolOFF;                   /* Flag to turn off main RX command validation and ACK, NAK handshaking. 0 = ON, 1 = OFF */
    int enableHeartBeat;                 /* Flag to turn on the drivers TX/RX heartbeat. 0 = OFF, 1 = ON */
    int serverHBinterval;                /* Interval in seconds between the servers heartbeat TX/RX. */
    int clientHBinterval;                /* Interval in seconds between the clients heartbeat TX/RX. */
    char nMaxSeverConnections;           /* Maximum number of socket connections the server will accept. */ 
    char nMaxClientConnections;          /* Maximum number of client socket connections created. */
}TCPIP_INIT_PARAMS;


// API to get Library version
void GetTCPIPLibVersion(char *pszLibVersion, int len);

/**********************************************************************
* Function: initTCPIP()
*
* Purpose: Initializes the network, assigns OMNI7K's - IPAddress, 
*          gateway address, primary/secondary DNS addresses, network mask,
*          and enables/disables the debug port.
*
* Inputs:  struct TCPIP_INIT_PARAMS *pNetConfig
*          Pointer to a TCPIP_INIT_PARAMS structure.
*          
*          int nEID
*          A unique ID for this device to help distinguish it on the network.
*
* Outputs: N/A.
*
* Returns: 0 - Failure
*          1 - Success
*        100 - The network is already open, #define = EISOPEN 
*
*
* Notes:   To let the DHCP server configure the device's network parameters,
*          null set the following TCPIP_INIT_PARAMS members.
*          
*          deviceIP  
*          netMask
*          gatewayIP
*          dns1IP
*          dns2IP
*
*          Once initTCPIP() is successfully called, all subsequent calls will 
*          fail until a call is made to the method closeTCPIP().
*
***********************************************************************/
int initTCPIP(TCPIP_INIT_PARAMS *pNetConfig, int nEID);

/**********************************************************************
* Function: closeTCPIP()
*
* Purpose: Closes all sockets and queues, disables the network and disarms
*          all traps.
*
* Inputs:  N/A. 
*
* Outputs: N/A.
*
* Returns: N/A.
*
*
***********************************************************************/
void closeTCPIP(void);

/**********************************************************************
* Function: pollTCPIP()
*
* Purpose: Checks for data in the command FIFO (first in first out 
*          buffer).  If data is present, the input buffer is NULL set 
*          and the command is removed from the FIFO and copied into 
*          the buffer.            
*
* Inputs:  unsigned char *buffer
*          A pointer to the buffer that will receive the command. 
*
*          int nSize
*          The size of the buffer that will receive the command.

* Outputs: N/A.
*
* Returns: The number of bytes copied into the buffer.
*
*
* Notes:   If a command is in the FIFO, the IPAddress of the socket that
*          sent the command becomes the default address.  All subsequent 
*          calls to postMessage() or sendMessage() that do not specify a 
*          IPAddress will use this address.
*
***********************************************************************/
int pollTCPIP(unsigned char *buffer, int nSize);

/**********************************************************************
* Function: postMessage()
*
* Purpose: Takes the message length parameter, converts it to a big endian 
*          short (two byte), prepends it to the input message and sends it 
*          to the desired IPAddress. 
*
* Inputs:  char *szIPAddress
*          A pointer to a null terminated string containing the address
*          in which to send the message.  If this value is null the driver 
*          will use address of the last command taken from the command 
*          queue (pollTCPIP method);  
*
*          unsigned char *msg 
*          A pointer to a null terminated string containing the data to send.
*
*          int nLength
*          The length ot the method to send.
*
* Outputs: N/A.
*
* Returns: The number of bytes actually sent.
*
* NOTES:   A successful call will return the lentgh paramater passed in + 2
*          for the prepended short.
*
*
***********************************************************************/
int postMessage(char *szIPAddress, unsigned char *msg, int nLength);

/* 
 * Dom - added 30-Jul-04: 
 * No handshaking, no length prefix (for streaming data to other side)
 * E.g. for streaming a large file - first use the postMessage() message,
 * which will contain the length of the subsequent 'packet'/'record'.
 * The leading postMessage() function will contain only a portion of the 
 * large file; then subsequent portions of the file should be sent using
 * this function.
 */
int postMessageEx(char *szIPAddress, unsigned char *msg, int nLength);

/**********************************************************************
* Function: sendMessage()
*
* Purpose: Takes the message length parameter, converts it to a big endian 
*          short (two byte), prepends it to input message and sends it 
*          to the desired IPAddress. 
*
* Inputs:  char *szIPAddress
*          A pointer to a null terminated string containing the address
*          in which to send the message.  If this value is null the driver 
*          will use address of the last command taken from the command 
*          queue (pollTCPIP method);  
*
*          unsigned char *msg 
*          A pointer to a null terminated string containing the data to send.
*
*          int nLength
*          The length of the message to send.
* 
*          int nTimeoutInterval
*          The time (in seconds) to wait for receiving end to respond with a
*          <ACK>, <NAK>, or <EOT>.
*  
*          int nRetryCount
*          The number of times to send the message, valid only when the the receiving
*          end fails to respond or responds with a <NAK>.  
*
* Outputs: N/A.
*
* Returns: -1  A Network error ocuured.
*           0  The receiving end responded with a ACK.
*           1  The receiving end sent at least one NAK but the retry count expired.
*           2  The receiving end sent a EOT, terminating the hand shake.
*           3  The receiving end failed to respond and the retry count expired.
*
***********************************************************************/
int sendMessage(char *szIPAddress, unsigned char *msg, int nLength, int nTimeoutInterval, int nRetryCount);

/**********************************************************************
* Function: createClient()
*
* Purpose: Creates a new socket and attempts to connect to a server
*          at the supplied address.
*
* Inputs:  char *szHostNameWithPortNumber 
*          A pointer to a null terminated string containing the address 
*          and port number of the host to connect to.  The string must 
*          be in the following format: xxx.xxx.xxx.xxx:nnnn
*
*          void (*OnConnectCallback)(char *szIPAddress)
*          Call back function to indicate when a connection has been established.
*          This must be either a pointer to a valid method or NULL (0).
*          
*
* Outputs: N/A.
*
* Returns: -1 if the parameter is invalid.
*           0 if a network error occurred. 
*           1 if successfull.
*
* Notes:   If the call is unsuccessful due to a network error (return code 0),
*          and the OnConnectCallback parameter is valid, the driver will continue 
*          it's attempt to connect.  Once a connection has been established, 
*          the OnConnectCallback() method will be called, passing the IPAddress 
*          of the new connection. 
*
***********************************************************************/
int createClient(char *szHostNameWithPortNumber, void (*OnConnectCallback)(char *szIPAddress));

/**********************************************************************
* Function: removeClient()
*
* Purpose: Closes the socket connection and removes the client from the 
*          driver's internal queue. 
*
* Inputs:  char *szHostNameWithPortNumber 
*          A pointer to a null terminated string containing the address 
*          and port number of the host to disconnect from.  The string must 
*          be in the following format: xxx.xxx.xxx.xxx:nnnn  
*
* Outputs: N/A.
*
* Returns: N/A
*
*
***********************************************************************/
void removeClient(char *szHostNameWithPortNumber);

/**********************************************************************
* Function: startServer()
*
* Purpose: Creates a socket, binds it, and starts listening on the 
*          supplied port number. 
*
* Inputs:  char *szServerPortNumber 
*          A pointer to a null terminated string containing the port 
*          number to listen on.
*
*          void (*OnConnectCallback)(void)
*          Call back function to indicate when a connection has been accepted.
*          This must be either a pointer to a valid method or NULL (0).
*
* Outputs: N/A.
*
* Returns: -1 if the parameter is invalid.
*           0 if a network error occurred. 
*           1 if successfull.
*
* Notes:   If the OnConnectCallback parameter is valid, the OnConnectCallback() 
*          method will be called for every new connection to the server, The  
*          OnConnectCallback method will supply the IPAddress of the new connection.
*          The call back is essentially an OS interrupt and should not be used to do
*          lengthy routines. 
*
***********************************************************************/
int startServer(char *szServerPortNumber, void (*OnConnectCallback)(char *szIPAddress));


/**********************************************************************
* Function: stopServer()
*
* Purpose: Closes the socket
*
* Inputs:  N/A
*
*
* Outputs: N/A.
*
* Returns: 
*
*
***********************************************************************/
void stopServer(void);

#endif /* __TCPIP_H */








