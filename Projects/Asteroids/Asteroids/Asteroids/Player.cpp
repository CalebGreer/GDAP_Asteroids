#include "GameCore.h"
#include "Player.h"
#include "InputController.h"
#include "PrefabAsset.h"
#include "Transform.h"
#include "Asteroid.h"

IMPLEMENT_DYNAMIC_CLASS(Player)

using namespace std::placeholders;

void Player::initialize()
{
	Sprite::initialize();

	if (NetworkServer::Instance().isServer())
	{
		position = sf::Vector2f(400.0f, -300.0f);
		gameObject->getTransform()->setPosition(position);
	} 

	playerID = gameObject->getUID();
}

void Player::update(float deltaTime)
{
	Sprite::update(deltaTime);

	if (NetworkServer::Instance().isServer())
	{
		// Check for collision with Player TODO: FIX, Causes a transform crash
		auto gameObjects = GameObjectManager::Instance().GetAllRootGameObjects();
		for (auto gameObject : gameObjects)
		{
			Asteroid* asteroid = dynamic_cast<Asteroid*>(
				gameObject->GetComponentByUUID(Asteroid::getClassHashCode()));

			//	if (asteroid != nullptr)
			//	{
			//		sf::Vector2f tempPos = this->gameObject->getTransform()->getPosition();
			//		if (asteroid->isWithinBounds(tempPos.x, tempPos.y))
			//		{
			//			GameObjectManager::Instance().DestroyGameObject(gameObject);
			//			GameObjectManager::Instance().DestroyGameObject(this->gameObject);
			//			break;
			//		}
			//	}
		}

		gameObject->getTransform()->move(velocity);
	}



	if (NetworkClient::Instance().isClient())
	{
		if (!hasInput)
		{
			assignPlayer();
			hasInput = true;
		}
	}
}

void Player::load(XMLElement * element)
{
	Sprite::load(element);

	XMLElement* speedElement = element->FirstChildElement("Speed");
	THROW_RUNTIME_ERROR(speedElement == nullptr, "No Speed Element");
	speed = sf::Vector2f(speedElement->FloatAttribute("x"), speedElement->FloatAttribute("y")); 
}

void Player::writeCreate(RakNet::BitStream & bs) const
{
	Sprite::writeCreate(bs);

	bs.Write(speed.x);
	bs.Write(speed.y);
}

void Player::readCreate(RakNet::BitStream & bs)
{
	Sprite::readCreate(bs);

	bs.Read(speed.x);
	bs.Read(speed.y);
}


void Player::assignPlayer()
{
	auto gameObjects = GameObjectManager::Instance().GetAllRootGameObjects();
	for (auto gameObject : gameObjects)
	{
		InputController* inputCtrl = dynamic_cast<InputController*>(
			gameObject->GetComponentByUUID(InputController::getClassHashCode()));

		if (inputCtrl != nullptr)
		{
			inputCtrl->setPlayer(this);
		}
	}
}