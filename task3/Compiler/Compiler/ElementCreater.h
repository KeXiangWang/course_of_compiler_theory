#pragma once
#include "Table.h"
#include "Quad.h"

class ElementCreater
{
public:
	ElementCreater() { currentTable = &globalTable; };
	bool createConst(DataType dataType, string name, int value);
	bool createVar(DataType dataType, string name);
	bool createArray(DataType dataType, string name, int arraySize);
	bool createFunc(DataType dataType, string name);
	bool createPara(DataType dataType, string name);
	void createJump(Label *label);
	Function *findFunc(string identifier);
	Function *getCurrentFunction();
	TableElement *findElement(string identifier);
	void actStatement(Quad *quad);

private:
	Table globalTable;
	Table *currentTable;
	FunctionTable functionTable;
	Function *currentFunction;
	QuadTable *currentQuadTable;
};

