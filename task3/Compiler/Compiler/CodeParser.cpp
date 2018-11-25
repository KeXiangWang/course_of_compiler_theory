#include "pch.h"
#include "CodeParser.h"
#include "Quad.h"
#include "ErrorHandler.h"
#include "Lexicon.h"
#include "ElementCreater.h"

using std::cout;
using std::endl;

Quantity *CodeParser::parseExpression() {
	cout << "parse an expression " << endl;
	Quantity *quantity;
	OPCode opCode = OP_PLUS;
	bool start = true;
	if (token == PLUS || token == MINUS) {
		// TODO add sign
		opCode = token == PLUS ? OP_PLUS : OP_SUB;
		token = lexicon.nextToken();
	}
	quantity = parseTerm();
	if (quantity == nullptr) {
		jumpToToken(SEMICOLON);
		token = lexicon.nextToken();
		return nullptr;
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
		quantity = new Caculator(); // TODOK optimizer
	}
	return quantity;
}
// tested
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
		quantity = new Caculator(); // TODO optimizer
	}
	return quantity;

}
// tested
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
			if ((function = elementCreater.findFunc(identifier)) = nullptr) {
				reportAndJumpOver(WRONG_FUNCTION_CALL, RPARENTHESE);
				return nullptr;
			}
			if (function->functionType == TYPEVOID) {
				reportAndJumpOver(WRONG_FUNCTION_CALL, RPARENTHESE);
				return nullptr;
			}
			// TODO make a parameter list
			do {
				token = lexicon.nextToken();
				Quantity *arg = parseExpression();
				if (arg == nullptr) {
					return nullptr;
				}
				// TODO add to the parameter list
			} while (token == COMMA);
			if (token != RPARENTHESE) {
				return nullptr;
			}
			quantity = new FunctionCall();
			token = lexicon.nextToken();
			cout << "call a function " << functionName << endl;
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
					token = lexicon.nextToken();
					Quantity *index = parseExpression();
					if (index == nullptr) {
						return nullptr;
					}
					if (token != RBRACKET) {
						errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), RIGHT_BRACKET_EXPECTED);
						return nullptr;
					}
					token = lexicon.nextToken();
					quantity = new Array();
					return quantity;
				}
				else {
					errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNDEFINED_IDENTIFIER);
					return nullptr;
				}
			}
			else {
				token = lexicon.nextToken();
				quantity = new Variable(); // TODO creat var;
				return quantity;
			}
		}
		break;
	case LPARENTHESE:
		token = lexicon.nextToken();
		quantity = parseExpression();
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
		return new Constant();
	case ALPHA:
		alpha = lexicon.getStringWord()[0];
		token = lexicon.nextToken();
		return new Constant();
	}

	return nullptr;
}
// tested
void CodeParser::parseStatement() {
	switch (token)
	{
	case SEMICOLON:
		cout << "encounter a blank statement " << endl;
		token = lexicon.nextToken();
		break;
	case LBRACE:
		cout << "enter a statement sqquence" << endl;
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
		// TODO should judge whether is a array
		if (token == LBRACKET) {
			token = lexicon.nextToken();
			Quantity *index;
			index = parseExpression();
			// TODO deal with idex
			if (index == nullptr) {
				jumpToToken(SEMICOLON);
				token = lexicon.nextToken();
			}
		}
		if (token == BECOME) {
			cout << "assign a value to ident " << identifier<<endl;
			token = lexicon.nextToken();
			Quantity *quantity;
			quantity = parseExpression();
			// TODO deal with idex
			if (quantity == nullptr) {
				jumpToToken(SEMICOLON);
				token = lexicon.nextToken();
			}
			if (token != SEMICOLON) {
				errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
			}
			token = lexicon.nextToken();
		}
		else if (token == LPARENTHESE) {
			cout << "use a self-defined function " << endl;
			// TODO make a list
			
			Quantity *quantity;
			do {
				token = lexicon.nextToken();
				if (token == RPARENTHESE) {
					break;
				}
				quantity = parseExpression();
				if (quantity == nullptr) {
					jumpToToken(SEMICOLON);
				}
			} while (token == COMMA);
			if (token != RPARENTHESE) {
				reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, SEMICOLON);
				return;
			}

			// TODO check number of args
			token = lexicon.nextToken();
			if (token != SEMICOLON) {
				errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
			}
			token = lexicon.nextToken();
		}
		else {
			reportAndJumpOver(UNEXPECTED_SIGN, SEMICOLON);
			return;
		}

		// TODO add become / func
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
// tested
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
	if (statementHeadSet.find(token) == statementHeadSet.end())	{
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), UNEXPECTED_SIGN);
		jumpToSet(statementHeadSet);
	}
	parseStatementSequence();
}
// tested
void CodeParser::parseFunction() {
	gotFunction = true;
	if (!elementCreater.creatFunc(dataType, identifier)) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), IDENTIFIER_ALREADY_DEFINED);
	}
	cout << "creat a function " << identifier << endl;
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
					if (!elementCreater.creatPara(dataType, identifier)) { // creat arguement 
						errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), IDENTIFIER_ALREADY_DEFINED);
					}
					cout << "creat a " << dataType << " arguement " << identifier << endl;
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
// tested
Token CodeParser::parseCondition() {
	cout << "cope with condition " << endl;
	Quantity *quantity1 = parseExpression();
	Token compareToken;
	if (quantity1 == nullptr) {
		return VOIDSYM;
	}
	if (compareSet.find(token) == compareSet.end()) {
		return NULLSYM;
	}
	compareToken = token;
	token = lexicon.nextToken();
	Quantity *quantity2 = parseExpression();
	// TODO set a compare 
	if (quantity2 == nullptr) {
		return VOIDSYM;
	}
	return compareToken;
}
// tested
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
							if (!elementCreater.creatConst(dataType, identifier, sign*valueTemp)) { // creat const
								token = reportAndJumpOver(IDENTIFIER_ALREADY_DEFINED, SEMICOLON);
								return;
							}
							cout << "creat a const int" << endl;
							token = lexicon.nextToken();
						}
						else {
							token = reportAndJumpOver(NUMBER_EXPECTED, SEMICOLON);
							return;
						}
					}
					else {
						if (token == ALPHA) {
							valueTemp = lexicon.getStringWord()[0]; // get the char as int
							if (!elementCreater.creatConst(dataType, identifier, valueTemp)) { // creat const
								token = reportAndJumpOver(IDENTIFIER_ALREADY_DEFINED, SEMICOLON);
								return;
							}
							cout << "creat a const char" << endl;
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
// tested
void CodeParser::parseVarDeclare() {
	if (token == LBRACKET) {
		token = lexicon.nextToken();
		if (token == NUM) {
			arraySize = lexicon.getNumberTemp();
			token = lexicon.nextToken();
			if (token == RBRACKET) {
				if (!elementCreater.creatArray(dataType, identifier, arraySize)) { // creat Array
					token = reportAndJumpOver(IDENTIFIER_ALREADY_DEFINED, SEMICOLON);
					return;
				}
				cout << "creat a array " << identifier << endl;
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
		if (!elementCreater.creatVar(dataType, identifier)) { // creat var
			token = reportAndJumpOver(IDENTIFIER_ALREADY_DEFINED, SEMICOLON);
			return;
		}
		cout << "creat a variable with comma" << identifier << endl;
		token = lexicon.nextToken();
		if (token != IDENT) {
			token = reportAndJumpOver(IDENTIFIER_EXPECTED, SEMICOLON);
		}
		identifier = lexicon.getStringWord();
		token = lexicon.nextToken();
		parseVarDeclare();
	}
	else { // deal with the situation of semicolon
		if (!elementCreater.creatVar(dataType, identifier)) { // creat const
			token = reportAndJumpOver(IDENTIFIER_ALREADY_DEFINED, SEMICOLON);
			return;
		}
		cout << "creat a variable with semicolon " << identifier << endl;
		token = lexicon.nextToken();
	}
}
// tested
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
// tested
void CodeParser::parseIf() {
	cout << "set a if " << endl;
	token = lexicon.nextToken();
	if (token != LPARENTHESE) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), LEFT_PARENTHESES_EXPECTED);
		jumpToSet(statementHeadSet);
		return;
	}
	token = lexicon.nextToken();
	Token gotCompareToken = parseCondition();
	if (gotCompareToken == VOIDSYM) { // TODO deal with error

	}
	if (token != RPARENTHESE) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), RIGHT_PARENTHESES_EXPECTED);
		jumpToSet(statementHeadSet);
		return;
	}
	token = lexicon.nextToken();
	parseStatement();
	// TODO set else then next 
	if (gotCompareToken == NULLSYM) { // TODO set j and 

	}
	else {

	}
	if (token == ELSESYM) {
		cout << "set a else " << endl;
		token = lexicon.nextToken();
		parseStatement();
		// TODO set else then 
	}
}
// tested
void CodeParser::parseDoWhile() {
	// do＜语句＞while '('＜条件＞')'
	cout << "set a do while loop " << endl;
	token = lexicon.nextToken();
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
	Token gotCompareToken = parseCondition();
	if (gotCompareToken) {

	}
	else {

	}
	// TODO set jump next then
	if (token != RPARENTHESE) {
		reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, RPARENTHESE);
		return;
	}
	token = lexicon.nextToken();
}

