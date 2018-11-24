#pragma once
#include "Table.h"

class ElementCreater
{
public:
	bool creatConst();
	bool creatVar();
	bool creatArray();
	bool creatFunc();
	bool creatArg();
	TableElement *find(string identifier);

	Table globalTable;
	Table *currentTable;
private:
	bool parsingFunction;
};

