#include "UdpThread.h"
#include "ClientConnection.h"

UdpThread::UdpThread(std::string host, int port, std::shared_ptr<Statistics> statistics)
	: m_bExit(false)

{
	Log(L"Using async communication");
	mHost = host;
	mPort = port;
	mSocket = INVALID_SOCKET;
	mStatistics = statistics;
	mClientAddr.sin_family = 0;
	BindSocket();
}

void UdpThread::Run() {
	while (!m_bExit) {
		char packet[MAX_PACKET_UDP_PACKET_SIZE];

		socklen_t socklen = sizeof(mClientAddr);
		int packetSize = static_cast<int>(recvfrom(mSocket, packet, MAX_PACKET_UDP_PACKET_SIZE, 0,
			(sockaddr*)& mClientAddr,
			&socklen));

		if (packetSize <= 0)
		{
			Log(L"UdpThread: receive error %d", packetSize);

		}
		else {
			m_packetCallback(packet, packetSize, &mClientAddr);
		}

		
	}
}

UdpThread::~UdpThread() {

}

void UdpThread::Shutdown() {
	m_bExit = true;

	if (mSocket != INVALID_SOCKET) {
		closesocket(mSocket);
	}
	mSocket = INVALID_SOCKET;
}

bool UdpThread::BindSocket()
{

	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 0), &wsaData);

	mSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (mSocket == INVALID_SOCKET) {
		FatalLog(L"UdpSocket::BindSocket socket creation error: %d %s", WSAGetLastError(), GetErrorStr(WSAGetLastError()).c_str());
		return false;
	}

	int val = 1;
	setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)& val, sizeof(val));

	//sets socket non blocking
	/*val = 1;
	ioctlsocket(mSocket, FIONBIO, (u_long*)& val);
	*/

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(mPort);
	inet_pton(AF_INET, mHost.c_str(), &addr.sin_addr);

	int ret = bind(mSocket, (sockaddr*)& addr, sizeof(addr));
	if (ret != 0) {
		FatalLog(L"UdpSocket::BindSocket bind error : Address=%hs:%d %d %s", mHost.c_str(), mPort, WSAGetLastError(), GetErrorStr(WSAGetLastError()).c_str());
		return false;
	}
	Log(L"UdpSocket::BindSocket successfully bound to %hs:%d", mHost.c_str(), mPort);

	return true;
}





bool UdpThread::Send(char* buf, int len, uint64_t frameIndex) {

	int ret2 = sendto(mSocket, buf, len, 0, (sockaddr*)& mClientAddr, sizeof(mClientAddr));
	if (ret2 >= 0) {
		mStatistics->CountPacket(len);
		return true;
	}
	if (WSAGetLastError() != WSAEWOULDBLOCK) {
		Log(L"UdpSocket::DoSend() Error on sendto. %d %s", WSAGetLastError(), GetErrorStr(WSAGetLastError()).c_str());
	}

	return true;
}


void UdpThread::setPacketCallback(std::function<void(char*, int, sockaddr_in*)> callback) {
	m_packetCallback = callback;
	
}

bool UdpThread::Startup() {
	this->Start();
	return true;
}



