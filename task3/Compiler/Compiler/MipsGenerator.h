#pragma once
#include "Quad.h"
#include "ElementCreater.h"

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

class Reg {
public:
	string name;
	Quantity *quantity;
	bool free;
	bool fixed;
	Reg(string n) :name(n), free(true), fixed(false) {};
};

class MipsGenerator
{
public:
	MipsGenerator(ElementCreater *elementCreater) :elementCreater(elementCreater) { labelCount = 0; };
	void generate();
	void initData();
	void initRegs();
	void generateFunction(Function *function);
	void generateAddSub(Function *function, Quad *quad, OPCode opCode);
	void generateMultDiv(Function *function, Quad *quad, OPCode opCode);
	void generateVar(Function *function, Quad *quad);
	void generateArray(Function *function, Quad *quad);
	void generateBranch(Function *function, Quad *quad, string branchName);
	void generateBranchZ(Function *function, Quad *quad);
	void generateJump(Function *function, Quad *quad);
	void generateReturn(Function *function, Quad *quad);
	void generateFunc(Function *function, Quad *quad, int offset);
	void generateVoidFunc(Function *function, Quad *quad, int offset);
	void generateScanf(Function *function, Quad *quad);
	void generatePrintf(Function *function, Quad *quad);

	string getReg(Function *function, Quantity *quantity, bool write = false, int temp = 0);
	void decreaseRef(Quantity * value);
	Reg * overflow(Function *function);
	void allocateGloabal(Function * function);
	int getVarNumber(Function * function);
	void writeBack(Function * function);
	void loadValue(Function *function, Quad *quad, string reg, int temp);
	void loadValueGlobal(Function *function, Quad *quad, string reg, int temp);
	void storeValue(Function *function, Quad *quad, string reg);
	void storeValueArray(Function *function, Array *arr, string reg, string freeReg);
	void moveToReg(Function *functoin, Quantity *value, string reg, int temp = 0);
	void printCode(std::fstream &output);

private:
	QuadTable *quadTable;
	ElementCreater *elementCreater;
	vector<string> finalCode;
	vector<string> initCode;
	vector<string> exertCode;
	vector<Reg> usedTempRegs;

	unordered_map<QuadTable *, int> bb2label;
	unordered_map<string, int> storeRegs; // $s: used (var)
	unordered_map<string, Reg*> tempRegs; // $t: temporaray
	unordered_set<string> loadedToStore;

	unordered_map<string, int> refCount;  // used (var mid-var direct-const)
	unordered_map<string, int> stackOffset; // all (para declared-const var)
	int labelCount;
};

