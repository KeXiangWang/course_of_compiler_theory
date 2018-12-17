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

void Optimizer::dagAnalyze(QuadTable *quadTable) {

}