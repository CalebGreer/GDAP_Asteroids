#include "GameCore.h"
#include "PlayerManager.h"
#include "PrefabAsset.h"

IMPLEMENT_DYNAMIC_CLASS(PlayerManager); 

using namespace std::placeholders;

void PlayerManager::initialize()
{ 
	Component::initialize();

    registerRPC(getHashCode("rpcConnected"), std::bind(&PlayerManager::rpcConnected, this, _1)); 
}

void PlayerManager::update(float deltaTime)
{
	Component::update(deltaTime); 

	if (spawning && NetworkServer::Instance().isServer())
	{
		spawning = false;
		CreatePlayer();
	}

	if (!spawned && NetworkClient::Instance().getState() == NetworkClient::NetworkClientState::CONNECTED)
	{
		spawned = true;
		if (NetworkClient::Instance().isClient())
		{
			RakNet::BitStream bitStream;
			bitStream.Write((unsigned char)ID_RPC_MESSAGE);
			bitStream.Write(gameObject->getUID());
			bitStream.Write(PlayerManager::getClassHashCode());
			bitStream.Write(getHashCode("rpcConnected"));

			bitStream.Write(spawning = true);
			NetworkClient::Instance().callRPC(bitStream);
		} 
	}
}

void PlayerManager::load(XMLElement * element)
{
	Component::load(element);

	tinyxml2::XMLElement* prefabElement = element->FirstChildElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset element");
	const char* id = prefabElement->GetText();
	prefabCode[0] = getHashCode(id);

	prefabElement = prefabElement->NextSiblingElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset element");
	const char* id2 = prefabElement->GetText();
	prefabCode[1] = getHashCode(id2);
}

void PlayerManager::CreatePlayer()
{
	Asset* asset = AssetManager::Instance().getAsset(prefabCode[counter]);
	if (asset != nullptr)
	{
		PrefabAsset* prefab = (PrefabAsset*)asset;
		GameObject* go = prefab->CreatePrefab(); 

		counter++; 
	}
}

void PlayerManager::assignPlayer(GameObject* gObj)
{
	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_RPC_MESSAGE);
	bitStream.Write(gameObject->getUID());
	bitStream.Write(PlayerManager::getClassHashCode());
	bitStream.Write(getHashCode("rpcRegisterPlayer"));

	bitStream.Write(gObj->getUID());
	NetworkClient::Instance().callRPC(bitStream);
}

void PlayerManager::rpcConnected(RakNet::BitStream& bitStream)
{
	bitStream.Read(spawning);
}