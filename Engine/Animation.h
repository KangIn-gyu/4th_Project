#pragma once

class AnimationNode;
class Animation
{
public:
	Animation() = default;
	~Animation() = default;

	float GetTickPerSecond() { return tickPerSecond; }
	float GetDuration() { return duration; }
	float GetTotalTime() { return totalTime; }

	void SetName(std::string_view _name);
	void SetDuration(float _val);
	void SetTickPerSecond(float _val);
	void SetTotalTime(float _val);
	void SettingTotalTime();

	void AddAnimationNode(AnimationNode* _aniNode);

	std::vector<AnimationNode*> GetAnimationNodes() { return nodes; }
	std::string GetName() { return name; }
private:

public:

private:
	std::vector<AnimationNode*> nodes;
	std::string name{};
	float duration {};      // 전체 틱(Tick) 수
	float tickPerSecond {}; // 초당 틱
	float totalTime {};		// 애니메이션 총 시간
	float currTime {};		// 현재 애니메이션 시간
};

