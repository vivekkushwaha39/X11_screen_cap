/*
 * CommChannel.h
 *
 *  Created on: 28-Jan-2019
 *      Author: VivekK4
 */

#ifndef SRC_COMMCHANNEL_H_
#define SRC_COMMCHANNEL_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

#define COMMUNICATION_PORT 8989

class CommChannel {
private:
	sockaddr_in serv_addr, clientAddress;
	int sock, clientSocket;
	socklen_t clientSockLen;
public:
	CommChannel();
	virtual ~CommChannel();
	bool Connect();
	bool Disconnect();
	void RecvData();
	bool Init();
};

#endif /* SRC_COMMCHANNEL_H_ */
