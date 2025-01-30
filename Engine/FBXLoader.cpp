#include "pch.h"
#include "FBXLoader.h"
#include "Helper.h"
#include "AiNode.h"

#include "Model.h"
#include "Mesh.h"
#include "StaticMesh.h"
#include "SkeletalMesh.h"
#include "Material.h"
#include "Transform.h"
#include "Texture.h"
#include <filesystem>

DirectX::XMMATRIX ConvertMatrix(const aiMatrix4x4& _matrix); // 여기서만 사용하는 함수


std::shared_ptr<Model> FBXLoader::FBXLoad(std::string_view _filePath)
{
	std::string filePathKEY = _filePath.data(); // 파일 경로가 모든 맵의 키값이다

	importFlags = 0; // 시작 플래그 초기화
	importFlags = aiProcess_Triangulate |	 // vertex 삼각형 으로 출력         
		aiProcess_GenUVCoords |				 // UV 좌표 생성
		aiProcess_CalcTangentSpace |		 // 메시의 Tangent와 Bitangent를 계산한다.(Bitangent는 Tangent와 수직인 벡터이다)
		aiProcess_GenNormals |				 // Normal 정보 생성  
		aiProcess_ConvertToLeftHanded |		 // 우측 좌표계를 사용하는 모델을 좌측 좌표계로 변환해줌 단순히 좌표를 반대로 바꾸는 것이 아니라, 법선 벡터, 카메라 방향, 뼈대 애니메이션의 방향등 좌표계 변환에 따라 영향을 받는 요소들을 모두 적절하게 변환
		aiProcess_LimitBoneWeights |		 // 본의 영향을 받는 정점의 최대 개수를 4개로 제한
		aiProcess_RemoveRedundantMaterials;// | // 사용되지 않는 메테리얼을 제거한다. 


	// 초기 스테틱 메시 확인
	isStaticMesh = true;
	const aiScene* scene = importer.ReadFile(filePathKEY, importFlags);

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
		scene = importer.ReadFile(filePathKEY, importFlags);
	}
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);

	aiNode* rootaiNode = scene->mRootNode; // 어심프 노드
	std::shared_ptr<Model> modelData = std::make_shared<Model>(); // 모델에 관련된 정보 데이터 저장용
	AiNode* rootNode{}; // 내가 만든 AiNode 데이터 저장용
	isStaticMesh = true;
	auto treeNode = aiNodeMap.find(filePathKEY); // aiNode가 있는지 확인
	if (treeNode != aiNodeMap.end()) // 맵을 통해 해당 노드를 생성한지 확인해 본다. 맵에서 찾았을때 없으면 처음 로드하는 것
	{ // 존재할 경우
		modelData->GetModelData()->treeNode = &treeNode->second;
	}
	else
	{ // 없을 경우는 트리 노드를 받아서 생성
		rootNode = ProcessNode(rootaiNode, scene, nullptr, filePathKEY); // 재귀를 통한 데이터 저장
		if (nullptr != rootNode) // 예외 처리
		{
			std::vector<AiNode*> treeNodeSave;
			// ProcessNode함수에서 맵에 저장했을 경우 깊은 복사로 인해 객체 자체를 복사해서 생성함 
			// 그래서 _parent 파라미터가 AddChild한 값이 없음. 그래서 재귀를 한번더 해서 처리함. 
			CollectNodes(rootNode, &treeNodeSave); 
			aiNodeMap[filePathKEY] = std::move(treeNodeSave);
		
			if (scene->HasMaterials())
			{
				ProcessMaterial(scene, filePathKEY);
			}

			modelData->GetModelData()->treeNode = &aiNodeMap[filePathKEY];
		}	
	} 
	modelData->GetModelData()->rootNode = rootNode;
	modelData->SetMesh(&meshMap.find(filePathKEY)->second);
	modelData->SetMateria(&materials.find(filePathKEY)->second);

	CalculateBoundingBox(scene, modelData);
	nameCountMap.clear();

	AllShow();

	NodeAndMeshIndex(filePathKEY);
	importer.FreeScene();
	return modelData;
}

