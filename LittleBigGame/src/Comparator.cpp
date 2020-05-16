#include "Comparator.hh"

Comparator::Comparator()
{ }

Comparator::Comparator(TileMap m):_tilemap(m)
{ }

bool Comparator::haveBlockDown(sf::Vector2f posSprite)
{
	if (this->_tilemap.GetTilesByCoords((posSprite.x + 5) / 16,( posSprite.y + 26) / 16))
	{
		return true;
	}
	return false;
}
bool Comparator::haveBlockUp(sf::Vector2f posSprite)
{
	if (this->_tilemap.GetTilesByCoords(posSprite.x / 16,( posSprite.y - 16) / 16) || this->_tilemap.GetTilesByCoords(posSprite.x / 16,(posSprite.y) / 16))
	{
		return true;
	}
	return false;
}
bool Comparator::haveBlockLeft(sf::Vector2f posSprite)
{
	if (this->_tilemap.GetTilesByCoords((posSprite.x - 16 ) / 16,posSprite.y / 16) || this->_tilemap.GetTilesByCoords((posSprite.x - 16) / 16, (posSprite.y + 16) / 16))
	{
		return true;
	}
	return false;
}
bool Comparator::haveBlockRight(sf::Vector2f posSprite)
{

	if (this->_tilemap.GetTilesByCoords((posSprite.x + 16) / 16,posSprite.y) || this->_tilemap.GetTilesByCoords((posSprite.x + 16) / 16, (posSprite.y + 16) / 16))
	{
		return true;
	}
	return false;
}
