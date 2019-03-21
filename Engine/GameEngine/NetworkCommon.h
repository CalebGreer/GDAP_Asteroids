#ifndef _NETWORKCOMMON_H_
#define _NETWORKCOMMON_H_
#pragma once

#include <MessageIdentifiers.h>

enum NetworkPacketIds
{
	ID_DEFAULT = ID_USER_PACKET_ENUM,

	// SERVER PACKET
    ID_GAMEOBJECT,
	
    // GAME SNAPSHOT
    ID_SNAPSHOT,

	// GAME OBJECT MANAGER PACKETS
    ID_GAMEOBJECT_COMPONENT,
    ID_GAMEOBJECT_CREATE,
    ID_GAMEOBJECT_UPDATE,
    ID_GAMEOBJECT_DESTROY,

};

#endif

