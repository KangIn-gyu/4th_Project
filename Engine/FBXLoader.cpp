#include "pch.h"
#include "FBXLoader.h"
#include "Helper.h"
#include "AiNode.h"

#include "Model.h"
#include "Mesh.h"
#include "StaticMesh.h"
#include "SkeletalMesh.h"
#include "Material.h"

#include <filesystem>

DirectX::XMMATRIX ConvertMatrix(const aiMatrix4x4& _matrix); // 여기서만 사용하는 함수

FBXLoader::~FBXLoader()
{
	for (auto& it : vertexBufferMap)
	{
		SafeExtinction::SAFE_DELETE(it.second);
	}
	vertexBufferMap.clear();

	for (auto& it : indexBufferMap)
	{
		SafeExtinction::SAFE_DELETE(it.second);
	}
	indexBufferMap.clear();

	for (auto& it : meshs)
	{
		it.second.clear(); // 각 vector 내 요소들을 제거
	}
	meshs.clear();

	for (auto& it : materials)
	{
		for (auto& data : it.second)
		{
			SafeExtinction::SAFE_DELETE(data);
		}
		it.second.clear();
	}
	materials.clear();

	if (vertexBufferMap.empty() && indexBufferMap.empty() && meshs.empty() && materials.empty())
	{ 
		std::cout << "FBXLoader 데이터 전부 비움" << std::endl; // 확인용 
	}
}

std::shared_ptr<Model> FBXLoader::FBXLoad(std::wstring_view _filePath)
{
	importFlags = 0; // 시작 플래그 초기화

	importFlags = aiProcess_Triangulate |	// vertex 삼각형 으로 출력         
		aiProcess_GenUVCoords |				// UV 좌표 생성
		aiProcess_CalcTangentSpace |		// 메시의 Tangent와 Bitangent를 계산한다.(Bitangent는 Tangent와 수직인 벡터이다)
		aiProcess_GenNormals |				// Normal 정보 생성  
		aiProcess_ConvertToLeftHanded |		// 우측 좌표계를 사용하는 모델을 좌측 좌표계로 변환해줌 단순히 좌표를 반대로 바꾸는 것이 아니라, 법선 벡터, 카메라 방향, 뼈대 애니메이션의 방향등 좌표계 변환에 따라 영향을 받는 요소들을 모두 적절하게 변환
		aiProcess_LimitBoneWeights |		// 본의 영향을 받는 정점의 최대 개수를 4개로 제한
		aiProcess_RemoveRedundantMaterials; // 사용되지 않는 메테리얼을 제거한다.  

	// 초기 스테틱 메시 확인
	isStaticMesh = true;
	const aiScene* scene = importer.ReadFile(StringConverter::WideToString(_filePath), importFlags);

	if (nullptr == scene)
	{ // 추후 로그 시스템 만들자
		std::runtime_error("Error loading model" + std::string(importer.GetErrorString()));
		return nullptr;
	} 

	// 이 조건문은 좀 더 고민이 필요하다 뼈가 없어도 애니메이션이 가능한 것도 있는데
	if (scene->HasAnimations() || HasBones(scene)) 
	{ // 추후 애니메이션 처리용
		isStaticMesh = false;
	}

	if (true == isStaticMesh)
	{ // 스태틱 매쉬일 경우 플래그 변경
		importFlags |= aiProcess_PreTransformVertices;
		scene = importer.ReadFile(StringConverter::WideToString(_filePath), importFlags);
	}

	aiNode* RootaiNode = scene->mRootNode;
	std::shared_ptr<Model> modelData = std::make_shared<Model>();
	AiNode* rootNode = ProcessNode(RootaiNode, scene, nullptr); // 내가 만든 AiNode로 만든다
	
	if (nullptr != rootNode)
	{
		modelData->SetAiNode(rootNode);
		// 추후 애니메이션 등 추가 필요
	}

	if (scene->HasMaterials())
	{
		ProcessMaterial(scene, _filePath);
	}

//	modelData->SetMesh(meshs.find());

	// 테스트용 매쉬 어떻게 들어가는지 확인용
	for (auto& data : meshs)
	{
		std::cout << "Mesh KEY : " << data.first << std::endl;
		for (auto& it : data.second)
		{
			std::cout << "Meshindex : " << it.GetFbxIndex() << " " << it.GetName() << std::endl;
		}
	}

	for (auto& data : materials)
	{
		std::cout << "Material KEY : " << data.first << std::endl;
		for (auto& it : data.second)
		{
			std::cout << "MaterialName : " << it->GetName() << std::endl;
		}
	}

	importer.FreeScene();
	return modelData;
}

