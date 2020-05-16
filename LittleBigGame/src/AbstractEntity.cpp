#include	"AbstractEntity.hh"

AbstractEntity::~AbstractEntity() { }

void AbstractEntity::Draw() { }
void AbstractEntity::Update() { }
std::string AbstractEntity::GetImage() { return this->path; }
void AbstractEntity::SetImage(std::string path) { this->path = path;}
