
#pragma once
#include"scanner.h"

struct Zero_divide{};
struct ID_error {
	const char * p;
	unsigned int Line;
	const char* name;
	ID_error(const char*q, unsigned int n,const char* f1) : p(q), Line(n),name(f1) {}
};
struct Syntax_error{
	const char * p;
	unsigned int Line;
	Token err_t;
	Syntax_error(const char*q,const unsigned int n,Token& t): p(q),Line(n),err_t(t) {}
};