// 여기서 노드와 매쉬를 같이 만든다
AiNode* FBXLoader::ProcessNode(aiNode* _node, const aiScene* _scene, AiNode* _parent)
{
	if (nullptr == _node)
		return nullptr;

	AiNode* currentNode = new AiNode(); // 내가 만든 노드 핵갈리지말자
	currentNode->SetName(_node->mName.C_Str());
	currentNode->GetTransform().SetLocalMatrix(ConvertMatrix(_node->mTransformation));
	currentNode->SetParent(_parent);

	if (nullptr != _parent)
	{
		_parent->AddChild(currentNode);
	}

	if (_scene->HasMeshes())
	{
		for (size_t index = 0; index < _node->mNumMeshes; index++)
		{
			unsigned int meshIndex = _node->mMeshes[index];
			aiMesh* mesh = _scene->mMeshes[meshIndex];
			
			if (isStaticMesh) 
			{ // 스태틱 매쉬
				StaticMesh staticMesh;  // 스태틱 매쉬 생성
				staticMesh.SetName(mesh->mName.C_Str());  // 매쉬 이름 설정
				staticMesh.SetFBXMeshIndex(meshIndex);	  // 인덱스 번호 만들기 없어도 될거 같은데 일단 테스트용
				staticMesh.SetTransform(currentNode->GetPointTransform()); // AiNode라고 내가 만든 어심프의 aiNode의 데이터를 저장한 객체의 트랜스폼 설정
				
				if(nullptr != _parent) // 예외처리
				staticMesh.SetTransformParent(_parent->GetPointTransform()); // 부모 설정

				ProcessMesh(mesh, _scene); // 프로세스매쉬를 하고선 버텍스버퍼/인덱스버퍼가 정보 복사

				// 복사된 데이터의 자료형을 언오더드맵을 통해서 포인터로 받는다.
				staticMesh.SetVertexBuffer(vertexBufferMap.find(staticMesh.GetMeshName())->second);
				staticMesh.SetIndexBuffer(indexBufferMap.find(staticMesh.GetMeshName())->second);
				SaveMeshData(staticMesh.GetMeshName(), staticMesh);
			}
			else
			{ // 스켈레탈 매쉬  나중에 처리 자료형 적립을 다 못함
			  // SkeletalMesh skeletalMesh;
				ProcessMesh(mesh, _scene);
			}
		}
	}

	for (size_t i = 0; i < _node->mNumChildren; i++)
	{
		ProcessNode(_node->mChildren[i], _scene, currentNode);
	}

	return currentNode;
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

void FBXLoader::ProessIndexs(aiMesh* _mesh, unsigned int _indexSize)
{
	std::vector<DWORD> indexBufferData;
	indexBufferData.reserve(static_cast<int>(_indexSize)); // 미리 사이즈 확장

	for (unsigned int j = 0; j < _indexSize; j++) // 인덱스 처리
	{
		aiFace face = _mesh->mFaces[j];

		for (unsigned int k = 0; k < face.mNumIndices; k++)
		{
			DWORD index = face.mIndices[k]; // unsigned short
			indexBufferData.emplace_back(index);
		}
	}

	IndexBuffer* newIndexBuffer = new IndexBuffer;
	newIndexBuffer->Create(indexBufferData);
	indexBufferMap.emplace(_mesh->mName.C_Str(), newIndexBuffer);
}

void FBXLoader::ProcessVertexs(aiMesh* _mesh, unsigned int _vertexSize)
{
	// 일단 맵에 있는지 확인 있으면 해당 값을 준다
	auto it = vertexBufferMap.find(_mesh->mName.C_Str());
	if (it != vertexBufferMap.end())
	{ 
		return; // 이미 맵에 존재하는 경우
	}

	std::vector<Vertex> vertexBufferData;
	vertexBufferData.reserve(static_cast<int>(_vertexSize)); // 미리 사이즈 확장

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

		if (_mesh->HasVertexColors(0))
		{
			vertex.color = { _mesh->mColors[0][i].r, _mesh->mColors[0][i].g, _mesh->mColors[0][i].b, _mesh->mColors[0][i].a }; // 컬러
		}
	
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

	VertexBuffer* newVertexBuffer = new VertexBuffer;
	newVertexBuffer->Create(vertexBufferData);
	vertexBufferMap.emplace(_mesh->mName.C_Str(), newVertexBuffer);
}

void FBXLoader::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
	ProcessVertexs(_mesh, _mesh->mNumVertices);
	ProessIndexs(_mesh, _mesh->mNumFaces);
}

