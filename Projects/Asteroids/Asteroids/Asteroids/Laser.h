#pragma once

#include "Sprite.h"
#include <SFML\System.hpp>

class Laser : public Sprite
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Laser, Sprite)

public:
	void initialize() override;
	virtual void update(float deltaTime) override;
	virtual void load(XMLElement* element) override;

	void writeCreate(RakNet::BitStream & bs) const override;
	void readCreate(RakNet::BitStream & bs) override;

private:
	sf::Vector2f speed;

	float topBorder;
};

