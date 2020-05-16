#include "TimeManager.hh"


TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}

TimeManager::TimeManager(const TimeManager &)
{
}

void TimeManager::operator=(const TimeManager &)
{
}

TimeManager &TimeManager::GetInstance() noexcept
{
	static TimeManager singleton;
	return singleton;
}

void TimeManager::Start()
{
	_start = std::chrono::steady_clock::now();
	while (!_updated.empty())
	{
		_updated.pop();
	}
	_updated.push(_start);
}

void TimeManager::Update()
{
	_updated.push(std::chrono::steady_clock::now());
	if (_updated.size() > 2)
	{
		_updated.pop();
	}
}

unsigned int TimeManager::GetElapsedTime() const
{
	return (unsigned int)(_updated.size() == 2 ?
			std::chrono::duration_cast<std::chrono::milliseconds>(_updated.back() - _updated.front()).count()
			: 0);
}

unsigned int TimeManager::GetStartedTime() const
{
	return (unsigned int)std::chrono::duration_cast<std::chrono::milliseconds>(_updated.back() - _start).count();
}


