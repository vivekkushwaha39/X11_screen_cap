#ifndef _TCPIPDBG_H
#define _TCPIPDBG_H

#define TCP                 1
#define UDP                 2

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* Program startup modes */
#define INVALID_SOCKET      -1
#define MAX_DEBUG_MSG       512+1 /*1024+1*/

/* Open debug socket to debug host (SocketListener) */
extern int  iOpenDebugSocket(void);

/* Close debug socket */
extern int  iCloseDebugSocket(void);

/* Send null-terminated debug string to debug host (SocketListener) */
extern void DebugMsg(char *pszMessage);

/* Send debug string of specified length to debug host (SocketListener) */
extern void DebugMsgEx(unsigned char *puchMessage, unsigned short ushMsgLen);

#endif /* _TCPIPDBG_H */


