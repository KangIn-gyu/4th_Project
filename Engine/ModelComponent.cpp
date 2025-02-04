#include "pch.h"
#include "Object.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

#include "ResourceSystem.h"
#include "Mesh.h"
#include "Model.h"
#include "AiNode.h"
#include "Animation.h"
#include "AnimationNode.h"
#include "Helper.h"
#include "SkeletalMesh.h"
#include "D3DClass.h"

ModelComponent::ModelComponent(std::string_view _filePath)
{
	model = RESOURCESYSTEM->Load<Model>(_filePath);
	model->SetOwner(this);
    model->GetModelData()->Show();
    rootNode = DeepCopyNode(model->GetModelData()->rootNode, nullptr); // 여기서 모델에 사용할 node 생성
    modelAnimation = model->GetModelData()->animations;
    model->GetModelData()->matrixPallete = &matrixPalletBuffer; // 모델 데이터한테 
}

ModelComponent::~ModelComponent()
{
    SafeExtinction::SAFE_CLEAR_CONTAINER(nodeList);
}

void ModelComponent::ComponentInitialize()
{	
	objectTransform = owner->GetComponent<TransformComponent>(0); // 오너의 트랜스폼을 넣는다.
    rootNode->GetPtrTransform()->SetParent(objectTransform->GetTransform());
    model->Initialize();
}

void ModelComponent::ComponentUpdate(const float _deltaTime)
{
    if (nullptr != activeAnimation)
    {
        progressAnimTime += _deltaTime;

        if (true == activeAnimation->GetLoop())
        {
            if (progressAnimTime >= activeAnimation->GetTotalTime())
            {
                progressAnimTime = 0.f;
            }
        }
        else
        {
            if (progressAnimTime >= activeAnimation->GetTotalTime())
            {
                progressAnimTime = activeAnimation->GetTotalTime(); // 애니메이션 끝에 고정
            }
        }
        activeAnimation->SetCurrTime(progressAnimTime);
    }

    rootNode->Update(_deltaTime, progressAnimTime); // 애니메이션 프로세스 시간 넣어야 됨

    auto& meshs = *model->GetModelData()->meshs;
    if (nullptr != activeAnimation)
    {
        for (int i = 0; i < meshs.size(); i++)
        {
            // 스태틱 매쉬일 경우 처리 해야 됨
            if (typeid(*meshs[i]) == typeid(SkeletalMesh))
            {
                auto skeletalMesh = static_cast<SkeletalMesh*>(meshs[i]);
                size_t boneCount = skeletalMesh->GetBoneReferencesSize();
                for (UINT j = 0; j < boneCount; j++)
                {
                    BoneReference& boneRef = skeletalMesh->GetBoneReferences()[j];
                    AiNode* node = nodeList.find(boneRef.GetName())->second;
                    Transform* parentTransform = node->GetPtrTransform();
                    boneRef.SetNodeWolrdTransform(parentTransform->GetPtrWorldMatrix());
                }
                skeletalMesh->UpdateMatrixPallete(&matrixPalletBuffer, model->GetModelData()->skeletonInfo);
            }
        }
    }
}

Transform* ModelComponent::GetTransform()
{
	return objectTransform->GetTransform();
}

void ModelComponent::SetAnimation(int _index)
{
    progressAnimTime = 0;

    if (_index < 0 || _index >= modelAnimation->size() || (*modelAnimation)[_index] == nullptr)
    {
        return;
    }

    // 모든 AiNode의 AnimationNode 포인터 초기화
    for (auto& [name, node] : nodeList)
    {
        if (nullptr != node)
        { // 이전 애니메이션 노드 초기화
            node->SetAnimationNode(nullptr);
        }
    }

    activeAnimation = (*modelAnimation)[_index];

    for (auto* animNode : activeAnimation->GetAnimationNodes()) // AnimationNode* 벡터 접근
    {
        // 애니메이션 노드 이름으로 AiNode를 찾음
        auto it = nodeList.find(animNode->GetName());
        if (it != nodeList.end() && nullptr != it->second)
        {   // 매칭된 AiNode에 AnimationNode 설정    
            it->second->SetAnimationNode(animNode);
        }
        else
        {
            // 매칭되지 않는 노드 로그 출력  TDOD : 추후 로그시스템으로 처리하자
            std::cout << "SetAnimation : 애니메이션 노드 '" << animNode->GetName()
                << "'에 해당하는 AiNode를 찾을 수 없습니다.\n";
        }
    }
}

int ModelComponent::GetActiveAnimationIndex()
{
    auto it = std::find(modelAnimation->begin(), modelAnimation->end(), activeAnimation);
    return (it != modelAnimation->end()) ? std::distance(modelAnimation->begin(), it) : -1;
}

void ModelComponent::StopAnimation()
{
    activeAnimation = nullptr;
}

AiNode* ModelComponent::DeepCopyNode(AiNode* _originalNode, AiNode* _parentNode)
{
    if (!_originalNode)
        return nullptr;

    AiNode* copiedNode = new AiNode(*_originalNode);
    copiedNode->SetParent(_parentNode);

    nodeList.emplace(copiedNode->GetName(), copiedNode);
    copiedNode->GetChildren().clear();
    for (AiNode* child : _originalNode->GetChildren())
    {
        AiNode* copiedChild = DeepCopyNode(child, copiedNode);
        if (copiedChild)
        {
            copiedNode->AddChild(copiedChild);
        }
    }

    return copiedNode;
}

                                 
