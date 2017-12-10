#pragma once
#include"scanner.h"
#include<iostream>
#include<fstream>
#include"error.h"
extern std::ifstream input;

void Parser(std::string);

typedef double(*FuncPtr)(double);
struct ExprNode{
	Token_Type OpCode;
	union {
		struct {
			ExprNode *Left, *Right;
		} CaseOperator;
		struct {
			ExprNode * Child;
			FuncPtr MathFuncPtr;
		} CaseFunc;
		double CaseConst;
		double * CaseParmPtr;
	} Content;
};

//struct ExprNode
//draw ExprNode
void DrawExprTree(const ExprNode*, const unsigned int);
