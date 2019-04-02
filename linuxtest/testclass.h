#pragma once
#include <stdio.h>
#include <iostream>
#include <thread>
#include <queue>
#include <map>

#include "boost/asio.hpp"

#include "childclass.h"

#include <unistd.h>

using namespace std;

class childclass;

class testclass
{
public:
	~testclass();
	testclass();

	int a;
	int b;
	bool endflag;
	static void fun1(void* __this);

	void run();

};