void CodeParser::parseFor() {
	// for'('＜标识符＞＝＜表达式＞; ＜条件＞; ＜标识符＞＝＜标识符＞(+| -)＜步长＞')'＜语句＞
	cout << "set a for loop " << endl;
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
		token = lexicon.nextToken();
		if (token != BECOME) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), BECOME_EXPECTED);
		}
		token = lexicon.nextToken();
		Quantity *quantity = parseExpression();
		if (quantity == nullptr) {
			jumpToToken(SEMICOLON);
		}
	}
	if (token != SEMICOLON) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
	}
	token = lexicon.nextToken();
	Token gotCompareToken = parseCondition();
	// TODO set jump then next
	if (gotCompareToken) {

	}
	else {

	}
	if (token != SEMICOLON) {
		errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
	}
	token = lexicon.nextToken();
	if (token != IDENT) {
		reportAndJumpOver(BECOME_EXPECTED, RBRACE);
		return;
	}
	identifier = lexicon.getStringWord();
	// TODO check whether the ident is legal
	cout << "the loop ident is " << identifier << endl;
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
	// TODO check whether the ident is legal
	token = lexicon.nextToken();
	if (token != MINUS && token != PLUS) {
		reportAndJumpOver(UNEXPECTED_SIGN, RBRACE);
		return;
	}
	sign = token == MINUS ? -1 : 1; // TODO deal with the special signal, current sign is not proper
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
}

