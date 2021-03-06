#include "pch.h"
#include "MipsGenerator.h"

using std::to_string;
using std::map;

void MipsGenerator::generate() {
	initData();
	initRegs();
	finalCode.push_back(".text");
	finalCode.push_back("j f_main");
	//finalCode.push_back("nop");
	finalCode.push_back("\n");
	for (auto function = elementCreater->functionTable.functionVector.begin();
		function != elementCreater->functionTable.functionVector.end(); function++) {
		initCode.clear();
		exertCode.clear();
		stackOffset.clear();
		refCount.clear();
		generateFunction(*function);
		finalCode.insert(finalCode.end(), initCode.begin(), initCode.end());
		finalCode.insert(finalCode.end(), exertCode.begin(), exertCode.end());
	}
}

void MipsGenerator::initData() {
	finalCode.push_back(".data");
	for (auto element = elementCreater->globalTable.elementVector.begin();
		element != elementCreater->globalTable.elementVector.end(); element++) {
		switch ((*element)->kind)
		{
		case KINDCONST:
			finalCode.push_back("global_" + (*element)->name + ": .word " + to_string((long long)(*element)->value));
			break;
		case KINDARRAY:
			finalCode.push_back("global_" + (*element)->name + ": .space " + std::to_string((long long)(4 * (*element)->value)));
			break;
		case KINDVAR:
			finalCode.push_back("global_" + (*element)->name + ": .word 0");
			break;
		default:
			break;
		}
		globalElements.push_back((*element)->name);
	}
	for (auto strInt = 0; strInt != elementCreater->constStringVector.size(); strInt++) {
		finalCode.push_back("string_" + std::to_string((long long)strInt) +
			": .asciiz  \"" + elementCreater->constStringVector[strInt] + "\"");
	}
}

void MipsGenerator::initRegs() {
	for (int i = 0; i < 8; i++) {
		usedTempRegs.push_back(Reg("$t" + to_string(i)));
	}
}

