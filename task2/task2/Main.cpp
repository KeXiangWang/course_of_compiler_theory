#include "pch.h"
#include <iostream>
#include "Lexicon.h"


using std::string;

void verify(Lexicon lexicon) {
	Token token;
	while ((token = lexicon.nextToken()) != EOFSYM) {
		std::cout << lexicon.tokenToString[token] << "  \t" << lexicon.currentString << std::endl;
		lexicon.currentString.clear();
	}
	std::cout << "Lexicon analysing finished"<<std::endl;
}

void compile(){
	string filename;
	std::cout << "Input file name to compile:\n";
	std::cin >> filename;
	std::fstream inFile = std::fstream(filename, std::fstream::in);
	Lexicon lexicon = Lexicon(inFile);
	verify(lexicon);
}

int main()
{
	do{
		compile();
	} while (0);
	return 0;
}