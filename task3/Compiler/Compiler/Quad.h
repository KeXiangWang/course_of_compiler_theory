#pragma once


class Quad
{
public:
	Quad();
	~Quad();
};

class Quantity :public Quad {
public:
	Quantity();
	~Quantity();
};

class Caculator :public Quantity {
public:
	Caculator();
	~Caculator();
};

class Constant :public Quantity {
public:
	Constant();
	~Constant();
};

class FunctionCall :public Quantity {
public:
	FunctionCall();
	~FunctionCall();
};

class Variable :public Quantity {
public:
	Variable();
	~Variable();
};

class Array :public Quantity {
public:
	Array();
	~Array();
};