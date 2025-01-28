#include "pch.h"
#include "Animation.h"

void Animation::SetName(std::string_view _name)
{
	name.assign(_name);
}

void Animation::SetDuration(float _val)
{
	duration = _val;
}

void Animation::SetTickPerSecond(float _val)
{
	tickPerSecond = _val;
}

void Animation::SetTotalTime(float _val)
{
	totalTime = _val;
}

void Animation::SettingTotalTime()
{
	if (duration > 0 && tickPerSecond > 0)
	{
		totalTime = duration / tickPerSecond;
	}
	else
	{ // TODO : 추후 로그 시스템으로 처리해야 됨.
		std::cout << "SettingTotalTime 오류" << '\n';
	}
}

void Animation::SetCurrTime(float _val)
{
	currTime = _val;
}

void Animation::AddAnimationNode(AnimationNode* _aniNode)
{
	nodes.emplace_back(_aniNode);
}
