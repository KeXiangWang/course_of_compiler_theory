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

bool Quantity::equals(Quantity *quantity) const {
	return opCode == quantity->opCode;
}

bool Caculator::equals(Quantity *quantity) const {
	if (quantity->opCode != opCode)
		return false;
	Caculator *caculator = static_cast<Caculator *>(quantity);
}

bool Constant::equals(Quantity *quantity) const {
	if (quantity->opCode != opCode)
		return false;
	Constant *constant = static_cast<Constant *>(quantity);
}

bool Variable::equals(Quantity *quantity) const {
	if (quantity->opCode != opCode)
		return false;
}

bool Array::equals(Quantity *quantity) const {
	if (quantity->opCode != opCode)
		return false;
}

bool FunctionCall::equals(Quantity *quantity) const {
	if (quantity->opCode != opCode)
		return false;
}
