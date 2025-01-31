#pragma once
#include "Component.h"

enum class ShaderType  
{ // 추후 더 추가될 수 있음
	VS,
	PS,
	End
};

class Shader;
class Material;
class Model;
class AiNode;
class D2DFont;
class RenderComponent : public Component
{
public:
	RenderComponent();
	virtual ~RenderComponent();

	virtual void ComponentInitialize() override;
	virtual void ComponentUpdate(const float _deltaTime) override {}


	std::shared_ptr<Model> GetModelData() { return modelData; }
	std::shared_ptr<D2DFont> GetD2DFont() { isD2D = TRUE; return d2dFontData; }
	std::shared_ptr<Shader> GetShader(ShaderType _shaderType);
	void SetShader(ShaderType _shaderType , std::string_view _filePath); // 이건 명시적으로 불러서 하는게 좋다고 판단함.
	std::unordered_map<std::string, AiNode*>* GetNodeData() { return nodeData; }
private:

public:
	bool isD2D = FALSE;
private:
	std::unordered_map<std::string, AiNode*>* nodeData;
	std::shared_ptr<Model> modelData;
	std::shared_ptr<D2DFont> d2dFontData;
	std::unordered_map<ShaderType , std::shared_ptr<Shader>> shaders;


	// 불값 만들기 이유 D2D 드로우 할 것들 처리();
};