void MipsGenerator::generateFunction(Function * function) {
	storeRegs.clear();
	tempRegs.clear();
	globalVariableCount = 0;
	int offset = getVarNumber(function);
	allocStoreRegs(function);
	initCode.push_back("\n");
	initCode.push_back("f_" + function->name + ":");
	vector<TableElement *> &elementVector = function->elementTable.elementVector;

	initCode.push_back("addiu $sp $sp -" + to_string(offset));
	if (function->name != "main") {
		initCode.push_back("sw $ra " + to_string(-4 + offset) + "($sp)");
		initCode.push_back("sw $fp " + to_string(-8 + offset) + "($sp)");
		for (int i = 0; i < 8 ; i++) {
			if (optimizeGlobalRegs) {
				if (i >= globalVariableCount) {
					continue;
				}
			}
			initCode.push_back("sw $s" + to_string(i) + " " + to_string(-12 - (i << 2) + offset) + "($sp)");
		}
		// store $a0~a3
		for (int i = 0; i < (int)(function->parameters.size()); i++) {
			int tmp = (i << 2) + offset + (8 << 2);
			if (i < 4) {
				initCode.push_back("sw $a" + to_string(i) + " " + to_string(tmp) + "($sp)");
			}
			stackOffset[function->parameters[i]->name] = tmp;
		}
	}
	// init const
	for (auto i = elementVector.begin(); i != elementVector.end(); i++) {
		if ((*i)->kind == KINDCONST) {
			if (storeRegs.find((*i)->name) != storeRegs.end()) {
				loadedToStore.insert((*i)->name);
				std::cout << "unreachable" << std::endl;
				//assert(5 == 0);
				initCode.push_back("li " + storeRegs[(*i)->name]->name + " " + to_string((long long)((*i)->value)));
			}
			else {
				initCode.push_back("li $t0 " + to_string((*i)->value));
				initCode.push_back("sw $t0 " + to_string(stackOffset[(*i)->name]) + "($sp)");
			}
		}
	}
	for (auto quadTable = function->quadTableVector.begin(); quadTable != function->quadTableVector.end(); quadTable++) {
		if (quadTableToLabel.find(*quadTable) == quadTableToLabel.end()) {
			quadTableToLabel[*quadTable] = labelCount++;
		}
		exertCode.push_back("label_" + to_string(quadTableToLabel[*quadTable]) + ":"); // each quadTable set a label
		vector<Quad *> *quads = &(*quadTable)->quads; // 

		for (auto quad = quads->begin(); quad != quads->end(); quad++) {
			switch ((*quad)->opCode)
			{
			case OP_PLUS:
			case OP_SUB: // tested
				generateAddSub(function, (*quad), (*quad)->opCode);
				break;
			case OP_MULT:
			case OP_DIV: // tested
				generateMultDiv(function, (*quad), (*quad)->opCode);
				break;
			case OP_VAR: // tested 
				generateVar(function, (*quad));
				break;
			case OP_ARRAY:
				generateArray(function, (*quad));
				break;
			case OP_BEQZ:
				generateBranchZ(function, (*quad));
				break;
			case OP_BEQ:
				generateBranch(function, (*quad), "beq ");
				break;
			case OP_BGE:
				generateBranch(function, (*quad), "bge ");
				break;
			case OP_BGT:
				generateBranch(function, (*quad), "bgt ");
				break;
			case OP_BLE:
				generateBranch(function, (*quad), "ble ");
				break;
			case OP_BLT:
				generateBranch(function, (*quad), "blt ");
				break;
			case OP_BNE:
				generateBranch(function, (*quad), "bne ");
				break;
			case OP_JUMP:
				generateJump(function, (*quad));
				break;
			case OP_RETURN:
				generateReturn(function, (*quad));
				break;
			case OP_FUNC:
				generateFunc(function, (*quad), offset);
				break;
			case OP_VOIDFUNC:
				generateVoidFunc(function, (*quad), offset);
				break;
			case OP_SCANF: // tested
				generateScanf(function, (*quad));
				break;
			case OP_PRINTF: // tested
				generatePrintf(function, (*quad));
				break;
			default:
				break;
			}
			if ((*quad)->opCode == OP_RETURN) {
				break;
			}
		}
		if (!(/*function->name == "main" &&*/(quadTable + 1 == function->quadTableVector.end()))) {
			writeBack(function);
		}
		if (seperateCodeByBlock) std::cout << "# end a basicBlock " << std::endl;
	}
	exertCode.push_back("f_" + function->name + "_return:");
	if (function->name != "main") {
		// load $s0~$s7
		for (int i = 0; i < 8; i++) {
			if (optimizeGlobalRegs) {
				if (i >= globalVariableCount) {
					continue;
				}
			}
			exertCode.push_back("lw $s" + to_string(i) + " " + to_string(-12 - (i << 2) + offset) + "($sp)");
		}
		// load $sp %fp
		exertCode.push_back("lw $ra " + to_string(-4 + offset) + "($sp)");
		exertCode.push_back("lw $fp " + to_string(-8 + offset) + "($sp)");
		exertCode.push_back("addiu $sp $sp " + to_string(offset));

		// return
		exertCode.push_back("jr $ra");
		//exertCode.push_back("nop");
	}
}

void MipsGenerator::generateAddSub(Function *function, Quad *quad, OPCode opCode) {
	Caculator *caculator = static_cast<Caculator *>(quad);
	if (caculator->quantity1->opCode == OP_CONST) {
		Constant *quantity1 = static_cast<Constant*>(caculator->quantity1);
		if (caculator->quantity2->opCode == OP_CONST) { // constant optimize : n = 3 + 2
			Constant *quantity2 = static_cast<Constant*>(caculator->quantity2);
			string reg0 = getReg(function, caculator, true) + " ";
			if (opCode == OP_PLUS)
				exertCode.push_back("li " + reg0 + to_string(quantity1->value + quantity2->value) + " " + "\t# " + caculator->id);
			else
				exertCode.push_back("li " + reg0 + to_string(quantity1->value - quantity2->value) + " " + "\t# " + caculator->id);
			return;
		}
		string reg2 = getReg(function, caculator->quantity2) + " "; // constant optimize : n = 2 + n
		string reg0 = getReg(function, caculator, true) + " ";
		if (opCode == OP_PLUS) {
			exertCode.push_back("addiu " + reg0 + reg2 + to_string(quantity1->value) + " " + "\t# " + caculator->id);
		}
		else {
			exertCode.push_back("subu " + reg0 + "$0 " + reg2 + "\t# -" + caculator->quantity2->id);
			if (quantity1->value != 0) {
				exertCode.push_back("addiu " + reg0 + reg0 + to_string(quantity1->value) + " " + "\t# " + caculator->id);
			}
		}
	}
	else {
		if (caculator->quantity2->opCode == OP_CONST) { // constant optimize: n = n + 2
			Constant *quantity2 = static_cast<Constant*>(caculator->quantity2);
			string reg1 = getReg(function, caculator->quantity1) + " ";
			string reg0 = getReg(function, caculator, true) + " ";
			if (opCode == OP_PLUS)
				exertCode.push_back("addiu " + reg0 + reg1 + to_string(quantity2->value) + " " + "\t# " + caculator->id);
			else
				exertCode.push_back("subiu " + reg0 + reg1 + to_string(quantity2->value) + " " + "\t# " + caculator->id);
			return;
		}
		string reg1 = getReg(function, caculator->quantity1) + " ";
		usedTempRegs[reg1[2] - '0'].fixed = true;
		string reg2 = getReg(function, caculator->quantity2) + " ";
		usedTempRegs[reg1[2] - '0'].fixed = false;
		string reg0 = getReg(function, caculator, true) + " ";
		if (opCode == OP_PLUS)
			exertCode.push_back("addu " + reg0 + reg1 + reg2 + "\t# " + caculator->id);
		else
			exertCode.push_back("subu " + reg0 + reg1 + reg2 + "\t# " + caculator->id);
	}
}

