#include "pch.h"
#include "TimeSystem.h"
#include <sstream>
#include <iomanip>

void TimeSystem::Initialize()
{
	m_prevTime = std::chrono::high_resolution_clock::now();  // 최초 시작 카운트
	m_StartTime = m_prevTime;							     // 타이머 시작 시간 설정
	m_Stopped = false;										 // 타이머가 처음에는 멈춰있지 않도록 설정
	m_PausedTime = std::chrono::duration<double>::zero();    // 일시 정지 시간 초기화
}

void TimeSystem::Update()
{
	m_curTime = std::chrono::high_resolution_clock::now(); // 현재 시간 계산

	// 이전 프레임의 카운팅과 현재프레임 카운팅 값의 차이를 구한다.1
	std::chrono::duration<double> elapsed = m_curTime - m_prevTime;
	if (m_deltaTime < 0.0) { m_deltaTime = 0.0; } // 혹시 모르게 음수인 경우 방어용
	m_deltaTime = elapsed.count() * m_TimeScale; // 초를 구한다
	m_prevTime = m_curTime; // 이전 시간에 현재시간 넣기

	if (m_Stopped)
	{
		m_deltaTime = 0.0f;
		return;
	}

	++m_iCallCount;			// 프레임간의 걸리는 시간을 뒤집으면 초당 프레임 횟수를 알 수 있다
	m_dAcc += m_deltaTime;  // 델타타임을 누적 시킨다. 결론 흐른 시간을 알 수 있다.

	if (m_dAcc >= 1.0) // 안정적인 FPS 정보를 제공
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0.0;
		m_iCallCount = 0.0;
	}
}

double TimeSystem::GetTotalTime() const
{
	return std::chrono::duration_cast<std::chrono::duration<double>>(m_curTime - m_StartTime - m_PausedTime).count();
}

void TimeSystem::Start()
{
	if (m_Stopped)
	{
		auto now = std::chrono::high_resolution_clock::now();
		// 멈춰 있었던 시간을 빼줘서 일시 정지된 시간만큼 조정
		m_PausedTime += now - m_StopTime; // 멈춰있던 시간을 누적
		m_prevTime = now; // 다시 시작할 때 기준으로 이전 시간 업데이트
		m_Stopped = false;  // 타이머를 다시 시작
		m_TimeScale = 0;
	}
}

void TimeSystem::Stop()
{
	if (!m_Stopped)
	{
		m_TimeScale = 1;
		m_Stopped = true;  // 타이머를 멈춤
	}
}

std::string TimeSystem::GetCurrentDateTime() const
{
	auto now = std::chrono::system_clock::now(); // 현재 시간 얻기
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now); // std::time_t로 변환
	std::tm now_tm; // std::tm로 변환
	localtime_s(&now_tm, &now_time_t);
	std::stringstream stream;
	stream << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S"); // 연월일 시:분:초 형식으로 포맷
	return stream.str();
}

std::string TimeSystem::GetCurrentDate() const
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	std::tm now_tm;
	localtime_s(&now_tm, &now_time_t);
	std::stringstream stream;
	stream << std::put_time(&now_tm, "%Y-%m-%d"); // 연월일 형식
	return stream.str();
}

std::string TimeSystem::GetCurrentTimes() const
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	std::tm now_tm;
	localtime_s(&now_tm, &now_time_t);
	std::stringstream stream;
	stream << std::put_time(&now_tm, "%H:%M:%S"); // 시간:분:초 형식
	return stream.str();
}
