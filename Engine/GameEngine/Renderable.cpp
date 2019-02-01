#include "Core.h"
#include "Renderable.h"
#include "RenderSystem.h"

IMPLEMENT_ABSTRACT_CLASS(Renderable);

Renderable::Renderable()
{
    RenderSystem::Instance().addRenderable(this);
}

Renderable::~Renderable()
{
    RenderSystem::Instance().removeRenderable(this);
}
