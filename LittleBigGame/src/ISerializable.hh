#ifndef	__ISERIALIZABLE_HH__
#define	__ISERIALIZABLE_HH__

#include <string>
#include "json.hh"

class ISerializable
{
	public:
		virtual ~ISerializable() { }
		virtual std::string &Serialize() = 0;
};

#endif	/* !__ISERIALIZABLE_HH__ */
