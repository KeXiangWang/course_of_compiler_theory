#pragma once
enum DataType {
	TYPEINT,
	TYPECHAR,
	TYPEVOID
};

enum ElementKind {
	KINDCONST,
	KINDVAR,
	KINDPARA,
	KINDARRAY
};

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

enum OPCode {
	OP_PLUS,
	OP_SUB,
	OP_MULT,
	OP_DIV,
	OP_BNE,
	OP_BEQ,
	OP_BLE,
	OP_BLT,
	OP_BGE,
	OP_BGT,
	OP_BEQZ,
	OP_CONST,
	OP_VAR,
	OP_ARRAY,
	OP_FUNC,
	OP_VOIDFUNC,
	OP_SCANF,
	OP_PRINTF,
	OP_RETURN,
	OP_LABEL,
	OP_JUMP
};