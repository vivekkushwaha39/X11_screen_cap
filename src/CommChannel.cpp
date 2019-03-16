/*
 * CommChannel.cpp
 *
 *  Created on: 28-Jan-2019
 *      Author: VivekK4
 */

#include "CommChannel.h"
#include <string.h>
CommChannel::CommChannel()
{
	memset(&sock, 0x00, sizeof sock);
	memset(&clientSocket, 0x00, sizeof clientSocket);
	clientSockLen = sizeof(clientAddress);
}

CommChannel::~CommChannel()
{
	// TODO Auto-generated destructor stub
	memset(&sock, 0x00, sizeof(sock));
}

bool CommChannel::Connect()
{
	syslog(LOG_DEBUG, "Listening for client");
	if ( listen(sock, 1) < 0)
	{
		syslog(LOG_DEBUG, "Listen failed");
		return false;
	}
	int retryCount = 0;
	int isConnected = -1;

	if (  (clientSocket = accept(sock, (sockaddr *)&clientAddress, &clientSockLen )) > 0  )
	{
		syslog(LOG_DEBUG, "Connected.....");
	}
	else
	{
		syslog(LOG_DEBUG, "Connection Failed");
	}


	if ( isConnected < 0 )
		return false;
	else
		return true;
}
bool CommChannel::Disconnect()
{

	clientSocket != 0 ?  close(clientSocket) : 0 ;
	sock != 0 ? close(sock) : 0;
	return true;
}

bool CommChannel::Init()
{

	bool res = true;

	if ( (sock = socket(AF_INET, SOCK_STREAM, 0))  < 0 )
	{
		res = false;
		syslog(LOG_ERR, "Unable to create socket");
		goto INIT_END;
	}

	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(COMMUNICATION_PORT);

	memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero);


	if ( bind(sock, (sockaddr * )&serv_addr, sizeof(serv_addr)) < 0 )
	{
		res = false;
		syslog(LOG_DEBUG, "Unable to bind socket");
		goto INIT_END;
	}

INIT_END:
	return res;
}