void MipsGenerator::generateMultDiv(Function *function, Quad *quad, OPCode opCode) {
	Caculator *caculator = static_cast<Caculator *>(quad);
	if (caculator->quantity1->opCode == OP_CONST) {
		Constant *quantity1 = static_cast<Constant*>(caculator->quantity1);
		if (caculator->quantity2->opCode == OP_CONST) { // constant optimize : n = 3 + 2
			Constant *quantity2 = static_cast<Constant*>(caculator->quantity2);
			string reg0 = getReg(function, caculator, true) + " ";
			if (opCode == OP_MULT)
				exertCode.push_back("li " + reg0 + to_string((long long)quantity1->value * (long long)quantity2->value) + " " + "\t# " + caculator->id);
			else
				exertCode.push_back("li " + reg0 + to_string((long long)quantity1->value / (long long)quantity2->value) + " " + "\t# " + caculator->id);
			return;
		}
	}
	string reg1 = getReg(function, caculator->quantity1) + " ";
	usedTempRegs[reg1[2] - '0'].fixed = true;
	string reg2 = getReg(function, caculator->quantity2) + " ";
	usedTempRegs[reg1[2] - '0'].fixed = false;
	string reg0 = getReg(function, caculator, true) + " ";;
	if (opCode == OP_MULT)
		exertCode.push_back("mul " + reg0 + reg1 + reg2 + "\t# " + caculator->id);
	else
		exertCode.push_back("div " + reg0 + reg1 + reg2 + "\t# " + caculator->id);
}

void MipsGenerator::generateVar(Function *function, Quad *quad) {
	Variable *variable = static_cast<Variable *>(quad);
	if (variable->value != nullptr) {
		if (variable->value->opCode == OP_CONST && function->elementTable.find(variable->name) != nullptr) {
			string lReg = getReg(function, variable, true);
			Constant *value = static_cast<Constant*>(variable->value);
			exertCode.push_back("li " + lReg + " " + to_string((long long)value->value) + "\t# " + variable->id);
			return;
		}
		string rReg = getReg(function, variable->value);
		if (function->elementTable.find(variable->name) != nullptr) {
			string lReg = getReg(function, variable, true);
			if (rReg != lReg)
				exertCode.push_back("move " + lReg + " " + rReg + "\t# " + variable->id);
		}
		else {
			exertCode.push_back("\t# " + variable->id); // global
			storeValue(function, variable, rReg);
			clearGlobalInTempRegs(function);
		}
	}
}

void MipsGenerator::generateArray(Function *function, Quad *quad) {
	Array *arr = static_cast<Array *>(quad);
	if (arr->value != nullptr) {
		string reg = getReg(function, arr->value);
		//exertCode.push_back("# " + arr->name);
		//storeValueArray(function, arr, reg, "$t1");
		string instr = (arr->dataType == TYPEINT) ? "sw " : "sb ";
		string name = arr->name;
		Quantity *offset = arr->index;
		string offReg = "$t8 ";
		moveToReg(function, offset, offReg);
		if (function->elementTable.find(name) != nullptr) {
			exertCode.push_back("sll " + offReg + offReg + to_string(2));											  // reg is address offset
			exertCode.push_back("addu " + offReg + offReg + "$sp");													  // reg = $sp + address offset
			exertCode.push_back(instr + reg + " " + to_string(stackOffset[name]) + "(" + offReg + ")"); // reg += base address
		}
		else {
			exertCode.push_back("sll " + offReg + offReg + to_string(2));
			exertCode.push_back(instr + reg + " global_" + name + "(" + offReg + ")");
		}
	}
}

