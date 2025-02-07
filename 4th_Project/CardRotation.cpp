#include "pch.h"
#include "CardRotation.h"
#include "../Engine/TransformComponent.h"
#include "Card.h"
#include "Deck.h"
#include "BlackJack.h"
CardRotation::CardRotation()
{
	// 원형 배치를 위한 설정
	const float radius = 30.0f;        // 원의 반지름
	const float angleStep = 360.0f / ACTIVE_SLOTS;  // 각 위치 사이의 각도 (36도)

	// positions 초기화
	positions.resize(MAX_SLOTS);
	for (int i = 0; i < MAX_SLOTS; i++) {
		float angle = DirectX::XMConvertToRadians(i * angleStep);
		positions[i] = {
			radius * cos(angle),
			0.0f,
			radius * sin(angle)
		};
	}

	slots.resize(MAX_SLOTS);

	slots[MAX_SLOTS - 1].isActive = false;
}

void CardRotation::Init(Deck* deckPtr)
{
	if (!deckPtr) return;  // 덱 포인터 유효성 검사
	deck = deckPtr;

	// 덱의 위치 저장
	auto deckTransform = deck->GetComponent<TransformComponent>();
	if (deckTransform) {
		deckPosition = deckTransform->GetPosition();
		// 디버깅용 로그 출력
		//std::cout <<(("Deck Position: " +
		//	std::to_string(deckPosition.x) + ", " +
		//	std::to_string(deckPosition.y) + ", " +
		//	std::to_string(deckPosition.z) + "\n").c_str());
	}
	else {
		//std::cout << ("Failed to get deck transform\n");
		return;
	}

	// 초기 슬롯 설정
	int numInitialCards = (std::min)(static_cast<int>(deck->cards.size()), ACTIVE_SLOTS);
	for (int i = 0; i < numInitialCards; i++) {
		slots[i].card = deck->cards[i];
		slots[i].isActive = true;
	}
	currentCardIndex = numInitialCards - 1;

	float rotat = 90.0f;
	float eulerAngle = DirectX::XMConvertToRadians(rotat);
	DXMath::Quaternion initialRotation = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);

	for (int i = 0; i < ACTIVE_SLOTS; i++) {
		if (slots[i].isActive && slots[i].card) {
			auto transform = slots[i].card->GetComponent<TransformComponent>();
			if (transform) {
				transform->SetQuaternion(initialRotation);
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

	// 모든 슬롯을 순회하면서 업데이트
	for (int i = 0; i < MAX_SLOTS; i++) {
		if (slots[i].isActive) {
			// 현재 슬롯의 상대적 위치 계산
			int relativePosition = (currentIndex + i) % MAX_SLOTS;

			// 카드가 비활성화 위치에 도달했는지 확인
			if (relativePosition == MAX_SLOTS - 1) {
				// 현재 카드는 비활성화 상태로 변경
				slots[i].isActive = false;
				if (slots[i].card) {
					auto transform = slots[i].card->GetComponent<TransformComponent>();
					if (transform) {
						auto pos = BLACKJACK->deck->GetComponent<TransformComponent>()->GetPosition();
						transform->SetPosition(pos);
					}
				}

				// 다음 순번의 카드로 변경
				currentCardIndex++;
				int nextCardIndex = currentCardIndex % cards.size();  // 카드 배열 범위 내에서 순환
				slots[i].card = cards[nextCardIndex];
				slots[i].isActive = true;

				// 새로운 카드의 초기 위치와 회전 설정
				float rotat = 90.0f;
				float eulerAngle = DirectX::XMConvertToRadians(rotat);
				DXMath::Quaternion rotation = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);

				auto newTransform = slots[i].card->GetComponent<TransformComponent>();
				if (newTransform) {
					newTransform->SetQuaternion(rotation);
					auto deckPos = BLACKJACK->deck->GetComponent<TransformComponent>()->GetPosition();
					newTransform->SetPosition(deckPos);
				}
			}
		}
	}

	// 한 바퀴 완료되었을 때
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
		if (!slots[i].card) continue;

		auto transform = slots[i].card->GetComponent<TransformComponent>();
		if (!transform) continue;

		if (slots[i].isActive) {
			int currentPos = (currentIndex + i) % MAX_SLOTS;
			int nextPos = (currentPos + 1) % MAX_SLOTS;

			// 정상적인 위치 보간
			DXMath::Vector3 interpolatedPosition;
			if (nextPos == MAX_SLOTS - 1) {
				// 비활성화 슬롯으로 이동하는 경우
				DXMath::Vector3 deckPos = BLACKJACK->deck->GetComponent<TransformComponent>()->GetPosition();
				transform->SetPosition(deckPos);
			}
			else {
				DXMath::Vector3::Lerp(
					positions[currentPos],
					positions[nextPos],
					t,
					interpolatedPosition
				);
			}

			transform->SetPosition(deckPosition + interpolatedPosition);
		}
	}
}
