#include "GameCore.h"
#include "Player.h"
#include "PrefabAsset.h"
#include "InputManager.h"
#include "Transform.h"

IMPLEMENT_DYNAMIC_CLASS(Player)
 
void Player::initialize()
{
	Sprite::initialize();

	if (NetworkServer::Instance().isServer())
	{ 
		sf::Vector2f pos;

		pos.x = RenderSystem::Instance().getView().getSize().x * Random.Random();
		//pos.y = (RenderSystem::Instance().getView().getSize().y * -1.0f) - 64.0f;
		pos.y = -300.0f;

		gameObject->getTransform()->setPosition(pos);

		position = &gameObject->getTransform()->getPosition();
	}
}

void Player::update(float deltaTime)
{
	Sprite::update(deltaTime); 

	if (NetworkClient::Instance().isClient() == true)
	{
		return;
	}
	
	if(InputManager::Instance().keyPressed(sf::Keyboard::A))
	{ 
		position->x -= 10.0f;
	}
}

void Player::load(XMLElement * element)
{
	Sprite::load(element);
}

void Player::writeCreate(RakNet::BitStream & bs) const
{
	Sprite::writeCreate(bs);
}

void Player::readCreate(RakNet::BitStream & bs)
{
	Sprite::readCreate(bs);
}
