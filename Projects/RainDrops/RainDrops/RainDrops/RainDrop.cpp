#include "GameCore.h"
#include "RainDrop.h"
#include "PrefabAsset.h"
#include "Transform.h"

IMPLEMENT_DYNAMIC_CLASS(RainDrop)

void RainDrop::initialize()
{
	Sprite::initialize();

    if (NetworkServer::Instance().isServer())
    {
        speed.x = (maxSpeed.x - minSpeed.x) * Random.Random();
        speed.y = (maxSpeed.y - minSpeed.y) * Random.Random();

        sf::Vector2f pos;
        pos.x = RenderSystem::Instance().getView().getSize().x * Random.Random();
        pos.y = (RenderSystem::Instance().getView().getSize().y * -1.0f) - 64.0f;
        gameObject->getTransform()->setPosition(pos);
    }
}

void RainDrop::load(XMLElement* element)
{
	Sprite::load(element);

	XMLElement* minElement = element->FirstChildElement("MinSpeed");
	THROW_RUNTIME_ERROR(minElement == nullptr, "No Min Element");
	minSpeed = sf::Vector2f(minElement->FloatAttribute("x"), minElement->FloatAttribute("y"));

	XMLElement* maxElement = element->FirstChildElement("MaxSpeed");
	THROW_RUNTIME_ERROR(maxElement == nullptr, "No Max Element");
	maxSpeed = sf::Vector2f(maxElement->FloatAttribute("x"), maxElement->FloatAttribute("y"));
}

void RainDrop::writeCreate(RakNet::BitStream & bs) const
{
    Sprite::writeCreate(bs);

    // Write the data needed
    bs.Write(maxSpeed.x);
    bs.Write(maxSpeed.y);
    bs.Write(minSpeed.x);
    bs.Write(minSpeed.y);

    bs.Write(speed.x);
    bs.Write(speed.y);
}

void RainDrop::readCreate(RakNet::BitStream & bs)
{
    Sprite::readCreate(bs);

    // Read the data needed
    bs.Read(maxSpeed.x);
    bs.Read(maxSpeed.y);
    bs.Read(minSpeed.x);
    bs.Read(minSpeed.y);

    bs.Read(speed.x);
    bs.Read(speed.y);
}

void RainDrop::update(float deltaTime)
{
    Sprite::update(deltaTime);

    if (NetworkClient::Instance().isClient() == true)
    {
        return;
    }

    if (gameObject->getTransform()->getPosition().y < 50)
    {
        gameObject->getTransform()->move(speed.x * deltaTime, speed.y * deltaTime);
    }
    else
    {
        GameObjectManager::Instance().DestroyGameObject(gameObject);
    }
}
