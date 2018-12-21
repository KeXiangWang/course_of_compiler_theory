#include "pch.h"
#include "Lexicon.h"
#include "global.h"

using std::string;
using std::fstream;

Lexicon::Lexicon(fstream &inputFile) : inFile(inputFile)
{
	currentToken = NULLSYM;
	currentChar = ' ';
	currentLine.clear();
	currentString.clear();
	stringTemp.clear();
	stringWord.clear();
	numberTemp = 0;
	lineCount = 0;
	charToToken['+'] = PLUS;
	charToToken['-'] = MINUS;
	charToToken['*'] = MULT;
	charToToken['/'] = DIV;
	charToToken['='] = BECOME;
	charToToken[','] = COMMA;
	charToToken[';'] = SEMICOLON;
	charToToken[':'] = COLON;
	charToToken['('] = LPARENTHESE;
	charToToken[')'] = RPARENTHESE;
	charToToken['['] = LBRACKET;
	charToToken[']'] = RBRACKET;
	charToToken['{'] = LBRACE;
	charToToken['}'] = RBRACE;

	stringToToken["const"] = CONSTSYM;
	stringToToken["int"] = INTSYM;
	stringToToken["char"] = CHARSYM;
	stringToToken["void"] = VOIDSYM;

	stringToToken["if"] = IFSYM;
	stringToToken["else"] = ELSESYM;
	stringToToken["do"] = DOSYM;
	stringToToken["while"] = WHILESYM;
	stringToToken["for"] = FORSYM;
	stringToToken["return"] = RETURNSYM;
	stringToToken["scanf"] = SCANFSYM;
	stringToToken["printf"] = PRINTFSYM;
	stringToToken["main"] = MAINSYM;
}

Token Lexicon::nextToken() {
	while (isspace(currentChar)) {
		nextChar();
	}
	switch (currentChar) {
	case '+':
	case '-':
	case '*':
	case ',':
	case ';':
	case ':':
	case '(':
	case ')':
	case '[':
	case ']':
	case '{':
	case '}': // transfer directly from char to token
		currentToken = charToToken[currentChar];
		currentString += currentChar;
		nextChar();
		return currentToken;
	case '/':
		nextChar();
		if (currentChar == '/') { // deal with comment
			jumpToLineEnd();
			return nextToken();
		}
		else {
			currentToken = charToToken['/'];
			currentString + '/';
			return currentToken;
		}
	case '!':
		nextChar();
		if (currentChar == '=') {
			currentToken = NEQ;
			currentString += "!=";
			nextChar();
		}
		else { // set as an error
			currentToken = ERRORSYM;
			currentString += '!';
		}
		return currentToken;
	case '<':
		nextChar();
		if (currentChar == '=') {
			currentToken = LEQ;
			currentString += "<=";
			nextChar();
		}
		else {
			currentToken = LESS;
			currentString += '<';
		}
		return currentToken;
	case '>':
		nextChar();
		if (currentChar == '=') {
			currentToken = GEQ;
			currentString += ">=";
			nextChar();
		}
		else {
			currentToken = GRT;
			currentString += '>';
		}
		return currentToken;
	case '=':
		nextChar();
		if (currentChar == '=') {
			currentToken = EQU;
			currentString += "==";
			nextChar();
		}
		else {
			currentToken = BECOME;
			currentString = '=';
		}
		return currentToken;
	case '\'':
		nextChar();
		if (isDefineChar(currentChar)) {
			currentString = currentString + currentChar;
			numberTemp = currentChar;
			nextChar();
			if (currentChar == '\'') {
				nextChar();
				currentToken = ALPHA;
				return currentToken;
			}
		}
		currentToken = ERRORSYM;
		reportError();
		jumpToLineEnd();
		return currentToken;

	case '\"':
		nextChar();
		stringTemp.clear();
		char temp;
		while (isCharInString(currentChar)) {
			currentString += currentChar;
			stringTemp += currentChar;
			temp = currentChar;
			nextChar();
			if (temp == '\\') {
				currentString += '\\';
				stringTemp += '\\';

			}
		}
		if (currentChar == '\"') {
			currentToken = STR;
			nextChar();
			return currentToken;
		}
		else {
			currentToken = ERRORSYM;
			reportError();
			jumpToLineEnd();
			return currentToken;
		}

	case EOF:
		currentToken = EOFSYM;
		return currentToken;
	default:

		if (isdigit(currentChar)) {
			numberTemp = 0;
			numberTemp = 10 * numberTemp + currentChar - '0';
			currentString += currentChar;
			if (currentChar == '0') {
				nextChar();
				if (isdigit(currentChar)) {
					currentToken = ERRORSYM;
					reportError();
					jumpToSentenceEnd();
					return currentToken;
				}
				currentToken = NUM;
				return currentToken;
			}
			else {
				while (isdigit(nextChar())) {
					numberTemp = 10 * numberTemp + currentChar - '0';
					currentString += currentChar;
				}
				currentToken = NUM;
				return currentToken;
			}
		}
		else  if (isalpha(currentChar) || currentChar == '_') {
			stringWord.clear();
			while (isalnum(currentChar) || currentChar == '_') {
				stringWord += currentChar;
				currentString += currentChar;
				nextChar();
			}
			if (stringToToken.find(stringWord) != stringToToken.end()) {
				currentToken = stringToToken[stringWord];
			}
			else {
				currentToken = IDENT;
			}
			return currentToken;
		}
		else {
			currentToken = ERRORSYM;
			reportError();
			jumpToLineEnd();
			return currentToken;
		}
	}
}

char Lexicon::nextChar() {
	if (currentChar == '\n') {
		lineCount++;
		currentLine.clear();
	}
	if ((currentChar = inFile.get()) != EOF) {
		currentLine += currentChar;
	}
	return currentChar;
}

void Lexicon::jumpToLineEnd() {
	while (currentChar != '\n' && currentChar != EOF) {
		nextChar();
	}
}

void Lexicon::jumpToSentenceEnd() {
	while (currentChar != ';' && currentChar != EOF) {
		nextChar();
	}
}

void Lexicon::reportError() {
	std::cout << "-------------------Error occured at line "
		<< lineCount << ": " << currentLine << "-------------------" << std::endl;
}

inline bool Lexicon::isDefineChar(char c) {
	return c == '*' || c == '/' || c == '+' || c == '-' || isalnum(c);
}

inline bool Lexicon::isCharInString(char c) {
	return c == 32 || c == 33 || c >= 35 && c <= 126;
}

int Lexicon::getLineCount() const {
	return lineCount;
}

const string Lexicon::getCurrentLine() const {
	return currentLine;
}

const string Lexicon::getStringWord() const {
	return stringWord;
}

const string Lexicon::getStringTemp() const
{
	return stringTemp;
}

const int Lexicon::getNumberTemp() const
{
	return numberTemp;
}
