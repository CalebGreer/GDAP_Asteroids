#pragma once

#include "Component.h"

class RainDropFactory : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(RainDropFactory, Component)

	enum NetworkPackets
	{
		SPAWN
	};

public:
	virtual void update(float deltaTime);
	virtual void load(XMLElement* element);

    virtual void writeUpdate(RakNet::BitStream & bs) const override;
    virtual void readUpdate(RakNet::BitStream & bs) override;

private:
	bool enabled = true;
	float spawnDelay;
	float currentSpawnDelay = 0.0f;
	STRCODE prefabID;

    STRCODE spawnedRainDrop;
};

