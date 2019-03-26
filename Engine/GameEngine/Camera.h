#pragma once
#include "Component.h"
#include <SFML/System.hpp>

class Camera : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Camera, Component)

public:
    Camera();
    ~Camera();

    void initialize() override;

    void writeCreate(RakNet::BitStream& bitStream) const override;
    void readCreate(RakNet::BitStream& bitStream) override;
    void load(XMLElement* node) override;

    sf::View& getView() { return view; }
    int getPriority() { return priority; }

protected:
    int priority;
    sf::View view;
    sf::FloatRect viewDetails;
};

