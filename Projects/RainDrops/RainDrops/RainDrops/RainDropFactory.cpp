#include "GameCore.h"
#include "RainDropFactory.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "PrefabAsset.h"
#include "Transform.h"
#include "RainDrop.h"

IMPLEMENT_DYNAMIC_CLASS(RainDropFactory)

void RainDropFactory::load(XMLElement* element)
{
    Component::load(element);

    tinyxml2::XMLElement* prefabElement = element->FirstChildElement("PrefabAsset");
    THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset element");
    const char* id = prefabElement->GetText();
    prefabID = getHashCode(id);

    tinyxml2::XMLElement* spawnDelayElement = element->FirstChildElement("SpawnDelay");
    THROW_RUNTIME_ERROR(spawnDelayElement == nullptr, "No SpawnDelay element");
    spawnDelay = spawnDelayElement->FloatAttribute("value");
}

void RainDropFactory::processPacket(RakNet::BitStream & bs)
{
    // Now we need to read the data and get these to spawn

    unsigned char command = 0;
    bs.Read(command);

    switch (command)
    {
    case ID_SPAWN:
        {
            STRCODE goUID = NoName;
            bs.Read(goUID);

            sf::Vector2f position;
            bs.Read(position.x);
            bs.Read(position.y);

            // Read the Raindrop Speed
            sf::Vector2f speed;
            bs.Read(speed.x);
            bs.Read(speed.y);

            // Create a new RainDrop
            Asset* asset = AssetManager::Instance().getAsset(prefabID);
            if (asset != nullptr)
            {
                PrefabAsset* prefab = (PrefabAsset*)asset;
                GameObject* go = prefab->CreatePrefab();

                // Set the UID for the RainDrop
                go->setUID(goUID);

                // Set the Raindrop position
                go->getTransform()->setPosition(position);

                // Set the Raindrop Speed
                RainDrop* rainDrop = (RainDrop*)go->GetComponentByUUID(RainDrop::getClassHashCode());
                rainDrop->setSpeed(speed);
            }
        }
        break;
    }

}

void RainDropFactory::writePacket(GameObject* go)
{
    RakNet::BitStream bs;

    bs.Write((unsigned char)ID_GAMEOBJECT);
    bs.Write((unsigned char)ID_GAMEOBJECT_COMPONENT);
    bs.Write(gameObject->getUID());
    bs.Write(RainDropFactory::getClassHashCode());

    bs.Write((unsigned char)ID_SPAWN);
    bs.Write(go->getUID());
    bs.Write(go->getTransform()->getPosition().x);
    bs.Write(go->getTransform()->getPosition().y);

    // Write the Raindrop Speed
    RainDrop* rainDrop = (RainDrop*)go->GetComponentByUUID(RainDrop::getClassHashCode());
    sf::Vector2f speed = rainDrop->getSpeed();
    bs.Write(speed.x);
    bs.Write(speed.y);

    NetworkServer::Instance().sendPacket(bs);
}

void RainDropFactory::update(float deltaTime)
{
    Component::update(deltaTime);

    if (NetworkClient::Instance().isClient() == true)
    {
        return;
    }

    if (InputManager::Instance().keyReleased(sf::Keyboard::Return))
    {
        enabled = !enabled;
        currentSpawnDelay = 0.0f;
    }

    if (enabled)
    {
        currentSpawnDelay += deltaTime;
        if (currentSpawnDelay > spawnDelay)
        {
            currentSpawnDelay = 0.0f;
            Asset* asset = AssetManager::Instance().getAsset(prefabID);
            if (asset != nullptr)
            {
                PrefabAsset* prefab = (PrefabAsset*)asset;
                GameObject* go = prefab->CreatePrefab();

                writePacket(go);
            }
        }
    }
}

