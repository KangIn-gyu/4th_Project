#include "pch.h"
#include "D2DBitMapFontScript.h"

#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/Helper.h"
#include "../Engine/SceneManager.h"

void D2DBitMapFontScript::ComponentSetting()
{// 걍 하드 코딩 함
	if (typeid(*ownerObject) == typeid(D2DBaseObj))
	{
		{ // 비트맵 처리
			ownerD2D = ownerObject->GetComponent<D2DRenderComponent>();
			startImage = static_cast<D2DBaseObj*>(ownerObject)->start;
			endImage = static_cast<D2DBaseObj*>(ownerObject)->end;

			std::string basePath = "DialogScenes/Textures/";
			for (int i = startImage; i <= endImage; i++)
			{
				std::string filePath =  basePath + std::to_string(i);
				ownerD2D->Load2DImage(filePath + ".png");
			}

			changeSceneName = static_cast<D2DBaseObj*>(ownerObject)->sceneName;
		}

		{ // 폰트 초기화 예시
			ownerD2D->LoadFont(static_cast<D2DBaseObj*>(ownerObject)->fontFilePath);  // 폰트로드
			ownerD2D->SetDialog(L"");
			//ownerD2D->SetTextSize(20.0f, { 0, static_cast<UINT32>(text.length()) }); // 사이즈 변경
			ownerD2D->SetTextSize(35);
			ownerD2D->SetFontPos(450, 850);
			ownerD2D->SetFontBoxSize(1200, 200);
			ownerD2D->SetAlignment(D2DFont::Setting::LEFT, D2DFont::Setting::TOP);
			ownerD2D->SetLineSpacing(45.f);
			ownerD2D->SetFontColor(D2D1::ColorF(D2D1::ColorF::White));
		}
	
		ownerD2D->SceneCSVDataLoad(static_cast<D2DBaseObj*>(ownerObject)->CsvFilePath); // CSV처리
		csvData = ownerD2D->GetCSVDData();
	}
}

void D2DBitMapFontScript::Update(const float _deltaTime)
{
	static float elapsedTime = 0.0f;  // 시간 누적 변수
	const float interval = 0.1f;      // 글자 추가 간격 (초 단위)

	if (index >= csvData.size())
	{
		return;
		// 더 이상 출력할 데이터가 없으면 종료
	}
	if (displayedText.length() < csvData[index].second.length())
	{
		elapsedTime += _deltaTime;

		if (elapsedTime >= interval)
		{
			elapsedTime = 0.0f;  // 타이머 초기화
			displayedText += csvData[index].second[displayedText.length()];  // 한 글자 추가
			ownerD2D->SetDialog(displayedText);  // 텍스트 업데이트
		}
	}
}

void D2DBitMapFontScript::ResetInformation()
{
	index = 0;
	displayedText = L"";
	ownerD2D->SetDialog(displayedText);
}

void D2DBitMapFontScript::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
	if (ownerObject->IsActive() == true)
	{
		if (_MouseTracker.leftButton == DX::Mouse::ButtonStateTracker::PRESSED)
		{
			if (index == csvData.size())
			{
				if (true == isClicked)
				{
					isClicked = false;
					ownerD2D->IsFadeIn = TRUE;
				}
			}
			else
			{
				if (displayedText.length() < csvData[index].second.length())
				{
					isClicked = true;
					ownerD2D->SetDialog(csvData[index].second);
					displayedText = csvData[index].second;
				}
				else
				{
					index++;
					if (index < csvData.size())
					{
						int ChangeBitmapindex = csvData[index].first;
						ownerD2D->ChangeBitmap(ChangeBitmapindex - startImage);
						displayedText = L"";
					}
				}
			}
		}
	}
}
