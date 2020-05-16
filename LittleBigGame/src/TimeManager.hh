#ifndef	__TIMEMANAGER_HH__
#define	__TIMEMANAGER_HH__

#include <chrono>
#include <queue>

class TimeManager
{
	std::chrono::time_point<std::chrono::steady_clock> _start;
	std::queue<std::chrono::time_point<std::chrono::steady_clock>> _updated;

	protected:
	TimeManager();
	~TimeManager();
	TimeManager(const TimeManager&);
	void operator=(const TimeManager &);

	public:
	static TimeManager &GetInstance() noexcept;
	void Start();
	void Update();
	unsigned int GetElapsedTime() const;
	unsigned int GetStartedTime() const;
};



#endif	/* !__TIMEMANAGER_HH__ */
