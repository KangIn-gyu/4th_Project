#pragma once

class Transform
{
public:
	Transform() : position(DXMath::Vector3::Zero), rotation(DXMath::Quaternion::Identity), scale(DXMath::Vector3::One) {}
	virtual ~Transform() { parent = nullptr; }
	void UpdateTransform();

	DXMath::Matrix  GetWorldMatrix();
	DXMath::Matrix  GetLocalMatrix() const;

	DXMath::Vector3	   GetPosition() const;
	DXMath::Quaternion GetQuaternion() const;
	DXMath::Vector3    GetScale() const;

	// 로컬 회전만 고려한 방향 계산
	DXMath::Vector3 GetLocalForward() const;
	DXMath::Vector3 GetLocalUp()	  const;
	DXMath::Vector3 GetLocalRight()	  const;

	// 부모 트랜스폼의 영향을 받은 계산
	DXMath::Vector3 GetWorldForward() const { return forward; }
	DXMath::Vector3 GetWorldRight() const { return right; }
	DXMath::Vector3 GetWorldUp() const { return up; }
	DXMath::Vector3 GetWorldLook() const { return look; }

	void SetLocalMatrix(const DXMath::Matrix _localMatrix);
	void SetPosition(const DXMath::Vector3 _position);
	void SetQuaternion(const DXMath::Quaternion _rotation);
	void SetScale(const DXMath::Vector3 _scale);

	void SetParent(Transform* _parent) { parent = _parent; UpdateTransform(); }
private:

public:

protected:
	DXMath::Vector3     position;   // 위치
	DXMath::Quaternion  rotation;   // 회전 (쿼터니언)
	DXMath::Vector3     scale;      // 스케일
	Transform* parent{};

	DXMath::Vector3     forward{};
	DXMath::Vector3     right{};
	DXMath::Vector3     up{};
	DXMath::Vector3     look{};

	DXMath::Matrix		localMatrix = DXMath::Matrix::Identity;
	DXMath::Matrix		worldMatrix = DXMath::Matrix::Identity;

private:

};

