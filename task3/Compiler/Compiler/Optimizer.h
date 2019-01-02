#pragma once
#include "Quad.h"
#include "ElementCreater.h"

using std::unordered_map;

class Optimizer
{
public:
	Optimizer(ElementCreater *elementCreater) : elementCreater(elementCreater) {};
	void optimize(string originalQuads ="", string newQuads = "");
	void printQuad(string filename);
	void deleteGlobal();
	//bool eraseQuads(QuadTable *quadTable);
	Quantity *traceNode(Quantity *quantity);
	Quantity *find(Quantity *quantity);
	//void dagAnalyze(QuadTable *quadTable);
	void dagAnalyze(QuadTable *quadTable);
	//bool dagAnalyze(QuadTable *quadTable);
	unordered_map<string, Quantity *> nameToQuantity;
	//unordered_map<Quantity *, Quantity *> var2var;
private:
	QuadTable *quadTable;
	ElementCreater *elementCreater;
	unordered_map<Quantity *, Quantity*> currentToBefore;
	Function *currentFunction;
};

