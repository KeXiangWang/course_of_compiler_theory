#pragma once
#include "global.h"
using std::string;
using std::vector;

class Quad
{
public:
	Quad(OPCode opCode) : opCode(opCode) { count++; };
	OPCode opCode;
	static int count; // for name
	string id; // for distinguish
};

class Quantity :public Quad {
public:
	Quantity(OPCode opCode, DataType dataType) :Quad(opCode), dataType(dataType) {};
	DataType dataType;
};

class Caculator :public Quantity {
public:
	Caculator(OPCode opCode, Quantity *quantity1, Quantity *quantity2) : Quantity(opCode, TYPEINT) {};
	Quantity *quantity1;
	Quantity *quantity2;
};

class Constant :public Quantity {
public:
	Constant(DataType dataType, int value) : Quantity(OP_CONST, dataType), value(value) {};
	int value;
};

class Variable :public Quantity {
public:
	Variable(DataType dataType, string name) :Quantity(OP_VAR, dataType), name(name) {};
	string name;
	Quantity *value;
};

class Array :public Quantity {
public:
	Array();
};

class FunctionCall :public Quantity {
public:
	FunctionCall(DataType dataType, string name, vector<Quantity *> parameters) : Quantity(OP_FUNC, dataType), parameters(parameters) {};
	string name;
	vector<Quantity *> parameters;
};

class VoidCall :public Quad {
public:
	VoidCall(string name, vector<Quantity *> parameters) :Quad(OP_VOIDFUNC), parameters(parameters) {};
	string name;
	vector<Quantity *> parameters;
};

class QuadTable {
public:
	vector<Quad *> quads;
	void addQuad(Quad *quad);
};