void FBXLoader::SaveMeshData(std::string_view _name, Mesh _mesh)
{
	if (meshs.find(_name.data()) == meshs.end())
	{ // 예외 처리 맵을 찾았을때 비웠을 경우 맵[키] = std::vector를 만들어서 넣는다
		meshs[_name.data()] = std::vector<Mesh>{};
	}
	meshs[_name.data()].push_back(_mesh);
}

void FBXLoader::ProcessMaterial(const aiScene* _scene, const std::wstring_view _modelFilePath)
{
	std::string key = StringConverter::WideToString(_modelFilePath.data());
	auto it = materials.find(key);
	if (it != this->materials.end())
	{ // 이미 맵이 있다면 종료
		return;
	}

	// 내가 쓸 데이터 초기화
	std::vector<Material*> materialvector;
	materialvector.reserve(_scene->mNumMaterials);
	
	aiString texturePath; // 텍스처 절대 경로 나옴

	const std::wstring resourcePrefix = L"Resource/";
	size_t pos = std::wstring(_modelFilePath).find(resourcePrefix);
	std::wstring basePath = L"";
	basePath.reserve(50); // 미리 공간 확장
	if (pos != std::wstring::npos)
	{
		std::wstring remainingPath = std::wstring(_modelFilePath).substr(pos + resourcePrefix.length());

		// 첫 번째 슬래시를 찾아서 그 이전까지의 경로를 basePath로 설정 (예: STAGE1/ 또는 STAGE11/)
		size_t nextSlashPos = remainingPath.find(L"/");
		if (nextSlashPos != std::wstring::npos)
		{
			basePath =  remainingPath.substr(0, nextSlashPos + 1); // "Resource/STAGE1/" 문자 추출
		}
	}
	
	for (int i = 0; i < _scene->mNumMaterials; ++i) // 메테리얼을 생성하고 초기화하는 반복문
	{
		aiMaterial* material = _scene->mMaterials[i];
		Material* materialData = new Material;
		materialData->SetName(material->GetName().C_Str());

		// 여기서 메테리얼한테 모델에 해당하는 모든 텍스처를 저장한다.
		for (int type = aiTextureType_DIFFUSE; type <= aiTextureType_UNKNOWN; ++type)
		{
			int textureCount = material->GetTextureCount((aiTextureType)type);
			for (int texIndex = 0; texIndex < textureCount; ++texIndex)
			{
				if (material->GetTexture((aiTextureType)type, texIndex, &texturePath) == AI_SUCCESS)
				{
					// 아래 코드를 통해서 뒤에서 /이후의 문자열이 나온다
					std::wstring file = StringConverter::GetFileNameFromPath(StringConverter::StringToWide(texturePath.C_Str()));
					std::wstring filePath = basePath + texturesFolder + file; // 최종 경로
					materialData->Load(filePath, (aiTextureType)type);
				}
			}
			materialvector.push_back(materialData);
		}

		if (it == this->materials.end())
		{ // 없으면 생성
			materials[key] = std::vector<Material*>{ materialData };
		}
	}
}

DX::XMMATRIX ConvertMatrix(const aiMatrix4x4& _matrix) // 여기서만 사용하는 함수
{
	return DX::XMMATRIX(
		_matrix.a1, _matrix.b1, _matrix.c1, _matrix.d1,   // 1열
		_matrix.a2, _matrix.b2, _matrix.c2, _matrix.d2,   // 2열
		_matrix.a3, _matrix.b3, _matrix.c3, _matrix.d3,   // 3열
		_matrix.a4, _matrix.b4, _matrix.c4, _matrix.d4    // 4열
	);
}
