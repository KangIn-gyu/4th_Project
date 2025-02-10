#pragma once
#include "../Engine/Script.h"
class D2DRenderComponent;

class FadeEffectScript : public Script
{
public:
    FadeEffectScript(Object* _own) : Script(_own) {}
    virtual ~FadeEffectScript() = default;

    virtual void ComponentSetting() override;               // 컴포넌트가 다 생성된 이후 수치 조정하는것
    void SetFadeSpeed(float speed);
    void StartFadeIn();
    void StartFadeOut();
    virtual void Update(const float _deltaTime) override;  // 업데이트
    virtual void ResetInformation()override {}
private:
    D2DRenderComponent* ownerD2D;

    float alpha=0;       // 현재 알파 값 (0.0 ~ 1.0)
    float fadeSpeed=0.1;   // 페이드 속도
    bool active = TRUE;
    bool fadingIn = FALSE;
};

