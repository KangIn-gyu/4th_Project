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
#include "../Engine/SceneManager.h"
#include "../Engine/Scene.h"
#include "../Engine/FactorySystem.h"
#include "Button.h"
Card::Card(std::string_view _name, Object::ObjectType _type,Suit _suit, std::string _rank) : Object(_name, _type)
{
	suit = _suit;
	rank = _rank;
}

Card::~Card()
{
}

void Card::Initialize()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/gun.fbx");  // char2 / gun // asdq
	//CreateComponent<ModelComponent>("STAGE1/FBX/" + GetName() + ".fbx");
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

	if (prevRotat != rotat)
	{
		auto quater = GetComponent<TransformComponent>()->GetQuaternion();
		float eulerAngle = DirectX::XMConvertToRadians(rotat);
		DXMath::Quaternion eulerToQuaternion = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);
		newQuat = quater * eulerToQuaternion;
		GetComponent<TransformComponent>()->SetQuaternion(eulerToQuaternion);
		prevRotat = rotat;
	}

	
}

void Card::Open()
{
	
	if (isOpen == false)
	{
		isOpen = true;
		new DOTween(rotat, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, rotat, rotat + 180);
	}
	
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
	std::cout << "이 카드는 : " << GetName() << " 입니다. " << std::endl;
	if (BLACKJACK->endBet == true && BLACKJACK->canClick == true)
	{
		if (BLACKJACK->GetState() == PlayerState::OPEN || !PLAYER->Open2Card())
		{
			for (auto& card : PLAYER->hand.hand) //
			{

				if (card != nullptr && card->GetName() == GetName() && isOpen == false) //누른카드가 패에있고 아직 뒷면이면
				{
					Open();
					if (card->rank == "A")
						OpenA();
					PLAYER->turnEnd = true;
				}
			}
		}
		if (BLACKJACK->GetState() == PlayerState::HIT && PLAYER->needDiscard == true)
		{
			for (auto& card : PLAYER->hand.hand) //
			{
				if (card != nullptr && card->GetName() == GetName() && isOpen == false) //누른카드가 패에있고 아직 뒷면이면
				{
					card->GetComponent<TransformComponent>()->SetPosition({ -700,0,0 }); //풀을만든들 없애든하기 일단 위치만변경
					BLACKJACK->trashDeck->cards.push_back(card);
					for (int i = 0; i < PLAYER->hand.numCard(); i++)
					{
						if (PLAYER->hand.hand[i] == card)
							PLAYER->hand.hand[i] = nullptr;
					}
					//card = nullptr;
				}
			}
		}
	}
}

void Card::OnMouse()
{
	//std::cout << "현재 마우스가 " << GetName() << " 오브젝트 위에 있습니다" << std::endl;
}

void Card::OpenA()
{
	auto btn1 = SCENEMANAGER->GetCurrentScene()->ObjectCreator<Button>("Ato1", Object::ObjectType::Basic, DXMath::Vector3(0, -50, 0), []() {;});
	auto btn2 = SCENEMANAGER->GetCurrentScene()->ObjectCreator<Button>("Ato2", Object::ObjectType::Basic, DXMath::Vector3(100, -50, 0), []() {;});
	btn1->SetOnclick([this, btn1, btn2]() {this->AtoOne = true,
		btn1->GetComponent<TransformComponent>()->SetPosition({ -1000,0,0 }),
		btn2->GetComponent<TransformComponent>()->SetPosition({ -1000,0,0 });});
		//SCENEMANAGER->GetCurrentScene()->EraseGameObject(Layer::Tag::Basic, btn1),
		//
		//SCENEMANAGER->GetCurrentScene()->EraseGameObject(Layer::Tag::Basic, btn2);});// *****
	btn2->SetOnclick([this, btn1, btn2]() {this->AtoOne = false,
		btn1->GetComponent<TransformComponent>()->SetPosition({ -1000,0,0 }),
		btn2->GetComponent<TransformComponent>()->SetPosition({ -1000,0,0 });});
			//SCENEMANAGER->GetCurrentScene()->EraseGameObject(Layer::Tag::Basic, btn1),
			//SCENEMANAGER->GetCurrentScene()->EraseGameObject(Layer::Tag::Basic, btn2);});// *****
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