void CodeParser::parseScanf() {
	cout << "use scanf to get something " << endl;
	token = lexicon.nextToken();
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
		cout << "scan into " << identifier << endl;
		// TODO add to scanf list
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
}

void CodeParser::parsePrintf() {
	cout << "use printf to print something " << endl;
	Quantity *quantity;
	token = lexicon.nextToken();
	if (token != LPARENTHESE) {
		reportAndJumpOver(RIGHT_PARENTHESES_EXPECTED, SEMICOLON);
		return;
	}
	token = lexicon.nextToken();
	if (token == STR) {
		string stringPrintf = lexicon.getStringTemp();
		cout << "        print string \"" << stringPrintf << "\"" << endl;
		// TODO add string
		token = lexicon.nextToken();
		if (token == COMMA) {
			token = lexicon.nextToken();
			quantity = parseExpression();
			cout << "        print expression " << endl;
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
	}
	else {
		cout << "        print expression " << endl;
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
	}
}
// tested
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
// tested
void CodeParser::parseReturn() {
	token = lexicon.nextToken();
	cout << "return something " << endl;
	// TODO talk about the type of function
	if (token == LPARENTHESE) {
		token = lexicon.nextToken();
		Quantity *quantity = parseExpression();
		if (token != RPARENTHESE) {
			reportAndJumpOver(RIGHT_BRACE_EXPECTED, SEMICOLON);
			return;
		}
		token = lexicon.nextToken();
		if (token != SEMICOLON) {
			errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), SEMICOLON_EXPECTED);
		}
		else {
			token = lexicon.nextToken();
		}
	}
}
// tested
void CodeParser::jumpToToken(Token token) {
	Token tokenGot = NULLSYM;
	while (tokenGot != token){
		tokenGot = lexicon.nextToken();
	} 
}

Token CodeParser::reportAndJumpOver(ErrorType errorType, Token targetToken) {
	errorHandler.report(lexicon.getLineCount(), lexicon.getCurrentLine(), errorType);
	jumpToToken(targetToken);
	token = lexicon.nextToken();
	return token;
}

Token CodeParser::jumpToSet(unordered_set<Token> &tokenSet)
{
	while (statementHeadSet.find(token) != statementHeadSet.end()) {
		token = lexicon.nextToken();
	}
	return token;
}
