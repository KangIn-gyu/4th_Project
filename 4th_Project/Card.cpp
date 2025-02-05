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
	//CreateComponent<ModelComponent>("STAGE1/FBX/Card/Clover_Ace.fbx");  // char2 / gun // asdq
	CreateComponent<ModelComponent>("STAGE1/FBX/Card/" + GetName() + ".fbx");
	CreateComponent<RenderComponent>();
	CreateComponent<BoxCollider>();

	DXMath::Vector3 extent = GetComponent<ModelComponent>()->GetModel().get()->extent;
	DXMath::Vector3 center = GetComponent<ModelComponent>()->GetModel().get()->center;
	GetComponent<BoxCollider>()->SetBox(center, extent, GetComponent<TransformComponent>()->GetQuaternion());
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void Card::Init(DXMath::Vector3 _pos)
{
	if (isOpen)
	{
		float eulerAngle = DirectX::XMConvertToRadians(rotat +180);
		DXMath::Quaternion eulerToQuaternion = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);
		GetComponent<TransformComponent>()->SetQuaternion(eulerToQuaternion);
	}
	isOpen = false;
	needRevers = false;
	rotat = 0;
	prevRotat = rotat;
	AtoOne = true;
	isSeleted = false;
	elpasedTime = 0;
	GetComponent<TransformComponent>()->SetPosition(_pos);
	//GetComponent<TransformComponent>()->SetQuaternion(_quater); //회전값
}

void Card::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);

	if (needRevers)
	{
		isOpen = true;
		elpasedTime += _deltaTime;   //단순 오픈과 이동후 오픈 구별 가능하게끔  수정필요 *****
		if (elpasedTime >= 1.0f) 
		{
			Reverse();
			needRevers = false;
			elpasedTime = 0;
		}

	}


	if (prevRotat != rotat)
	{
		//auto quater = GetComponent<TransformComponent>()->GetQuaternion();
		float eulerAngle = DirectX::XMConvertToRadians(rotat);
		DXMath::Quaternion eulerToQuaternion = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);
		//newQuat = quater * eulerToQuaternion;
		GetComponent<TransformComponent>()->SetQuaternion(eulerToQuaternion);
		prevRotat = rotat;
	}




}


void Card::Open()
{
	needRevers = true;
	/*if (isOpen == false)
	{
		isOpen = true;
		new DOTween(rotat, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, rotat, rotat + 180);
	}*/
	
}

void Card::Close()
{
	if(true == isOpen)
		needRevers = true;
}

void Card::Reverse()
{
	new DOTween(rotat, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, rotat, rotat + 180);
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
					BLACKJACK->trashDeck->cards.push_back(PLAYER->hand.hand[i]); //카메라 문젠지 자꾸 잘못된게 지워지는듯? 잘모르겠음
					PLAYER->hand.hand[i] = nullptr;
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
		btn1->SetActive(false),
		btn2->SetActive(false);});
		//SCENEMANAGER->GetCurrentScene()->EraseGameObject(Layer::Tag::Basic, btn1),
		//
		//SCENEMANAGER->GetCurrentScene()->EraseGameObject(Layer::Tag::Basic, btn2);});// *****
	btn2->SetOnclick([this, btn1, btn2]() {this->AtoOne = false,
		btn1->SetActive(false),
		btn2->SetActive(false);});
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
