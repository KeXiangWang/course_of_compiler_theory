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
		statementHeadSet.insert(IDENT); // ���з���ֵ����������䣾��/���޷���ֵ����������䣾��/����ֵ��䣾;
		statementHeadSet.insert(INTSYM); // ���з���ֵ����������䣾��/���޷���ֵ����������䣾��/����ֵ��䣾;
		statementHeadSet.insert(CHARSYM); // ���з���ֵ����������䣾��/���޷���ֵ����������䣾��/����ֵ��䣾;
		statementHeadSet.insert(VOIDSYM); // ���з���ֵ����������䣾��/���޷���ֵ����������䣾��/����ֵ��䣾;
		statementHeadSet.insert(IFSYM); // ��������䣾��
		statementHeadSet.insert(DOSYM); // ��ѭ����䣾��
		statementHeadSet.insert(FORSYM); // ��ѭ����䣾��
		statementHeadSet.insert(SEMICOLON); // ���գ�
		statementHeadSet.insert(LBRACE); // '{'������У�'}'
		statementHeadSet.insert(SCANFSYM); // ������䣾��
		statementHeadSet.insert(PRINTFSYM); // ��д��䣾��
		statementHeadSet.insert(RETURNSYM); // ��������䣾��

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

