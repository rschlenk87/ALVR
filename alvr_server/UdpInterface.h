#pragma once
#include "Utils.h"
#include <functional>
#include "ThrottlingBuffer.h"
#include "Statistics.h"

class UdpInterface
{

public:
	
	virtual ~UdpInterface() {}

	 virtual bool Send(char* buf, int len, uint64_t frameIndex = 0) = 0;
	 virtual bool Recv(char* buf, int* buflen, sockaddr_in* addr, int addrlen) { return true; };

	 virtual void Run() = 0;

	 virtual void Shutdown() = 0;
	 virtual bool Startup() = 0;


	 virtual bool IsLegitClient(const sockaddr_in* addr);
	 virtual void InvalidateClient();
	 virtual bool IsClientValid() const;

	 sockaddr_in GetClientAddr() const;

	 virtual void SetClientAddr(const sockaddr_in* addr);



	 virtual void setPacketCallback(std::function<void(char*, int, sockaddr_in*)> callback) {};
protected:
	std::string mHost;
	int mPort;

	SOCKET mSocket;
	sockaddr_in mClientAddr;
	std::shared_ptr<Statistics> mStatistics;
};