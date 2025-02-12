#pragma once
#include "../Engine/Object.h"
class JustBox : public Object
{
public:
	JustBox(std::string_view _name, ObjectType _type, DXMath::Vector3 _pos, DXMath::Vector3 _extent, DXMath::Vector3 _center);
	virtual ~JustBox() = default;
	virtual void Initialize() override;                   // 용도 : 내가 필요한 컴포넌트 생성하는 곳 초기화나
	virtual void Update(const float _deltaTime) override; // 용도 : 오브젝트 개인의 업데이트가 필요할때 정의
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	virtual void ResetInformation() {}
private:

public:

private:

};

