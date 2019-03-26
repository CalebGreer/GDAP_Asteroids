#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_
#pragma once

#include "ISystem.h"
#include <SFML\Graphics.hpp>

class Renderable;
class GameObject;
class Camera;

class RenderSystem : public ISystem
{
private:
	// File properties
	bool loadedSettings;
	std::string name;
	int width;
	int height;
	bool fullscreen;
	sf::View view;

    /// Cameras
    std::list<Camera*> cameras;

	/// Render components
	std::list<Renderable*> renderComponents;

	/// The Window
	sf::RenderWindow* window;

public:
	inline static RenderSystem& Instance()
	{
		static RenderSystem instance;
		return instance;
	}

	sf::View& getView() { return view; }
	sf::RenderWindow* GetRenderWindow() { return window; }

	virtual void update(float deltaTime);

    void addCamera(Camera* camera) { cameras.push_back(camera); }
    void removeCamera(Camera* camera) { cameras.remove(camera); }

protected:
	virtual void initialize();

private:
	void loadSettings();

    void addRenderable(Renderable* renderable);
    void removeRenderable(Renderable* renderable);

private:

	RenderSystem() = default;
	~RenderSystem() = default;
	RenderSystem(RenderSystem const&) = delete;

    friend class GameEngine;
    friend class Renderable;
};

#endif

