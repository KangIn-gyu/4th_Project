#pragma once
#include "../Engine/Object.h"
#include "../Engine/SingletonBase.h"
#include "Deck.h"
#include "Hand.h"
#include "Card.h"
#include "../Engine/IColliderNotify.h"
#include "../Engine/DirectXInput.h"
#include "CardRotation.h"
#define PLAYER Player::GetInstance()

class CardRotation;
class TransformComponent;
enum class PSkill
{
	fastEye,
	//베짱
	guts, 
	meditation,
	Insurance
};
class Player : public Object, public IColliderNotify , public IinputProcesser
{
public:
	Player(std::string_view _name, Object::ObjectType _type);
	virtual void Initialize();
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	virtual void ResetInformation();
	virtual void OnBlock(Collider* _myCol, Collider* _otherCol) override;
	virtual void EnterRayCollision(Collider* _otherCol) override;
	virtual void EndRayCollision(Collider* _otherCol)override;
	static Player* GetInstance() { return g_player; }
	void Init();
	void FirstDraw(Deck* _deck);
	bool CardDraw(Deck* _deck);
	int* Bet() { return &betChip; }
	int GetScore(); //카드합 점수
	bool ActiveSkill();
	bool Open2Card(); //첨에 2개뒤집었는지 확인용
	void ShuffleHand();
	bool CheckGameOver();

	void SetSkill(PSkill _skill);
	//손은 눈보다 빠르다
	bool fastEye();
	//베짱
	bool guts();
	//명상
	bool meditation();
	//인슈어런스
	bool Insurance();
	
	void AddInputVec(const DXMath::Vector3& input);
	// skill 버튼 누르면  스킬 1,2,3,4 버튼등장 -> 플레이어 기력받아와서 기력이없으면 스킬버튼 불가능 ->
	// 스킬 1,2,3,4 버튼 클릭되면 플레이어  기력-하고 스킬func 바뀌면서 해당스킬 실행

	virtual void OnInputProcess(const DX::Keyboard::State& _KeyState,
		const DX::Keyboard::KeyboardStateTracker& _KeyTracker,
		const DX::Mouse::State& _MouseState,
		const DX::Mouse::ButtonStateTracker& _MouseTracker) override;
private:
	float love{};    //호감도 딜러랑만의 호감
	int   betChip{}; //베팅떄 걸칩갯수 마우스휠로 조절
	DXMath::Vector3 inputVector = {};
	float speed = 100.0f;
	float RotationSpeed = 0.004f;
public:
	static Player* g_player;
	bool turnEnd     = false;
	bool drawFirst   = false;
	bool isDrawOne   = false;
	bool Shuffle     = false;
	bool needDiscard = false;
	int  openCard  = 0; //d
	int  skillPoint = 10; 
	int  chip = 1000; //겜블때 쓸 칩 수,전체 돈
	Card* selectCard = nullptr;  //스킬 쓸때 손패에서 선택중인 카드
	Hand hand;
	std::function<bool()> skill;

	TransformComponent* cameraTransform = nullptr;
	bool canSkill = true;
	bool useRot = false; //회전문스킬 사용중
	bool isRotTrash = false; //회전문중 버렸다
	bool fasteye = false;
	CardRotation cardrot;

};


