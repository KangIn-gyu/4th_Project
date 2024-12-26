#pragma once
#include "Component.h"

class Mesh;
class Texture;
class Material;
class RenderComponent : public Component
{
public:

private:

public:

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Texture> tuexture;
	std::shared_ptr<Material> material;
};

// 일단 추상화만 하고 나중에 기반 잡을 예정 
// 생각하는 구조는 이 컴포넌트를 만들면 자동으로 Renderer클래스에서 벡터에 자동 저장이 되어
// 여기에 있는 매쉬, 텍스트, 메테리얼을 내보내서 그림을 그리게 할 예정