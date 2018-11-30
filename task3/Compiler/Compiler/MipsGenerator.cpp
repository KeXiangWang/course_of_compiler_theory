#include "pch.h"
#include "MipsGenerator.h"

using std::to_string;

void MipsGenerator::generate() {
	finalCode.push_back(".text");
	finalCode.push_back("j f_main");
	finalCode.push_back("nop");
	Function *function = *elementCreater->functionTable.functionVector.begin();
	for (auto i = elementCreater->functionTable.functionVector.begin(); 
		i != elementCreater->functionTable.functionVector.end(); i++) {
		initCode.clear();
		exertCode.clear();
		generateFunction(*i);
	}
}

void MipsGenerator::generateFunction(Function * function) {
	int offset = getVarNumber(function);
	allocateGloabal(function);
	TempReg.clear();
	initCode.push_back("\nf_" + function->name + ":");
	vector<TableElement *> elementVector = function->elementTable.elementVector;

	initCode.push_back("addiu $sp $sp -" + to_string((long long)offset));
	initCode.push_back("sw $ra " + to_string((long long)(-4 + offset)) + "($sp)");
	initCode.push_back("sw $fp " + to_string((long long)(-8 + offset)) + "($sp)");
	for (int i = 0; i < 8; i++)
		initCode.push_back("sw $s" + to_string((long long)i) + " " + to_string((long long)(-12 - (i << 2) + offset)) + "($sp)");
	// store $a0~a3
	for (int i = 0; i < function->parameters.size(); i++)
	{
		int tmp = (i << 2) + offset + (8 << 2);
		if (i < 4)
			initCode.push_back("sw $a" + to_string((long long)i) + " " + to_string((long long)tmp) + "($sp)");
		stackOffset[function->parameters[i]->name] = tmp;
	}
	// initialize const
	for (auto i = elementVector.begin(); i != elementVector.end(); i++) {
		if ((*i)->kind == KINDCONST) {
			if (GlobalReg.find((*i)->name) != GlobalReg.end()) {
				loadedToGloabal.insert((*i)->name);
				initCode.push_back("li $s" + to_string((long long)GlobalReg[(*i)->name]) + " " + to_string((long long)(*i)->value));
			}
			else {
				initCode.push_back("li $t0 " + to_string((long long)(*i)->value));
				initCode.push_back("sw $t0 " + to_string((long long)stackOffset[(*i)->name]) + "($sp)");
			}
		}
	}

	// init global
	for (auto quadTable = function->headQuadTable; quadTable != nullptr; quadTable = quadTable->next)
	{
		if (bb2label.find(quadTable) == bb2label.end())
			bb2label[quadTable] = labelCount++;
		exertCode.push_back("label_" + to_string((long long)bb2label[quadTable]) + ":");
		auto &quads = quadTable->quads;
		for (auto quad = quads.begin(); quad != quads.end(); quad++)
		{
			switch ((*quad)->opCode )
			{
			default:
				break;
			}
		}
	}
}
typedef std::pair<string, int> V;
bool cmp(V &a, V &b)
{
	return a.second > b.second;
}
void MipsGenerator::allocateGloabal(Function * function) {
	int globalRegNum = 8;
	vector<V> v;
	for (auto var = refCount.begin(); var != refCount.end(); var++)
		v.emplace_back(std::make_pair(var->first, var->second));
	std::sort(v.begin(), v.end(), cmp);
	GlobalReg.clear();
	for (int i = 0, rest = globalRegNum; rest > 0 && i < v.size(); i++)
	{
		auto element = function->elementTable.find(v[i].first);
		if (element == nullptr)
			continue;
		if (element->kind == KINDPARA)
			continue;
		GlobalReg[v[i].first] = 8 - rest;
		rest--;
	}
}

int MipsGenerator::getVarNumber(Function * function) {
	stackOffset.clear();
	refCount.clear();
	int offset = 10 * 4;
	for (auto i = function->elementTable.elementVector.begin(); i != function->elementTable.elementVector.end(); i++) {
		switch ((*i)->kind)
		{
		case KINDCONST:
		case KINDVAR:
		case KINDPARA:
			stackOffset[(*i)->name] = offset;
			offset += 4;
			break;
		case KINDARRAY:
			stackOffset[(*i)->name] = offset + ((*i)->value << 2) - 4;
			offset += (*i)->value << 2;
			break;
		}
	}
	for (auto quadTable = function->headQuadTable; quadTable == nullptr; quadTable = quadTable->next) {
		for (auto quad = quadTable->quads.begin(); quad != quadTable->quads.end(); quad++) {
			switch ((*quad)->opCode) {
			case OP_PLUS:
			case OP_SUB:
			case OP_MULT:
			case OP_DIV: {
				auto ca = static_cast<Caculator*>(*quad);
				refCount[ca->quantity1->id]++;
				refCount[ca->quantity2->id]++;
				stackOffset[(*quad)->id] = offset;
				offset += 4;
				break;
			}
			case OP_FUNC: {
				auto func = static_cast<FunctionCall*>(*quad);
				for (auto i = func->parameters.begin(); i != func->parameters.end(); i++)
					refCount[(*i)->id]++;
				stackOffset[(*quad)->id] = offset;
				offset += 4;
				break;
			}
			case OP_VOIDFUNC: {
				auto func = static_cast<VoidCall*>(*quad);
				for (auto i = func->parameters.begin(); i != func->parameters.end(); i++)
					refCount[(*i)->id]++;
				break;
			}
			case OP_CONST: {
				auto op = static_cast<Constant*>(*quad);
				refCount[op->id]++;
				break;
			}
			case OP_VAR: {
				auto op = static_cast<Variable *>(*quad);
				refCount[op->id]++;
				if (op->value != nullptr)
					refCount[op->value->id]++;
				break;
			}
			case OP_ARRAY: {
				auto op = static_cast<Array *>(*quad);
				refCount[op->index->id]++;
				if (op->value != nullptr)
					refCount[op->value->id]++;
				break;
			}
			case OP_BEQ:
			case OP_BEQZ:
			case OP_BNE:
			case OP_BLE:
			case OP_BGE:
			case OP_BGT:
			case OP_BLT: {
				auto op = static_cast<Branch *>(*quad);
				if (op->quantity1 != nullptr)
					refCount[op->quantity1->id]++;
				if (op->quantity2 != nullptr)
					refCount[op->quantity2->id]++;
				break;
			}
			case OP_PRINTF: {
				auto op = static_cast<Printf*>(*quad);
				if (op->quantity != nullptr)
					refCount[op->quantity->id]++;
				break;
			}
			case OP_RETURN: {
				auto op = static_cast<Return*>(*quad);
				if (op->quantity != nullptr)
					refCount[op->quantity->id]++;
				break;
			}
			}
		}
	}
	offset -= 4;
	for (auto i = stackOffset.begin(); i != stackOffset.end(); i++)
		(*i).second = offset - (*i).second;
	return offset + 4;
}