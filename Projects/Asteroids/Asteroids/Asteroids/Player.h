#pragma once

#include "Sprite.h"
#include <SFML\System.hpp>

class Player : public Sprite
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Player, Sprite)

public:
	void initialize() override;
	virtual void update(float deltaTime) override;
	virtual void load(XMLElement* element) override;

	void writeCreate(RakNet::BitStream & bs) const override;
	void readCreate(RakNet::BitStream & bs) override;

	const sf::Vector2f& getSpeed() const { return speed; }
	void setSpeed(const sf::Vector2f& _speed) { speed = _speed; }

private:
	sf::Vector2f speed;
	sf::Vector2f *position;
};

