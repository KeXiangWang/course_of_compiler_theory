#pragma once
#include "global.h"
#include "Quad.h"

using std::string;
using std::vector;

class Quad
{
public:

	Quad(OPCode opCode) : opCode(opCode) { count++; id = ""; };
	virtual string toString() const;
	OPCode opCode;
	string id; // for distinguish
protected:
	static int count; // for name
};

class QuadTable {
public:
	QuadTable() { count++; next = nullptr; };
	vector<Quad *> quads;
	void addQuad(Quad *quad);
	static int count; // for name
	QuadTable* next;
};

class Quantity :public Quad {
	friend class MipsGenerator;
public:
	Quantity(OPCode opCode, DataType dataType) :Quad(opCode), dataType(dataType) { isGlobalVarPara = false; };
	virtual bool equals(Quantity *quantity) const { return opCode == quantity->opCode; };
	virtual string toString() const;
	DataType dataType;
	bool isGlobalVarPara;
};
// Quantity: caculator, Constant, Variable, Array, FunctionCall
class Caculator :public Quantity {
public:
	Caculator(OPCode opCode, Quantity *quantity1, Quantity *quantity2) : Quantity(opCode, TYPEINT), quantity1(quantity1), quantity2(quantity2) {
		switch (opCode)
		{
		case OP_PLUS:
			id = "add_" + std::to_string((long long)caculatorCount);
			break;
		case OP_SUB:
			id = "sub_" + std::to_string((long long)caculatorCount);
			break;
		case OP_DIV:
			id = "div_" + std::to_string((long long)caculatorCount);
			break;
		case OP_MULT:
			id = "mult_" + std::to_string((long long)caculatorCount);
			break;
		}
		caculatorCount++;
	};
	virtual bool equals(Quantity *quantity) const;  
	virtual string toString() const;
	Quantity *quantity1;
	Quantity *quantity2;
	static int caculatorCount; // for name
};

class Constant :public Quantity {
public:
	Constant(DataType dataType, int value) : Quantity(OP_CONST, dataType), value(value) {
		if (dataType == TYPEINT) {
			id = "const_int_" + std::to_string((long long)count);
		}
		else {
			id = "const_char_" + std::to_string((long long)count);
		}
	};
	virtual bool equals(Quantity *quantity) const;
	virtual string toString() const;
	int value;
};

class Variable :public Quantity {
public:
	Variable(DataType dataType, string name, Quantity *value = nullptr) :Quantity(OP_VAR, dataType), name(name), value(value) { id = name; };
	virtual bool equals(Quantity *quantity) const;
	virtual string toString() const;

	string name;
	Quantity *value;
};

class Array :public Quantity {
public:
	Array(DataType dataType, string name, Quantity *index, Quantity *value = nullptr) :
		Quantity(OP_ARRAY, dataType), index(index), name(name), value(value) {
		id = "array_" + std::to_string((long long)count);
	};
	virtual bool equals(Quantity *quantity) const ;
	virtual string toString() const;
	string name;
	Quantity *value; // the value of the array element
	Quantity *index; // the index
};

class FunctionCall :public Quantity {
public:
	FunctionCall(DataType dataType, string name, vector<Quantity *> parameters) :
		Quantity(OP_FUNC, dataType), name(name), parameters(parameters) {
		id = "fcall_" + std::to_string((long long)count);
	};
	virtual bool equals(Quantity *Quantity) const { return false; };
	virtual string toString() const;
	string name;
	vector<Quantity *> parameters;
};

class VoidCall :public Quad {
public:
	VoidCall(string name, vector<Quantity *> parameters) :Quad(OP_VOIDFUNC), name(name), parameters(parameters) {};
	virtual string toString() const;
	string name;
	vector<Quantity *> parameters;
};

class Scanf :public Quad {
public:
	Scanf(vector<Variable *> parameters) :Quad(OP_SCANF), parameters(parameters) {};
	virtual string toString() const;
	vector<Variable *> parameters;
};

class Printf :public Quad {
public:
	Printf(Quantity *quantity) :Quad(OP_PRINTF), quantity(quantity) { stringInt = -1; };
	Printf(int stringInt, string stringPrintf, Quantity *quantity = nullptr) :Quad(OP_PRINTF), stringInt(stringInt), stringPrintf(stringPrintf), quantity(quantity) {};
	virtual string toString() const;
	int stringInt;
	string stringPrintf;
	Quantity *quantity;
};

class Return :public Quad {
public:
	Return(Quantity *quantity = nullptr) :Quad(OP_RETURN), quantity(quantity) {};
	virtual string toString() const;
	Quantity *quantity;
};

class Label :public Quad {
public:
	Label(QuadTable *quadTable = nullptr) :Quad(OP_LABEL), labelQuadTable(quadTable) { label_number = count; };
	virtual string toString() const;
	QuadTable *labelQuadTable;
	int label_number;
};

class Jump :public Quad {
public:
	Jump(Label *label) :Quad(OP_JUMP), label(label) {};
	virtual string toString() const;
	Label *label;
};

class Branch :public Quad {
public:
	Branch(OPCode opCode, Quantity *quantity1, Quantity *quantity2, Label *label)
		:Quad(opCode), quantity1(quantity1), quantity2(quantity2), label(label) {};
	virtual string toString() const;
	Quantity *quantity1;
	Quantity *quantity2;
	Label *label;
};