// 여기서 노드와 매쉬를 같이 만든다
AiNode* FBXLoader::ProcessNode(aiNode* _node, const aiScene* _scene, AiNode* _parent, const std::string_view _filePath)
{
	if (nullptr == _node)
		return nullptr;

	std::string baseNodeName = std::string(_node->mName.C_Str());

	// 이름 중복 체크
	int count = nameCountMap[baseNodeName]++;
	std::string nodeName = count == 0 ? baseNodeName : baseNodeName + "_" + std::to_string(count);
	// 나중에 라이트나 카메라 관련 정보 받아 오는 거 필요할 거 같다 
	AiNode* currentNode = new AiNode(); // 힙에 할당 내가 만든 노드 핵갈리지말자
	currentNode->SetName(nodeName);
	currentNode->SetParent(_parent);
	currentNode->SetLocalTransform(ConvertMatrix(_node->mTransformation)); // 어심프 노드의 _node->mTransformation 트랜스폼을 ConvertMatrix 함수로 변경

	if (nullptr != _parent)
	{
		currentNode->SetParent(_parent);
		_parent->AddChild(currentNode);
	}

	if (_scene->HasMeshes())
	{
		for (size_t index = 0; index < _node->mNumMeshes; index++)
		{
			unsigned int meshIndex = _node->mMeshes[index];
			aiMesh* mesh = _scene->mMeshes[meshIndex];
			int materialIndex = mesh->mMaterialIndex;

			if (isStaticMesh) 
			{ // 스태틱 매쉬
				StaticMesh* staticMesh = new StaticMesh;  // 스태틱 매쉬 생성
				staticMesh->SetFBXMeshIndex(meshIndex);	  // 인덱스 번호 만들기 없어도 될거 같은데 일단 테스트용 여기서 매쉬 인포 생성
				staticMesh->SetMaterialIndex(materialIndex);
				staticMesh->SetTransform(currentNode->GetPointTransform()); // AiNode라고 내가 만든 어심프의 aiNode의 데이터를 저장한 객체의 트랜스폼 설정
				staticMesh->SetName(nodeName);
				if (nullptr != _parent) // 예외처리
				{
					staticMesh->SetTransformParent(_parent->GetPointTransform()); // 부모 설정
				}
				else
				{
					staticMesh->SetName(nodeName);  // 매쉬 이름 설정
				}
				currentNode->SetMesh(staticMesh); // 노드안에 매쉬 넣기 
				ProcessMesh(mesh, _scene, _filePath); // 프로세스매쉬를 하고선 버텍스버퍼/인덱스버퍼가 정보 복사

				// 복사된 데이터의 자료형을 언오더드맵을 통해서 포인터로 받는다.
				staticMesh->SetVertexBuffer(vertexBufferMap.find(_filePath.data())->second[meshIndex]);
				staticMesh->SetIndexBuffer(indexBufferMap.find(_filePath.data())->second[meshIndex]);
				SaveMeshData(_filePath, std::move(staticMesh));
			}
			else
			{ // 스켈레탈 매쉬 나중에 처리 자료형 적립을 다 못함
			    SkeletalMesh* skeletalMesh = new SkeletalMesh;
				currentNode->SetMesh(skeletalMesh); // 노드안에 매쉬 넣기 
				ProcessMesh(mesh, _scene, _filePath);
			}
		}
	}

	for (size_t i = 0; i < _node->mNumChildren; i++)
	{  // 자식 노드 재귀 처리
		ProcessNode(_node->mChildren[i], _scene, currentNode, _filePath);
	}

	return currentNode;
}

