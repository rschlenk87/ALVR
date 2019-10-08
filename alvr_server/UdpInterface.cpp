#include "UdpInterface.h"

sockaddr_in UdpInterface::GetClientAddr() const {
	return mClientAddr;
}

void UdpInterface::SetClientAddr(const sockaddr_in* addr)
{
	mClientAddr = *addr;
}

bool UdpInterface::IsClientValid( )const {
	return mClientAddr.sin_family != 0;
}

bool UdpInterface::IsLegitClient(const sockaddr_in* addr)
{
	if (mClientAddr.sin_family == AF_INET && mClientAddr.sin_addr.S_un.S_addr == addr->sin_addr.S_un.S_addr && mClientAddr.sin_port == addr->sin_port) {
		return true;
	}
	else {
		return false;
	}
}

void UdpInterface::InvalidateClient()
{
	mClientAddr.sin_family = 0;
}