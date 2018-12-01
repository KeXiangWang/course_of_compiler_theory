#include "pch.h"
#include "ElementCreater.h"
#include "Table.h"
#include "Quad.h"


bool ElementCreater::createConst(DataType dataType, string name, int value) {
	if (currentTable->find(name) != nullptr) {
		return false;
	}
	currentTable->insert(new TableElement(KINDCONST, dataType, name, value));
	return true;
}

bool ElementCreater::createVar(DataType dataType, string name) {
	if (currentTable->find(name) != nullptr) {
		return false;
	}
	currentTable->insert(new TableElement(KINDVAR, dataType, name));
	return true;
}

bool ElementCreater::createArray(DataType dataType, string name, int arraySize) {
	if (currentTable->find(name) != nullptr) {
		return false;
	}
	currentTable->insert(new TableElement(KINDARRAY, dataType, name, arraySize));
	return true;
}

bool ElementCreater::createPara(DataType dataType, string name) {
	currentFunction->withParameters = true;
	if (currentTable->find(name) != nullptr) {
		return false;
	}
	TableElement * tableElement = new TableElement(KINDPARA, dataType, name);
	currentFunction->parameters.push_back(tableElement);
	currentTable->insert(tableElement);
	return true;
}

void ElementCreater::createBranch(Token token, Quantity * quantity1, Quantity * quantity2, QuadTable * elseTable) {
	OPCode opCode;
	switch (token) {
	case NULLSYM: 
		opCode = OP_BEQZ;
		break;
	case NEQ:
		opCode = OP_BNE;
		break;
	case EQU:
		opCode = OP_BEQ;
		break;
	case LESS:
		opCode = OP_BLE;
		break;
	case LEQ:
		opCode = OP_BLT;
		break;
	case GEQ:
		opCode = OP_BGE; 
		break;
	case GRT:
		opCode = OP_BGT;
		break;
	default: // trash
		opCode = OP_BEQZ;
		break;
	}
	currentQuadTable->addQuad(new Branch(opCode, quantity1, quantity2, new Label(elseTable)));
}

void ElementCreater::createJump(Label *label) {
	currentQuadTable->addQuad(new Jump(label));
}

int ElementCreater::addString(string stringPrintf) {
	int size = (int)(constStringVector.size());
	constStringVector.push_back(stringPrintf);
	return size;
}

bool ElementCreater::createFunc(DataType dataType, string name) {
	currentFunction = new Function(dataType, name);
	currentTable = &currentFunction->elementTable;
	currentQuadTable = currentFunction->currentQuadTable;
	functionTable.insert(currentFunction);
	return true;
}

Function * ElementCreater::findFunc(string identifier) {
	return functionTable.find(identifier);
}

Function * ElementCreater::getCurrentFunction() {
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

void ElementCreater::actStatement(Quad * quad) {
	currentQuadTable->addQuad(quad);
}

void ElementCreater::setQuadTable(QuadTable * quadTable) {
	currentFunction->addQuadTable(quadTable); // add the quadTable into the current function
	currentQuadTable = quadTable; // set the quadTable as working quadTable
}
