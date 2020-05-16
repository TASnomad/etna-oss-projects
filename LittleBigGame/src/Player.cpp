#include "Player.hh"

Player::Player(): Character(0.0, 0.0, 1, 2.0 / 1000.0, "../assets/sprite_player1.png"), _jumpImage("../assets/sprite_player0.png")
{ }

Player::Player(double x, double y, int l, double s) :
	Character(x, y, l, s)
{ }


Player::~Player() { }

void  Player::Update()
{
	double pos;
	pos = (double) TimeManager::GetInstance().GetElapsedTime() * GetSpeed();
	SetX(GetX() + pos);
	SetY(GetY() + pos);
}

std::string &Player::Serialize()
{
	std::string *tmp = new std::string();
	cJSON		*root = nullptr;

	tmp->assign(Character::Serialize());
	root = cJSON_Parse(tmp->c_str());
	if (root)
		cJSON_AddItemToObject(root, "class", cJSON_CreateString("Player"));
	tmp->assign(cJSON_Print(root));
	return (*tmp);
}

std::string Player::getJumpImage()
{
	return this->_jumpImage;
}

sf::Sprite Player::getSprite()
{
	return this->_sprite;
}

void Player::setSprite(sf::Sprite s)
{
	this->_sprite = s;
}

void Player::syncSprite()
{
	sf::Vector2f pos = this->_sprite.getPosition();
	this->SetY(pos.y);
	this->SetX(pos.x);
}


void Player::jump()
{
	if (!this->_cmp.haveBlockUp(this->_sprite.getPosition()))
	{
		this->_sprite.move(0,-20);
	}
	syncSprite();
}
void Player::move(double tileNumber)
{
	
	if ((!this->_cmp.haveBlockLeft(this->_sprite.getPosition()) && tileNumber < 0) || (!this->_cmp.haveBlockRight(this->_sprite.getPosition()) && tileNumber > 0))
	{

		this->_sprite.move(tileNumber * 16,0);
	}
	syncSprite();
}
void Player::changeOrientation(int number)
{
	sf::Vector2f firstScale = this->_sprite.getScale();
	if (firstScale.x != number && ( (number < 0 && !this->_cmp.haveBlockLeft(this->_sprite.getPosition())) ||  (number > 0 && !this->_cmp.haveBlockRight(this->_sprite.getPosition()))))
	{
		this->_sprite.setScale(number,2);
		this->_sprite.move(-number * 10 ,0);
	}
	syncSprite();
}
int Player::fall()
{
	if (!this->_cmp.haveBlockDown(this->_sprite.getPosition()))
	{
		this->_sprite.move(0, 1);
		syncSprite();
		return 1;
	}
	return 0;
}

void Player::setComparator(Comparator c)
{
	this->_cmp = c;
}
Comparator Player::getComparator()
{
	return this->_cmp;
}
