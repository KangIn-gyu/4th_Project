#pragma once
class Card;
class Deck;
struct CardSlot {
    Card* card;
    bool isActive;

    CardSlot() : card(nullptr), isActive(false) {}
};

class CardRotation
{
private:
    static const int MAX_SLOTS = 11;
    static const int ACTIVE_SLOTS = 10;
    std::vector<DXMath::Vector3> positions;
    std::vector<CardSlot> slots;
    float timer = 0.0f;
    const float ROTATION_INTERVAL = 10.f; // 몇 초마다 한 칸씩 이동할지
    int currentIndex = 0;
    int completedRotations = 0;
    int currentCardIndex = 0;
    DXMath::Vector3 deckPosition;
    Deck* deck = nullptr;
public:
    CardRotation();

    void Init(Deck* deck);

    void Update(float deltaTime, std::vector<Card*>& cards);

    void RotateCards(std::vector<Card*>& cards);

    void UpdateCardPositions(float t);

};

