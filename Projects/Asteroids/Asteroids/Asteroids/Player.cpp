#include "GameCore.h"
#include "Player.h"
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

	registerRPC(getHashCode("rpcCall"), std::bind(&Player::rpcCall, this, _1));
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

			if (asteroid != nullptr)
			{
				sf::Vector2f tempPos = this->gameObject->getTransform()->getPosition();
				if (asteroid->isWithinBounds(tempPos.x, tempPos.y))
				{
					GameObjectManager::Instance().DestroyGameObject(gameObject);
					GameObjectManager::Instance().DestroyGameObject(this->gameObject);
					break;
				}
			}
		}
	}

	if (NetworkClient::Instance().isClient())
	{
		if (InputManager::Instance().keyPressed(sf::Keyboard::A))
		{
			velocity.x = -speed.x * deltaTime;
			InputUpdate();
		}

		if (InputManager::Instance().keyPressed(sf::Keyboard::D))
		{
			velocity.x = speed.x * deltaTime;
			InputUpdate();
		}

		if (InputManager::Instance().keyPressed(sf::Keyboard::S))
		{
			velocity.y = speed.y * deltaTime;
			InputUpdate();
		}

		if (InputManager::Instance().keyPressed(sf::Keyboard::W))
		{
			velocity.y = -speed.y * deltaTime;
			InputUpdate();
		}

		velocity = sf::Vector2f(0, 0);
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

void Player::rpcCall(RakNet::BitStream& bitStream)
{
	sf::Vector2f v;
	bitStream.Read(v.x);
	bitStream.Read(v.y);

	auto gameObjects = GameObjectManager::Instance().GetAllRootGameObjects();
	for (auto gameObject : gameObjects)
	{
		Player* p = dynamic_cast<Player*>(
			gameObject->GetComponentByUUID(Player::getClassHashCode()));

		if (p == this)
		{
			p->gameObject->getTransform()->move(v);
		}
	}
} 

void Player::InputUpdate()
{
	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_RPC_MESSAGE);
	bitStream.Write(gameObject->getUID());
	bitStream.Write(Player::getClassHashCode());
	bitStream.Write(getHashCode("rpcCall"));

	bitStream.Write(velocity.x);
	bitStream.Write(velocity.y);

	NetworkClient::Instance().callRPC(bitStream);
}