void MipsGenerator::generateBranch(Function *function, Quad *quad, string branchName) {
	Branch *branch = static_cast<Branch *>(quad);
	string reg1 = getReg(function, branch->quantity1) + " ";
	usedTempRegs[reg1[2] - '0'].fixed = true;
	string reg2 = getReg(function, branch->quantity2) + " ";
	usedTempRegs[reg1[2] - '0'].fixed = false;
	writeBack(function);
	if (quadTableToLabel.find(branch->label->labelQuadTable) != quadTableToLabel.end()) {
		exertCode.push_back(branchName + reg1 + reg2 + "label_" + to_string(quadTableToLabel[branch->label->labelQuadTable]));
	}
	else {
		quadTableToLabel[branch->label->labelQuadTable] = labelCount;
		exertCode.push_back(branchName + reg1 + reg2 + "label_" + to_string(labelCount));
		labelCount++;
	}
}

void MipsGenerator::generateBranchZ(Function *function, Quad *quad) {
	Branch *branch = static_cast<Branch *>(quad);
	string reg = getReg(function, branch->quantity1) + " ";
	writeBack(function);
	if (quadTableToLabel.find(branch->label->labelQuadTable) != quadTableToLabel.end()) {
		exertCode.push_back("beqz " + reg + " label_" + to_string(quadTableToLabel[branch->label->labelQuadTable]));
	}
	else {
		quadTableToLabel[branch->label->labelQuadTable] = labelCount;
		exertCode.push_back("beqz " + reg + " label_" + to_string(labelCount));
		labelCount++;
	}
}

void MipsGenerator::generateJump(Function *function, Quad *quad) {
	Jump *jump = static_cast<Jump *>(quad);
	writeBack(function);
	if (quadTableToLabel.find(jump->label->labelQuadTable) != quadTableToLabel.end()) {
		exertCode.push_back("j label_" + to_string(quadTableToLabel[jump->label->labelQuadTable]));
	}
	else {
		quadTableToLabel[jump->label->labelQuadTable] = labelCount;
		exertCode.push_back("j label_" + to_string(labelCount));
		labelCount++;
	}
}

void MipsGenerator::generateReturn(Function *function, Quad *quad) {
	Return *ret = static_cast<Return *>(quad);
	if (ret->quantity != nullptr) {
		moveToReg(function, ret->quantity, "$v0");
	}
	exertCode.push_back("j f_" + function->name + "_return");
	//exertCode.push_back("nop");
	writeBack(function);
}

void MipsGenerator::generateFunc(Function *function, Quad *quad, int offset) {
	if (changeGlobal) writeBack(function);
	FunctionCall *func = static_cast<FunctionCall *>(quad);
	int paraSize4 = func->parameters.size() << 2;
	exertCode.push_back("# call func: " + func->name);
	if (paraSize4 > 0) exertCode.push_back("addiu $sp $sp -" + to_string(paraSize4));
	for (int i = 0; i < static_cast<int>(func->parameters.size()); i++) {
		if (i < 4) { // front 4 para to $a0-3
			moveToReg(function, func->parameters[i], "$a" + to_string(i), paraSize4);
		}
		else { // para more than 4 to ram
			moveToReg(function, func->parameters[i], "$t8", paraSize4);
			exertCode.push_back("sw $t8 " + to_string(i << 2) + "($sp)");
		}
	}
	exertCode.push_back("addiu $sp $sp -32"); // save for $t
	if (!optimizeTempRegs) {
		for (int i = 0; i < 8; i++) {
			exertCode.push_back("sw $t" + to_string(i) + " " + to_string(i << 2) + "($sp)");
		}
	}
	exertCode.push_back("jal f_" + func->name);
	if (!optimizeTempRegs) {
		for (int i = 0; i < 8; i++) {
			exertCode.push_back("lw $t" + to_string(i) + " " + to_string(i << 2) + "($sp)");
		}
	}
	exertCode.push_back("addiu $sp $sp 32");
	if (paraSize4 > 0) exertCode.push_back("addiu $sp $sp " + to_string(paraSize4));
	for (int i = 0; i < (int)(function->parameters.size()) && i < 4; i++) {
		int tmp = (i << 2) + offset + (8 << 2);
		exertCode.push_back("lw $a" + to_string(i) + " " + to_string(tmp) + "($sp)");
	}
	string reg = getReg(function, func, true);
	exertCode.push_back("move " + reg + " $v0");
	exertCode.push_back("# back from func: " + func->name);
	clearGlobalInTempRegs(function);
}

