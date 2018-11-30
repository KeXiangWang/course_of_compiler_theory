#pragma once
#include "Quad.h"
#include "ElementCreater.h"

using std::string;
using std::vector;

class MipsGenerator
{
public:
	MipsGenerator(ElementCreater *elementCreater) :elementCreater(elementCreater) {};
	void generate();

	void midcode2asm();
	void insertaddress();
	void pushstack();
	void funcasm();
	int varaddr();
	void dataseg();
	void jmpasm();
	void printint();
	void callasm();
	void setlabasm();
	void addasm();
	void subasm();
	void mulasm();
	void divasm();
	void greasm();
	void geqasm();
	void lssasm();
	void leqasm();
	void eqlasm();
	void neqasm();
	void assasm();
	void aassasm();
	void assaasm();
	void scfasm();
	void prtasm();
	void fupaasm();
	void retasm();
	void paraasm();
	void jneasm();
	void intcharasm();
	void constdefasm();
	void intcharaasm();

private:
	QuadTable *quadTable;
	ElementCreater *elementCreater;
	vector<string> finalCode;
};

