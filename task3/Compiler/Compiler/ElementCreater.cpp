#include "pch.h"
#include "ElementCreater.h"
#include "Table.h"


ElementCreater::ElementCreater()
{
}


ElementCreater::~ElementCreater()
{
}

bool ElementCreater::creatConst()
{
	return true;
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
	TableElement * tableElement = new TableElement();
	return tableElement;
}
