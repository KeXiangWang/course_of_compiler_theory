#include "pch.h"
#include "ErrorHandler.h"
#include "Lexicon.h"

ErrorHandler::ErrorHandler()
{
	errorOccured = false;
	errorToString[TYPE_OF_IDENTIFIER_EXPECTED	] = "expect the type of the declared identifier ";
	errorToString[UNEXPECTED_IDENTIFIER			] = "unexpected identifier appear ";
	errorToString[UNEXPECTED_ASSIGNMENT			] = "unexpected assignment to constant appear ";
	errorToString[UNDEFINED_IDENTIFIER			] = "undefined identifier appear ";
	errorToString[UNEXPECTED_VAR_DECLARE		] = "unexpected variable declare appear";
	errorToString[UNEXPECTED_SIGN				] = "unexpected sign appear ";
	errorToString[IDENTIFIER_ALREADY_DEFINED	] = "the identifier already defined ";
	errorToString[NONAGTIVE_INT_EXPECTED		] = "expect an Non-negative ";
	errorToString[RIGHT_PARENTHESES_EXPECTED	] = "expect a right parentheses ";
	errorToString[RIGHT_BRACKET_EXPECTED		] = "expect a right bracket ";
	errorToString[RIGHT_BRACE_EXPECTED			] = "expect a right brace ";
	errorToString[EQUAL_EXPECTED				] = "expect an equal symbol ";
	errorToString[BECOME_EXPECTED				] = "expect a become ";
	errorToString[NUMBER_EXPECTED				] = "expect a number ";
	errorToString[IDENTIFIER_EXPECTED			] = "expect an identifier ";
	errorToString[ALPHA_EXPECTED				] = "expect a char letter ";
	errorToString[SEMICOLON_EXPECTED			] = "expect a semicolon ";
	errorToString[LEFT_BRACE_EXPECTED			] = "expect a left brace ";
	errorToString[LEFT_PARENTHESES_EXPECTED		] = "expect a left parentheses ";
	errorToString[WHILE_EXPECTED				] = "expect a while ";
	errorToString[COLON_EXPECTED				] = "expect a colon ";
	errorToString[ASSIGN_EXPECTED				] = "expect an assignment ";
	errorToString[VARIABLE_EXPECTED				] = "expect a variable ";
	errorToString[INVALID_RETURN				] = "encounter an invalid return ";
	errorToString[WRONG_FUNCTION_CALL			] = "encounter a wrong function call ";
	errorToString[WRONG_RETURN_TYPE				] = "encounter a wrong return type ";
	errorToString[WRONG_ARGUMENT_LIST			] = "encounter a wrong arguement list ";
	errorToString[MODIFY_CONST_VALUE			] = "encounter a modification of constant ";
	errorToString[WRONG_MAIN_TYPE				] = "main expected to return void ";
	errorToString[MAIN_REPEAT_DEFINE			] = "main have been defined ";
	errorToString[ARRAY_OVERFLOW				] = "index is out of array ";
	errorToString[WRONG_QUANTITY_TYPE			] = "the type of quantity does't meet the requirements ";
	errorToString[WRONG_ASSIGNMENT_TYPE			] = "the type of assignment does't meet the requirements ";
}

void ErrorHandler::report(int lineCount, string currentLine, ErrorType errortype, bool ignore) {
	string temp = "";
	for (auto it = currentLine.begin(); it != currentLine.end(); it++) {
		if (*it != '\n') {
			temp += *it;
		}
	}
	std::cout << "Error\t" << errortype << ": " << "\tLine: " << lineCount 
		<< "\t \"" << temp << "\" " << errorToString[errortype] <<std::endl;
	if (!ignore) errorOccured = true;
}

bool ErrorHandler::errorHaveOccured() {
	return errorOccured;
}
