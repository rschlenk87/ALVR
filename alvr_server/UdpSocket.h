#pragma once

#include <string>
#include <memory>
#include <vector>
#include <list>
#include "Poller.h"
#include "Statistics.h"
#include "Utils.h"
#include "ThrottlingBuffer.h"
#include "UdpInterface.h"

#define CONTROL_NAMED_PIPE "\\\\.\\pipe\\RemoteGlass_Control"

class UdpSocket: public UdpInterface
{
public:
	UdpSocket(std::string host, int port, std::shared_ptr<Poller> poller, std::shared_ptr<Statistics> statistics, const Bitrate &bitrate);
	virtual ~UdpSocket();

	 bool Startup();
	 void Shutdown();


	 bool Recv(char *buf, int *buflen, sockaddr_in *addr, int addrlen);

	 bool Send(char *buf, int len, uint64_t frameIndex = 0);

	 void Run();

	bool BindSocket();

private:
	
	std::shared_ptr<Poller> mPoller;
	
	ThrottlingBuffer mBuffer;

	bool DoSend(char *buf, int len);
};

