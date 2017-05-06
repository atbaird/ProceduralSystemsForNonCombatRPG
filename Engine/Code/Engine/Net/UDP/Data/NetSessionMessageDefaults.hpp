#pragma once

#ifndef NETSESSIONMESSAGEDEFAULTS_HPP
#define NETSESSIONMESSAGEDEFAULTS_HPP
#include <stdint.h>

struct NetSenderTemplate;
class NetMessage;
struct UDPNetConnection;

class NetSessionMessageDefaults
{
public:

	//DefaultMessageFunctions
	static void OnPingReceived(const NetSenderTemplate& sender, NetMessage &msg);
	static void OnPongReceived(const NetSenderTemplate& sender, NetMessage& msg);
	static void OnHeartbeatReceived(const NetSenderTemplate& sender, NetMessage& msg);
	static void OnAckReceived(const NetSenderTemplate& sender, NetMessage& msg);
	static void OnJoinRequestReceived(const NetSenderTemplate& sender, NetMessage& msg);
	static void OnJoinAcceptReceived(const NetSenderTemplate& sender, NetMessage& msg);
	static void OnJoinDeniedReceived(const NetSenderTemplate& sender, NetMessage& msg);
	static void OnNewConnectionStatusUpdateFromHost(const NetSenderTemplate& sender, NetMessage& msg);
	static void OnLeaveReceived(const NetSenderTemplate& sender, NetMessage& msg);

protected:
	static void SendNewConnectionStatusUpdateFromHostMessages(const NetSenderTemplate& sender, UDPNetConnection* conn);
	static void OnNewConnectionStatusUpdateFromHostDidNotAlreadyHadTests(UDPNetConnection* conn, uint8_t alreadyHadIdx);
};
#endif