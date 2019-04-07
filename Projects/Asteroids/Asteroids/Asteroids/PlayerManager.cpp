#include "GameCore.h"
#include "PlayerManager.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "PrefabAsset.h"
#include "Transform.h"

IMPLEMENT_DYNAMIC_CLASS(PlayerManager);

void PlayerManager::update(float deltaTime)
{
	Component::update(deltaTime); 

	if (!started)
	{
		Asset* asset = AssetManager::Instance().getAsset(prefabID);
		if (asset != nullptr)
		{
			PrefabAsset* prefab = (PrefabAsset*)asset;
			GameObject* go = prefab->CreatePrefab();
			players.push_back(go);
		}

		started = true;
	}
}

void PlayerManager::load(XMLElement * element)
{
	Component::load(element);

	tinyxml2::XMLElement* prefabElement = element->FirstChildElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset element");
	const char* id = prefabElement->GetText();
	prefabID = getHashCode(id);
}
