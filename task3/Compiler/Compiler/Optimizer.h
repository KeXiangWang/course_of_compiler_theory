#pragma once
#include "Quad.h"
#include "ElementCreater.h"

using std::unordered_map;

class Optimizer
{
public:
	Optimizer(ElementCreater *elementCreater) : elementCreater(elementCreater) {};
	void optimize();
	void deleteNode();
	void recordNode();
	Quantity *traceNode(Quantity *quantity);
	Quantity *find(Quantity *quantity);
	void dagAnalyze(QuadTable *quadTable);
private:
	QuadTable *quadTable;
	ElementCreater *elementCreater;
	unordered_map<Quantity *, Quantity*> currentToBefore;
};

