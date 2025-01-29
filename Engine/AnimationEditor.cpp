#include "pch.h"
#include "AnimationEditor.h"
#include "Object.h"
#include "ModelComponent.h"
#include "Animation.h"

AnimationEditor::AnimationEditor()
{
	SetName("Animation");
	state = State::Active;
    loopText.reserve(11);
}

void AnimationEditor::Initialize()
{

}

void AnimationEditor::Update()
{

}

void AnimationEditor::Draw()
{
    if (state == State::Active && nullptr != selectedObject)
    {
        auto* modelComponent = selectedObject->GetComponent<ModelComponent>();
        if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto animations = *modelComponent->GetAnimations(); // 애니메이션 벡터 포인터 가져오기
            static int activeIndex = -1; // 현재 활성 애니메이션 인덱스
            static int selectedIndex = -1; // 콤보박스에서 선택된 인덱스

            // 현재 활성화된 애니메이션 인덱스를 가져옴 (초기 동기화)
            if (activeIndex == -1)
            {
                activeIndex = modelComponent->GetActiveAnimationIndex();
                selectedIndex = activeIndex; // 콤보박스 초기화
            }

            std::vector<std::string> animationNames;
            for (const auto& animation : animations)
            {
                animationNames.push_back(animation->GetName()); // std::string을 직접 추가
            }

            // 애니메이션 이름 리스트 생성
            std::vector<const char*> animationNamePtrs;
            for (const auto& name : animationNames)
            {
                animationNamePtrs.push_back(name.c_str()); // const char*로 변환하여 사용
            }

            // 콤보박스 UI 생성
            if (ImGui::Combo("Select Animation", &selectedIndex, animationNamePtrs.data(), static_cast<int>(animationNamePtrs.size())))
            {
                // 선택된 애니메이션이 활성 애니메이션과 다르면 새로 설정
                if (selectedIndex != activeIndex)
                {
                    activeIndex = selectedIndex;
                    modelComponent->SetAnimation(activeIndex);
                }
            }

            // 정지 버튼 추가
            if (ImGui::Button("Stop Animation"))
            {
                modelComponent->StopAnimation();
                activeIndex = -1;
                selectedIndex = -1;
            }

            // 활성 애니메이션 정보 표시
            if (activeIndex != -1)
            {
                auto& activeAnimation = animations[activeIndex];
                ImGui::Separator();
                ImGui::Text("Active Animation: %s", activeAnimation->GetName().c_str());
                ImGui::Value("Duration : ", activeAnimation->GetDuration());
                ImGui::Value("TickPerSecond : ", activeAnimation->GetTickPerSecond());
                ImGui::Value("TotalTime : ", activeAnimation->GetTotalTime());
                ImGui::Value("CurrTime : ", activeAnimation->GetCurrTime());
                std::string is_loop = activeAnimation->GetLoop() ? "TRUE" : "FALSE";
                loopText.assign("Loop : " + is_loop);
                ImGui::Text(loopText.c_str());

                static bool loop;  // 현재 루프 상태 가져오기
                if (ImGui::Checkbox("Loop", &loop))
                {
                    activeAnimation->SetLoop(loop);  // 루프 상태 업데이트
                }
            }
        }
    }
}

void AnimationEditor::OnEnable()
{
    state = State::Active;
}

void AnimationEditor::OnDisable()
{
    state = State::Disabled;
}

void AnimationEditor::SetSelectedObject(Object* _obj)
{
	if (nullptr != _obj)
	{
		if (nullptr != _obj->GetComponent<ModelComponent>()->GetAnimations())
		{
			selectedObject = _obj;
            OnEnable();
            return;
		}
        OnDisable();
	}
}
