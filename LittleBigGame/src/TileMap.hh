#ifndef	__TILEMAP_HH__
#define	__TILEMAP_HH__

#include	<SFML/Graphics.hpp>

struct TileDescriptor
{
	int x;
	int y;
	int id;
};

struct Tile
{
	int tilesetId;
	// int x;
	// int y;
	int width;
	int height;
	sf::Sprite sp;
};

struct RenderTile
{
	struct Tile tile;
	int x;
	int y;
};

class TileMap
{
	private:
		sf::Texture _tileset;
		sf::VertexArray _vertices;
		std::map<int, Tile> _tiles;
		std::vector<RenderTile> _drawTiles;
		sf::Texture _backTex;
		sf::Sprite _back;

	public:
		bool LoadTiles(const std::string &, sf::Vector2u);
		// bool Test();
		bool InflateMap(const struct TileDescriptor *, int size);
		bool InflateMap(std::vector<TileDescriptor>);
		bool InflateMapFromFile(const std::string &);
		bool SetBackground(const std::string &);
		sf::Vector2u GetMapSquareSize(int, int);
		Tile *GetTileById(int);
		RenderTile *GetTilesByCoords(int, int);
		RenderTile *GetTilesByCoords(sf::Vector2i);
		void Draw();
		// bool Load(const std::string &, sf::Vector2u, const int *, unsigned int, unsigned int);
};

#endif	/* !__TILEMAP_HH__ */
