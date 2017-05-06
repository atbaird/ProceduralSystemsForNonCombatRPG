#pragma once

#ifndef PACKETCHANNEL_HPP
#define PACKETCHANNEL_HPP
#include "Engine/Net/UDP/UDPSocket.hpp"
#include "Engine/Math/Specialty/Range.hpp"
#include "Engine/Utils/ObjectPool.hpp"
#include "Engine/Net/UDP/TimeStampedPacket.hpp"
#include <vector>

#define STATIC 

class NetSession;
class PacketChannel
{
private:
protected:
	static size_t s_numOfInboundPacketCnt;
	static float s_drop_rate;
	static Range<double> s_additional_lag;
	UDPSocket* my_socket;
	std::vector<TimeStampedPacket*> m_inboundPackets;
	ObjectPool<TimeStampedPacket> m_pool;
public:
	NetSession* m_session;
	//Constructors
	PacketChannel();
	PacketChannel(const char* port, const char* minPort = NetConstants::s_minPortDefault,
		const char* maxPort = NetConstants::s_maxPortDefault, const char* addr = NetConstants::s_localHost);
	virtual ~PacketChannel();

	//Binding
	bool Bind(const char* addr, const char* port, const char* minPort = NetConstants::s_minPortDefault, const char* maxPort = NetConstants::s_maxPortDefault);
	void Unbind();
	bool IsBound();

	//Send Recv
	size_t SendTo(sockaddr_in to_addr, void* data, size_t datalength);
	size_t RecvFrom(sockaddr_in *from_addr, NetPacket* buffer, const size_t buffer_size);
protected:
	bool ReadInTimeStampedPacket(size_t buffer_size);
	bool InsertInOrder(TimeStampedPacket* packet);
public:

	//Getters
	const UDPSocket* GetUDPSocket() const;
	UDPSocket* GetEditableUDPSocket() const;
	SOCKET GetSocket() const;
	sockaddr_in GetAddress() const;

	//Static methods

	static void SetPacketSimulatedDropRate(float dropRate);
	static void SetPacketSimulatedAdditionalLag(Range<double> additionalLag);
	static Range<double>GetAdditionalLag();
	static float GetDropRate();
};

#endif