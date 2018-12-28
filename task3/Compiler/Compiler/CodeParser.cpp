#include "pch.h"
#include "CodeParser.h"
#include "Quad.h"
#include "ErrorHandler.h"
#include "Lexicon.h"
#include "ElementCreater.h"

using std::cout;
using std::endl;
using std::vector;
using std::to_string;

bool forParametersSequenceRequirement = true;


int CodeParser::shadows = 0;
Quantity *CodeParser::parseExpression() {
	if(printDetail) cout << "parse an expression " << endl;
	Quantity *quantity;
	OPCode opCode = OP_PLUS;
	bool start = true;
	if (token == PLUS || token == MINUS) {
		// add sign
		opCode = token == PLUS ? OP_PLUS : OP_SUB;
		token = lexicon.nextToken();
	}
	quantity = parseTerm();
	if (quantity == nullptr) {
		jumpToToken(SEMICOLON);
		token = lexicon.nextToken();
		return nullptr;
	}
	if (opCode == OP_SUB) { // deal with -+
		quantity = new Caculator(opCode, new Constant(TYPEINT, 0), quantity);
	}
	while (token == PLUS || token == MINUS) {
		opCode = token == PLUS ? OP_PLUS : OP_SUB;
		token = lexicon.nextToken();
		Quantity *quantity_next = parseTerm();
		if (quantity_next == nullptr) {
			jumpToToken(SEMICOLON);
			token = lexicon.nextToken();
			return nullptr;
		}
		quantity = new Caculator(opCode, quantity, quantity_next); // optimizer
	}
	return quantity;
}

Quantity *CodeParser::parseTerm() {
	//cout << "parse a term " << endl;
	Quantity *quantity;
	OPCode opCode = OP_MULT;
	quantity = parseFactor();
	if (quantity == nullptr) {
		jumpToToken(SEMICOLON);
		token = lexicon.nextToken();
		return nullptr;
	}
	while (token == MULT || token == DIV) {
		opCode = token == MULT ? OP_MULT : OP_DIV;
		token = lexicon.nextToken();
		Quantity *quantity_next = parseFactor();
		if (quantity_next == nullptr) {
			jumpToToken(SEMICOLON);
			token = lexicon.nextToken();
			return nullptr;
		}
		quantity = new Caculator(opCode, quantity, quantity_next); // optimizer
	}
	return quantity;

}

