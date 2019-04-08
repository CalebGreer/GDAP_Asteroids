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
	void rpcCall(RakNet::BitStream & bitStream);

	const sf::Vector2f& getSpeed() const { return speed; }

	const sf::Vector2f& getVelocity() const { return velocity; }
	void setVelocity(const sf::Vector2f& _velocity) { velocity = _velocity; } 

private:
	void InputUpdate();

private:
	sf::Vector2f speed;
	sf::Vector2f position;
	sf::Vector2f velocity;
};

