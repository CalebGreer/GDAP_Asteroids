#include "Core.h"
#include "GameObjectManager.h"
#include "NetworkServer.h"

#include "GameObject.h"
#include "Component.h"
#include "Sprite.h"
#include "Transform.h"

void GameObjectManager::initialize()
{
	ISystem::initialize();

	REGISTER_ABSTRACT_CLASS(Component);
	REGISTER_DYNAMIC_CLASS(Sprite);
	REGISTER_DYNAMIC_CLASS(Transform);
	REGISTER_DYNAMIC_CLASS(GameObject);
}

void GameObjectManager::AddRootGameObject(GameObject* gameObject)
{
	rootGameObjects.push_back(gameObject);
}

void GameObjectManager::AddChildGameobject(GameObject* parentObject, GameObject* gameObject)
{
	parentObject->getChildren().push_back(gameObject);
}

void GameObjectManager::RemoveGameObject(STRCODE id)
{
	std::list<GameObject*>::iterator iter = rootGameObjects.begin();
	while (iter != rootGameObjects.end())
	{
		if ((*iter)->getUID() == id)
		{
			ReleaseGameObject((*iter));
			delete (*iter);
			rootGameObjects.erase(iter);
			break;
		}
		else
		{
			RemoveGameObject(id, (*iter));
		}
		++iter;
	}
}

void GameObjectManager::RemoveGameObject(STRCODE id, GameObject* parent)
{
	std::list<GameObject*>::iterator iter = parent->getChildren().begin();
	while (iter != parent->getChildren().end())
	{
		if ((*iter)->getUID() == id)
		{
			std::cout << "Remove Game Object: " << id << std::endl;
			ReleaseGameObject((*iter));
			delete (*iter);
			parent->getChildren().erase(iter);
			break;
		}
		else
		{
			RemoveGameObject(id, (*iter));
		}
		++iter;
	}
}

void GameObjectManager::ReleaseGameObject(GameObject* go)
{
	std::list<GameObject*>::iterator iter = go->getChildren().begin();
	while (iter != go->getChildren().end())
	{
		ReleaseGameObject((*iter));
		delete (*iter);
		++iter;
	}

	delete go->parent;
}

GameObject* GameObjectManager::_FindGameObject(GameObject* parent, STRCODE id)
{
	std::list<GameObject*>::iterator iter = parent->getChildren().begin();
	while (iter != parent->getChildren().end())
	{
		if ((*iter)->getUID() == id)
		{
			return (*iter);
		}

		GameObject* wp = _FindGameObject((*iter), id);
		if (wp != nullptr)
		{
			return wp;
		}

		++iter;
	}
	return nullptr;
}

GameObject* GameObjectManager::FindGameObject(STRCODE id)
{
	std::list<GameObject*>::iterator iter = rootGameObjects.begin();
	while (iter != rootGameObjects.end())
	{
		if ((*iter)->getUID() == id)
		{
			return (*iter);
		}

		GameObject* wp = _FindGameObject((*iter), id);
		if (wp != nullptr)
		{
			return wp;
		}

		++iter;
	}
	
	return nullptr;
}

GameObject* GameObjectManager::CreateGameObject(tinyxml2::XMLDocument* doc)
{
	XMLElement* gameObjElement = doc->FirstChildElement("GameObject");
	THROW_RUNTIME_ERROR(gameObjElement == nullptr, "Unable to Load Game Object Prefab");

	GameObject* gObj = new GameObject();

	gObj->setFileID(NoName);
	gObj->load(gameObjElement);

	UUID uid;
	CreateUUID(&uid);
	gObj->setUID(GUIDToSTRCODE(uid));

	rootGameObjects.push_back(gObj);

    // Send a game object create over the network
    if (NetworkServer::Instance().isServer())
    {
        RakNet::BitStream bitStream;
        bitStream.Write((unsigned char)ID_GAMEOBJECT);
        bitStream.Write((unsigned char)ID_GAMEOBJECT_CREATE);
        bitStream.Write(gObj->getUID());
        gObj->writeCreate(bitStream);

        NetworkServer::Instance().sendPacket(bitStream);
    }

	return gObj;
}

void GameObjectManager::load(XMLElement* node, STRCODE idOfFile)
{
	XMLElement* gameObjElement = node->FirstChildElement("GameObject");
	while (gameObjElement != NULL)
	{
		GameObject* gObj = new GameObject();
		gObj->setFileID(idOfFile);
		gObj->load(gameObjElement);

		rootGameObjects.push_back(gObj);

		gameObjElement = gameObjElement->NextSiblingElement("GameObject");
	}
}

void GameObjectManager::unload(STRCODE idOfFile)
{
}

void GameObjectManager::save(tinyxml2::XMLElement* node)
{
}

const std::list<GameObject*>& GameObjectManager::GetAllRootGameObjects()
{
	return rootGameObjects;
}

