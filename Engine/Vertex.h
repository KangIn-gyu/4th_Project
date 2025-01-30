#pragma once
#include <assimp/mesh.h>

struct Vertex // 76  // 88
{
	Vertex() : position(DXMath::Vector4::Zero) {}
	Vertex(const Vertex& _other) = default;
	Vertex(Vertex&& _other) noexcept = default; // 이동 생성자
	Vertex& operator = (const Vertex& _other) = default;
	Vertex& operator = (Vertex&& _other) noexcept = default; // 이동 대입 연산자
	~Vertex() = default;

	void LoadAiMeshToVertex(aiMesh* _aiMesh, int _index);

//	UINT vertexID {};            // 버텍스 아이디     4
	DXMath::Vector4 position{};  // 정점 위치 정보.  16
	DXMath::Color   color{};     // 기본 컬러        16
	DXMath::Vector2 uv{};        // uv 좌표          8
	DXMath::Vector3 normal{};    // 노말 벡터        12
	DXMath::Vector3 tangent{};	 // 탄젠트 벡터      12
	DXMath::Vector3 binormal{};	 // 바이노멀 벡터	    12
};

