#pragma once
#include"../Engine/Object.h"

class D3DAniObj : public Object
{
public:
	D3DAniObj(std::string_view _name, ObjectType _type, std::string _filePath) : filePath(_filePath), Object(_name, _type) {}
	virtual ~D3DAniObj() = default;

	virtual void Initialize() override;                   // 용도 : 내가 필요한 컴포넌트 생성하는 곳 초기화나
	virtual void Update(const float _deltaTime) override; // 용도 : 오브젝트 개인의 업데이트가 필요할때 정의
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	virtual void ResetInformation() {}

private:

public:
	std::string filePath;

private:
};

