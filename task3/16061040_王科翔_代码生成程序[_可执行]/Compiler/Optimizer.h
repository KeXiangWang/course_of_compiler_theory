#pragma once
#include "Quad.h"

class Optimizer
{
public:
	Optimizer();
	~Optimizer();
	void optimize(Quad quad);
	void deleteNode();
	void dagAnalyze();
private:
	QuadTable *quadTable;

};

