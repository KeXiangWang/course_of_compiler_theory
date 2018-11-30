#include "pch.h"
#include "Quad.h"
#include "global.h"

int Quad::count = 0;
int QuadTable::count = 0;
void QuadTable::addQuad(Quad * quad) {
	if (quad == nullptr) {
		return;
	}
	switch (quad->opCode)
	{
	case OP_LABEL:
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

	case OP_BEQZ:
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

	default:
		break;
	}
	quads.push_back(quad);
}
