#pragma once
#include "Table.h"

class ElementCreater
{
public:
	ElementCreater() { currentTable = &globalTable; };
	bool creatConst(DataType dataType, string name, int value);
	bool creatVar(DataType dataType, string name);
	bool creatArray(DataType dataType, string name, int arraySize);
	bool creatFunc(DataType dataType, string name);
	bool creatPara(DataType dataType, string name);
	Function *findFunc(string identifier);
	TableElement *findElement(string identifier);

	Table globalTable;
	Table *currentTable;
	FunctionTable functionTable;
	Function *currentFunction;
	QuadTable *currentQuadTable;
};

