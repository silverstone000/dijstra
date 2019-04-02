#include "testclass.h"




testclass::~testclass()
{
}




testclass::testclass()
{
	a = 5;
	b = 5;
	endflag = false;

	return;
}


void testclass::fun1(void* __this)
{
	testclass* _this = (testclass*)__this;
	while (_this->a == _this->b)
	{
		if (_this->endflag)
		{
			return;
		}
		cout << _this->a << " and " << _this->b << endl;
		_this->a++;
		_this->b++;
		sleep(2);

	}
	return;
}

void testclass::run()
{
	childclass child = childclass(this);
	thread testthread(childclass::testrun, &child);
	testthread.join();
	return;
}