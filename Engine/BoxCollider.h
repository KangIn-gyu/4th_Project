#pragma once
#include "Collider.h"

enum class Type
{
	None, // 단순 클릭검사등
	Block, //충돌시 못 넘어가게할것
};

enum class Touch
{
	None, //터치불가능
	Basic,
};
class BoxCollider : public Collider
{
public:
	BoxCollider() = default;

	//나중에 인규형한테 물어보기 따로 해야하는지 
	~BoxCollider() = default;
	//쿼터니언으로 회전값 주기 이미 다 쿼터니언이라 걍주면될듯?
	void SetBox(const DXMath::Vector3 center, const DXMath::Vector3 extents, const DXMath::Quaternion orientation);
	D2D1_RECT_F GetBoundBox();
	void SetBox(const DXMath::Vector3 center, const DXMath::Vector3 extents, const DXMath::Quaternion orientation,Type _tpye = Type::None);
	void DrawBoundBox();

	// 충돌 검사 함수 다른거랑 충돌할일이 있을까
	virtual bool CheckCollision(Collider* _other)override ;
	bool Check2D(float mousex, float mousey);
	virtual void ComponentInitialize() override;                    // 초기화용
	virtual void ComponentUpdate(const float _deltaTime) override;  // 업데이트
	// OBB 업데이트 함수
	//클릭용 레이검사할 함수
	bool IntersectsRay(const  DXMath::Vector3& rayOrigin, const  DXMath::Vector3& rayDirection, float& distance) const;

	Touch TouchType = Touch::Basic;
	Type colliderType = Type::None;
	bool isDiamond = FALSE;
protected:
private:
public:
private:
	DirectX::BoundingOrientedBox obBox; //계산때쓸 움직인지점 센터
	DXMath::Vector3 modelCenter;	   //모델고유센터
	DXMath::Vector3 modelExtent;
	DXMath::Vector3 objScale;
};

