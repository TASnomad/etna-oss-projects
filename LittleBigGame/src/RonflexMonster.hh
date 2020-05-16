#ifndef	__RONFLEXMONSTER_HH__
#define	__RONFLEXMONSTER_HH__

#include "Character.hh"
#include "TimeManager.hh"

class RonflexMonster: public Character
{
	public:
		RonflexMonster();
		RonflexMonster(double, double, int, double);
		~RonflexMonster();
		virtual void Update() override;
		virtual std::string &Serialize() override;
};

#endif	/* !__RONFLEXMONSTER_HH__ */
