#pragma once
#include "global.h"

using std::string;
using std::vector;

class TableElement {
public:
	TableElement(ElementKind kind, DataType dataType, string name, int value = 0) :kind(kind), dataType(dataType), name(name), value(value) {};
	string name;	// name
	int addr;	// address
	ElementKind kind;	// const or var or func
	DataType dataType;	// datatype
	int value;	// value
	int number;	// number of parameter or array elements
};

class Table{
public:
	TableElement *find(string name); 
	void insert(TableElement * tableElement);
private: 
	vector<TableElement *> elementVector;
};

class Function {
public:
	Function(DataType dataType, string name) :functionType(dataType), name(name), withParameters(false) {};
	DataType functionType; // Type of return
	string name;	// name
	Table elementTable;
	bool withParameters;
};

class FunctionTable {
public:
	Function *find(string name);
	void insert(Function * function);
private:
	vector<Function *> functionVector;
};