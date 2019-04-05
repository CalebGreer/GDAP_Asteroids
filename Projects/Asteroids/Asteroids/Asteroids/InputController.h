#pragma once

#include "Component.h"
#include <SFML\System.hpp>

class InputController : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(InputController, Component)

public:
    void initialize() override;
    void update(float deltaTime) override;

    void rpcCallback(RakNet::BitStream& bitStream);
};

