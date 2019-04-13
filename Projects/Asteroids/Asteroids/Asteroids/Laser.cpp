#include "GameCore.h"
#include "Laser.h"
#include "Transform.h"
#include "PrefabAsset.h"
#include "Asteroid.h"

IMPLEMENT_DYNAMIC_CLASS(Laser)

void Laser::initialize()
{
	Sprite::initialize();

	if (NetworkServer::Instance().isServer())
	{
		topBorder = (RenderSystem::Instance().getView().getSize().y * -1.0f) - 64.0f;

		sf::Vector2f pos;
		pos.x = 200.0f;
		pos.y = -50.0f;
		gameObject->getTransform()->setPosition(pos);
	}
}

void Laser::update(float deltaTime)
{
	Sprite::update(deltaTime);

	if (NetworkClient::Instance().isClient() == true)
	{
		return;
	}

	if (gameObject->getTransform()->getPosition().y > topBorder)
	{
		gameObject->getTransform()->move(speed.x * deltaTime, speed.y * deltaTime);
	}
	else
	{
		GameObjectManager::Instance().DestroyGameObject(gameObject);
	}

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
				asteroid->takeDamage();
				if (asteroid->getHealth() <= 0)
				{
					GameObjectManager::Instance().DestroyGameObject(gameObject);
				}
				GameObjectManager::Instance().DestroyGameObject(this->gameObject);
				break;
			}
		}
	}
}

void Laser::load(XMLElement * element)
{
	Sprite::load(element);

	Sprite::load(element);

	XMLElement* speedElement = element->FirstChildElement("Speed");
	THROW_RUNTIME_ERROR(speedElement == nullptr, "No Speed Element");
	speed = sf::Vector2f(speedElement->FloatAttribute("x"), speedElement->FloatAttribute("y"));
}

void Laser::writeCreate(RakNet::BitStream & bs) const
{
	Sprite::writeCreate(bs);

	bs.Write(speed.x);
	bs.Write(speed.y);
}

void Laser::readCreate(RakNet::BitStream & bs)
{
	Sprite::readCreate(bs);

	bs.Read(speed.x);
	bs.Read(speed.y);
}