void MipsGenerator::generateVoidFunc(Function *function, Quad *quad, int offset) {
	if (changeGlobal) writeBack(function);
	VoidCall *func = static_cast<VoidCall *>(quad);
	int paraSize4 = func->parameters.size() << 2;
	exertCode.push_back("# call voidfunc: " + func->name);
	if (paraSize4 > 0) exertCode.push_back("addiu $sp $sp -" + to_string(paraSize4));
	for (int i = 0; i < static_cast<int>(func->parameters.size()); i++) {
		if (i < 4) { // front 4 para to $a0-3
			moveToReg(function, func->parameters[i], "$a" + to_string(i), paraSize4);
		}
		else { // para more than 4 to ram
			moveToReg(function, func->parameters[i], "$t8", paraSize4);
			exertCode.push_back("sw $t8 " + to_string(i << 2) + "($sp)");
		}
	}
	exertCode.push_back("addiu $sp $sp -32");
	if (!optimizeTempRegs) {
		for (int i = 0; i < 8; i++) {
			exertCode.push_back("sw $t" + to_string(i) + " " + to_string(i << 2) + "($sp)");
		}
	}
	exertCode.push_back("jal f_" + func->name);
	if (!optimizeTempRegs) {
		for (int i = 0; i < 8; i++) {
			exertCode.push_back("lw $t" + to_string(i) + " " + to_string(i << 2) + "($sp)");
		}
	}

	exertCode.push_back("addiu $sp $sp 32");
	if (paraSize4 > 0) exertCode.push_back("addiu $sp $sp " + to_string(paraSize4));
	for (int i = 0; i < (int)(function->parameters.size()) && i < 4; i++) {
		int tmp = (i << 2) + offset + (8 << 2);
		exertCode.push_back("lw $a" + to_string(i) + " " + to_string(tmp) + "($sp)");
	}
	exertCode.push_back("# back from voidfunc: " + func->name);
	clearGlobalInTempRegs(function);
}

void MipsGenerator::generateScanf(Function *function, Quad *quad) {
	Scanf * scan = static_cast<Scanf *>(quad);
	for (auto parameter = scan->parameters.begin(); parameter != scan->parameters.end(); parameter++) {
		if ((*parameter)->dataType == TYPEINT) {
			exertCode.push_back("li $v0 5");
			exertCode.push_back("syscall");
			if (function->elementTable.find((*parameter)->name) != nullptr) {
				string reg = getReg(function, *parameter, true);
				exertCode.push_back("move " + reg + " $v0");
			}
			else {
				storeValue(function, *parameter, "$v0");
			}
		}
		else if ((*parameter)->dataType == TYPECHAR) {
			exertCode.push_back("li $v0 12");
			exertCode.push_back("syscall");
			if (function->elementTable.find((*parameter)->name) != nullptr) {
				string reg = getReg(function, *parameter, true);
				exertCode.push_back("move " + reg + " $v0");
			}
			else {
				storeValue(function, *parameter, "$v0");
			}
		}
	}
}

void MipsGenerator::generatePrintf(Function *function, Quad *quad) {
	Printf * prin = static_cast<Printf *>(quad);
	if (prin->stringInt >= 0) {
		exertCode.push_back("li $v0 4  # printf string");
		exertCode.push_back("la $a0 string_" + to_string((long long)prin->stringInt));
		exertCode.push_back("syscall");
	}
	if (prin->quantity != nullptr) {
		if (prin->quantity->dataType == TYPEINT) {
			exertCode.push_back("li $v0 1  # printf int");
			moveToReg(function, prin->quantity, "$a0");
			exertCode.push_back("syscall");
		}
		else if (prin->quantity->dataType == TYPECHAR) {
			exertCode.push_back("li $v0 11  # printf char");
			moveToReg(function, prin->quantity, "$a0");
			exertCode.push_back("syscall");
		}
	}
	//exertCode.push_back("li $v0 11  # char");
	//exertCode.push_back("li $a0 10");
	//exertCode.push_back("syscall");
}

