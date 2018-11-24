#include "pch.h"
#include "Table.h"

TableElement * Table::find(string name)
{
	for (auto iter = elementVector.begin(); iter != elementVector.end(); iter++) {
		if ((*iter)->name == name) {
			return (*iter);
		}
	}
	return nullptr;
}
void Table::insert(TableElement * tableElement)
{
	elementVector.push_back(tableElement);
}
