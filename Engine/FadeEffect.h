#pragma once
class FadeEffect
{
private:
    float alpha;       // 현재 알파 값 (0.0 ~ 1.0)
    float fadeSpeed;   // 페이드 속도
    bool active;       // 활성화 여부

public:
    // 생성자: 페이드 속도 설정 (기본값 1.0)
    FadeEffect(float speed = 1.0f) : alpha(1.0f), fadeSpeed(speed), active(false) {}

    // 업데이트 (deltaTime 사용)
    void Update(float deltaTime) {
        if (!active) return;

    	alpha -= deltaTime * fadeSpeed;
    	if (alpha <= 0.0f) { alpha = 0.0f;}
        
    	alpha += deltaTime * fadeSpeed;
    	if (alpha >= 1.0f) { alpha = 1.0f; active = false; }
    }

    // 현재 알파 값 가져오기
    float GetAlpha() const { return alpha; }

    // 페이드 효과가 진행 중인지 확인
    bool IsActive() const { return active; }
};

