#pragma once
#include"../Engine/Object.h"

class D2DBaseObj : public Object
{
public:
	// 기본
	D2DBaseObj(std::string_view _name, ObjectType _type) : Object(_name, _type) {}
	// 비트맵용
	D2DBaseObj(std::string_view _name, ObjectType _type, std::vector<std::string> _bitmapFilePath) : bitmapFilePath(_bitmapFilePath), Object(_name, _type) {}
	// 폰트 전용
	D2DBaseObj(std::string_view _name, ObjectType _type, std::string _fontFilePath) :fontFilePath(_fontFilePath), Object(_name, _type) {}
	// 폰트 비트맵, 폰트
	D2DBaseObj(std::string_view _name, ObjectType _type, std::vector<std::string> _bitmapFilePath, std::string _fontFilePath) :bitmapFilePath(_bitmapFilePath), fontFilePath(_fontFilePath), Object(_name, _type) {}
	// 폰트 비트맵, 폰트, CSV, 다음씬 이름
	D2DBaseObj(std::string_view _name, ObjectType _type, std::vector<std::string> _bitmapFilePath, std::string _fontFilePath, std::string _CsvFilePath, std::string _sceneName) :bitmapFilePath(_bitmapFilePath), fontFilePath(_fontFilePath), CsvFilePath(_CsvFilePath), sceneName(_sceneName), Object(_name, _type) {}
	virtual ~D2DBaseObj() = default;

	virtual void Initialize() override;                       // 용도 : 내가 필요한 컴포넌트 생성하는 곳 초기화나
	virtual void Update(const float _deltaTime) override {}; // 용도 : 오브젝트 개인의 업데이트가 필요할때 정의
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	virtual void ResetInformation() {}

private:

public:
	std::vector<std::string> bitmapFilePath;
	std::string fontFilePath{};
	std::string CsvFilePath{};
	std::string sceneName{};

private:
};