void FBXLoader::CollectNodes(AiNode* _rootNode, std::vector<AiNode*>* _nodes)
{
	if (_rootNode == nullptr)
		return;

	_nodes->push_back(_rootNode);
	auto children = _rootNode->GetChildren(); // 여기 까지는 노드이름을 가지고 있다
	for (AiNode* child : children)
	{
		CollectNodes(child, _nodes);
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

void FBXLoader::ProessIndexs(aiMesh* _mesh, unsigned int _indexSize, const std::string_view _filePath)
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

	if (indexBufferMap.find(_filePath.data()) != indexBufferMap.end())
	{   // 기존 벡터에 추가
		indexBufferMap[_filePath.data()].push_back(newIndexBuffer);
	}
	else 
	{   // 새 벡터 생성 후 추가
		indexBufferMap.emplace(_filePath, std::vector<IndexBuffer*>{newIndexBuffer});
	}
}

void FBXLoader::ProcessVertexs(aiMesh* _mesh, unsigned int _vertexSize, const std::string_view _filePath)
{
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
	if (vertexBufferMap.find(_filePath.data()) != vertexBufferMap.end())
	{   // 기존 벡터에 추가
		vertexBufferMap[_filePath.data()].push_back(newVertexBuffer);
	}
	else
	{   // 새 벡터 생성 후 추가
		vertexBufferMap.emplace(_filePath, std::vector<VertexBuffer*>{newVertexBuffer});
	}
}

void FBXLoader::ProcessMesh(aiMesh* _mesh, const aiScene* _scene, const std::string_view _filePath)
{
	ProcessVertexs(_mesh, _mesh->mNumVertices, _filePath);
	ProessIndexs(_mesh, _mesh->mNumFaces, _filePath);
}

void FBXLoader::SaveMeshData(std::string_view _filePath, Mesh* _mesh)
{
	if (meshMap.find(_filePath.data()) == meshMap.end())
	{ // 예외 처리 맵을 찾았을때 비웠을 경우 맵[키] = std::vector를 만들어서 넣는다
		meshMap[_filePath.data()] = std::vector<Mesh*>{};
	}
	// 해당 맵이 있으면 그 키값에 저장
	meshMap[_filePath.data()].emplace_back(std::move(_mesh));
}

void FBXLoader::ProcessMaterial(const aiScene* _scene, const std::string_view _modelFilePath)
{
   std::string key = _modelFilePath.data();
   auto it = materials.find(key);
   if (it != this->materials.end())
   { // 이미 맵이 있다면 종료 새로 만들필요가 없으니 패스
      return;
   }

   // 내가 쓸 데이터 초기화
   std::vector<Material*> materialvector;
   materialvector.reserve(_scene->mNumMaterials); //미리 크기 초기화
   
   aiString texturePath; // 텍스처 절대 경로 나옴

   const std::string resourcePrefix = "Resource/";
   size_t pos = std::string(_modelFilePath).find(resourcePrefix);
   std::string basePath = "";
   basePath.reserve(50); // 미리 공간 확장
   if (pos != std::string::npos)
   {
      std::string remainingPath = std::string(_modelFilePath).substr(pos + resourcePrefix.length());
      // 첫 번째 슬래시를 찾아서 그 이전까지의 경로를 basePath로 설정 (예: STAGE1/ 또는 STAGE11/)
      size_t nextSlashPos = remainingPath.find("/");
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

      for (int type = aiTextureType_DIFFUSE; type <= aiTextureType_TRANSMISSION; ++type)
      {  // 여기서 메테리얼한테 모델에 해당하는 모든 텍스처를 저장한다.
         int textureCount = material->GetTextureCount((aiTextureType)type);
         for (int texIndex = 0; texIndex < textureCount; ++texIndex)
         {
            if (material->GetTexture((aiTextureType)type, 0, &texturePath) == AI_SUCCESS)
            {   // 아래 코드를 통해서 뒤에서 /이후의 문자열이 나온다
               std::string file = StringConverter::GetFileNameFromPath<std::string>(StringConverter::StringToWide(texturePath.C_Str()));
               std::string filePath = basePath + texturesFolder + file; // 최종 경로
               materialData->Load(filePath , (aiTextureType)type);
			}
         }
      }
      materialvector.push_back(materialData);

      if (it == this->materials.end())
      { // 없으면 생성
         materials[key] = materialvector;
      }
   }
}

void FBXLoader::AllShow()
{
	ShowAiNode();
	ShowMesh();
	ShowVertexBuffer();
	ShowIndexBuffer();
	ShowMaterials();
}

void FBXLoader::FindShow(std::string_view _filePath)
{
	std::string filepath = _filePath.data();

	auto aiNodeIt = aiNodeMap.find(filepath);
	if (aiNodeIt != aiNodeMap.end()) 
	{
		for (int i = 0; i < aiNodeIt->second.size(); i++) 
		{
			std::cout << aiNodeIt->second[i]->GetName() << '\n';
			aiNodeIt->second[i]->ShowChild();
		}
	}
	else 
	{
		std::cout << "해당 경로의 AiNodeMap에 정보가 없습니다. " << filepath << '\n';
	}

	// vertexBufferMap에서 해당 파일 경로에 대한 결과 찾기
	auto vertexBufferIt = vertexBufferMap.find(filepath);
	if (vertexBufferIt != vertexBufferMap.end()) 
	{
		for (auto& data : vertexBufferIt->second) 
		{
			std::cout << "VertexBuffer Size : " << data->vertices.size() << std::endl;
		}
	}
	else 
	{
		std::cout << "해당 경로의 VertexBuffer에 정보가 없습니다. " << filepath << '\n';
	}

	// indexBufferMap에서 해당 파일 경로에 대한 결과 찾기
	auto indexBufferIt = indexBufferMap.find(filepath);
	if (indexBufferIt != indexBufferMap.end()) 
	{
		for (auto& data : indexBufferIt->second) 
		{
			std::cout << "IndexBuffer Size : " << data->indices.size() << std::endl;
		}
	}
	else 
	{
		std::cout << "해당 경로의 IndexBuffer에 정보가 없습니다. " << filepath << '\n';
	}

	// meshMap에서 해당 파일 경로에 대한 결과 찾기
	auto meshIt = meshMap.find(filepath);
	if (meshIt != meshMap.end()) 
	{
		for (auto& it : meshIt->second) 
		{
			std::cout << "MeshIndex(" << it->GetFbxIndex() << ") " << it->GetName() << '\n';
		}
	}
	else 
	{
		std::cout << "해당 경로의 Mesh에 정보가 없습니다. " << filepath << '\n';
	}

	// materials에서 해당 파일 경로에 대한 결과 찾기
	auto materialsIt = materials.find(filepath);
	if (materialsIt != materials.end()) 
	{
		for (int i = 0; i < materialsIt->second.size(); i++) 
		{
			std::cout << i << "." << " " << materialsIt->second[i]->GetName() << "\n";
		}
	}
	else 
	{
		std::cout << "해당 경로의 Materials에 정보가 없습니다. " << filepath << '\n';
	}
}

std::vector<AiNode*> FBXLoader::DeepCopyAiNodes(std::string_view key)
{
	std::vector<AiNode*> copiedNodes;

	if (aiNodeMap.find(key.data()) != aiNodeMap.end()) 
	{
		for (const auto& node : aiNodeMap[key.data()])
		{
			AiNode* newNode = new AiNode(*node);
			copiedNodes.push_back(newNode);
		}
	}

	return copiedNodes;
}

void FBXLoader::ShowMaterials()
{
	for (auto& it : materials)
	{
		std::cout << "Materials KEY : " << it.first << '\n';
		for (int i = 0; i < it.second.size(); i++)
		{
			std::cout << i << '.' << " " << it.second[i]->GetName() << "\n";
			for (auto& data : it.second[i]->GetTextures())
			{
				for (auto index : data->GetTextureTypeIndexs())
				{
					std::cout << "Texture Index : " << index << " " << data->GetName() << '\n';
				}
			}
		}
		std::cout << '\n';
	}
}

void FBXLoader::ShowMesh()
{
	for (auto& data : meshMap)
	{
		std::cout << "Mesh KEY : " << data.first << std::endl;
		for (auto& it : data.second)
		{
			std::cout << "MeshIndex(" << it->GetFbxIndex() << ") " << it->GetName() << '\n';
		}
		std::cout << '\n';
	}
}

void FBXLoader::ShowAiNode()
{
	for (auto& it : aiNodeMap)
	{
		std::cout << "AiNode KEY : " << it.first << '\n';
		for (int i = 0; i < it.second.size(); i++)
		{
			std::cout << it.second[i]->GetName() << '\n';
			it.second[i]->ShowChild();
		}
		std::cout << '\n';
	}
}

void FBXLoader::ShowVertexBuffer()
{
	for (auto& it : vertexBufferMap)
	{
		std::cout << "VertexBuffer KEY : " << it.first << '\n';
		for (auto& data : it.second)
		{
			std::cout << "VertexBuffer Size :" << data->vertices.size() << std::endl;
		}
	}
}

void FBXLoader::ShowIndexBuffer()
{
	for (auto& it : indexBufferMap)
	{
		std::cout << "indexBufferMap KEY : " << it.first << '\n';
		for (auto& data : it.second)
		{
			std::cout << "indexBufferMap Size : " << data->indices.size() << std::endl;
		}
	}
}

void FBXLoader::NodeAndMeshIndex(const std::string_view _filePath)
{
	std::cout << "노드와 매쉬 인덱스 맞는지 확인하는 코드" << '\n';
	for (auto& data : aiNodeMap[_filePath.data()])
	{
		std::cout << data->GetName() << "\n";
	}

	for (auto& data : meshMap[_filePath.data()])
	{
		std::cout << data->GetFbxIndex() << " " << data->GetName() << '\n';
	}
}

FBXLoader::~FBXLoader()
{
	SafeExtinction::SAFE_CLEAR_CONTAINER(vertexBufferMap);
	SafeExtinction::SAFE_CLEAR_CONTAINER(indexBufferMap);
	SafeExtinction::SAFE_CLEAR_CONTAINER(meshMap);
	SafeExtinction::SAFE_CLEAR_CONTAINER(materials);
	SafeExtinction::SAFE_CLEAR_CONTAINER(aiNodeMap);
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


DXMath::Vector3 FBXLoader::CalculateBoundingBox(const aiScene* scene, std::shared_ptr<Model> _modelData)
{
	if (!scene || !scene->mMeshes || scene->mNumMeshes == 0) {
		std::cerr << "Invalid scene or no meshes available!" << std::endl;
		return DXMath::Vector3();
	}


	aiVector3D min = aiVector3D((10000, 10000, 10000));
	aiVector3D max = aiVector3D(-10000, -10000, -10000);
	// 모든 메쉬를 순회
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];

		if (mesh->mNumVertices == 0) {
			continue; // 빈 메쉬는 무시
		}

		// 각 메쉬의 모든 버텍스를 순회
		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			aiVector3D vertex = mesh->mVertices[j];

			// 최소/최대 좌표 갱신
			min.x = std::min(min.x, vertex.x);
			min.y = std::min(min.y, vertex.y);
			min.z = std::min(min.z, vertex.z);

			max.x = (std::max)(max.x, vertex.x);
			max.y = (std::max)(max.y, vertex.y);
			max.z = (std::max)(max.z, vertex.z);
		}
	}

	// 디버깅: 최종 min/max 확인
	//std::cout << "Final min:****************************************************************** (" << min.x << ", " << min.y << ", " << min.z << ")\n";
	//std::cout << "Final max: (" << max.x << ", " << max.y << ", " << max.z << ")\n";

	DXMath::Vector3 center;
	center.x = (min.x + max.x) / 2.0f;
	center.y = (min.y + max.y) / 2.0f;
	center.z = (min.z + max.z) / 2.0f;

	_modelData.get()->center = center;
	// 바운딩 박스 Extent 계산
	DXMath::Vector3 extent;
	extent.x = (max.x - min.x) / 2.0f;
	extent.y = (max.y - min.y) / 2.0f;
	extent.z = (max.z - min.z) / 2.0f;
	_modelData.get()->extent = extent;
	return extent;
}
