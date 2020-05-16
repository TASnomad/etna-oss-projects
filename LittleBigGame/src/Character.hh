#ifndef	__CHARACTER_HH__
#define	__CHARACTER_HH__

#include "AbstractEntity.hh"
#include "IObservable.hh"
#include "IObserver.hh"
#include <list>
#include <string>

class Character: public AbstractEntity, public IObservable {
	private:
		double		_x;
		double		_y;
		int		_life;
		double		_speed;
		std::list<IObserver *> _observer;
		void 		_Notify();
	protected:
		Character();
		Character(double, double, int, double);
		Character(double, double, int, double,std::string);
	public:
		virtual		~Character() = 0;

		virtual void	Draw() override;
		virtual void	Update() override;
		virtual std::string GetImage() override;
		virtual void SetImage(std::string path) override;
		virtual double	GetX() const;
		virtual void	SetX(double);
		virtual double	GetY() const;
		virtual void	SetY(double);
		virtual int	GetLife() const;
		virtual void	SetLife(int);
		virtual void	SetSpeed(double);
		virtual double	GetSpeed() const;
		virtual void	AddObserver(IObserver *) override;
		virtual void	RemoveObserver(IObserver *) override;
		virtual std::string &Serialize() override;
};

#endif	/* !__CHARACTER_HH__ */