Quantity *CodeParser::parseFactor() {
	//cout << "parse a factor " << endl;
	sign = 1;
	int number;
	char alpha;
	Quantity *quantity;
	switch (token)
	{
	case IDENT:
		identifier = lexicon.getStringWord();
		token = lexicon.nextToken();
		if (token == LPARENTHESE) { // deal with int/char func
			Function *function;
			string functionName = identifier;

			if ((function = elementCreater.findFunc(identifier)) == nullptr) {
				reportAndJumpOver(WRONG_FUNCTION_CALL, RPARENTHESE);
				return nullptr;
			}
			if (function->functionType == TYPEVOID) {
				reportAndJumpOver(WRONG_FUNCTION_CALL, RPARENTHESE);
				return nullptr;
			}
			// make a parameter list
			vector<Quantity *> parameters;
			
			do {
				token = lexicon.nextToken();
				if (token == RPARENTHESE) {
					break;
				}
				Quantity *parameter = parseExpression();
				if (parameter == nullptr) {
					return nullptr;
				}
				if (forParametersSequenceRequirement) {
					TableElement *te;
					if ((te = elementCreater.findElementFromGlobal(parameter->id)) != nullptr && parameter->opCode == OP_VAR && parameter->opCode == OP_ARRAY) {
						if (te->kind != KINDCONST) {
							if (!elementCreater.createVar(te->dataType, "shadow_" + parameter->id + "_" + to_string(shadows))) { // creat var
								errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), WRONG_ARGUMENT_LIST, true);
							}
							Variable *var = new Variable(te->dataType, "shadow_" + parameter->id + "_" + to_string(shadows), parameter);
							parameter = var;
							shadows++;
						}
					}
				}
				// add to the parameter list
				parameters.push_back(parameter);
			} while (token == COMMA);
			if (token != RPARENTHESE) {
				return nullptr;
			}
			if (function->parameters.size() != parameters.size()) {
				errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), WRONG_ARGUMENT_LIST);
			}
			for (auto i = 0; i < (int)parameters.size(); i++) { // check type of args
				if (parameters[i]->dataType != function->parameters[i]->dataType) {
					errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), WRONG_ARGUMENT_LIST, true);
				}
			}
			quantity = new FunctionCall(function->functionType, functionName, parameters);
			token = lexicon.nextToken();
			if (printDetail) cout << "call a function " << functionName << endl;
			return quantity;
		}
		else { // not calling a funtion
			TableElement * tableElement;
			if ((tableElement = elementCreater.findElement(identifier)) == nullptr) {
				errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNDEFINED_IDENTIFIER);
				return nullptr;
			}
			if (tableElement->kind == KINDARRAY) {
				if (token == LBRACKET) {
					string arrayName = identifier; // deletable
					token = lexicon.nextToken();
					Quantity *index = parseExpression();
					if (index == nullptr) {
						return nullptr;
					}
					if (index->opCode == OP_CONST) {
						Constant * constant = static_cast<Constant *>(index);
						if (constant->value >= tableElement->value) {
							errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), ARRAY_OVERFLOW);
						}
					}
					if (index->opCode == OP_VAR) {
						Variable * variable = static_cast<Variable *>(index);
						TableElement *te = elementCreater.findElement(variable->name);
						if (te->kind == KINDCONST) {
							if (te->value >= tableElement->value) {
								errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), ARRAY_OVERFLOW);
							}
						}
					}
					if (token != RBRACKET) {
						errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), RIGHT_BRACKET_EXPECTED);
						return nullptr;
					}
					token = lexicon.nextToken();
					quantity = new Array(tableElement->dataType, tableElement->name, index);
					return quantity;
				}
				else {
					errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNDEFINED_IDENTIFIER);
					return nullptr;
				}
			}
			else { // TODO simplify if KINDCONST
				string varName = identifier;
				quantity = new Variable(tableElement->dataType, tableElement->name); // creat var;
				return quantity;
			}
		}
		break;
	case LPARENTHESE:
		token = lexicon.nextToken();
		quantity = parseExpression();
		// temp for debug
		if (quantity->dataType == TYPECHAR) {
			quantity->dataType = TYPEINT;
			cout << "Transfer to int compulsively because of the PARENTHESE" << endl;
		}
		if (token != RPARENTHESE) {
			return nullptr;
		}
		else {
			token = lexicon.nextToken();
			return quantity;
		}
		break;
	case MINUS:
		sign = -1;
	case PLUS:
		token = lexicon.nextToken();
		if (token != NUM) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), NUMBER_EXPECTED);
			return nullptr;
		}
	case NUM:
		number = lexicon.getNumberTemp();
		token = lexicon.nextToken();
		return new Constant(TYPEINT, sign * number);
	case ALPHA:
		alpha = lexicon.getNumberTemp();
		token = lexicon.nextToken();
		return new Constant(TYPECHAR, alpha);
	}
	return nullptr;
}

