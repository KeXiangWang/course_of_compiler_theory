#include "pch.h"
#include "ElementCreater.h"
#include "Table.h"
#include "Quad.h"


bool ElementCreater::creatConst(DataType dataType, string name, int value) {
	if (currentTable->find(name) != nullptr) {
		return false;
	}
	currentTable->insert(new TableElement(KINDCONST, dataType, name, value));
	return true;
}

bool ElementCreater::creatVar(DataType dataType, string name) {
	if (currentTable->find(name) != nullptr) {
		return false;
	}
	currentTable->insert(new TableElement(KINDVAR, dataType, name));
	return true;
}

bool ElementCreater::creatArray(DataType dataType, string name, int arraySize){
	if (currentTable->find(name) != nullptr) {
		return false;
	}
	currentTable->insert(new TableElement(KINDARRAY, dataType, name, arraySize));
	return true;
}

bool ElementCreater::creatPara(DataType dataType, string name) {
	currentFunction->withParameters = true;
	if (currentTable->find(name) != nullptr) {
		return false;
	}
	TableElement * tableElement = new TableElement(KINDPARA, dataType, name);
	currentFunction->parameters.push_back(tableElement);
	currentTable->insert(tableElement);
	return true;
}

bool ElementCreater::creatFunc(DataType dataType, string name) {
	currentFunction = new Function(dataType, name);
	currentTable = &currentFunction->elementTable;
	currentQuadTable = &currentFunction->quadTable;
	functionTable.insert(currentFunction);
	return true;
}

Function * ElementCreater::findFunc(string identifier) {
	return functionTable.find(identifier);
}

Function * ElementCreater::getCurrentFunction()
{
	return currentFunction;
}

TableElement * ElementCreater::findElement(string identifier) {
	TableElement * tableElement;
	if ((tableElement = currentTable->find(identifier)) != nullptr) {
		return tableElement;
	}
	else if ((tableElement = globalTable.find(identifier)) != nullptr) {
		return tableElement;
	}
	return nullptr;
}

void ElementCreater::actStatement(Quad * quad)
{
	currentQuadTable->addQuad(quad);
}
