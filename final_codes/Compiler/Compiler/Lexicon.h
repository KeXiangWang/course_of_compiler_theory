#pragma once
#include "global.h"

using std::unordered_map;
using std::vector;
using std::string;
using std::fstream;

class Lexicon
{
public:
	Lexicon(fstream &inputFile);
	Token nextToken();
	int getLineCount() const;
	const string getCurrentLine() const;
	const string getStringWord() const;
	const string getStringTemp() const;
	const int getNumberTemp() const;
	void reportError();
	vector<string> tokenToString = { "PLUS",	"MINUS",	"MULT",	"DIV",	"LESS",	"LEQ",	"GRT",	"GEQ",	"NEQ",
	"EQU",	"BECOME",	"COMMA",	"SEMICOLON",	"COLON",	"LPARENTHESE",	"RPARENTHESE",	"LBRACKET",	"RBRACKET",
	"LBRACE",	"RBRACE",	"ALPHA",	"NUM",	"STR",	"CONSTSYM",	"INTSYM",	"CHARSYM",	"VOIDSYM",	"IFSYM",
	"ELSESYM",	"DOSYM",	"WHILESYM",	"FORSYM",	"RETURNSYM",	"SCANFSYM",	"PRINTFSYM",	"MAINSYM",
	"IDENT",	"EOFSYM",	"ERRORSYM",	"NULLSYM" };
	string currentString; // only for print
	
private:
	fstream &inFile;
	void jumpToLineEnd();
	void jumpToSentenceEnd();
	char nextChar();
	inline bool isDefineChar(char c);
	inline bool isCharInString(char c);

	string stringWord; // store name of sym or ident
	string stringTemp; //  store constant string in ""
	int numberTemp; // store constant 
	int lineCount; // location of dealing

	string currentLine; 
	Token currentToken;
	char currentChar;
	unordered_map<char, Token> charToToken;
	unordered_map<string, Token> stringToToken;
};

