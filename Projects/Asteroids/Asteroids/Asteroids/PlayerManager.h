#pragma once

#include "Component.h"
#include "Player.h"

class PlayerManager : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(PlayerManager, Component)
 
public:
	virtual void update(float deltaTime);
	virtual void load(XMLElement* element); 

private:
	STRCODE prefabID;
	bool started = false; 
};