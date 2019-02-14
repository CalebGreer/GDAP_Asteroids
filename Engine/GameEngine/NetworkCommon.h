#ifndef _NETWORKCOMMON_H_
#define _NETWORKCOMMON_H_
#pragma once

#include <MessageIdentifiers.h>

enum NetworkPacketIds
{
	ID_DEFAULT = ID_USER_PACKET_ENUM,

	// SERVER PACKET
    ID_GAMEOBJECT,
	
	// GAME OBJECT MANAGER PACKETS
    ID_GAMEOBJECT_COMPONENT,
    ID_GAMEOBJECT_DESTROY,

	// USER or GAME PACKETS
	ID_GAME_PACKET_ENUM,

    // Raindrop Factory
    ID_SPAWN
};

#endif

