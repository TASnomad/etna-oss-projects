#ifndef	__IOBSERVABLE_HH__
#define	__IOBSERVABLE_HH__

#include "IObserver.hh"

class IObservable
{
	public:
		virtual ~IObservable() { };
		virtual void AddObserver(IObserver *) = 0;
		virtual void RemoveObserver(IObserver *) = 0;
};

#endif	/* !__IOBSERVABLE_HH__ */
