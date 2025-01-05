#include "pch.h"
#include "RenderComponent.h"
#include "Renderer.h"

RenderComponent::RenderComponent()
{
	RENDERER->AddRenderComponent(this);
}

void RenderComponent::ComponentInitialize()
{

}

void RenderComponent::ComponentRender()
{

}
