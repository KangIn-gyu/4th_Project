#pragma once
#include "../Engine/Object.h"
#include "../Engine/IClick.h"
#include "../Engine/IOnmouse.h"
enum class Suit
{
	Spade, 
	Diamond,
	Heart, 
	Clover
};

std::string enumToString(Suit _suit);

class Card : public Object, public IClick, public IOnmouse
{
public:
	Card(std::string_view _name, Object::ObjectType _type, Suit _suit = Suit::Spade, std::string _rank = "Ace");
	virtual ~Card() override;
	virtual void Initialize() override;
	virtual void Update(const float _deltaTime) override;
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	void Init(DXMath::Vector3 _pos);
	void Open(); //뒤집어져있다가 오픈할떄 쓸꺼
	void Close(); //오픈되있으면 뒤집을떄 쓸꺼
	void MoveOpen(); 
	void Reverse(); //뒤집음 호출
	int GetValue(); //숫자 계산할때슬 카드 값리턴
	bool RevereseSec(float _sec); 
	virtual void OnClick() override;
	virtual void OnMouse() override;
	virtual void ExitMouse() override;
	void OpenA();
	Suit suit;
	std::string rank; //카드 숫자 1~10 J Q K A // 0
	bool isOpen = false; //카드가 오픈상태인지
	bool needRevers = false;   //뒤집을 필요가있는지
	float elpasedTime = 0;
private:
	float rotat = 0;
	float prevRotat = rotat;
	//DXMath::Quaternion newQuat;
	bool isSeleted = false;  //선택된 카드 표시용 
	bool AtoOne = true; //Ace를 1로쓸지 11로쓸지 true면 1로
	float reversTime = 0;
};

