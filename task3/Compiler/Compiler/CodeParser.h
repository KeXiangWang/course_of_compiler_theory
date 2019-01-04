#pragma once
#include "global.h"
#include "ErrorHandler.h"
#include "Lexicon.h"
#include "ElementCreater.h"
#include "Quad.h"

using std::string;
using std::unordered_set;

class CodeParser
{
private:
	Token token; // temp token
	string identifier; // temp identifier
	DataType dataType; // temp dataType
	int sign; // temp sign of int
	int valueTemp; // temp value
	bool gotSign; // whether sign read
	bool gotFunction; // whether have got function and finished var declaration
	bool gotMain; // whether have got main
	int arraySize; // size of array
	bool printDetail = false;
	ErrorHandler &errorHandler;
	Lexicon &lexicon;
	ElementCreater &elementCreater;

	unordered_set<Token> statementHeadSet;
	unordered_set<Token> compareSet; 
	static int shadows;

public:
	CodeParser(ErrorHandler &errorHandler, Lexicon &lexicon, ElementCreater &elementCreater)
		: errorHandler(errorHandler), lexicon(lexicon), elementCreater(elementCreater)
	{
		statementHeadSet.insert(IDENT); // ＜有返回值函数调用语句＞；/＜无返回值函数调用语句＞；/＜赋值语句＞;
		statementHeadSet.insert(INTSYM); // ＜有返回值函数调用语句＞；/＜无返回值函数调用语句＞；/＜赋值语句＞;
		statementHeadSet.insert(CHARSYM); // ＜有返回值函数调用语句＞；/＜无返回值函数调用语句＞；/＜赋值语句＞;
		statementHeadSet.insert(VOIDSYM); // ＜有返回值函数调用语句＞；/＜无返回值函数调用语句＞；/＜赋值语句＞;
		statementHeadSet.insert(IFSYM); // ＜条件语句＞；
		statementHeadSet.insert(DOSYM); // ＜循环语句＞；
		statementHeadSet.insert(FORSYM); // ＜循环语句＞；
		statementHeadSet.insert(SEMICOLON); // ＜空＞
		statementHeadSet.insert(LBRACE); // '{'＜语句列＞'}'
		statementHeadSet.insert(SCANFSYM); // ＜读语句＞；
		statementHeadSet.insert(PRINTFSYM); // ＜写语句＞；
		statementHeadSet.insert(RETURNSYM); // ＜返回语句＞；

		compareSet.insert(LESS);		// <
		compareSet.insert(LEQ);			// <=
		compareSet.insert(GRT);			// >
		compareSet.insert(GEQ);			// >=
		compareSet.insert(NEQ);			// !=
		compareSet.insert(EQU);			// ==
	};

	Quantity *parseExpression();
	Quantity *parseTerm();
	Quantity *parseFactor();
	void parseStatement();
	void parseStatementSequence();
	void parseCompoundStatement();
	void parseFunction();
	Token parseCondition(Quantity **quantity1, Quantity **quantity2);
	void parseConstDeclare();
	void parseVarDeclare();
	void parseVarAndFuncDeclare();
	void parseIf();
	void parseDoWhile();
	void parseFor();
	void parseScanf();
	void parsePrintf();
	void parseCode();
	void parseReturn();

	void jumpToToken(Token innerToken);
	Token reportAndJumpOver(ErrorType errorType, Token targetToken);
	Token jumpToSet(unordered_set<Token> &tokenSet);

};

