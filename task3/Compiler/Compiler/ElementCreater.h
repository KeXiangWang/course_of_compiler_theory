#pragma once
#include "Table.h"

class ElementCreater
{
public:
	ElementCreater();
	~ElementCreater();
	bool creatConst();
	bool creatVar();
	bool creatArray();
	bool creatFunc();
	bool creatArg();
	TableElement *find(string identifier);
};

