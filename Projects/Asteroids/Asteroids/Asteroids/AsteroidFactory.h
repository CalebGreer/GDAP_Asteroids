#pragma once

#include "Component.h"

class AsteroidFactory : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(AsteroidFactory, Component)

	enum NetworkPackets
	{
		SPAWN
	};

public:
	virtual void update(float deltaTime);
	virtual void load(XMLElement* element);

private:
	bool enabled = true;
	float spawnDelay;
	float currentSpawnDelay = 0.0f;
	STRCODE prefabID;
};

