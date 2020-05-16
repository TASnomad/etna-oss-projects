#ifndef	__IOBSERVER_HH__
#define	__IOBSERVER_HH__

class IObservable;

class IObserver
{
	public:
		virtual ~IObserver() { };
		virtual void Notify(IObservable *) = 0;
};

#endif	/* !__IOBSERVER_HH__ */
