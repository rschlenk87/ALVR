#pragma once
#include "threadtools.h"
#include "Statistics.h"
#include "Utils.h"
#include "ThrottlingBuffer.h"
#include "Logger.h"
#include <functional>


class UdpThread : public CThread
{
public:
	UdpThread(std::string host, int port, std::shared_ptr<Statistics> statistics, const Bitrate& bitrate);
	virtual ~UdpThread();


	virtual bool Send(char* buf, int len, uint64_t frameIndex = 0);

	virtual void Run();

	virtual void Shutdown();


	bool IsLegitClient(const sockaddr_in* addr);
	void InvalidateClient();
	virtual bool IsClientValid()const;

	virtual sockaddr_in GetClientAddr()const;
	void SetClientAddr(const sockaddr_in* addr);

	void setPacketCallback(std::function<void(char*, int ,sockaddr_in*)> callback);


private:
	bool BindSocket();


	std::function<void(char*, int, sockaddr_in*)> m_packetCallback;
	std::string mHost;
	int mPort;

	SOCKET mSocket;
	sockaddr_in mClientAddr;
	std::shared_ptr<Statistics> mStatistics;

	ThrottlingBuffer mBuffer;

	bool m_bExit;

};
