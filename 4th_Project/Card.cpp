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
#include "../Engine/TimeSystem.h"
#include "UIButton.h"
#include "../Engine/SoundSystem.h"
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
	//CreateComponent<ModelComponent>("STAGE1/FBX/Card/Clover_Ace.fbx");  // char2 / gun // asdq
	CreateComponent<ModelComponent>("GambleScene/FBX/" + GetName() + ".fbx");
	CreateComponent<RenderComponent>();
	CreateComponent<BoxCollider>();

	//SetEffect(Object::Effect::OutLine);
	DXMath::Vector3 extent = GetComponent<ModelComponent>()->GetModel().get()->extent;
	DXMath::Vector3 center = GetComponent<ModelComponent>()->GetModel().get()->center;
	GetComponent<BoxCollider>()->SetBox(center, extent, GetComponent<TransformComponent>()->GetQuaternion());
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/StaticVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
	float eulerAngle = DirectX::XMConvertToRadians(rotat);
	DXMath::Quaternion eulerToQuaternion = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);
	GetComponent<TransformComponent>()->SetQuaternion(eulerToQuaternion);
}

void Card::Init(DXMath::Vector3 _pos)
{

	if (isOpen)
	{
		float eulerAngle = DirectX::XMConvertToRadians(rotat + 180);
		DXMath::Quaternion eulerToQuaternion = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);
		GetComponent<TransformComponent>()->SetQuaternion(eulerToQuaternion);
	}
	isOpen = false;
	needRevers = false;
	rotat = 180;
	prevRotat = rotat;
	AtoOne = true;
	isSeleted = false;
	elpasedTime = 0;
	GetComponent<TransformComponent>()->SetPosition(_pos);
	SetActive(true);
	SetOutlineColor({ 0.0f, 1.0f, 0.0f, 1.0f });
}

void Card::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);

	if (needRevers)
	{
		elpasedTime += _deltaTime;      //단순 오픈과 이동후 오픈 구별 가능하게끔  수정필요 *****
		if (elpasedTime >= 0.5f)		// ************************************************************** 수정하3// 
		{
			Reverse();
			needRevers = false;
			elpasedTime = 0;
		}

	}

	if (prevRotat != rotat)
	{
		float eulerAngle = DirectX::XMConvertToRadians(rotat);
		DXMath::Quaternion eulerToQuaternion = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);
		GetComponent<TransformComponent>()->SetQuaternion(eulerToQuaternion);
		prevRotat = rotat;
	}
}

void Card::Open()
{
	SOUNDSYSTEM->PlayMusic(eSoundList::SE_Card_Pick, eSoundChannel::Effect2);
	if (false == isOpen)
		Reverse();
	isOpen = true;
}

void Card::Close()
{
	if(true == isOpen)
		needRevers = true;
	isOpen = false;
}

void Card::MoveOpen()
{
	SOUNDSYSTEM->PlayMusic(eSoundList::SE_Card_Gather, eSoundChannel::Effect2);
	if (false == isOpen)
		needRevers = true;
	isOpen = true;

}

void Card::Reverse()
{
	SOUNDSYSTEM->PlayMusic(eSoundList::SE_Card_Flap, eSoundChannel::Effect3);
	// 수정하3 *********************************************************************
	new DOTween(rotat, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 0.5f, rotat, rotat + 180);
}

int Card::GetValue()
{
	if (rank == "Ace")
	{
		if (AtoOne)
			return 1;
		else
			return 11;
	}
	if (rank == "Jack" || rank == "Queen" || rank == "King") return 10;

	return std::stoi(rank);
}

bool Card::RevereseSec(float _sec)
{
	Open();
	float delta = TIMESYSTEM.get()->GetFloatDeltaTime();
	reversTime += delta;
	if (reversTime >= _sec)
	{
		Close();
		reversTime = 0;
		return true;
	}
	return false;
}

