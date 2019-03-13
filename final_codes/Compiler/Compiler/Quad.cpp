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
		else {
			return;
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
		if (arr->value != nullptr) {
			addQuad(arr->value);
		}
		else { // TODO check this
			return;
		}
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
	//if(quantity1->opCode!=OP_CONST)
	return quantity1->equals(caculator->quantity1) && quantity2->equals(caculator->quantity2);
}

bool Constant::equals(Quantity *quantity) const {
	if (quantity->opCode != opCode)
		return false;
	Constant *constant = static_cast<Constant *>(quantity);
	return dataType == constant->dataType && value == constant->value;
}

bool Variable::equals(Quantity *quantity) const {
	if (quantity->opCode != opCode)
		return false;
	Variable *variable = static_cast<Variable *>(quantity);
	return dataType == variable->dataType && name == variable->name;
}

bool Array::equals(Quantity * quantity) const { // TODO test if expression convert to int, whether dag ok
	if (quantity->opCode != opCode)
		return false;
	Array *arr = static_cast<Array *>(quantity);
	return dataType == arr->dataType && index->equals(arr->index);
}

string getStandardQuad(string s1, string s2 = "", string s3 = "", string s4 = "") {
	return s1 + " \t" + s2 + " \t" + s3 + " \t" + s4;
}

// Quantity: caculator, Constant, Variable, Array, FunctionCall
string Caculator::toString() const {
	if (quantity1 == nullptr || quantity2 == nullptr) {
		return "Wrong Caculator";
	}
	return getStandardQuad(id, quantity1->id, quantity2->id);
}

string Constant::toString() const {
	return getStandardQuad(id, "=", std::to_string(value));
}

string Variable::toString() const {
	if (value == nullptr) {
		return getStandardQuad("PARSED", "VARIABLE", name);
	}
	else {
		return getStandardQuad("VAR_" + id, "=", value->id);
	}
}

string Array::toString() const {
	if (value == nullptr) {
		return getStandardQuad("PARSED", "ARRAY", name, index->id);
	}
	else {
		return getStandardQuad("ARR_" + id, "[" + index->id + "]", "=", value->id);
	}
}

string FunctionCall::toString() const {
	string s = "";
	s = s + "FUNCCALL \t" + id + " (";
	for (auto para = parameters.begin(); para != parameters.end(); para++) {
		s += (*para)->id + ",";
	}
	s += ")";
	return s;
}

string Quad::toString() const {
	return id;
}

string Quantity::toString() const {
	return id;
}

string VoidCall::toString() const {
	string s = "";
	s = s + "VOIDCALL \t" + name + " (";
	for (auto para = parameters.begin(); para != parameters.end(); para++) {
		s += (*para)->id + ",";
	}
	s += ")";
	return s;
}

string Scanf::toString() const {
	string s = "";
	s = s + "SCANF \t(";
	for (auto para = parameters.begin(); para != parameters.end(); para++) {
		s = s + (*para)->id + ",";
	}
	s = s + ")";
	return s;
}

string Printf::toString() const {
	if (quantity == nullptr) {
		return getStandardQuad("PRINTF", stringPrintf);
	}
	else {
		return getStandardQuad("PRINTF", stringPrintf, quantity->id);
	}
}

string Return::toString() const {
	if (quantity != nullptr) {
		return getStandardQuad("RETURN", quantity->id);
	}
	else {
		return getStandardQuad("RETURN Nothing");
	}
}

string Label::toString() const {
	return id;
}

string Jump::toString() const {
	return "JUMPTO \t" + label->toString();
}

string Branch::toString() const {
	if (quantity1 == nullptr) {
		return "Wrong Branch!!!";
	}
	switch (opCode)
	{
	case OP_BEQZ:
		return getStandardQuad("BEQZ", quantity1->id, label->toString());
	case OP_BGE:
		return getStandardQuad("BGE", quantity1->id, quantity2->id, label->toString());
	case OP_BGT:
		return getStandardQuad("BGT", quantity1->id, quantity2->id, label->toString());
	case OP_BEQ:
		return getStandardQuad("BEQ", quantity1->id, quantity2->id, label->toString());
	case OP_BLE:
		return getStandardQuad("BLE", quantity1->id, quantity2->id, label->toString());
	case OP_BLT:
		return getStandardQuad("BLT", quantity1->id, quantity2->id, label->toString());
	case OP_BNE:
		return getStandardQuad("BNE", quantity1->id, quantity2->id, label->toString());
	}
	return "Wrong Branch!!!";
}

