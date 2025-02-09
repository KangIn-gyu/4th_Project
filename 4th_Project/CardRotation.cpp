#include "pch.h"
#include "CardRotation.h"
#include "../Engine/TransformComponent.h"
#include "Card.h"
#include "Deck.h"
#include "BlackJack.h"

CardRotation::CardRotation()
{
	// 원형 배치를 위한 설정
	const float radius = 30.0f;
	const float angleStep = 360.0f / ACTIVE_SLOTS;  // 10개의 위치에 대한 각도
	const float startAngle = 90.0f;		// 시작 각도 기준 오른쪽

	// positions 초기화 (마지막 위치는 첫 번째 위치와 동일하게)
	positions.resize(MAX_SLOTS);
	for (int i = 0; i < ACTIVE_SLOTS; i++) {
		float angle = DirectX::XMConvertToRadians(startAngle + (i * angleStep));
		positions[i] = {
			radius * cos(angle),
			90.0f,			// 높이
			radius * sin(angle)
		};
	}
	// MAX_SLOTS - 1 (마지막) 위치를 첫 번째 위치와 동일하게 설정
	positions[MAX_SLOTS - 1] = positions[0];

	slots.resize(MAX_SLOTS);
	slots[MAX_SLOTS - 1].isActive = false;  // 마지막 슬롯은 비활성화 상태로 시작
}


void CardRotation::Init(Deck* deckPtr)
{
	if (!deckPtr) return;
	deck = deckPtr;

	auto deckTransform = deck->GetComponent<TransformComponent>();
	if (deckTransform) {
		deckPosition = deckTransform->GetPosition();
	}
	else {
		return;
	}

	// 초기 활성 슬롯만 카드 설정 (ACTIVE_SLOTS 개수만큼)
	int numInitialCards = (std::min)(static_cast<int>(deck->cards.size()), ACTIVE_SLOTS);
	for (int i = 0; i < numInitialCards; i++) {
		slots[i].card = deck->cards[i];
		slots[i].isActive = true;
	}

	// 마지막 슬롯은 명시적으로 비활성화
	slots[MAX_SLOTS - 1].isActive = false;
	currentCardIndex = numInitialCards - 1;

	// 초기 회전 설정
	float rotatX = 90.0f;
	float eulerAngleX = DirectX::XMConvertToRadians(rotatX);
	DXMath::Quaternion xRotation = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngleX, 0.f);

	// 활성 슬롯의 카드만 회전 적용
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

	// 새로운 카드를 미리 준비
	currentCardIndex++;
	int nextCardIndex = currentCardIndex % cards.size();
	Card* newCard = cards[nextCardIndex];

	// 모든 슬롯을 순회하면서 업데이트
	for (int i = 0; i < MAX_SLOTS; i++) {
		if (slots[i].isActive) {
			int relativePosition = (currentIndex + i) % MAX_SLOTS;

			if (relativePosition == MAX_SLOTS - 1) {
				// 현재 카드를 비활성화하고 덱으로 이동
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

				// 즉시 새로운 카드를 첫 번째 위치(positions[0])에 배치
				int newSlotIndex = (i + 1) % MAX_SLOTS;
				slots[newSlotIndex].card = newCard;
				slots[newSlotIndex].isActive = true;

				// 새 카드의 초기 회전과 위치 설정
				float rotat = 90.0f;
				float eulerAngle = DirectX::XMConvertToRadians(rotat);
				DXMath::Quaternion rotation = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);

				auto newTransform = newCard->GetComponent<TransformComponent>();
				if (newTransform) {
					newTransform->SetQuaternion(rotation);
					// 새 카드를 positions[0] 위치에 배치
					DXMath::Vector3 firstPosition = deckPosition + positions[0];
					newTransform->SetPosition(firstPosition);
				}
			}
		}
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

        // positions[0]에 도달하는 카드는 덱 위치로 이동
        if (nextPos == 0) {
            auto deckPos = BLACKJACK->deck->GetComponent<TransformComponent>()->GetPosition();
            targetPosition = deckPos;
        } else {
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

		// 카드가 중심을 향하도록 회전 계산
		DXMath::Vector3 toCenter = deckPosition - interpolatedPosition;
		toCenter.y = 0; // y축 회전만 고려
		toCenter.Normalize();

		// 카드의 기본 90도 x축 회전 유지
		float pitch = DirectX::XMConvertToRadians(90.0f);
		float roll = DirectX::XMConvertToRadians(180.0f);
		// 중심을 향하는 y축 회전 계산
		float yaw = atan2(toCenter.x, toCenter.z);

		// 최종 회전 쿼터니언 생성
		DXMath::Quaternion rotation = DXMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
		transform->SetQuaternion(rotation);
    }
}