void CodeParser::parseStatement() {
	switch (token)
	{
	case SEMICOLON:
		if (printDetail) cout << "encounter a blank statement " << endl;
		token = lexicon.nextToken();
		break;
	case LBRACE:
		if (printDetail) cout << "enter a statement sqquence" << endl;
		token = lexicon.nextToken();
		parseStatementSequence();
		if (token != RBRACE) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), RIGHT_BRACE_EXPECTED);
		}
		token = lexicon.nextToken();
		break;
	case IFSYM:
		parseIf();
		break;
	case DOSYM:
		parseDoWhile();
		break;
	case FORSYM:
		parseFor();
		break;
	case RETURNSYM:
		parseReturn();
		break;
	case SCANFSYM:
		parseScanf();
		break;
	case PRINTFSYM:
		parsePrintf();
		break;
	case IDENT:
		identifier = lexicon.getStringWord();
		token = lexicon.nextToken();
		TableElement *tableElement;
		Function *function;
		if ((tableElement = elementCreater.findElement(identifier)) != nullptr) {
			if (tableElement->kind == KINDCONST) {
				errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNEXPECTED_ASSIGNMENT);
			}
			Quantity *index = nullptr;
			if (token == LBRACKET) { // parse array
				token = lexicon.nextToken();
				if (tableElement->kind != KINDARRAY) {
					reportAndJumpOver(UNDEFINED_IDENTIFIER, SEMICOLON);
					return;
				}
				index = parseExpression();
				if (index == nullptr) {
					jumpToToken(SEMICOLON);
					token = lexicon.nextToken();
				}
				if (index->opCode == OP_CONST) {
					Constant * constant = static_cast<Constant *>(index);
					if (constant->value >= tableElement->value || constant->value < 0) {
						errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), ARRAY_OVERFLOW);
					}
				}
				if (index->opCode == OP_VAR) {
					Variable * variable = static_cast<Variable *>(index);
					TableElement *te = elementCreater.findElement(variable->name);
					if (te->kind == KINDCONST) {
						if (te->value >= tableElement->value || te->value < 0) {
							errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), ARRAY_OVERFLOW);
						}
					}
				}
				if (token != RBRACKET) {
					reportAndJumpOver(RIGHT_BRACKET_EXPECTED, SEMICOLON);
					return;
				}
				token = lexicon.nextToken();
			}
			if (token == BECOME) { // parse become
				if (printDetail) cout << "assign a value to ident " << identifier << endl;
				token = lexicon.nextToken();
				Quantity *quantity;
				quantity = parseExpression();
				if (quantity == nullptr) {
					jumpToToken(SEMICOLON);
					token = lexicon.nextToken();
				}
				if (quantity->dataType != tableElement->dataType) { // match assignment
					errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), WRONG_ASSIGNMENT_TYPE, true);
				}
				if (token != SEMICOLON) {
					errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
				}
				token = lexicon.nextToken();
				if (tableElement->kind == KINDARRAY) {
					elementCreater.actStatement(new Array(tableElement->dataType, tableElement->name, index, quantity)); // array[index] = quantity
				}
				else {
					elementCreater.actStatement(new Variable(tableElement->dataType, tableElement->name, quantity));
				}
			}
			else { // not become
				reportAndJumpOver(UNDEFINED_IDENTIFIER, SEMICOLON);
				return;
			}
		}
		else if ((function = elementCreater.findFunc(identifier)) != nullptr) {		
			if (token == LPARENTHESE) { // function and voidfunc
				if (printDetail) cout << "use a self-defined function " << endl;
				vector<Quantity *> parameters;
				if (function->withParameters) {
					Quantity *parameter;
					do {
						token = lexicon.nextToken();
						if (token == RPARENTHESE) {
							break;
						}
						parameter = parseExpression();
						if (parameter == nullptr) {
							jumpToToken(SEMICOLON);
						}
						if (forParametersSequenceRequirement) {
							TableElement *te;
							if ((te = elementCreater.findElementFromGlobal(parameter->id)) != nullptr && parameter->opCode == OP_VAR && parameter->opCode == OP_ARRAY) {
								if (te->kind != KINDCONST) {
									if (!elementCreater.createVar(te->dataType, "shadow_" + parameter->id + "_" + to_string(shadows))) { // creat var
										errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), WRONG_ARGUMENT_LIST, true);
									}
									Variable *var = new Variable(te->dataType, "shadow_" + parameter->id + "_" + to_string(shadows), parameter);
									parameter = var;
									shadows++;
								}
							}
						}
						parameters.push_back(parameter);
					} while (token == COMMA);
				}
				else {
					token = lexicon.nextToken();
				}
				if (token != RPARENTHESE) {
					reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, SEMICOLON);
					return;
				}
				if (parameters.size() != function->parameters.size()) { // check number of args
					reportAndJumpOver(WRONG_ARGUMENT_LIST, SEMICOLON);
					return;
				}
				for (auto i = 0; i < (int)parameters.size(); i++) { // check type of args
					if (parameters[i]->dataType != function->parameters[i]->dataType) {
						reportAndJumpOver(WRONG_ARGUMENT_LIST, SEMICOLON);
						return;
					}
				}
				token = lexicon.nextToken();
				if (token != SEMICOLON) {
					errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
				}
				token = lexicon.nextToken();
				if (function->functionType == TYPEVOID) {
					elementCreater.actStatement(new VoidCall(function->name, parameters));
				}
				else {
					elementCreater.actStatement(new FunctionCall(function->functionType, function->name, parameters));
				}
			}
		}
		else {
			reportAndJumpOver(UNDEFINED_IDENTIFIER, SEMICOLON);
			return;
		}
		break;
	default:
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNEXPECTED_SIGN);
		jumpToSet(statementHeadSet);
		break;
	}
}

void CodeParser::parseStatementSequence() {  // tested
	while (statementHeadSet.find(token) != statementHeadSet.end()) {
		parseStatement();
	}
}

void CodeParser::parseCompoundStatement() // 符合语句 // tested
{
	while (token == CONSTSYM) {
		parseConstDeclare();
	}
	while (token == INTSYM || token == CHARSYM) {
		dataType = token == INTSYM ? TYPEINT : TYPECHAR;
		token = lexicon.nextToken();
		if (token == IDENT) {
			identifier = lexicon.getStringWord();
			token = lexicon.nextToken();
			parseVarDeclare();
		}
		else {
			token = reportAndJumpOver(IDENTIFIER_EXPECTED, SEMICOLON);
		}
	}
	parseStatementSequence();
}

