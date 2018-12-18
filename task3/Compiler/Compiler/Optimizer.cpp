#include "pch.h"
#include "Optimizer.h"

void Optimizer::optimize() {
	for (auto function = elementCreater->functionTable.functionVector.begin();
		function != elementCreater->functionTable.functionVector.end(); function++) {
		for (auto quadTable = (*function)->quadTableVector.begin();
			quadTable != (*function)->quadTableVector.end(); quadTable++) {
			dagAnalyze((*quadTable));
		}
	}
}


void Optimizer::deleteNode() {

}

void Optimizer::recordNode() {

}


Quantity * Optimizer::traceNode(Quantity * quantity) {
	switch (quantity->opCode) {
	case OP_CONST:
		return quantity;
	//case OP_VAR: 
	//	return currentToBefore[quantity];
	case OP_MULT:
	case OP_DIV:
	case OP_PLUS:
	case OP_SUB:
		return currentToBefore[quantity];
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
			//Variable *variable = static_cast<Variable *>(*quad);
			//auto pare = currentToBefore.begin();
			//for (; pare != currentToBefore.end(); pare++) {
			//	if (variable->equals((*pare).first)) {
			//		currentToBefore[variable] = (*pare).second;
			//		break;
			//	}
			//}
			//if (pare == currentToBefore.end()) {
			//	currentToBefore[variable] = variable;
			//}
			//break;
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
