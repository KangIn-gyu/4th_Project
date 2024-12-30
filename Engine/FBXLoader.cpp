#include "pch.h"
#include "FBXLoader.h"
#include "Helper.h"
#include "AiNode.h"

DirectX::XMMATRIX ConvertMatrix(const aiMatrix4x4& matrix); // 여기서만 사용하는 함수

void FBXLoader::FBXLoad(std::wstring_view _filePath)
{
	importFlags = 0; // 시작 플래그 초기화

	importFlags = aiProcess_Triangulate |				// vertex 삼각형 으로 출력         
				  aiProcess_GenUVCoords |				// UV 좌표 생성
				  aiProcess_CalcTangentSpace |			// 메시의 Tangent와 Bitangent를 계산한다.(Bitangent는 Tangent와 수직인 벡터이다)
				  aiProcess_GenNormals |				// Normal 정보 생성  
				  aiProcess_ConvertToLeftHanded |		// 우측 좌표계를 사용하는 모델을 좌측 좌표계로 변환해줌 단순히 좌표를 반대로 바꾸는 것이 아니라, 법선 벡터, 카메라 방향, 뼈대 애니메이션의 방향등 좌표계 변환에 따라 영향을 받는 요소들을 모두 적절하게 변환
		          aiProcess_LimitBoneWeights;			// 본의 영향을 받는 정점의 최대 개수를 4개로 제한

	const aiScene* scene = importer.ReadFile(StringConverter::WideToString(_filePath), importFlags);

	if (nullptr == scene)
	{ // 추후 로그 시스템 만들자
		std::runtime_error("Error loading model" + std::string(importer.GetErrorString()));
		return;
	}

	aiNode* RootNode = scene->mRootNode;

	if (scene->HasAnimations())
	{ // 추후 애니메이션 처리용
		
	}

	ProcessNode(RootNode, scene, nullptr);
}

void FBXLoader::ProcessNode(aiNode* _node, const aiScene* _scene, AiNode* _parent)
{
	if (nullptr == _node)
		return;

	AiNode* currentNode = new AiNode();
	currentNode->SetName(_node->mName.C_Str());
	currentNode->GetTransform().SetLocalMatrix(ConvertMatrix(_node->mTransformation));
	currentNode->SetParent(_parent);

	if (nullptr != _parent)
	{
		_parent->AddChild(currentNode);
	}

	if (_scene->HasMeshes())
	{

	}
}

DX::XMMATRIX ConvertMatrix(const aiMatrix4x4& matrix)
{
	return DX::XMMATRIX(
		matrix.a1, matrix.b1, matrix.c1, matrix.d1,   // 1열
		matrix.a2, matrix.b2, matrix.c2, matrix.d2,   // 2열
		matrix.a3, matrix.b3, matrix.c3, matrix.d3,   // 3열
		matrix.a4, matrix.b4, matrix.c4, matrix.d4    // 4열
	);
}