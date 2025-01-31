#include "pch.h"
#include "Card.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/TransformComponent.h"
#include "Player.h"
#include "../Engine/EventSystem.h"
#include "BlackJack.h"
#include "../Engine/Model.h"
#include "../Engine/DOTween.h"

Card::Card(std::string_view _name, Object::ObjectType _type,Suit _suit, std::string _rank) : Object(_name, _type)
{
	suit = _suit;
	rank = _rank;
}

Card::~Card()
{
}

void Card::Start()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/gun.fbx");  // char2 / gun // asdq
	CreateComponent<RenderComponent>();
	CreateComponent<BoxCollider>();

	DXMath::Vector3 extent = GetComponent<ModelComponent>()->GetModel().get()->extent;
	DXMath::Vector3 center = GetComponent<ModelComponent>()->GetModel().get()->center;
	GetComponent<BoxCollider>()->SetBox(center, extent, GetComponent<TransformComponent>()->GetQuaternion());
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void Card::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);
	
	
	auto quater = GetComponent<TransformComponent>()->GetQuaternion();
	
	float eulerAngle = DirectX::XMConvertToRadians(rotat);
	DXMath::Quaternion eulerToQuaternion = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);
		//newQuat = quater * eulerToQuaternion;
	GetComponent<TransformComponent>()->SetQuaternion(eulerToQuaternion);
	
}

void Card::Open()
{
	
	if (isOpen == false)
	{
		if (this->rank == "A")
		{
			//화면에 팝업2개출력해서 1 or 11선택하게끔
			AtoOne = true;
		}
		isOpen = true;
		new DOTween(rotat, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, rotat, rotat + 180);
	}
	//카드 뒤집히는 연출 필요 A일경우 1,11정하는 코드필요 *****
}

int Card::GetValue()
{
	if (rank == "A")
	{
		if (AtoOne)
			return 1;
		else
			return 11;
	}
	if (rank == "J" || rank == "Q" || rank == "K") return 10;

	return std::stoi(rank);
}

void Card::OnClick()
{
	std::cout << "이 카드는 : " << name << " 입니다. " << std::endl;
	if (BLACKJACK->GetState() == PlayerState::OPEN && BLACKJACK->canSelect == true)
	{
		for (auto& card : PLAYER->hand.hand) //
		{
			if (card == this && isOpen == false) //누른카드가 패에있고 아직 뒷면이면
			{
				//PLAYER->selectCard.push_back(this);
				//isSeleted = true;
				Open();
				//PLAYER->turnEnd = false; 오픈했으면 딜러턴 깎기 베팅은오픈 전
				PLAYER->openCard++;
			}
		}
	}
	else if (BLACKJACK->GetState() == PlayerState::HIT && BLACKJACK->canSelect == true)
	{
		for (auto& card : PLAYER->hand.hand) //
		{
			if (card == this && isOpen == false) //누른카드가 패에있고 아직 뒷면이면 버리기가능
			{
				
				//카드제거 추가 *****
				//PLAYER->turnEnd = false;
				
			}
		}
	}
	
}

void Card::OnMouse()
{
	//std::cout << "현재 마우스가 " << name << " 오브젝트 위에 있습니다" << std::endl;
}

std::string enumToString(Suit _suit)
{
	switch (_suit)
	{
	case Suit::Spade:
		return "Spade";
	case Suit::Diamond:
		return "Diamond";
	case Suit::Heart:
		return "Heart";
	case Suit::Clover:
		return "Clover";
	default:
		return  "Unknown";
	}
}
