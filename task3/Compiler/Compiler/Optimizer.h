#pragma once
#include "Quad.h"
#include "ElementCreater.h"

class Optimizer
{
public:
	Optimizer(ElementCreater *elementCreater) : elementCreater(elementCreater) {};
	void optimize();
	void deleteNode();
	void dagAnalyze();
private:
	QuadTable *quadTable;
	ElementCreater *elementCreater;
};