void CodeParser::parseFunction() {
	gotFunction = true;
	if (!elementCreater.createFunc(dataType, identifier)) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), IDENTIFIER_ALREADY_DEFINED);
	}
	if (printDetail) cout << "creat a function " << identifier << endl;
	token = lexicon.nextToken();
	if (identifier == "main") {
		gotMain = true;
		if (token != RPARENTHESE) {
			token = reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, RPARENTHESE);
		}
		else {
			token = lexicon.nextToken();
		}
	}
	else { // only main mustn't have arguements
		while (1) {
			if (token == INTSYM || token == CHARSYM) {
				dataType = token == INTSYM ? TYPEINT : TYPECHAR;
				token = lexicon.nextToken();
				if (token == IDENT) {
					identifier = lexicon.getStringWord();
					token = lexicon.nextToken();
					if (!elementCreater.createPara(dataType, identifier)) { // creat arguement 
						errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), IDENTIFIER_ALREADY_DEFINED);
					}
					if (printDetail) cout << "creat a " << dataType << " arguement " << identifier << endl;
					if (token == COMMA) {  // token = ,
						token = lexicon.nextToken();
						continue; // token = idnet
					}
					else if (token == RPARENTHESE) {  // token = )
						token = lexicon.nextToken();
						break; // token = {
					}
					else {
						token = reportAndJumpOver(IDENTIFIER_EXPECTED, RPARENTHESE);
						break; // token = {
					}
				}
				else {
					token = reportAndJumpOver(IDENTIFIER_EXPECTED, RPARENTHESE);
					break; // token = {
				}
			}
			else if (token == RPARENTHESE) {
				token = lexicon.nextToken();
				break;
			}
			else {
				errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNEXPECTED_SIGN);
				jumpToToken(RPARENTHESE);
				break;
			}

		}
	}
	if (token == LBRACE) {
		token = lexicon.nextToken();
		parseCompoundStatement();
		if (token == RBRACE) {
			token = lexicon.nextToken();
		}
		else {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNEXPECTED_SIGN); // not }
		}
	}
	else { // not {
		token = reportAndJumpOver(LEFT_BRACE_EXPECTED, RBRACE);
	}
}

Token CodeParser::parseCondition(Quantity **quantity1, Quantity **quantity2) {
	cout << "cope with condition " << endl;
	*quantity1 = parseExpression();
	Token compareToken;
	if (*quantity1 == nullptr) {
		return VOIDSYM;
	}
	cout << "quantity1 type:" << ((*quantity1)->dataType == TYPEINT ? "int" : "char") << endl;;
	if ((*quantity1)->dataType != TYPEINT) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), WRONG_QUANTITY_TYPE, true);
	}
	if (compareSet.find(token) == compareSet.end() ) {
		if (token == RPARENTHESE) {
			return NULLSYM;
		}
		else {
			return BECOME;
		}
	}
	TableElement *te;
	if ((te = elementCreater.findElementFromGlobal((*quantity1)->id)) != nullptr && (*quantity1)->opCode == OP_VAR && (*quantity1)->opCode == OP_ARRAY) {
		if (te->kind != KINDCONST) {
			if (!elementCreater.createVar(te->dataType, "shadow_" + (*quantity1)->id + "_" + to_string(shadows))) { // creat var
				errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), WRONG_ARGUMENT_LIST, true);
			}
			Variable *var = new Variable(te->dataType, "shadow_" + (*quantity1)->id + "_" + to_string(shadows), (*quantity1));
			*quantity1 = var;
			shadows++;
		}
	}
	compareToken = token;
	token = lexicon.nextToken();
	*quantity2 = parseExpression();
	// set a compare 
	if (*quantity2 == nullptr) {
		return VOIDSYM;
	}
	cout << "quantity2 type:" << ((*quantity2)->dataType == TYPEINT ? "int" : "char") << endl;;
	if ((*quantity2)->dataType != TYPEINT) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), WRONG_QUANTITY_TYPE, true);
	}
	return compareToken;
}

