#ifndef	__ABSTRACT_ENTITY_HH__
#define	__ABSTRACT_ENTITY_HH__

#include "ISerializable.hh"

class AbstractEntity: public ISerializable {
	public:
		std::string path;	
		virtual ~AbstractEntity() = 0;
		virtual void Draw() = 0;
		virtual void Update() = 0;
		virtual std::string GetImage() = 0;
		virtual void SetImage(std::string path) = 0;
};

#endif	/* !__ABSTRACT_ENTITY_HH__ */
