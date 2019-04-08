#include "GameCore.h"
#include "InputController.h"
#include "Asteroid.h"

IMPLEMENT_DYNAMIC_CLASS(InputController)

using namespace std::placeholders;

void InputController::initialize()
{
    Component::initialize();

    registerRPC(getHashCode("rpcCallback"), std::bind(&InputController::rpcCallback, this, _1));
}

void InputController::update(float deltaTime)
{
    Component::update(deltaTime);

    if (NetworkServer::Instance().isServer() == true)
    {
        return;
    }

    if (InputManager::Instance().mousePressed(sf::Mouse::Left))
    {
        RakNet::BitStream bitStream;
        bitStream.Write((unsigned char)ID_RPC_MESSAGE);
        bitStream.Write(gameObject->getUID());
        bitStream.Write(InputController::getClassHashCode());
        bitStream.Write(getHashCode("rpcCallback"));

        sf::Vector2f mousePosition = InputManager::Instance().getMousePosition();
        bitStream.Write(mousePosition.x);
        bitStream.Write(mousePosition.y);

        NetworkClient::Instance().callRPC(bitStream);
    }
}

void InputController::rpcCallback(RakNet::BitStream& bitStream)
{
    sf::Vector2f mousePosition;
    bitStream.Read(mousePosition.x);
    bitStream.Read(mousePosition.y);

    auto gameObjects = GameObjectManager::Instance().GetAllRootGameObjects();
    for (auto gameObject : gameObjects)
    {
		Asteroid* asteroid = dynamic_cast<Asteroid*>(
                             gameObject->GetComponentByUUID(Asteroid::getClassHashCode()));

        if (asteroid != nullptr)
        {
            if (asteroid->isWithinBounds(mousePosition.x, mousePosition.y))
            {
                GameObjectManager::Instance().DestroyGameObject(gameObject);
                break;
            }
        }
    }
}
