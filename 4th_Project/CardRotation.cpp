#include "pch.h"
#include "CardRotation.h"
#include "../Engine/TransformComponent.h"
#include "Card.h"
#include "Deck.h"
#include "BlackJack.h"

CardRotation::CardRotation()
{
}

void CardRotation::Init(Deck* deckPtr)
{
	if (!deckPtr) return;
	deck = deckPtr;


	const float radius = 30.0f;
	const float angleStep = 360.0f / ACTIVE_SLOTS;
	const float startAngle = 90.0f;	
	const float xOffSet = 60.0f;

	RotCards.resize(MAX_SLOTS);
	positions.resize(MAX_SLOTS);
	for (int i = 0; i < ACTIVE_SLOTS; i++) {
		float angle = DirectX::XMConvertToRadians(startAngle + (i * angleStep));
		positions[i] = {
			radius * cos(angle) + xOffSet,
			30.0f,
			radius * sin(angle)
		};
	}

	positions[MAX_SLOTS - 1] = positions[0];

	slots.resize(MAX_SLOTS);
	slots[MAX_SLOTS - 1].isActive = false;

	auto deckTransform = deck->GetComponent<TransformComponent>();
	if (deckTransform) {
		deckPosition = deckTransform->GetPosition();
	}
	else {
		return;
	}

	int numInitialCards = (std::min)(static_cast<int>(deck->cards.size()), ACTIVE_SLOTS);
	for (int i = 0; i < numInitialCards; i++) {
		slots[i].card = deck->cards[i];
		slots[i].isActive = true;
	}

	slots[MAX_SLOTS - 1].isActive = false;
	currentCardIndex = numInitialCards - 1;

	float rotatX = 90.0f;
	float eulerAngleX = DirectX::XMConvertToRadians(rotatX);
	DXMath::Quaternion xRotation = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngleX, 0.f);

	for (int i = 0; i < ACTIVE_SLOTS; i++) {
		if (slots[i].isActive && slots[i].card) {
			auto transform = slots[i].card->GetComponent<TransformComponent>();
			if (transform) {
				auto currentRotation = transform->GetQuaternion();
				transform->SetQuaternion(xRotation);
			}
		}
	}
}

void CardRotation::Update(float deltaTime, std::vector<Card*>& cards)
{
	for (int i = 0; i < slots.size(); i++)
	{
		RotCards[i] = slots[i].card;
	}

	timer += deltaTime;

	if (timer >= ROTATION_INTERVAL) {
		timer = 0.0f;
		RotateCards(cards);
	}
	else {
		float t = timer / ROTATION_INTERVAL;
		UpdateCardPositions(t);
	}

}

void CardRotation::RotateCards(std::vector<Card*>& cards)
{
	currentIndex = (currentIndex + 1) % MAX_SLOTS;
	completedRotations++;

	currentCardIndex++;
	int nextCardIndex = currentCardIndex % cards.size();
	Card* newCard = cards[nextCardIndex];


	for (int i = 0; i < MAX_SLOTS; i++) {
		if (slots[i].isActive) {
			int relativePosition = (currentIndex + i) % MAX_SLOTS;

			if (relativePosition == MAX_SLOTS - 1) {

				slots[i].isActive = false;
				if (slots[i].card) {
					float rotat = 90.0f;
					float eulerAngle = DirectX::XMConvertToRadians(rotat);
					auto transform = slots[i].card->GetComponent<TransformComponent>();
					if (transform) {
						auto deckPos = BLACKJACK->deck->GetComponent<TransformComponent>()->GetPosition();
						transform->SetPosition(deckPos);

						DXMath::Quaternion xRotation = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);
						auto currentRotation = transform->GetQuaternion();
						auto afterXRotation = currentRotation * xRotation;
						transform->SetQuaternion(afterXRotation);
					}
				}

				int newSlotIndex = (i + 1) % MAX_SLOTS;
				slots[newSlotIndex].card = newCard;
				slots[newSlotIndex].isActive = true;

				float rotat = 90.0f;
				float eulerAngle = DirectX::XMConvertToRadians(rotat);
				DXMath::Quaternion rotation = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);

				auto newTransform = newCard->GetComponent<TransformComponent>();
				if (newTransform) {
					newTransform->SetQuaternion(rotation);
					// �� ī�带 positions[0] ��ġ�� ��ġ
					DXMath::Vector3 firstPosition = deckPosition + positions[0];
					newTransform->SetPosition(firstPosition);
				}
			}

		}

		//canCard = positions[0];
	}

	if (completedRotations == ACTIVE_SLOTS) {
		completedRotations = 0;
	}
}

void CardRotation::UpdateCardPositions(float t)
{
	if (!deck) return;

	auto deckTransform = deck->GetComponent<TransformComponent>();
	if (deckTransform) {
		deckPosition = deckTransform->GetPosition();
	}

	for (int i = 0; i < MAX_SLOTS; i++) {
		if (!slots[i].card || !slots[i].isActive) continue;

		auto transform = slots[i].card->GetComponent<TransformComponent>();
		if (!transform) continue;

		int currentPos = (currentIndex + i) % MAX_SLOTS;
		int nextPos = (currentPos + 1) % MAX_SLOTS;

		DXMath::Vector3 currentPosition = deckPosition + positions[currentPos];
		DXMath::Vector3 targetPosition;

		if (nextPos == 0) {
			auto deckPos = BLACKJACK->deck->GetComponent<TransformComponent>()->GetPosition();
			targetPosition = deckPos;
		}
		else {
			targetPosition = deckPosition + positions[nextPos];
		}

		DXMath::Vector3 interpolatedPosition;
		DXMath::Vector3::Lerp(
			currentPosition,
			targetPosition,
			t,
			interpolatedPosition
		);
		transform->SetPosition(interpolatedPosition);

		const float xOffSet = 60.0f;

		DXMath::Vector3 modifiedCenter = deckPosition;
		modifiedCenter.x += xOffSet;


		DXMath::Vector3 toCenter = modifiedCenter - interpolatedPosition;
		toCenter.y = 0; 
		toCenter.Normalize();


		float x = 90.0f;
		float y = 180.f;
		float pitch = DirectX::XMConvertToRadians(x);
		float roll = DirectX::XMConvertToRadians(y);

		float yaw = atan2(toCenter.x, toCenter.z);

		DXMath::Quaternion rotation = DXMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
		transform->SetQuaternion(rotation);
	}
}