void CodeParser::parseConstDeclare() {
	if (token != CONSTSYM) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNEXPECTED_SIGN);
		jumpToToken(SEMICOLON);
		token = lexicon.nextToken();
	}
	token = lexicon.nextToken();
	if (token == INTSYM || token == CHARSYM) {
		dataType = token == INTSYM ? TYPEINT : TYPECHAR;
		do {
			token = lexicon.nextToken();
			if (token == IDENT) {
				identifier = lexicon.getStringWord();
				token = lexicon.nextToken();
				if (token == BECOME) {
					token = lexicon.nextToken();
					sign = 1;
					gotSign = false;
					if (dataType == TYPEINT) {
						if (token == PLUS) {
							gotSign = true;
							token = lexicon.nextToken();
						}
						else if (token == MINUS) {
							sign = -1;
							gotSign = true;
							token = lexicon.nextToken();
						}
						if (token == NUM) {
							valueTemp = lexicon.getNumberTemp();
							if (!elementCreater.createConst(dataType, identifier, sign*valueTemp)) { // creat const
								token = reportAndJumpOver(IDENTIFIER_ALREADY_DEFINED, SEMICOLON);
								return;
							}
							if (printDetail) cout << "creat a const int" << endl;
							token = lexicon.nextToken();
						}
						else {
							token = reportAndJumpOver(NUMBER_EXPECTED, SEMICOLON);
							return;
						}
					}
					else {
						if (token == ALPHA) {
							valueTemp = lexicon.getNumberTemp(); // get the char as int
							if (!elementCreater.createConst(dataType, identifier, valueTemp)) { // creat const
								token = reportAndJumpOver(IDENTIFIER_ALREADY_DEFINED, SEMICOLON);
								return;
							}
							if (printDetail) cout << "creat a const char" << endl;
							token = lexicon.nextToken();
						}
						else {
							token = reportAndJumpOver(ALPHA_EXPECTED, SEMICOLON);
							return;
						}
					}
				}
				else {
					token = reportAndJumpOver(BECOME_EXPECTED, SEMICOLON);
					return;
				}
			}
			else {
				token = reportAndJumpOver(IDENTIFIER_EXPECTED, SEMICOLON);
				return;
			}
		} while (token == COMMA);
		if (token != SEMICOLON) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
			return;
		}
		token = lexicon.nextToken();
	}
	else {
		token = reportAndJumpOver(TYPE_OF_IDENTIFIER_EXPECTED, SEMICOLON);
		return;
	}
}

void CodeParser::parseVarDeclare() {
	if (token == LBRACKET) {
		token = lexicon.nextToken();
		if (token == NUM) {
			arraySize = lexicon.getNumberTemp();
			token = lexicon.nextToken();
			if (token == RBRACKET) {
				if (!elementCreater.createArray(dataType, identifier, arraySize)) { // creat Array
					token = reportAndJumpOver(IDENTIFIER_ALREADY_DEFINED, SEMICOLON);
					return;
				}
				if (printDetail) cout << "creat a array " << identifier << endl;
				token = lexicon.nextToken();
				if (token == COMMA) {
					token = lexicon.nextToken();
					if (token == IDENT) {
						identifier = lexicon.getStringWord();
						token = lexicon.nextToken();
						parseVarDeclare(); // possible: [ , ;
					}
					else {
						token = reportAndJumpOver(IDENTIFIER_EXPECTED, SEMICOLON);
					}
				}
				else if (token == SEMICOLON) {
					token = lexicon.nextToken();
				}
				else {
					token = reportAndJumpOver(UNEXPECTED_SIGN, SEMICOLON);
					return;
				}
			}
			else {
				token = reportAndJumpOver(RIGHT_BRACE_EXPECTED, SEMICOLON); // without ]
				return;
			}
		}
		else {
			token = reportAndJumpOver(NONAGTIVE_INT_EXPECTED, SEMICOLON); // nagetive size of other things between[]
			return;
		}
	}
	else if (token == COMMA) {
		if (!elementCreater.createVar(dataType, identifier)) { // creat var
			token = reportAndJumpOver(IDENTIFIER_ALREADY_DEFINED, SEMICOLON);
			return;
		}
		if (printDetail) cout << "creat a variable with comma" << identifier << endl;
		token = lexicon.nextToken();
		if (token != IDENT) {
			token = reportAndJumpOver(IDENTIFIER_EXPECTED, SEMICOLON);
		}
		identifier = lexicon.getStringWord();
		token = lexicon.nextToken();
		parseVarDeclare();
	}
	else { // deal with the situation of semicolon
		if (!elementCreater.createVar(dataType, identifier)) { // creat const
			token = reportAndJumpOver(IDENTIFIER_ALREADY_DEFINED, SEMICOLON);
			return;
		}
		if (printDetail) cout << "creat a variable with semicolon " << identifier << endl;
		token = lexicon.nextToken();
	}
}

