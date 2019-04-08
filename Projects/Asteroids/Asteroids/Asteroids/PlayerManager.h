#pragma once

#include "Component.h"
#include "Player.h"

class PlayerManager : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(PlayerManager, Component)
 
public:
	void initialize() override;
	virtual void update(float deltaTime);
	virtual void load(XMLElement* element); 

	void CreatePlayer();

	void rpcConnected(RakNet::BitStream & bitStream);

private:
	STRCODE prefabID;
	bool spawning = false; 
	bool spawned = false; 

	int counter = 0;
};