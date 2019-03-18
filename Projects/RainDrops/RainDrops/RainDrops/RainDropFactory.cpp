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
            }
        }
    }
}