void CodeParser::parseVarAndFuncDeclare() {
	if (token == INTSYM || token == CHARSYM) {
		dataType = token == INTSYM ? TYPEINT : TYPECHAR;
		token = lexicon.nextToken();
		if (token == IDENT) {
			identifier = lexicon.getStringWord();
			token = lexicon.nextToken();
			if (token == LPARENTHESE) { // function
				if (identifier == "main") { // ignore the error
					errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), WRONG_MAIN_TYPE);
				}
				parseFunction();
			}
			else if (token == LBRACKET || token == COMMA || token == SEMICOLON) { // variable
				if (gotFunction) {
					token = reportAndJumpOver(UNEXPECTED_VAR_DECLARE, SEMICOLON);
					token = lexicon.nextToken();
				}
				parseVarDeclare(); // not deal with semicolon
			}
			else {
				token = reportAndJumpOver(UNEXPECTED_SIGN, SEMICOLON);
			}
		}
		else {
			token = reportAndJumpOver(IDENTIFIER_EXPECTED, SEMICOLON);
		}
	}
	else if (token == VOIDSYM) {
		dataType = TYPEVOID;
		token = lexicon.nextToken();
		if (token == IDENT || token == MAINSYM) {
			identifier = lexicon.getStringWord();
			token = lexicon.nextToken();
			if (identifier == "main" && gotMain) {
				token = reportAndJumpOver(MAIN_REPEAT_DEFINE, RBRACE);
				return;
			}
			if (token == LPARENTHESE) { // function
				parseFunction();
			}
			else {
				errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), LEFT_PARENTHESES_EXPECTED);
				token = lexicon.nextToken();
				return;
			}
		}
		else {
			token = reportAndJumpOver(IDENTIFIER_EXPECTED, SEMICOLON);
			return;
		}
	}
	else {
		token = reportAndJumpOver(TYPE_OF_IDENTIFIER_EXPECTED, SEMICOLON);
		return;
	}
}

void CodeParser::parseIf() {
	if (printDetail) cout << "set a if " << endl;
	token = lexicon.nextToken();
	if (token != LPARENTHESE) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), LEFT_PARENTHESES_EXPECTED);
		jumpToSet(statementHeadSet);
		return;
	}
	token = lexicon.nextToken();
	QuadTable *thenTable = new QuadTable(); // if () {thenTable} else {elseTable} nextTable
	QuadTable *elseTable = new QuadTable();
	QuadTable *nextTable = new QuadTable();
	Quantity *quantity1 = nullptr;
	Quantity *quantity2 = nullptr;
	Token gotCompareToken = parseCondition(&quantity1, &quantity2);
	if (gotCompareToken == VOIDSYM) { // deal with error
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNEXPECTED_SIGN);
		jumpToSet(statementHeadSet);
		return;
	}
	elementCreater.createBranch(gotCompareToken, quantity1, quantity2, elseTable);
	if (token != RPARENTHESE) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), RIGHT_PARENTHESES_EXPECTED);
		jumpToSet(statementHeadSet);
		return;
	}
	elementCreater.setQuadTable(thenTable); // enter new quadTable
	token = lexicon.nextToken();
	parseStatement();
	elementCreater.createJump(new Label(nextTable)); // jump
	elementCreater.setQuadTable(elseTable); 
	if (token == ELSESYM) {
		if (printDetail) cout << "set a else " << endl;
		token = lexicon.nextToken();	
		parseStatement();
		// set else then 
	}
	elementCreater.setQuadTable(nextTable);
}

void CodeParser::parseDoWhile() {
	// do＜语句＞while '('＜条件＞')'
	if (printDetail) cout << "set a do while loop " << endl;
	QuadTable *loopTable = new QuadTable(); // do {loopTable} while() nextTable
	QuadTable *nextTable = new QuadTable();
	Quantity *quantity1;
	Quantity *quantity2;
	token = lexicon.nextToken();
	elementCreater.setQuadTable(loopTable); // enter loopTable
	parseStatement();
	if (token != WHILESYM) {
		reportAndJumpOver(WHILE_EXPECTED, RPARENTHESE);
		return;
	}
	token = lexicon.nextToken();
	if (token != LPARENTHESE) {
		reportAndJumpOver(LEFT_PARENTHESES_EXPECTED, RPARENTHESE);
		return;
	}
	token = lexicon.nextToken();
	Token gotCompareToken = parseCondition(&quantity1, &quantity2);
	if (gotCompareToken == VOIDSYM) { // deal with error
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNEXPECTED_SIGN);
		jumpToSet(statementHeadSet);
		return;
	}
	elementCreater.createBranch(gotCompareToken, quantity1, quantity2, nextTable);
	elementCreater.createJump(new Label(loopTable)); // jump back loop
	// set jump next then
	if (token != RPARENTHESE) {
		reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, RPARENTHESE);
		return;
	}
	token = lexicon.nextToken();
	elementCreater.setQuadTable(nextTable);
}

