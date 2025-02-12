#pragma once
#include "SingletonBase.h"
#include <chrono>
#define TIMESYSTEM TimeSystem::GetInstance()

class TimeSystem : public SingletonBase<TimeSystem>
{
	friend class SingletonBase<TimeSystem>; // 싱글톤이 timeSystem의 생성자와 소멸자를 접근할 수 있게 하기 위해

private:
	TimeSystem() = default;
	virtual	~TimeSystem() {};

public: // 복사와 이동 생성자 불가능하게 처리
	TimeSystem(const TimeSystem& Time) = delete;
	TimeSystem(TimeSystem&& Time) = delete;

	void Initialize();
	void Update();

	double GetDoubleDeltaTime() const { return m_deltaTime; }
	float GetFloatDeltaTime() const { return static_cast<float>(m_deltaTime); } // 큰 메모리에서 작은 자료형으로 캐스팅이라 문제 없음
	int GetFPS() const { return m_iFPS; }; // 프레임률 확인용
	double GetTotalTime() const;

	void Start(); // 시작
	void Stop();  // 정지 현재시간도 멈춤
	bool& GetStoppedBool() { return m_Stopped; }

	std::string GetCurrentDateTime() const;  // 연월일, 시간, 분, 초 반환
	std::string GetCurrentDate() const;      // 연월일 반환
	std::string GetCurrentTimes() const;     // 시간, 분, 초 반환

private:
	std::chrono::high_resolution_clock::time_point m_curTime;   // 현재 카운트
	std::chrono::high_resolution_clock::time_point m_prevTime;  // 이전 카운트
	std::chrono::duration<double> m_PausedTime;					// 일시 중지된 시간
	std::chrono::high_resolution_clock::time_point m_StartTime; // 타이머 시작 카운트
	std::chrono::high_resolution_clock::time_point m_StopTime;  // 정지 카운트

	double m_deltaTime = 0.0;	 // 프레임 사이의 시간 값
	double m_dAcc = 0.0;		 // accumulator 누적용 1초 체크
	int m_iCallCount = 0;		 // 함수 호출 회수 체크 
	int m_iFPS = 0;				 // 초당 호출 횟수
	bool m_Stopped;				 // 일시 정지 bool true면 정지
	double m_TimeScale = 1;		 // 타임 스케일
};
