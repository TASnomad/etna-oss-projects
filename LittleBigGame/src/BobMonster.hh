#ifndef	__BOBMONSTER_HH__
#define	__BOBMONSTER_HH__

#include "Character.hh"
#include "TimeManager.hh"

class BobMonster: public Character
{
	public:
		BobMonster();
		BobMonster(double, double, int, double);
		~BobMonster();
		virtual void Update() override;
		virtual std::string &Serialize() override;
};

#endif	/* !__BOBMONSTER_HH__ */
