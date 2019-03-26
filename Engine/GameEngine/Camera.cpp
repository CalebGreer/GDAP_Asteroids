#include "Core.h"
#include "Camera.h"
#include "RenderSystem.h"

IMPLEMENT_DYNAMIC_CLASS(Camera)

Camera::Camera()
{
    RenderSystem::Instance().addCamera(this);
}

Camera::~Camera()
{
    RenderSystem::Instance().removeCamera(this);
}

void Camera::initialize()
{
    Component::initialize();

    view = sf::View(sf::Vector2f(viewDetails.left, viewDetails.top), 
                    sf::Vector2f(viewDetails.width, viewDetails.height));
}

void Camera::load(XMLElement * node)
{
    Component::load(node);

    XMLElement* element = nullptr;

    element = node->FirstChildElement("Priority");
    ASSERT(element != nullptr, "Priority node is missing from component");
    priority = element->IntAttribute("value");

    element = node->FirstChildElement("View");
    ASSERT(element != nullptr, "View node is missing from component");
    viewDetails.left = element->FloatAttribute("x");
    viewDetails.top = element->FloatAttribute("y");
    viewDetails.width = element->FloatAttribute("width");
    viewDetails.height = element->FloatAttribute("height");
}

void Camera::writeCreate(RakNet::BitStream & bitStream) const
{
    Component::writeCreate(bitStream);

    bitStream.Write(viewDetails.left);
    bitStream.Write(viewDetails.top);
    bitStream.Write(viewDetails.width);
    bitStream.Write(viewDetails.height);
}

void Camera::readCreate(RakNet::BitStream & bitStream)
{
    Component::readCreate(bitStream);

    bitStream.Read(viewDetails.left);
    bitStream.Read(viewDetails.top);
    bitStream.Read(viewDetails.width);
    bitStream.Read(viewDetails.height);
}
