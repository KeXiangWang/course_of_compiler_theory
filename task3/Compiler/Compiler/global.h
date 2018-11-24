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

enum OPCode {
	OP_PLUS,
	OP_SUB,
	OP_MULT,
	OP_DIV,
	OP_BNE,
	OP_BEQ,
};