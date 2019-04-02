#include "childclass.h"



childclass::childclass()
{
}


childclass::~childclass()
{
}


childclass::childclass(void* _main)
{
	testclass* main = (testclass*)_main;
	ca = main->a;
	cb = main->b;
	return;
}


void childclass::testrun(void* __this)
{
	childclass* _this = (childclass*)__this;
	while (true)
	{
		cout << "test child running" << endl;
		cout << "ca:" << _this->ca << endl;
		sleep(1);
	}
}