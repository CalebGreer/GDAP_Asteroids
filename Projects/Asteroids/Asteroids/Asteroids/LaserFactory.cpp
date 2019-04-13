#include "GameCore.h"
#include "LaserFactory.h"
#include "PrefabAsset.h"
#include "Laser.h"

IMPLEMENT_DYNAMIC_CLASS(LaserFactory)

using namespace std::placeholders;

void LaserFactory::initialize()
{
	Component::initialize();

	registerRPC(getHashCode("rpcFireLaser"), std::bind(&LaserFactory::rpcFireLaser, this, _1));
}

void LaserFactory::update(float deltaTime)
{
	Component::update(deltaTime);

	if (NetworkClient::Instance().isClient() == true)
	{
		if (InputManager::Instance().keyReleased(sf::Keyboard::Space))
		{
			RakNet::BitStream bitStream;
			bitStream.Write((unsigned char)ID_RPC_MESSAGE);
			bitStream.Write(gameObject->getUID());
			bitStream.Write(LaserFactory::getClassHashCode());
			bitStream.Write(getHashCode("rpcFireLaser"));

			NetworkClient::Instance().callRPC(bitStream);
		}
	}
}

void LaserFactory::load(XMLElement * element)
{
	Component::load(element);

	tinyxml2::XMLElement* prefabElement = element->FirstChildElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset element");
	const char* id = prefabElement->GetText();
	prefabID = getHashCode(id);
}

void LaserFactory::CreateLaser()
{
	Asset* asset = AssetManager::Instance().getAsset(prefabID);
	if (asset != nullptr)
	{
		PrefabAsset* prefab = (PrefabAsset*)asset;
		GameObject* go = prefab->CreatePrefab();
	}
}

void LaserFactory::rpcFireLaser(RakNet::BitStream & bitStream)
{
	CreateLaser();
}
