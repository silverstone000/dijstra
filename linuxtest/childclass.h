#pragma once

#include <stdio.h>
#include <iostream>
#include <thread>
#include <queue>
#include <map>

#include "boost/asio.hpp"
#include "testclass.h"

#include <unistd.h>

using namespace std;


class testclass;

class childclass
{
public:
	childclass();
	~childclass();
	int ca, cb;


	childclass(void* _main);


	static void testrun(void* __this);



};

