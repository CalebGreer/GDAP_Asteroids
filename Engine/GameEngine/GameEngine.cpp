// Notes:
//		Will need a load method possibly?

#include "Core.h"
#include "GameEngine.h"

// The systems we support
#include "AssetManager.h"
#include "FileSystem.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "RenderSystem.h"
#include "NetworkClient.h"
#include "NetworkServer.h"

void GameEngine::initialize()
{
	// Determine what we are here (easier for our implementation)
	int serverClientChoice = -1;
	std::cout << "Server [0] or Client [1]: ";
	std::cin >> serverClientChoice;
	NetworkClient::Instance().isclient = (serverClientChoice == 0 ? false : true);
	NetworkServer::Instance().isserver = (serverClientChoice == 0 ? true : false);

	NetworkClient::Instance().initialize();
	NetworkServer::Instance().initialize();

	// Init the Random?
	AssetManager::Instance().initialize();
	GameObjectManager::Instance().initialize();
	RenderSystem::Instance().initialize();
	InputManager::Instance().initialize();

	// Needs to be at the end because we will load a default file
	FileSystem::Instance().initialize();
}

void GameEngine::GameLoop()
{
	float gameTime = 0.0f;
	clock_t _startTime;
	float deltaTime = 0.0f;

	while (true)
	{
        _startTime = clock();

		NetworkClient::Instance().update(deltaTime);
		NetworkServer::Instance().update(deltaTime);

		InputManager::Instance().update(deltaTime);

		FileSystem::Instance().update(deltaTime);

		AssetManager::Instance().update(deltaTime);

		GameObjectManager::Instance().update(deltaTime);

		if (gameUpdate != nullptr)
		{
			gameUpdate(deltaTime);
		}

        if (NetworkServer::Instance().isServer())
        {
            GameObjectManager::Instance().networkUpdate(deltaTime);
        }

		RenderSystem::Instance().update(deltaTime);

        // Its not pretty but hey we need it. When running the server we run too fast.... that means we don't get updates
        // but also we are sending a lot to the clients (we will fix this later).
        // For now we will assume that if the server is running faster than 60fps we will stall. When we actually have a render
        // window we are locked to 60fps because of SFML but the server doesn't have a window and we need to manually do some sleeping
		clock_t _time = clock() - _startTime;
        if (NetworkServer::Instance().isServer() && _time < 16)
        {
            int sleepTime = 16 - _time;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }

        _time = clock() - _startTime;
        deltaTime = ((float)_time) / ((clock_t)1000);
		gameTime += deltaTime;

        LOG("Current Game Time: " << gameTime << " Delta Time: " << deltaTime);
	}
}

