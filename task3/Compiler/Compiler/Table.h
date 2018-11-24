#pragma once
#include "global.h"

using std::string;
using std::vector;

class TableElement {
public:
	TableElement(ElementKind kind, string name, DataType dataType, int value = 0) :kind(kind), name(name), dataType(dataType), value(value) {};
	string name;	// name
	int addr;	// address
	ElementKind kind;	// const or var or func
	DataType dataType;	// datatype
	int value;	// value
	int number;	// number of parameter or array elements
};

class Table
{
public:

	TableElement *find(string name); 
	void insert(TableElement * tableElement);
private: 
	vector<TableElement *> elementVector;
};