string MipsGenerator::getReg(Function *function, Quantity *quantity, bool write, int temp) {
	string id = quantity->id;
	if (storeRegs.find(id) != storeRegs.end()) { // from $s
		string reg = storeRegs[id]->name;
		if (loadedToStore.find(id) == loadedToStore.end()) {
			loadedToStore.insert(id);
			TableElement *e = function->elementTable.find(id);
			if (e != nullptr && e->kind == KINDPARA) {
				loadValueGlobal(function, quantity, reg, temp);
			}
			else {
				if (!write) {
					loadValueGlobal(function, quantity, reg, temp);
				}
			}
		}
		return reg;
	}
	else if (tempRegs.find(id) != tempRegs.end()) {	// from $t
		return tempRegs[id]->name;
	}
	else { // allocate a reg from $t
		Reg *reg = overflow(function);
		reg->quantity = quantity;
		reg->free = false;
		tempRegs[id] = reg;
		if (!write)
			loadValue(function, quantity, reg->name, temp);
		return reg->name;
	}
}

Reg * MipsGenerator::overflow(Function *function) {
	int max = 0;
	Quantity* maxValue = nullptr;
	Reg *maxIter = nullptr;
	for (auto reg = usedTempRegs.begin(); reg != usedTempRegs.end(); reg++) {
		if ((*reg).free) {
			(*reg).free = false;
			return &(*reg);
		}
		Quantity *pre_value = (*reg).quantity;
		int ref = refCount[pre_value->id];
		if (ref == 0) {
			tempRegs.erase(pre_value->id);
			(*reg).free = true;
			return &(*reg);
		}
		if ((*reg).fixed == false && ref > max) {
			max = ref;
			maxValue = pre_value;
			maxIter = &(*reg);
		}
	}
	storeValue(function, maxValue, maxIter->name);
	tempRegs.erase(maxValue->id);
	return maxIter;
}

bool cmp(std::pair<string, int> &a, std::pair<string, int> &b) {
	return a.second > b.second;
}
void MipsGenerator::allocStoreRegs(Function * function) {
	vector<std::pair<string, int>> vec;
	for (auto var = refCount.begin(); var != refCount.end(); var++)
		vec.emplace_back(std::make_pair(var->first, var->second));
	std::sort(vec.begin(), vec.end(), cmp);

	for (int i = 0, regNum = 0; regNum < 8 && i < (int)(vec.size()); i++) {
		TableElement *element = function->elementTable.find(vec[i].first);
		if (element == nullptr)
			continue;
		if (element->kind == KINDPARA)
			continue;
		storeRegs[vec[i].first] = new Reg("$s" + to_string(regNum));
		regNum++;
		// globalVariableCount = regNum; // TODO  check this
	}
}