void Card::OnClick()
{
	std::cout << "이 카드는 : " << GetName() << " 입니다. " << std::endl;
	if (BLACKJACK->canClick == true)
	{
		if (BLACKJACK->GetState() == PlayerState::OPEN || !PLAYER->Open2Card())
		{
			for (auto& card : PLAYER->hand.hand) //
			{

				if (card != nullptr && card->GetName() == GetName() && isOpen == false) //누른카드가 패에있고 아직 뒷면이면
				{
					Open();
					if (card->rank == "Ace") 
						OpenA();
					PLAYER->turnEnd = true;
				}
			}
		}
		if (BLACKJACK->GetState() == PlayerState::HIT && PLAYER->needDiscard == true)
		{
			for (int i = 0; i < PLAYER->hand.numCard(); i++) 
			{
				if (PLAYER->hand.hand[i] != nullptr && PLAYER->hand.hand[i]->GetName() == GetName() && isOpen == false) //누른카드가 패에있고 아직 뒷면이면
				{		
					PLAYER->hand.hand[i]->SetActive(false);
					BLACKJACK->trashDeck->cards.push_back(PLAYER->hand.hand[i]); 
					PLAYER->hand.hand[i] = nullptr;
				}
					
			}
		}

		if (BLACKJACK->GetState() == PlayerState::Skill && BLACKJACK->player->fasteye && BLACKJACK->player->selectCard == nullptr)
		{
			for (auto card : PLAYER->hand.hand) //
			{

				if (card != nullptr && card->GetName() == GetName() && isOpen == false) //누른카드가 패에있고 아직 뒷면이면
				{
					PLAYER->selectCard = this;
				}
			}
		}

		if (BLACKJACK->GetState() == PlayerState::Skill && BLACKJACK->player->useRot && BLACKJACK->player->selectCard == nullptr)
		{
			if (false == BLACKJACK->player->isRotTrash)
			{
				for (auto card : PLAYER->hand.hand) //
				{

					for (int i = 0; i < PLAYER->hand.numCard(); i++)
					{
						if (PLAYER->hand.hand[i] != nullptr && PLAYER->hand.hand[i]->GetName() == GetName()) //누른카드가 패에있고 아직 뒷면이면
						{
							PLAYER->hand.hand[i]->SetActive(false);
							BLACKJACK->trashDeck->cards.push_back(PLAYER->hand.hand[i]);
							PLAYER->hand.hand[i] = nullptr;  //일단버려 
							BLACKJACK->player->isRotTrash = true;
						}

					}
				}
			}
			else
			{
				//하나골라서 넣어

				for (auto card : BLACKJACK->player->cardrot.RotCards)
				{
					if (card != nullptr && card->GetName() == GetName())
					{
						PLAYER->selectCard = this;
				
					}
				}
				
			}
		}
	}
}

void Card::OnMouse()
{
	//std::cout << "현재 마우스가 " << GetName() << " 오브젝트 위에 있습니다" << std::endl;
	if (isOn == true)
	{
		SOUNDSYSTEM->PlayMusic(eSoundList::SE_Card_Pick, eSoundChannel::Effect2);
		isOn = false;
	}
	
	AddEffect(Object::Effect::OutLine);

}

void Card::ExitMouse()
{
	isOn = true;
	RemoveEffect(Object::Effect::OutLine);
}

void Card::OpenA()
{
	auto btn1 = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "ButtonTen");
	auto btn2 = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "ButtonOne");
	btn1->SetActive(true);
	btn2->SetActive(true);
	UIButton* btn11 = dynamic_cast<UIButton*>(btn1);
	UIButton* btn22 = dynamic_cast<UIButton*>(btn2);

	btn11->SetOnClick([this, btn1, btn2]() {this->AtoOne = false; btn1->SetActive(false); btn2->SetActive(false); });
	btn22->SetOnClick([this, btn1, btn2]() {this->AtoOne = true; btn1->SetActive(false); btn2->SetActive(false); });

	//ButtonTen->SetOnClick([ButtonTen, ButtonOne]() {ButtonTen->SetActive(false) });
	//ButtonOne->SetOnClick();
	//btn1->SetOnclick([this, btn1, btn2]() {this->AtoOne = true,
	//	btn1->SetActive(false),
	//	btn2->SetActive(false);});
	//	//SCENEMANAGER->GetCurrentScene()->EraseGameObject(Layer::Tag::Basic, btn1),
	//	//
	//	//SCENEMANAGER->GetCurrentScene()->EraseGameObject(Layer::Tag::Basic, btn2);});// *****
	//btn2->SetOnclick([this, btn1, btn2]() {this->AtoOne = false,
	//	btn1->SetActive(false),
	//	btn2->SetActive(false);});
	//		//SCENEMANAGER->GetCurrentScene()->EraseGameObject(Layer::Tag::Basic, btn1),
	//		//SCENEMANAGER->GetCurrentScene()->EraseGameObject(Layer::Tag::Basic, btn2);});// *****
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
