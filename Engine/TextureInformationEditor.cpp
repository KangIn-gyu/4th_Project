#include "pch.h"
#include "TextureInformationEditor.h"
#include "Texture.h"

TextureInformationEditor::TextureInformationEditor()
{
	SetName("TextureInformation");
	state = State::Active;
}

void TextureInformationEditor::Initialize()
{

}

void TextureInformationEditor::Update()
{

}

void TextureInformationEditor::Draw()
{

}

void TextureInformationEditor::SetSelectedTexture(Texture* _texture)
{
	texture = _texture;
}