int MipsGenerator::getVarNumber(Function * function) {
	int offset = 10 * 4;
	for (auto i = function->elementTable.elementVector.begin(); i != function->elementTable.elementVector.end(); i++) {
		switch ((*i)->kind)
		{
		case KINDCONST:
		case KINDVAR:
		case KINDPARA:
			offset += 4;
			stackOffset[(*i)->name] = offset;
			globalVariableCount++;
			break;
		case KINDARRAY:
			offset += ((*i)->value << 2) + 4;
			stackOffset[(*i)->name] = offset;
			globalVariableCount++;
			globalVariableCount += (*i)->value;
			break;
		}
	}
	//for (QuadTable *quadTable = function->headQuadTable; quadTable != nullptr; quadTable = quadTable->next) {
		//for (auto quad = quadTable->quads.begin(); quad != quadTable->quads.end(); quad++) {
	for (auto quadTable = function->quadTableVector.begin(); quadTable != function->quadTableVector.end(); quadTable++) {
		for (auto quad = (*quadTable)->quads.begin(); quad != (*quadTable)->quads.end(); quad++) {
			switch ((*quad)->opCode) {
			case OP_PLUS:
			case OP_SUB:
			case OP_MULT:
			case OP_DIV: {
				auto ca = static_cast<Caculator*>(*quad);
				refCount[ca->quantity1->id]++;
				refCount[ca->quantity2->id]++;
				offset += 4;
				stackOffset[(*quad)->id] = offset;
				globalVariableCount++;
				break;
			}
			case OP_FUNC: {
				auto func = static_cast<FunctionCall*>(*quad);
				for (auto i = func->parameters.begin(); i != func->parameters.end(); i++)
					refCount[(*i)->id]++;
				offset += 4;
				stackOffset[(*quad)->id] = offset;
				globalVariableCount++;
				break;
			}
			case OP_VOIDFUNC: {
				auto func = static_cast<VoidCall*>(*quad);
				for (auto i = func->parameters.begin(); i != func->parameters.end(); i++)
					refCount[(*i)->id]++;
				break;
			}
			case OP_CONST: {
				auto con = static_cast<Constant*>(*quad);
				refCount[con->id]++;
				break;
			}
			case OP_VAR: {
				auto var = static_cast<Variable *>(*quad);
				refCount[var->id]++;
				if (var->value != nullptr)
					refCount[var->value->id]++;
				break;
			}
			case OP_ARRAY: {
				auto arr = static_cast<Array *>(*quad);
				refCount[arr->index->id]++;
				if (arr->value != nullptr)
					refCount[arr->value->id]++;
				break;
			}
			case OP_BEQZ: {
				auto branch = static_cast<Branch *>(*quad);
				refCount[branch->quantity1->id]++;
				break;
			}
			case OP_BEQ:
			case OP_BNE:
			case OP_BLE:
			case OP_BGE:
			case OP_BGT:
			case OP_BLT: {
				auto branch = static_cast<Branch *>(*quad);
				refCount[branch->quantity1->id]++;
				refCount[branch->quantity2->id]++;
				break;
			}
			case OP_PRINTF: {
				auto prin = static_cast<Printf*>(*quad);
				if (prin->quantity != nullptr)
					refCount[prin->quantity->id]++;
				break;
			}
			case OP_RETURN: {
				auto ret = static_cast<Return*>(*quad);
				if (ret->quantity != nullptr)
					refCount[ret->quantity->id]++;
				break;
			}
			}
		}
	}
	for (auto i = stackOffset.begin(); i != stackOffset.end(); i++)
		(*i).second = offset - (*i).second;
	return offset;
}

void MipsGenerator::writeBack(Function * function) {
	if (seperateCodeByBlock) exertCode.push_back("# writeBack");
	for (auto i = usedTempRegs.begin(); i != usedTempRegs.end(); i++) {
		auto& reg = *i;
		if (!reg.free && refCount[reg.quantity->id] > 0) {
			tempRegs.erase(reg.quantity->id);
			storeValue(function, reg.quantity, reg.name);
			reg.free = true;
		}
	}
	if (seperateCodeByBlock) exertCode.push_back("# end writeBack");
}

void MipsGenerator::clearGlobalInTempRegs(Function * function) { // TODO finish
	for (auto i = usedTempRegs.begin(); i != usedTempRegs.end(); i++) {
		if (!(*i).free) {
			auto foundGlobalElement = find(globalElements.begin(), globalElements.end(), (*i).quantity->id);
			if (foundGlobalElement != globalElements.end() && function->elementTable.find((*i).quantity->id) == nullptr) {
				tempRegs.erase((*i).quantity->id);
				(*i).free = true;
				std::cout << "delate global ! " << std::endl;
			}
		}
	}
}

void MipsGenerator::loadValue(Function *function, Quad *quad, string reg, int temp) {
	if (reg.back() != ' ')
		reg += ' ';
	string instr = (static_cast<Quantity *>(quad)->dataType == TYPEINT) ? "lw " : "lb ";
	if (quad->opCode == OP_CONST) {
		exertCode.push_back("li " + reg + to_string((long long)static_cast<Constant *>(quad)->value) + "\t#" + quad->id);
	}
	else if (quad->opCode == OP_VAR) {
		string name = static_cast<Variable *>(quad)->name;
		if (function->elementTable.find(name) != nullptr) { // local var
			exertCode.push_back(instr + reg + to_string((long long)stackOffset[name] + temp) + "($sp)" + "\t#" + name);
		}
		else { // global var
			exertCode.push_back(instr + reg + "global_" + name + "\t#" + name);
		}
	}
	else if (quad->opCode == OP_ARRAY) {
		string name = static_cast<Array *>(quad)->name;
		Quantity *offset = static_cast<Array *>(quad)->index;
		if (offset->opCode != OP_ARRAY) {
			auto offReg = getReg(function, offset, false, temp);
			moveToReg(function, offset, reg);
		}
		else {
			loadValue(function, offset, reg, temp);
		}
		if (function->elementTable.find(name) != nullptr) {
			exertCode.push_back("sll " + reg + reg + to_string(2ll));											// reg is address offset
			exertCode.push_back("addu " + reg + reg + "$sp");													// reg = $fp + address offset
			exertCode.push_back(instr + reg + to_string((long long)stackOffset[name] + temp) + "(" + reg + ")"); // reg += base address
		}
		else {
			exertCode.push_back("sll " + reg + reg + to_string(2ll));
			exertCode.push_back(instr + reg + "global_" + name + "(" + reg + ")");
		}
	}
	else {
		exertCode.push_back(instr + reg + to_string((long long)(stackOffset[quad->id] + temp)) + "($sp)" + "\t#" + quad->id);
	}
}

