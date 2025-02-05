#pragma once

class Player;
//버튼 함수 모아둘것 어케할지 몰라서 일단만듬
class ClickFunc
{
public:
	static void SelectCard();
	static void OpenButton();
	static void StayButton(); //
	static void HitButton();
	static void DoubleDown();

	static void SetPlayerSkill(Player* player);
};

