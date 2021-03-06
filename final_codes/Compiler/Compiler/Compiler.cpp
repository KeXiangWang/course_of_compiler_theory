// Compiler.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "Lexicon.h"
#include "ErrorHandler.h"
#include "ElementCreater.h"
#include "CodeParser.h"
#include "MipsGenerator.h"
#include "Optimizer.h"

using std::string;

void verify(Lexicon lexicon) {
	Token token;
	while ((token = lexicon.nextToken()) != EOFSYM) {
		std::cout << lexicon.tokenToString[token] << "  \t" << lexicon.currentString << std::endl;
		lexicon.currentString.clear();
	}
	std::cout << "Lexicon analysing finished" << std::endl;
}



int compile() {
	string filename;
	std::cout << "Input file name to compile:\n";
	//std::cin >> filename;
	filename = "D:\\OneDrive\\CLionProjects\\course_of_compiler_theory\\task3\\Compiler\\Compiler\\resource.txt";
	if (filename == "end") {
		std::cout << "End by order! " << std::endl;
		return 1;
	}
	std::fstream inFile = std::fstream(filename, std::fstream::in);
	if (!inFile) {
		std::cout << "Wrong path! Program shut down " << std::endl;
		return 2;
	}
	Lexicon lexicon = Lexicon(inFile);
	ErrorHandler errorHandler = ErrorHandler();
	ElementCreater elementCreater = ElementCreater();
	CodeParser codeParseer = CodeParser(errorHandler, lexicon, elementCreater);
	codeParseer.parseCode();
	//verify(lexicon);
	if (errorHandler.errorHaveOccured()) {
		std::cout << "Error occured while compiling! " << std::endl;
		return 3;
	}
	else {
		std::cout << "Grammartically parsing finished " << std::endl << std::endl;
	}
	Optimizer optimizer = Optimizer(&elementCreater);
	optimizer.optimize("../quadsBeforeOPT.txt", "../quadsAfterOPT.txt");
	MipsGenerator mipsGenerator = MipsGenerator(&elementCreater);
	mipsGenerator.generate();
	auto out = std::fstream("output.txt", std::fstream::out);
	mipsGenerator.printCode(out);
	return 0;
}

int main() {
	do {
		compile();
	} while (0);
	return 0;
}