void MipsGenerator::loadValueGlobal(Function *function, Quad *quad, string reg, int temp) {
	if (reg.back() != ' ')
		reg += ' ';
	string instr = (static_cast<Quantity *>(quad)->dataType == TYPEINT) ? "lw " : "lb ";
	if (quad->opCode == OP_CONST) {
		initCode.push_back("li " + reg + to_string((long long)static_cast<Constant *>(quad)->value) + "\t#" + quad->id);
	}
	else if (quad->opCode == OP_VAR) {
		string name = static_cast<Variable *>(quad)->name;
		if (function->elementTable.find(name) != nullptr) // local var
			initCode.push_back(instr + reg + to_string(stackOffset[name] + temp) + "($sp)" + "\t#" + name);
		else //global var
			initCode.push_back(instr + reg + "global_" + name + "\t#" + name);
	}
	else if (quad->opCode == OP_ARRAY) {
		string name = static_cast<Array *>(quad)->name;
		Quantity *offset = static_cast<Array *>(quad)->index;
		if (offset->opCode != OP_ARRAY) {
			auto offReg = getReg(function, offset, false, temp);
			moveToReg(function, offset, reg);
		}
		else {
			loadValue(function, offset, reg, temp);
		}
		if (function->elementTable.find(name) != nullptr) {
			initCode.push_back("sll " + reg + reg + to_string(2ll));									// reg is address offset
			initCode.push_back("addu " + reg + reg + "$sp");											// reg = $fp + address offset
			initCode.push_back(instr + reg + to_string(stackOffset[name] + temp) + "(" + reg + ")");	// reg += base address
		}
		else {
			initCode.push_back("sll " + reg + reg + to_string(2ll));
			initCode.push_back(instr + reg + "global_" + name + "(" + reg + ")");
		}
	}
	else {
		initCode.push_back(instr + reg + to_string((long long)(stackOffset[quad->id] + temp)) + "($sp)" + "\t#" + quad->id);
	}
}

void MipsGenerator::storeValue(Function *function, Quad *quad, string reg) {
	if (reg.back() != ' ')
		reg += ' ';
	string instr = (static_cast<Quantity *>(quad)->dataType == TYPEINT) ? "sw " : "sb ";
	if (quad->opCode == OP_VAR) {
		string name = static_cast<Variable *>(quad)->name;
		if (function->elementTable.find(name) != nullptr) {
			exertCode.push_back(instr + reg + to_string((long long)stackOffset[name]) + "($sp)" + "\t# " + name);
		}
		else {
			exertCode.push_back(instr + reg + " global_" + name + "\t#" + name);
		}
	}
	else if (quad->opCode != OP_ARRAY && quad->opCode != OP_CONST) {
		exertCode.push_back(instr + reg + to_string((long long)stackOffset[quad->id]) + "($sp)" + "\t# " + quad->id);
	}
	else {
		//std::cout << quad->opCode << "why" << std::endl;
	}
}

void MipsGenerator::moveToReg(Function *function, Quantity *quantity, string reg, int temp) {
	string id = quantity->id;
	if (storeRegs.find(id) != storeRegs.end()) {
		//string greg = " " + storeRegs[id]->name;
		string greg = storeRegs[id]->name;
		if (loadedToStore.find(id) == loadedToStore.end()) { // more than $8
			loadedToStore.insert(id);
			loadValueGlobal(function, quantity, greg, temp);
		}
		exertCode.push_back("move " + reg + " " + greg);
	}
	else if (tempRegs.find(id) != tempRegs.end()) {
		exertCode.push_back("move " + reg + " " + tempRegs[id]->name);
	}
	else {
		loadValue(function, quantity, reg, temp);
	}
}

void MipsGenerator::printCode(std::fstream &output) {
	for (auto i = finalCode.begin(); i != finalCode.end(); i++) {
		output << *i << std::endl;
	}
}
