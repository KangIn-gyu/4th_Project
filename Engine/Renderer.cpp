#include "pch.h"
#include "Renderer.h"

void Renderer::Initialize(WindowInfo* _windowInfo)
{
	D3DGraphics = std::make_unique<D3DClass>();
	D3DGraphics->Initialize(_windowInfo);

}


void Renderer::Render()
{
	D3DGraphics->BeginDraw({ 0.0f, 0.0f, 0.0f, 0.0f});

	D3DGraphics->EndDraw();
}
