#include "pch.h"
#include "Optimizer.h"

void Optimizer::optimize() {
	for (auto function = elementCreater->functionTable.functionVector.begin();
		function != elementCreater->functionTable.functionVector.end(); function++) {
		for (auto quadTable = (*function)->quadTableVector.begin();
			quadTable != (*function)->quadTableVector.end(); quadTable++) {
			dagAnalyze((*quadTable));
			std::cout << 1;
		}
	}
}


void Optimizer::deleteNode() {

}

void Optimizer::recordNode() {

}

// Quantity: caculator, Constant, Variable, Array, FunctionCall
Quantity * Optimizer::traceNode(Quantity * quantity) {
	switch (quantity->opCode) {
	case OP_CONST:
		return quantity;
	case OP_VAR: {
		Variable *variable = static_cast<Variable *>(quantity);
		auto pare = currentToBefore.begin();
		for (; pare != currentToBefore.end(); pare++) {
			if (variable->equals((*pare).first)) {
				currentToBefore[quantity] = (*pare).second;
				break;
			}
		}
		if (pare == currentToBefore.end()) {
			currentToBefore[quantity] = variable;
		}
		return currentToBefore[quantity];
	}
	case OP_ARRAY: {
		Array *arr = static_cast<Array *>(quantity);
		auto pare = currentToBefore.begin();
		for (; pare != currentToBefore.end(); pare++) {
			if (arr->equals((*pare).first)) {
				currentToBefore[quantity] = (*pare).second;
				break;
			}
		}
		if (pare == currentToBefore.end()) {
			currentToBefore[quantity] = arr;
		}
		return currentToBefore[quantity];
	}
	case OP_MULT:
	case OP_DIV:
	case OP_PLUS:
	case OP_SUB:
		return currentToBefore[quantity];
	case OP_FUNC:
		return quantity;
	default:
		return quantity;
	}
	return nullptr;
}

Quantity * Optimizer::find(Quantity * quantity) {
	return nullptr;
}

void Optimizer::dagAnalyze(QuadTable *quadTable) {
	for (auto quad = quadTable->quads.begin(); quad != quadTable->quads.end(); quad++) {
		switch ((*quad)->opCode) {
		case OP_MULT:
		case OP_DIV:
		case OP_PLUS:
		case OP_SUB: {
			Caculator *caculator = static_cast<Caculator *>(*quad);
			caculator->quantity1 = traceNode(caculator->quantity1);
			caculator->quantity2 = traceNode(caculator->quantity2);
			auto pare = currentToBefore.begin();
			for (; pare != currentToBefore.end(); pare++) {
				if (caculator->equals((*pare).first)) {
					currentToBefore[caculator] = (*pare).second;
					break;
				}
			}
			if (pare == currentToBefore.end()) {
				currentToBefore[caculator] = caculator;
			}
			break;
		}
		case OP_VAR: {
			Variable *variable = static_cast<Variable *>(*quad);
			variable->value = traceNode(variable->value);
			for (auto pare = currentToBefore.begin(); pare != currentToBefore.end(); pare++) {
				if (variable->equals((*pare).first)) {
					(*pare).second = variable->value;
					break;
				}
			}
			break;
		}
		case OP_ARRAY: {
			Array *arr = static_cast<Array*>(*quad);
			arr->value = traceNode(arr->value);
			arr->index = traceNode(arr->index);
			for (auto pare = currentToBefore.begin(); pare != currentToBefore.end(); pare++) {
				if (arr->equals((*pare).first)) {
					(*pare).second = arr->value;
					break;
				}
			}
			break;
		}
		case OP_FUNC: {
			FunctionCall *func = static_cast<FunctionCall *>(*quad);
			for (auto para = func->parameters.begin(); para != func->parameters.end(); para++) {
				*para = traceNode(*para);
			}
			currentToBefore.clear();
			break;
		}
		case OP_VOIDFUNC: {
			VoidCall *func = static_cast<VoidCall *>(*quad);
			for (auto para = func->parameters.begin(); para != func->parameters.end(); para++) {
				*para = traceNode(*para);
			}
			currentToBefore.clear();
			break;
		}
		}

	}
	for (auto quad = quadTable->quads.begin(); quad != quadTable->quads.end(); quad++) {
		switch ((*quad)->opCode) {
		case OP_MULT:
		case OP_DIV:
		case OP_PLUS:
		case OP_SUB:
			Caculator *caculator = static_cast<Caculator *>(*quad);
			if (currentToBefore[caculator] != caculator) {
				//delete (*quad);
				quad = quadTable->quads.erase(quad);
				quad--;
			}
			break;
		}
	}
}
