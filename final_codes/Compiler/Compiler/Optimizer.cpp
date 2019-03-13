#include "pch.h"
#include "Optimizer.h"

bool forDagOptimize = (bool)1;

void Optimizer::optimize(string originalQuads, string newQuads) {
	if (originalQuads != "") {
		printQuad(originalQuads);
	}
	if (forDagOptimize) {
		for (auto function = elementCreater->functionTable.functionVector.begin();
			function != elementCreater->functionTable.functionVector.end(); function++) {
			currentFunction = *function;
			for (auto quadTable = (*function)->quadTableVector.begin();
				quadTable != (*function)->quadTableVector.end(); quadTable++) {
				currentToBefore.clear();
				dagAnalyze((*quadTable));
				//std::cout << 1;
			}
		}
	}
	if (newQuads != "") {
		printQuad(newQuads);
	}
}

void Optimizer::printQuad(string filename) {
	auto out = std::fstream(filename, std::fstream::out);
	for (auto function = elementCreater->functionTable.functionVector.begin();
		function != elementCreater->functionTable.functionVector.end(); function++) {
		out << "Funtion: " << (*function)->name << std::endl;
		for (auto quadTable = (*function)->quadTableVector.begin();
			quadTable != (*function)->quadTableVector.end(); quadTable++) {
			out << "BasicBlock: " << std::endl;
			for (auto quad = (*quadTable)->quads.begin(); quad != (*quadTable)->quads.end(); quad++) {
				out << "\t" << (*quad)->toString() << std::endl;
			}
		}
	}
}

void Optimizer::deleteGlobal() {
	auto pare = currentToBefore.begin();
	while (pare != currentToBefore.end()) {
		if ((*pare).second->opCode == OP_VAR) {
			Variable * var = static_cast<Variable *>((*pare).first);
			if (currentFunction->elementTable.find(var->name) == nullptr) { // judge global
				pare = currentToBefore.erase(pare);
				continue;
			}
		}
		pare++;
	}

}
//
//void Optimizer::recordNode() {
//
//}

Quantity * Optimizer::find(Quantity * quantity) {
	return nullptr;
}


Quantity * Optimizer::traceNode(Quantity * quantity) {
	switch (quantity->opCode) {
	case OP_VAR: {
		auto var = static_cast<Variable *>(quantity);
		if (nameToQuantity.find(var->name) == nameToQuantity.end())
			nameToQuantity[var->name] = var;
		return nameToQuantity[var->name];
	}
	case OP_PLUS:
	case OP_SUB:
	case OP_MULT:
	case OP_DIV:
		return currentToBefore[quantity];
	case OP_FUNC:
		return quantity;
	case OP_ARRAY:{
		auto arr = static_cast<Array *>(quantity);
		arr->index = traceNode(arr->index);
		return quantity;
	}
	default:
		return quantity;
	};
}

void Optimizer::dagAnalyze(QuadTable *quadTable) {
	currentToBefore.clear();
	nameToQuantity.clear();
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
			if (variable->value->opCode != OP_ARRAY) {
				nameToQuantity[variable->name] = variable->value;
			}
			break;
		}
		case OP_ARRAY: {
			Array *arr = static_cast<Array*>(*quad);
			arr->value = traceNode(arr->value);
			arr->index = traceNode(arr->index);
			break;
		}
		case OP_FUNC: {
			FunctionCall *func = static_cast<FunctionCall *>(*quad);
			for (auto para = func->parameters.begin(); para != func->parameters.end(); para++) {
				*para = traceNode(*para);
			}
			//deleteGlobal();
			//currentToBefore.clear();
			nameToQuantity.clear();
			break;
		}
		case OP_VOIDFUNC: {
			VoidCall *func = static_cast<VoidCall *>(*quad);
			for (auto para = func->parameters.begin(); para != func->parameters.end(); para++) {
				*para = traceNode(*para);
			}
			//deleteGlobal();
			//currentToBefore.clear();
			nameToQuantity.clear();
			break;
		}
		case OP_BEQ:
		case OP_BEQZ:
		case OP_BGE:
		case OP_BGT:
		case OP_BLE:
		case OP_BLT:
		case OP_BNE: {
			auto branch = static_cast<Branch *>(*quad);
			if (branch->quantity1 != nullptr)
				branch->quantity1 = traceNode(branch->quantity1);
			if (branch->quantity2 != nullptr)
				branch->quantity2 = traceNode(branch->quantity2);
			break;
		}
		case OP_RETURN:	{
			auto ret = static_cast<Return *>(*quad);
			if (ret->quantity != nullptr)
				ret->quantity = traceNode(ret->quantity);
			break;
		}
		case OP_PRINTF:	{
			auto prin = static_cast<Printf*>(*quad);
			if (prin->quantity != nullptr)
				prin->quantity = traceNode(prin->quantity);
			break;
		}
		}

	}
	auto quad = quadTable->quads.begin();
	while (quad != quadTable->quads.end()) {
		switch ((*quad)->opCode) {
		case OP_MULT:
		case OP_DIV:
		case OP_PLUS:
		case OP_SUB: {
			Caculator *caculator = static_cast<Caculator *>(*quad);
			if (currentToBefore[caculator] != caculator) {
				//delete (*quad);
				if (currentToBefore[caculator] != nullptr) {
					std::cout << "fucking deleted " ;
					std::cout << (*quad)->toString() << std::endl;
					quad = quadTable->quads.erase(quad);
					break;
				}
			}
			quad++;
			break;
		}
		default: {
			quad++;
			break;
		}
		}
	}
}
