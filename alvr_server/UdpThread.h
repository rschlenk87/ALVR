#pragma once
#include "threadtools.h"
#include "Statistics.h"
#include "Utils.h"
#include "ThrottlingBuffer.h"
#include "Logger.h"
#include <functional>
#include "UdpInterface.h"


class UdpThread : public CThread, public UdpInterface
{
public:
	UdpThread(std::string host, int port, std::shared_ptr<Statistics> statistics);
	virtual ~UdpThread();


	bool Send(char* buf, int len, uint64_t frameIndex = 0);

	void Run();

	 void Shutdown();
	 bool Startup();


	void setPacketCallback(std::function<void(char*, int ,sockaddr_in*)> callback);


private:
	bool BindSocket();


	std::function<void(char*, int, sockaddr_in*)> m_packetCallback;
	


	bool m_bExit;

};
