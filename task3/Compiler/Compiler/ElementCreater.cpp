#include "pch.h"
#include "ElementCreater.h"
#include "Table.h"
#include "Quad.h"


bool ElementCreater::creatConst(string name, DataType dataType, int value)
{
	if (globalTable.find(name) != nullptr) {
		if (parsingFunction) {

		}
		else {
			return false;
		}
	}
	else {
		currentTable->insert(new TableElement(KINDCONST, name, dataType, value));
		return true;
	}
}

bool ElementCreater::creatVar()
{
	return true;
}

bool ElementCreater::creatArray()
{
	return true;
}

bool ElementCreater::creatFunc()
{
	return true;
}

bool ElementCreater::creatArg()
{
	return true;
}

TableElement * ElementCreater::find(string identifier)
{
	return nullptr;
}
