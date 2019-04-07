#include "GameCore.h"
#include "GameEngine.h"

#include "AsteroidFactory.h"
#include "Asteroid.h"
#include "PlayerManager.h"
#include "Player.h"
#include "InputController.h"

int main()
{
	// Register all custom components here
	REGISTER_DYNAMIC_CLASS(AsteroidFactory);
    REGISTER_DYNAMIC_CLASS(Asteroid);
    REGISTER_DYNAMIC_CLASS(PlayerManager);
    REGISTER_DYNAMIC_CLASS(Player);
    REGISTER_DYNAMIC_CLASS(InputController);
	
	GameEngine::Instance().initialize();

	// initialize anything we need then call GameLoop

	GameEngine::Instance().GameLoop();
}

