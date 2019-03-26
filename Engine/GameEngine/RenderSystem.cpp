#include "Core.h"
#include "RenderSystem.h"
#include "Renderable.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include "Camera.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

void RenderSystem::initialize()
{
	ISystem::initialize();

	loadedSettings = false;
	window = NULL;
	loadSettings();

	if (loadedSettings)
	{
        if (NetworkClient::Instance().isClient())
        {
            window = new sf::RenderWindow(sf::VideoMode(width, height), name.c_str());
            window->setFramerateLimit(60);
        }

		view = sf::View(sf::Vector2f((width * 0.5f), (-height * 0.5)), sf::Vector2f(width, height));
	}
}

void RenderSystem::loadSettings()
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->LoadFile("Assets/RenderSystemSettings.xml");

	if (doc->ErrorID() == XML_SUCCESS)
	{
		XMLElement* element = NULL;

		element = doc->FirstChildElement("name");
		if (element != NULL)
		{
			name = element->GetText();
		}
		element = doc->FirstChildElement("width");
		if (element != NULL)
		{
			element->QueryIntText(&width);
		}
		element = doc->FirstChildElement("height");
		if (element != NULL)
		{
			element->QueryIntText(&height);
		}
		element = doc->FirstChildElement("fullscreen");
		if (element != NULL)
		{
			element->QueryBoolText(&fullscreen);
		}
		loadedSettings = true;
	}
	delete doc;
}

void RenderSystem::addRenderable(Renderable* renderable)
{
    renderComponents.push_back(renderable);
}

void RenderSystem::removeRenderable(Renderable* renderable)
{
    renderComponents.remove(renderable);
}

void RenderSystem::update(float deltaTime)
{
    if (NetworkServer::Instance().isServer())
    {
        int priority = INT_MIN;
        for (auto camera : cameras)
        {
            if (camera->getPriority() > priority)
            {
                priority = camera->getPriority();
                view = camera->getView();
            }
        }
    }
	
    if (window != NULL)
	{
		window->clear();

        for (auto camera : cameras)
        {
            int priority = INT_MIN;
            if (camera->getPriority() > priority)
            {
                priority = camera->getPriority();
                view = camera->getView();
            }

            window->setView(camera->getView());

            for (auto renderable : renderComponents)
            {
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glScalef(1.0f, -1.0f, 1.0f);
                window->pushGLStates();

                renderable->render(window, renderable->gameObject->getTransform());

                window->popGLStates();
            }
        }

		window->display();
	}
}
