#include "GameCore.h"
#include "InputController.h"
#include "Asteroid.h"

IMPLEMENT_DYNAMIC_CLASS(InputController)

using namespace std::placeholders;

void InputController::initialize()
{
    Component::initialize();

    registerRPC(getHashCode("rpcCallback"), std::bind(&InputController::rpcCallback, this, _1));
    registerRPC(getHashCode("rpcMove"), std::bind(&InputController::rpcMove, this, _1));
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

void InputController::rpcMove(RakNet::BitStream& bitStream)
{ 
	int id;
	bitStream.Read(id);

	sf::Vector2f v;
	bitStream.Read(v.x);
	bitStream.Read(v.y); 

	auto gameObjects = GameObjectManager::Instance().GetAllRootGameObjects();
	for (auto gameObject : gameObjects)
	{
		Player* p = dynamic_cast<Player*>(
			gameObject->GetComponentByUUID(Player::getClassHashCode()));

		if (p != nullptr)
		{
			p->setVelocity(v);
		}
	}
}

void InputController::sendVelocity()
{
	RakNet::BitStream bitStream;
	bitStream.Write((unsigned char)ID_RPC_MESSAGE);
	bitStream.Write(gameObject->getUID());
	bitStream.Write(InputController::getClassHashCode());
	bitStream.Write(getHashCode("rpcMove"));

	bitStream.Write(player->getId());

	bitStream.Write(player->getVelocity().x);
	bitStream.Write(player->getVelocity().y);

	NetworkClient::Instance().callRPC(bitStream);
}

void InputController::ProcessInput(float deltaTime)
{
	if (InputManager::Instance().keyPressed(sf::Keyboard::A))
	{
		player->setVelocityX(-player->getSpeed().x * deltaTime);
		sendVelocity();
	}
	else if (InputManager::Instance().keyReleased(sf::Keyboard::A))
	{
		player->setVelocityY(0);
		sendVelocity();
	}

	if (InputManager::Instance().keyPressed(sf::Keyboard::D))
	{
		player->setVelocityX(player->getSpeed().x * deltaTime);
		sendVelocity();
	}
	else if (InputManager::Instance().keyReleased(sf::Keyboard::D))
	{
		player->setVelocityY(0);
		sendVelocity();
	}

	if (InputManager::Instance().keyPressed(sf::Keyboard::S))
	{
		player->setVelocityY(player->getSpeed().y * deltaTime);
		sendVelocity();
	}
	else if (InputManager::Instance().keyReleased(sf::Keyboard::S))
	{
		player->setVelocityY(0);
		sendVelocity();
	}

	if (InputManager::Instance().keyPressed(sf::Keyboard::W))
	{
		player->setVelocityY(-player->getSpeed().y * deltaTime);
		sendVelocity();
	}
	else if (InputManager::Instance().keyReleased(sf::Keyboard::W))
	{
		player->setVelocityY(0);
		sendVelocity();
	}
}
