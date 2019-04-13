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

	void assignPlayer();


	const sf::Vector2f& getSpeed() const { return speed; }

	const sf::Vector2f& getVelocity() const { return velocity; }
	void setVelocity(sf::Vector2f _velocity) { velocity = _velocity; } 
	void setVelocityX(int x) { velocity.x = x; } 
	void setVelocityY(int y) { velocity.y = y; } 

	STRCODE getId() const { return playerID; }
	void setId(int ID) { playerID = ID; } 

private:
	sf::Vector2f speed;
	sf::Vector2f position;
	sf::Vector2f velocity;

	STRCODE playerID = NoName;

	bool hasInput = false;
};

