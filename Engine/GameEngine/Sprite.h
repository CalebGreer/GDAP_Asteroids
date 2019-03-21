#ifndef _SPRITE_H_
#define _SPRITE_H_
#pragma once

#include "Renderable.h"

class Asset;
namespace sf { class Sprite; }

class Sprite : public Renderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Sprite, Renderable)

public :
	~Sprite();

	virtual void initialize();

    void writeCreate(RakNet::BitStream & bs) const override;
    void readCreate(RakNet::BitStream & bs) override;

	void load(XMLElement* node);
	void save(XMLElement* node);

	virtual void render(sf::RenderWindow* window, Transform* t) override;

    bool isWithinBounds(float x, float y);

protected:
	sf::Sprite* renderableSprite;
	Asset* textureAsset;
	std::string textureUID;
};

#endif

