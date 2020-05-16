#ifndef	__COMPARATOR_HH__
#define	__COMPARATOR_HH__

#include "TileMap.hh"


class Comparator
{
	private:
	TileMap	_tilemap;

	public:
		Comparator();
		Comparator(TileMap);

		bool haveBlockDown(sf::Vector2f); 
		bool haveBlockUp(sf::Vector2f); 
		bool haveBlockLeft(sf::Vector2f); 
		bool haveBlockRight(sf::Vector2f); 
};

#endif	/* !__COMPARATOR_HH__ */
