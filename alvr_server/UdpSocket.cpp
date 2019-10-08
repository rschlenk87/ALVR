#include "UdpSocket.h"
#include "Logger.h"
#include "Utils.h"
#include "Settings.h"

UdpSocket::UdpSocket(std::string host, int port, std::shared_ptr<Poller> poller, std::shared_ptr<Statistics> statistics, const Bitrate &bitrate)
	: mPoller(poller)
	, mBuffer(bitrate)
	
{
	Log(L"Using polling communication");

	mHost = host;
	mPort = port;
	mSocket = INVALID_SOCKET;
	mStatistics = statistics;


	mClientAddr.sin_family = 0;
}


UdpSocket::~UdpSocket()
{
}

bool UdpSocket::Startup() {
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 0), &wsaData);

	if (!BindSocket()) {
		return false;
	}

	mPoller->AddSocket(mSocket, PollerSocketType::READ);

	Log(L"UdpSocket::Startup success");

	return true;
}


bool UdpSocket::Recv(char *buf, int *buflen, sockaddr_in *addr, int addrlen) {
	bool ret = false;
	if (mPoller->IsPending(mSocket, PollerSocketType::READ)){
		ret = true;

		recvfrom(mSocket, buf, *buflen, 0, (sockaddr *)addr, &addrlen);
	}

	return ret;
}

void UdpSocket::Run()
{
	Log(L"Try to send.");
	while (mBuffer.Send([this](char *buf, int len) {return DoSend(buf, len); })) {}

	if (!mBuffer.IsEmpty()) {
		mPoller->WakeLater(1);
	}
}

bool UdpSocket::Send(char *buf, int len, uint64_t frameIndex) {
	if (!IsClientValid()) {
		return false;
	}
	mBuffer.Push(buf, len, frameIndex);

	return true;
}

void UdpSocket::Shutdown() {
	if (mSocket != INVALID_SOCKET) {
		closesocket(mSocket);
	}
	mSocket = INVALID_SOCKET;
}

bool UdpSocket::BindSocket()
{
	mSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (mSocket == INVALID_SOCKET) {
		FatalLog(L"UdpSocket::BindSocket socket creation error: %d %s", WSAGetLastError(), GetErrorStr(WSAGetLastError()).c_str());
		return false;
	}

	int val = 1;
	setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&val, sizeof(val));

	val = 1;
	ioctlsocket(mSocket, FIONBIO, (u_long *)&val);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(mPort);
	inet_pton(AF_INET, mHost.c_str(), &addr.sin_addr);

	int ret = bind(mSocket, (sockaddr *)&addr, sizeof(addr));
	if (ret != 0) {
		FatalLog(L"UdpSocket::BindSocket bind error : Address=%hs:%d %d %s", mHost.c_str(), mPort, WSAGetLastError(), GetErrorStr(WSAGetLastError()).c_str());
		return false;
	}
	Log(L"UdpSocket::BindSocket successfully bound to %hs:%d", mHost.c_str(), mPort);
	
	return true;
}

bool UdpSocket::DoSend(char * buf, int len)
{
	int ret2 = sendto(mSocket, buf, len, 0, (sockaddr *)&mClientAddr, sizeof(mClientAddr));
	if (ret2 >= 0) {
		mStatistics->CountPacket(len);
		return true;
	}
	if (WSAGetLastError() != WSAEWOULDBLOCK) {
		Log(L"UdpSocket::DoSend() Error on sendto. %d %s", WSAGetLastError(), GetErrorStr(WSAGetLastError()).c_str());
	}
	return false;
}
