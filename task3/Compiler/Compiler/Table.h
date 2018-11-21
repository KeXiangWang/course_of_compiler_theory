#pragma once
#include "global.h"

using std::string;
using std::vector;

class TableElement {
	string name;	// name
	int addr;	// address
	int kind;	// const or var or func
	DataType dataType;	// datatype
	int value;	// value
	int number;	// number of parameter or array elements
};

class Table
{
public:
	Table();
	~Table();
	vector<TableElement> tableVector;
	int top;
};

