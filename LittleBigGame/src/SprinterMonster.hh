#ifndef	__SPRINTERMONSTER_HH__
#define	__SPRINTERMONSTER_HH__

#include "Character.hh"
#include "TimeManager.hh"

class SprinterMonster: public Character
{
	public:
		SprinterMonster();
		SprinterMonster(double, double, int, double);
		~SprinterMonster();
		virtual void Update() override;
		virtual std::string &Serialize() override;
};

#endif	/* !__SPRINTERMONSTER_HH__ */
