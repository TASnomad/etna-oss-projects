#include	"TileMap.hh"
#include	"WindowManager.hh"

#include <fstream>
#include <iostream>
#include <cstring>

bool TileMap::LoadTiles(const std::string &tilesetPath, sf::Vector2u tileSize)
{
	int tileNum;
	int tu;
	int tv;
	int width;
	int height;
	int q = 0;

	if (!_tileset.loadFromFile(tilesetPath))
		return false;

	width = _tileset.getSize().x / tileSize.x;
	height = _tileset.getSize().y / tileSize.y;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Tile t;
			sf::Sprite tmp;

			tileNum = i + j * width;
			tu = tileNum % (_tileset.getSize().x / _tileset.getSize().x);
			tv = tileNum / (_tileset.getSize().y / _tileset.getSize().y);
			t.sp.setTexture(_tileset);
			t.sp.setTextureRect(
				sf::IntRect(tu, tv, tileSize.x, tileSize.y)
			);
			t.tilesetId = tileNum;
			t.width = tileSize.x;
			t.height = tileSize.y;
			_tiles.insert(std::make_pair(q, t));
			q++;
		}
	}
	return true;
}

// bool TileMap::Test()
// {
// 	Tile t = _tiles[2];
// 	sf::RenderWindow *tmp = WindowManager::GetInstance().GetWindow();
// 	// t.sp.setOrigin(10, 10);
// 	tmp->draw(t.sp);
// 	return true;
// 	// tmp->draw(t.sp);
// }

Tile *TileMap::GetTileById(int id)
{
	std::map<int, Tile>::iterator tmp;

	tmp = _tiles.find(id);
	return tmp != _tiles.end() ? &tmp->second : (Tile *) nullptr;
}

RenderTile *TileMap::GetTilesByCoords(int x, int y)
{
	std::vector<RenderTile>::iterator tmp;
	std::cout << "GetTilesByCoords() \n";
	std::cout << "x : ";
	std::cout << x;
	std::cout << "y : ";
	std::cout << y;
	std::cout << "\n";
	tmp = _drawTiles.begin();
	for (; tmp != _drawTiles.end(); tmp++) {
		RenderTile &rt = *tmp;
		if (tmp->x == x && tmp->y == y)
		{
			std::cout << "GetTilesByCoords return -> une case" << std::endl;
			std::cout << "GetTilesByCoords x :  " << x << std::endl;
			std::cout << "GetTilesByCoords y :  " << y << std::endl;
			return &rt;
		}
	}
	std::cout << "GetTilesByCoords return -> nullptr" << std::endl;
	return nullptr;
}


RenderTile *TileMap::GetTilesByCoords(sf::Vector2i coords)
{
	return GetTilesByCoords(coords.x, coords.y);
}

bool TileMap::SetBackground(const std::string &path)
{
	sf::RenderWindow *win = WindowManager::GetInstance().GetWindow();
	sf::Vector2u winSize = win->getSize();
	sf::Vector2u texSize;
	if (!_backTex.loadFromFile(path))
		return false;
	texSize = _backTex.getSize();
	_back.setTexture(_backTex);
	_back.setScale((float) winSize.x / texSize.x, (float) winSize.y / texSize.y);
	return true;
}

bool TileMap::InflateMap(const TileDescriptor *map, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		RenderTile rt;
		Tile tmp;

		tmp = _tiles.at(map[i].id);
		rt.tile = tmp;
		rt.x = map[i].x;
		rt.y = map[i].y;
		_drawTiles.push_back(rt);
		i++;
	}
	return true;
}

bool TileMap::InflateMap(std::vector<TileDescriptor> map)
{
	for (std::vector<TileDescriptor>::iterator it = map.begin(); it != map.end(); it++)
	{
		RenderTile rt;
		Tile tmp;
//	int i;

		std::distance(map.begin(), it);
		tmp = _tiles.at((*it).id);
		rt.tile = tmp;
		rt.x = (*it).x;
		rt.y = (*it).y;
		_drawTiles.push_back(rt);
	}
	return true;
}

bool TileMap::InflateMapFromFile(const std::string & path)
{
	std::vector<TileDescriptor> tmp;
	std::ifstream f;

	f.open(path);
	if (!f)
		return false;
	while (!f.eof())
	{
		TileDescriptor td;

		std::cout << "x : ";
		std::cout << td.x;
		std::cout << " y : ";
		std::cout << td.y;
		std::cout << " if : ";
		std::cout << td.id;
		std::cout << "\n";
		f >> td.id >> td.x >> td.y;
		tmp.push_back(td);
	}
	return InflateMap(tmp);
}

void TileMap::Draw()
{
	sf::RenderWindow *win = WindowManager::GetInstance().GetWindow();
	// sf::Vector2u winSize = win->getSize();

	win->draw(_back);
	for (RenderTile it: _drawTiles) {
		it.tile.sp.setPosition(it.x * it.tile.width, it.y * it.tile.height);
		// it.tile.sp.setScale((winSize.x / winSize.y) / it.tile.width, (winSize.x / winSize.y) / it.tile.height);
		// it.tile.sp.setScale(5.0f, 5.0f);
		win->draw(it.tile.sp);
	}
}

sf::Vector2u TileMap::GetMapSquareSize(int tileWidth, int tileHeight)
{
	sf::Vector2u res;
	sf::Vector2u winSize;
	sf::RenderWindow *win = WindowManager::GetInstance().GetWindow();

	winSize = win->getSize();
	res.x = winSize.x / tileWidth;
	res.y = winSize.y / tileHeight;
	return res;
}
