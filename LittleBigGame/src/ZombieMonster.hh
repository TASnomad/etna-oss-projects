#ifndef	__ZOMBIEMONSTER_HH__
#define	__ZOMBIEMONSTER_HH__

#include "Character.hh"
#include "TimeManager.hh"

class ZombieMonster: public Character
{
	public:
		ZombieMonster();
		ZombieMonster(double, double, int, double);
		~ZombieMonster();
		virtual void Update() override;
		std::string &Serialize() override;
};
#endif /* !__ZOMBIEMONSTER_HH__ */

