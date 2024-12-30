#include "pch.h"
#include "FBXLoader.h"
#include "Helper.h"
#include "AiNode.h"


#include "StaticMesh.h"
#include "SkeletalMesh.h"
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

	// 초기 스테틱 메시 확인
	isStaticMesh = true;
	const aiScene* scene = importer.ReadFile(StringConverter::WideToString(_filePath), importFlags);

	if (nullptr == scene)
	{ // 추후 로그 시스템 만들자
		std::runtime_error("Error loading model" + std::string(importer.GetErrorString()));
		return;
	}

	aiNode* RootNode = scene->mRootNode;

	// 이 조건문은 좀 더 고민이 필요하다 뼈가 없어도 애니메이션이 가능한 것도 있는데
	if (scene->HasAnimations() || HasBones(scene)) 
	{ // 추후 애니메이션 처리용
		isStaticMesh = false;
	}

	if (true == isStaticMesh)
	{
		importFlags |= aiProcess_PreTransformVertices;
		scene = importer.ReadFile(StringConverter::WideToString(_filePath), importFlags);
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
		for (size_t i = 0; i < _node->mNumMeshes; i++)
		{
			unsigned int meshIndex = _node->mMeshes[i];
			aiMesh* mesh = _scene->mMeshes[meshIndex];

			if (isStaticMesh)
			{
				ProcessMesh(mesh, _scene);
			}
			else
			{
				ProcessMesh(mesh, _scene);
			}
		}
	}
}

bool FBXLoader::HasBones(const aiScene* _scene)
{
	for (unsigned int i = 0; i < _scene->mNumMeshes; i++)
	{
		if (_scene->mMeshes[i]->mNumBones > 0)
		{
			return true;
		}
	}
	return false;
}

std::vector<DWORD> FBXLoader::ProessIndexs(aiMesh* _mesh, unsigned int _indexSize)
{
	std::vector<DWORD> indexBufferData;

	for (unsigned int j = 0; j < _indexSize; j++) // 인덱스 처리
	{
		aiFace face = _mesh->mFaces[j];

		for (unsigned int k = 0; k < face.mNumIndices; k++)
		{
			DWORD index = face.mIndices[k]; // unsigned short
			indexBufferData.emplace_back(index);
		}
	}

	return indexBufferData;
}

std::vector<Vertex> FBXLoader::ProcessVertexs(aiMesh* _mesh, unsigned int _vertexSize)
{
	std::vector<Vertex> vertexBufferData;
	for (unsigned int i = 0; i < _vertexSize; i++) // 버텍스 처리
	{
		Vertex vertex {};
		if (_mesh->HasPositions())   // Pos
		{
			vertex.position = { _mesh->mVertices[i].x,  _mesh->mVertices[i].y,  _mesh->mVertices[i].z };
		}

		if (_mesh->HasNormals()) // 노말
		{
			vertex.normal = { _mesh->mNormals[i].x,    _mesh->mNormals[i].y,  _mesh->mNormals[i].z };
		}

		if (_mesh->HasTangentsAndBitangents())
		{
			vertex.tangent = { _mesh->mTangents[i].x , _mesh->mTangents[i].y, _mesh->mTangents[i].z };           // 탄젠트         
			vertex.binormal = { _mesh->mBitangents[i].x,_mesh->mBitangents[i].y , _mesh->mBitangents[i].z };     // bi탄젠트
		}

		vertex.color = { _mesh->mColors[i]->r, _mesh->mColors[i]->g, _mesh->mColors[i]->b, _mesh->mColors[i]->a }; // 컬러

		if (_mesh->mTextureCoords[0]) // uv
		{
			vertex.uv = { _mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y };
		}
		else
		{
			vertex.uv = { 0.0f, 0.0f };
		}

		vertexBufferData.emplace_back(vertex);
	}
	return vertexBufferData;
}

void FBXLoader::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
	Mesh mesh;
	mesh.SetName(_mesh->mName.C_Str());
	mesh.CreateVertexBuffer(ProcessVertexs(_mesh, _mesh->mNumVertices));
	mesh.CreateIndexBuffer(ProessIndexs(_mesh, _mesh->mNumFaces));

	if (_scene->HasMaterials())
	{
		ProcessMaterial(_mesh, _scene);
	}
}

void FBXLoader::ProcessMaterial(aiMesh* _mesh, const aiScene* _scene)
{
	aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];
	aiString texturePath;

	// 나중에 처리하자
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