#include "Character.hh"

Character::Character(): AbstractEntity() { }
Character::Character(double x, double y, int life, double speed):
	_x(x), _y(y), _life(life), _speed(speed)
{ }
Character::Character(double x, double y, int life, double speed, std::string path):
	_x(x), _y(y), _life(life), _speed(speed)
{
	AbstractEntity::SetImage(path);
}
void Character::Draw() { }
void Character::Update() { }
std::string Character::GetImage() {return AbstractEntity::GetImage();}
void Character::SetImage(std::string path) {return AbstractEntity::SetImage(path);}
Character::~Character() { }

double Character::GetX() const { return this->_x; }

void Character::SetX(double x) {
	this->_x = x;
	_Notify();
}

double Character::GetY() const { return this->_y; }

void Character::SetY(double y) {
	this->_y = y;
	_Notify();
}

int Character::GetLife() const { return this->_life; }

void Character::SetLife(int life) {
	this->_life = life;
	_Notify();
}

void Character::SetSpeed(double speed) { this->_speed = speed; }

double Character::GetSpeed() const { return this->_speed; }

void Character::AddObserver(IObserver *obs) { _observer.push_back(obs); }

void Character::RemoveObserver(IObserver *obs) { _observer.remove(obs); }

void Character::_Notify()
{
	for (IObserver *one: _observer)
		one->Notify(this);
}

std::string &Character::Serialize()
{
	std::string *tmp = new std::string();
	cJSON		*root = cJSON_CreateObject();

	cJSON_AddItemToObject(root, "x", cJSON_CreateNumber(_x));
	cJSON_AddItemToObject(root, "y", cJSON_CreateNumber(_y));
	cJSON_AddItemToObject(root, "life", cJSON_CreateNumber(_life));
	cJSON_AddItemToObject(root, "speed", cJSON_CreateNumber(_speed));
	tmp->assign(cJSON_Print(root));
	return (*tmp);
}