void GameObjectManager::DestroyGameObject(GameObject* go)
{
    if (NetworkServer::Instance().isServer())
    {
        // Write the code to destroy the raindrop
        RakNet::BitStream bs;
        bs.Write((unsigned char)ID_GAMEOBJECT);
        bs.Write((unsigned char)ID_GAMEOBJECT_DESTROY);
        bs.Write(go->getUID());
        NetworkServer::Instance().sendPacket(bs);
    }

	destroyGameObjects.push_back(go);
}

void GameObjectManager::_update(float deltaTime, GameObject* gameObject)
{
	gameObject->update(deltaTime);

	std::list<GameObject*>::iterator iter = gameObject->getChildren().begin();
	while (iter != gameObject->getChildren().end())
	{
		_update(deltaTime, (*iter));
		++iter;
	}
}

void GameObjectManager::update(float deltaTime)
{
	std::list<GameObject*>::iterator iter = rootGameObjects.begin();
	while (iter != rootGameObjects.end())
	{
		_update(deltaTime, (*iter));
		iter++;
	}

	std::list<GameObject*>::iterator destIt = destroyGameObjects.begin();
	while (destIt != destroyGameObjects.end())
	{
		RemoveGameObject((*destIt)->getUID());
		++destIt;
	}
	destroyGameObjects.clear();
}

// Networking

void GameObjectManager::networkUpdate(float deltaTime)
{
    RakNet::BitStream bs;
    for (auto iter : rootGameObjects)
    {
        bs.Write((unsigned char)ID_GAMEOBJECT);
        bs.Write((unsigned char)ID_GAMEOBJECT_UPDATE);
        bs.Write((*iter).getUID());
        (*iter).writeUpdate(bs);

        NetworkServer::Instance().sendPacket(bs);

        bs.Reset();
    }
}

void GameObjectManager::writeSnapShot(RakNet::BitStream& bitStream)
{
    bitStream.Write((unsigned int)rootGameObjects.size());

    for (auto gameObject : rootGameObjects)
    {
        bitStream.Write(gameObject->getUID());
        gameObject->writeCreate(bitStream);
    }
}

void GameObjectManager::readSnapShot(RakNet::BitStream& bitStream)
{
    unsigned int count = 0;
    bitStream.Read(count);

    for (int i = 0; i < count; i++)
    {
        // Read the UID
        STRCODE UID;
        bitStream.Read(UID);

        GameObject* gameObject = FindGameObject(UID);
        if (gameObject == nullptr)
        {
            gameObject = new GameObject();
            gameObject->setFileID(NoName);
            gameObject->setUID(UID);
            gameObject->readCreate(bitStream);
            gameObject->initialize();

            rootGameObjects.push_back(gameObject);
        }
    }
}

void GameObjectManager::invokeRPC(RakNet::BitStream& bitStream)
{
    STRCODE goID;
    bitStream.Read(goID);

    GameObject* gameObject = FindGameObject(goID);
    if (gameObject != nullptr)
    {
        STRCODE componentId;
        bitStream.Read(componentId);

#ifdef _DEBUG
        NetworkRPC* networkRPC = dynamic_cast<NetworkRPC*>(gameObject->GetComponentByUUID(componentId));
#else
        NetworkRPC* networkRPC = (NetworkRPC*)(gameObject->GetComponentByUUID(componentId));
#endif // _DEBUG

        ASSERT(networkRPC != nullptr, "Component is not a NetworkRPC");
        networkRPC->invokeRPC(bitStream);
    }
}

void GameObjectManager::processPacket(RakNet::BitStream& bitStream)
{
	unsigned char packetId;
	bitStream.Read(packetId);

	switch (packetId)
	{
        // Receive a game object create
        case NetworkPacketIds::ID_GAMEOBJECT_CREATE:
            {
                STRCODE UID;
                bitStream.Read(UID);

                GameObject* gameObject = FindGameObject(UID);
                if (gameObject == nullptr)
                {
                    gameObject = new GameObject();
                    gameObject->setFileID(NoName);
                    gameObject->setUID(UID);
                    gameObject->readCreate(bitStream);
                    gameObject->initialize();
                    rootGameObjects.push_back(gameObject);
                }
            }
            break;

		case NetworkPacketIds::ID_GAMEOBJECT_DESTROY:
			{
				STRCODE goID;
				bitStream.Read(goID);
				GameObject* gameObject = FindGameObject(goID);
				if (gameObject != nullptr)
				{
					DestroyGameObject(gameObject);
					break;
				}
			}
			break;

        case NetworkPacketIds::ID_GAMEOBJECT_UPDATE:
            {
                STRCODE goID;
                bitStream.Read(goID);
                GameObject* gameObject = FindGameObject(goID);
                if (gameObject != nullptr)
                {
                    gameObject->readUpdate(bitStream);
                }
            }
            break;
	}
}
