#pragma once
#include "global.h"
#include "Table.h"
#include "Quad.h"

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
	friend class MipsGenerator;
public:
	TableElement *find(string name); 
	void insert(TableElement * tableElement);
private: 
	vector<TableElement *> elementVector;
};

class Function {
public:
	Function(DataType dataType, string name) :functionType(dataType), name(name), withParameters(false) { 
		QuadTable *quadTable = new QuadTable(); 
		quadTableVector.push_back(quadTable);
		currentQuadTable = quadTableVector.back();
	};
	void addQuadTable(QuadTable *quadTable);
	DataType functionType; // Type of return
	string name;	// name
	Table elementTable;	
	QuadTable *currentQuadTable;
	//QuadTable *headQuadTable;
	vector<TableElement *> parameters;
	vector<QuadTable *> quadTableVector; // temp
	bool withParameters;
};

class FunctionTable {
	friend class MipsGenerator;
public:
	Function *find(string name);
	void insert(Function * function);
private:
	vector<Function *> functionVector;
};