#pragma once
#include "pch.h"

enum Token
{
	PLUS,			// +
	MINUS, 			// -
	MULT,			// *
	DIV,			// /
	LESS,			// <
	LEQ,			// <=
	GRT,			// >
	GEQ,			// >=
	NEQ,			// !=
	EQU,			// ==
	BECOME,			// =
	COMMA,			// ,
	SEMICOLON,		// ;
	COLON,			// :
	LPARENTHESE,	// (
	RPARENTHESE,	// )
	LBRACKET,		// [
	RBRACKET,		// ]
	LBRACE,			// {
	RBRACE,			// }

	ALPHA,			// one letter
	NUM,			// number
	STR,			// string

	CONSTSYM,
	INTSYM,
	CHARSYM,
	VOIDSYM,

	IFSYM,
	ELSESYM,
	DOSYM,
	WHILESYM,
	FORSYM,
	RETURNSYM,
	SCANFSYM,
	PRINTFSYM,
	MAINSYM,
	IDENT,
	EOFSYM,
	ERRORSYM,
	NULLSYM
};

using std::map;
using std::vector;
using std::string;
using std::fstream;

class Lexicon
{
public:
	Lexicon(fstream &inputFile);
	Token nextToken();

	void reportError();
	inline bool isDefineChar(char c);
	inline bool isCharInString(char c);
	vector<string> tokenToString = { "PLUS",	"MINUS",	"MULT",	"DIV",	"LESS",	"LEQ",	"GRT",	"GEQ",	"NEQ",
	"EQU",	"BECOME",	"COMMA",	"SEMICOLON",	"COLON",	"LPARENTHESE",	"RPARENTHESE",	"LBRACKET",	"RBRACKET",
	"LBRACE",	"RBRACE",	"ALPHA",	"NUM",	"STR",	"CONSTSYM",	"INTSYM",	"CHARSYM",	"VOIDSYM",	"IFSYM",
	"ELSESYM",	"DOSYM",	"WHILESYM",	"FORSYM",	"RETURNSYM",	"SCANFSYM",	"PRINTFSYM",	"MAINSYM",
	"IDENT",	"EOFSYM",	"ERRORSYM",	"NULLSYM" };
	string currentString; // only for print
	int lineCount; // location of dealing
private:
	fstream &inFile;
	void jumpToLineEnd();
	void jumpToSentenceEnd();
	char nextChar();

	string stringWord; // store name of sym or ident
	string stringTemp; //  store constant string in ""
	int numberTemp; // store constant

	string currentLine; 
	Token currentToken;
	char currentChar;
	map<char, Token> charToToken;
	map<string, Token> stringToToken;
};

