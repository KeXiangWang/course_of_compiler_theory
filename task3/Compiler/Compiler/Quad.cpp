#include "pch.h"
#include "Quad.h"
#include "global.h"

int Quad::count = 0;
int QuadTable::count = 0;
int Caculator::caculatorCount = 0;
void QuadTable::addQuad(Quad * quad) {
	if (quad == nullptr) {
		return;
	}
	switch (quad->opCode)
	{
	case OP_LABEL:
		static_cast<Label *>(quad)->labelQuadTable = this;
		break;
	case OP_PLUS:
	case OP_SUB:
	case OP_MULT:
	case OP_DIV: {
		auto caculate = static_cast<Caculator *>(quad);
		addQuad(caculate->quantity1);
		addQuad(caculate->quantity2);
		break;
	}

	case OP_BEQZ: {
		auto branch = static_cast<Branch *>(quad);
		addQuad(branch->quantity1);
		break;
	}

	case OP_BEQ:
	case OP_BGE:
	case OP_BGT:
	case OP_BLE:
	case OP_BLT:
	case OP_BNE: {
		auto branch = static_cast<Branch *>(quad);
		addQuad(branch->quantity1);
		addQuad(branch->quantity2);
		break;
	}

	case OP_CONST:
		return;
	case OP_VAR: {
		auto variable = static_cast<Variable *>(quad);
		if (variable->value != nullptr) {
			addQuad(variable->value);
		}
		break;
	}

	case OP_FUNC: {
		auto function = static_cast<FunctionCall *>(quad);
		for (auto i = function->parameters.begin(); i != function->parameters.end(); i++) {
			addQuad(*i);
		}
		break;
	}

	case OP_VOIDFUNC: {
		auto function = static_cast<VoidCall *>(quad);
		for (auto i = function->parameters.begin(); i != function->parameters.end(); i++) {
			addQuad(*i);
		}
		break;
	}

	case OP_RETURN: {
		auto ret = static_cast<Return *>(quad);
		addQuad(ret->quantity);
		break;
	}

	case OP_PRINTF: {
		auto print = static_cast<Printf *>(quad);
		addQuad(print->quantity);
		break;
	}
	case OP_ARRAY: {
		Array *arr = static_cast<Array*>(quad);
		addQuad(arr->index);
		if (arr->value != nullptr)
			addQuad(arr->value);
		break;
	}
	default:
		break;
	}
	quads.push_back(quad);
}

bool Caculator::equals(Quantity *quantity) const {
	if (quantity->opCode != opCode)
		return false;
	Caculator *caculator = static_cast<Caculator *>(quantity);
	return quantity1->equals(caculator->quantity1) && quantity2->equals(caculator->quantity2);
}

string Caculator::
toString() const
{
	return string();
}

bool Constant::equals(Quantity *quantity) const {
	if (quantity->opCode != opCode)
		return false;
	Constant *constant = static_cast<Constant *>(quantity);
	return dataType == constant->dataType && value == constant->value;
}

string Constant::toString() const
{
	return string();
}

bool Variable::equals(Quantity *quantity) const {
	if (quantity->opCode != opCode)
		return false;
	Variable *variable = static_cast<Variable *>(quantity);
	return dataType == variable->dataType && name == variable->name;
}

string Variable::toString() const
{
	return string();
}

string Quad::toString() const
{
	return string();
}

string Quantity::toString() const
{
	return string();
}

string Array::toString() const
{
	return string();
}

string FunctionCall::toString() const
{
	return string();
}

string VoidCall::toString() const
{
	return string();
}

string Scanf::toString() const
{
	return string();
}

string Printf::toString() const
{
	return string();
}

string Return::toString() const
{
	return string();
}

string Label::toString() const
{
	return string();
}

string Jump::toString() const
{
	return string();
}

string Branch::toString() const
{
	return string();
}
