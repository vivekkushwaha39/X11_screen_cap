/*
 * CommChannel.cpp
 *
 *  Created on: 28-Jan-2019
 *      Author: VivekK4
 */

#include "CommChannel.h"
#include <string.h>
CommChannel::CommChannel() {

}

CommChannel::~CommChannel()
{
	// TODO Auto-generated destructor stub
	memset(&sock, 0x00, sizeof(sock));
}

bool CommChannel::Connect()
{
	int retryCount = 0;
	int isConnected = -1;
	while ( retryCount++ < 100 && ( isConnected = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ) < 0  )
	{
		printf("\nConnection Failed retry = %d\n",retryCount);
		sleep(1);
	}
	if ( isConnected < 0 )
		return false;
	else
		return true;
}
bool CommChannel::Disconnect()
{
	close(sock);
	return true;
}

bool CommChannel::Init()
{

	bool res = true;

	if ( (sock = socket(AF_INET, SOCK_STREAM, 0))  < 0 )
	{
		res = false;
		goto INIT_END;
	}

	serv_addr.sin_addr;


INIT_END:
	return res;

	return true;
}
