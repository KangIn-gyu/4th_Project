#pragma once
#include "Component.h"
#include "Transform.h"

class TransformComponent : public Component
{
public:
	TransformComponent() = default;
	virtual ~TransformComponent() {}

	virtual void ComponentInitialize() override {}
	// ComponentUpdate에 업데이트 메트릭스 계산 안넣은 이유 가만히 있을때 계산안하기 위해서
	virtual void ComponentUpdate(const float _deltaTime) override {}
	virtual void ComponentRender() override {}

	DXMath::Matrix  GetWorldMatrix() const;
	DXMath::Matrix  GetLocalMatrix() const;

	// 로컬 회전만 고려한 방향 계산
	DXMath::Vector3 GetLocalForward() const;
	DXMath::Vector3 GetLocalUp()	  const;
	DXMath::Vector3 GetLocalRight()	  const;

	// 부모 트랜스폼의 영향을 받은 계산
	DXMath::Vector3 GetWorldForward() const;
	DXMath::Vector3 GetWorldRight() const;
	DXMath::Vector3 GetWorldUp() const;
	DXMath::Vector3 GetWorldLook() const;

	void SetLocalMatrix(const DXMath::Matrix _localMatrix);
	void SetPosition(const DXMath::Vector3 _position);
	void SetQuaternion(const DXMath::Quaternion _rotation);
	void SetScale(const DXMath::Vector3 _scale);

	void SetParent(TransformComponent _parent);
private:

public:

private:
	Transform transform;
};

// 고민의 흔적 Transform을 상속하는거랑, Transform을 has_a로 했을때
// 각각의 장단점을 보았다.
// 성능을 고려한 선택은 상속을 하면 코드 중복 감소와 성능적으로 가상 함수 테이블을 거쳐서 호출 되므로 비용이 1번만 된다
// 단점으로는 확장성이 제한된다. 다중 상속 문제
// has_a로 했을 경우는
// 유연성과 다중상속 문제 방지
// 단점으로는 함수를 2번 호출한다.
// 2개의 코스트를 비교하면 매우 조그만 차이긴하다.. 그렇기 때문에 디버깅할때 가독성 좋은걸로 바꾸면서 테스트를 해봐야겠다