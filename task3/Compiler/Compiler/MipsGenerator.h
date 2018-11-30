#pragma once
#include "Quad.h"
#include "ElementCreater.h"

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
struct Reg {
	string name;
	Quantity *quantity;
	bool free;
	bool fix;
	Reg(string& n) :name(n), free(true), fix(false) {}
};
class MipsGenerator
{
public:
	MipsGenerator(ElementCreater *elementCreater) :elementCreater(elementCreater) {};
	void generate();
	void generateFunction(Function *function);
	void allocateGloabal(Function * function);
	int getVarNumber(Function * function);

private:
	QuadTable *quadTable;
	ElementCreater *elementCreater;
	vector<string> finalCode;
	vector<string> initCode;
	vector<string> exertCode;
	unordered_map<string, int> localOffset; // relative to $fp
	unordered_map<Quad *, int> tempOffset;  // relative to $fp

	unordered_map<QuadTable *, int> bb2label;
	unordered_map<string, int> GlobalReg;
	unordered_map<string, Reg*> TempReg;
	unordered_set<string> loadedToGloabal;

	unordered_map<string, int> refCount;
	unordered_map<string, int> stackOffset;
	int labelCount;
};

