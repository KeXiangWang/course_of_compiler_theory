#pragma once
#include "Table.h"
#include "Quad.h"
#include "global.h"

class ElementCreater
{
	friend class MipsGenerator;
	friend class Optimizer;
public:
	ElementCreater() { currentTable = &globalTable; };
	bool createConst(DataType dataType, string name, int value);
	bool createVar(DataType dataType, string name);
	bool createArray(DataType dataType, string name, int arraySize);
	bool createFunc(DataType dataType, string name);
	bool createPara(DataType dataType, string name);
	void createBranch(Token token, Quantity *quantity1, Quantity *quantity2, QuadTable * elseTable);
	void createJump(Label *label);
	int addString(string stringPrintf);
	Function *findFunc(string identifier);
	Function *getCurrentFunction();
	TableElement *findElement(string identifier);
	TableElement *findElementFromGlobal(string identifier);
	void actStatement(Quad *quad);
	void setQuadTable(QuadTable *quadTable);

private:
	Table globalTable;
	Table *currentTable;
	FunctionTable functionTable;
	Function *currentFunction;
	QuadTable *currentQuadTable;
	vector<string> constStringVector;
};