void CodeParser::parseFor() {
	// for'('＜标识符＞＝＜表达式＞; ＜条件＞; ＜标识符＞＝＜标识符＞(+| -)＜步长＞')'＜语句＞
	if (printDetail) cout << "set a for loop " << endl;
	QuadTable *loopTable = new QuadTable(); // {loopTable for ()}  nextTable
	QuadTable *nextTable = new QuadTable();
	Quantity *quantity1;
	Quantity *quantity2;

	token = lexicon.nextToken();
	if (token != LPARENTHESE) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), LEFT_PARENTHESES_EXPECTED);
		jumpToSet(statementHeadSet);
		return;
	}
	token = lexicon.nextToken();
	if (token != IDENT) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), IDENTIFIER_EXPECTED);
		jumpToToken(SEMICOLON);
	}
	else {
		identifier = lexicon.getStringWord();
		TableElement *tableElement;
		if ((tableElement = elementCreater.findElement(identifier)) == nullptr) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNDEFINED_IDENTIFIER);
		}
		token = lexicon.nextToken();
		if (token != BECOME) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), BECOME_EXPECTED);
		}
		token = lexicon.nextToken();
		Quantity *quantity = parseExpression();
		if (quantity == nullptr) {
			jumpToToken(SEMICOLON);
		}
		elementCreater.actStatement(new Variable(tableElement->dataType, tableElement->name, quantity)); // ＜标识符＞＝＜表达式＞
	}
	if (token != SEMICOLON) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
	}
	token = lexicon.nextToken();

	elementCreater.setQuadTable(loopTable); // set label loop

	Token gotCompareToken = parseCondition(&quantity1, &quantity2); // parseCondition
	if (gotCompareToken == VOIDSYM) { // deal with error
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNEXPECTED_SIGN);
		jumpToSet(statementHeadSet);
		return;
	}
	elementCreater.createBranch(gotCompareToken, quantity1, quantity2, nextTable);
	// set jump then next

	token = lexicon.nextToken();
	if (token != IDENT) {
		reportAndJumpOver(BECOME_EXPECTED, RBRACE);
		return;
	}
	identifier = lexicon.getStringWord(); // check whether the ident is legal
	TableElement *tableElement;
	if ((tableElement = elementCreater.findElement(identifier)) == nullptr) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNDEFINED_IDENTIFIER);
	}
	if (printDetail) cout << "the loop ident is " << identifier << endl;
	token = lexicon.nextToken();
	if (token != BECOME) {
		reportAndJumpOver(BECOME_EXPECTED, RBRACE);
		return;
	}
	token = lexicon.nextToken();
	if (token != IDENT) {
		reportAndJumpOver(BECOME_EXPECTED, RBRACE);
		return;
	}
	identifier = lexicon.getStringWord();
	TableElement *tableElement2;
	if ((tableElement2 = elementCreater.findElement(identifier)) == nullptr) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNDEFINED_IDENTIFIER);
	}
	token = lexicon.nextToken();
	if (token != MINUS && token != PLUS) {
		reportAndJumpOver(UNEXPECTED_SIGN, RBRACE);
		return;
	}
	OPCode opCode = token == PLUS ? OP_PLUS : OP_SUB; 
	token = lexicon.nextToken();
	if (token == NUM) {
		valueTemp = lexicon.getNumberTemp();
	}
	else {
		reportAndJumpOver(UNEXPECTED_SIGN, RBRACE);
		return;
	}
	token = lexicon.nextToken();
	if (token != RPARENTHESE) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), RIGHT_PARENTHESES_EXPECTED);
	}
	else {
		token = lexicon.nextToken();
	}
	parseStatement();
	Quantity *quantity = new Caculator(opCode, new Variable(tableElement2->dataType, tableElement2->name), new Constant(TYPEINT, valueTemp)); // ＜标识符＞(+| -)＜步长＞
	elementCreater.actStatement(new Variable(tableElement->dataType, tableElement->name, quantity)); // ＜标识符＞＝＜标识符＞(+| -)＜步长＞
	elementCreater.createJump(new Label(loopTable)); // jump back loop
	elementCreater.setQuadTable(nextTable); // set label next
}

