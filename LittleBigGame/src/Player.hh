#ifndef	__PLAYER_HH__
#define	__PLAYER_HH__

#include "Character.hh"
#include "Comparator.hh"
#include "TimeManager.hh"
#include <iostream>

#include	<SFML/Graphics.hpp>

class Player: public Character
{
	private:
		std::string _jumpImage;
		Comparator _cmp;
		sf::Sprite _sprite;
		
	public:
		Player();
		Player(double, double, int, double);
		~Player();
		virtual void Update() override;
		std::string &Serialize() override;
		std::string getJumpImage();
		sf::Sprite getSprite();
		void setComparator(Comparator);
		Comparator getComparator();
		void setSprite(sf::Sprite);
		void syncSprite();
		void jump();
		void move(double);
		int fall();
		void changeOrientation(int);

	
};
#endif /* !__PLAYER_HH__ */

