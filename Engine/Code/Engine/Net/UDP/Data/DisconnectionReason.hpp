#pragma once

#ifndef DISCONNECTIONREASON_HPP
#define DISCONNECTIONREASON_HPP

enum DisconnectionReason
{
	DisconnectionReason_None,
	DisconnectReason_FailedToStartNetSession, //Failed to Create Socket
	DisconnectionReason_HostConnectionTimeOut,
	DisconnectionReason_JoinDeniedNotAcceptingNewConnections,
	DisconnectionReason_JoinDeniedNotHost,
	DisconnectionReason_JoinDeniedFull,
	DisconnectionReason_JoinDeniedGuidInUse,
	DisconnectionReason_ErrorHostDisconnected,
	DisconnectionReason_Kicked,
	Number_Of_SessionDisconnectionReason
};

#endif