void CodeParser::parseScanf() {
	if (printDetail) cout << "use scanf to get something " << endl;
	token = lexicon.nextToken();
	vector<Variable *> parameters;
	if (token != LPARENTHESE) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), LEFT_PARENTHESES_EXPECTED);
	}
	do {
		token = lexicon.nextToken();
		if (token != IDENT) {
			reportAndJumpOver(IDENTIFIER_EXPECTED, SEMICOLON);
			return;
		}
		identifier = lexicon.getStringWord();
		TableElement *tableElement;
		if ((tableElement = elementCreater.findElement(identifier)) == nullptr) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNDEFINED_IDENTIFIER);
		}
		else {
			parameters.push_back(new Variable(tableElement->dataType, identifier));
		}
		if (printDetail) cout << "scan into " << identifier << endl;
		// add to scanf list
		token = lexicon.nextToken();
	} while (token == COMMA);
	if (token != RPARENTHESE) {
		reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, SEMICOLON);
		return;
	}
	token = lexicon.nextToken();
	if (token != SEMICOLON) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
	}
	token = lexicon.nextToken();
	elementCreater.actStatement(new Scanf(parameters));
}

void CodeParser::parsePrintf() {
	if (printDetail) cout << "use printf to print something " << endl;
	Quantity *quantity = nullptr;
	token = lexicon.nextToken();
	if (token != LPARENTHESE) {
		reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, SEMICOLON);
		return;
	}
	token = lexicon.nextToken();
	if (token == STR) {
		string stringPrintf = lexicon.getStringTemp();
		if (printDetail) cout << "        print string \"" << stringPrintf << "\"" << endl;
		// add string
		token = lexicon.nextToken();
		if (token == COMMA) {
			token = lexicon.nextToken();
			quantity = parseExpression();
			if (printDetail) cout << "        print expression " << endl;
			if (quantity == nullptr) {
				jumpToToken(SEMICOLON);
				token = lexicon.nextToken();
				return;
			}
		}
		if (token != RPARENTHESE) {
			reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, SEMICOLON);
			return;
		}
		token = lexicon.nextToken();
		if (token != SEMICOLON) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
		}
		token = lexicon.nextToken();
		int stringInt = elementCreater.addString(stringPrintf);
		elementCreater.actStatement(new Printf(stringInt, quantity));
	}
	else {
		if (printDetail) cout << "        print expression " << endl;
		quantity = parseExpression();
		if (quantity == nullptr) {
			jumpToToken(SEMICOLON);
			token = lexicon.nextToken();
			return;
		}
		if (token != RPARENTHESE) {
			reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, SEMICOLON);
			return;
		}
		token = lexicon.nextToken();
		if (token != SEMICOLON) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
		}
		token = lexicon.nextToken();
		elementCreater.actStatement(new Printf(quantity));
	}
}

void CodeParser::parseCode() {
	token = lexicon.nextToken();
	while (token == CONSTSYM) {
		parseConstDeclare();
	}
	while (token == INTSYM || token == CHARSYM || token == VOIDSYM) {
		parseVarAndFuncDeclare();
	}
	if (token != EOFSYM) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNEXPECTED_SIGN);
	}
}

void CodeParser::parseReturn() {
	Function *function = elementCreater.getCurrentFunction();
	token = lexicon.nextToken();
	if (printDetail) cout << "return something " << endl;
	// talk about the type of function
	if (function->functionType == TYPEVOID) {
		if (token != SEMICOLON) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
		}
		else {
			token = lexicon.nextToken();
		}
		elementCreater.actStatement(new Return());
	}
	else {
		if (token == LPARENTHESE) {
			token = lexicon.nextToken();
			Quantity *quantity = parseExpression();
			if (quantity == nullptr) {
				return;
			}
			if (token != RPARENTHESE) {
				reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, SEMICOLON);
				return;
			}
			token = lexicon.nextToken();
			if (quantity->dataType != function->functionType) {
				errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), WRONG_RETURN_TYPE, true);
			}
			if (token != SEMICOLON) {
				errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
			}
			else {
				token = lexicon.nextToken();
			}
			elementCreater.actStatement(new Return(quantity));
		}
		else {
			reportAndJumpOver(LEFT_PARENTHESES_EXPECTED, SEMICOLON);
		}
	}
}

void CodeParser::jumpToToken(Token innerToken) {
	Token tokenGot = NULLSYM;
	while (tokenGot != innerToken && tokenGot != EOFSYM) {
		tokenGot = lexicon.nextToken();
	}
}

Token CodeParser::reportAndJumpOver(ErrorType errorType, Token targetToken) {
	errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), errorType);
	jumpToToken(targetToken);
	token = lexicon.nextToken();
	return token;
}

Token CodeParser::jumpToSet(unordered_set<Token> &tokenSet) {
	while (statementHeadSet.find(token) != statementHeadSet.end() && token != EOFSYM) {
		token = lexicon.nextToken();
	}
	return token;
}
