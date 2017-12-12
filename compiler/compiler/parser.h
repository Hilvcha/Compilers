#pragma once
#include"scanner.h"
#include<iostream>
#include<fstream>
extern std::ifstream input;
extern double Parameter;

void Parser(std::string);

typedef double(*FuncPtr)(double);
struct ExprNode{
	Scan::Token_Type OpCode;
	union {
		struct {
			ExprNode *Left, *Right;
		} CaseOperator;
		struct {
			ExprNode * Child;
			FuncPtr MathFuncPtr;
		} CaseFunc;
		double CaseConst;
		double *CaseParmPtr;
	} Content;
};

//struct ExprNode
//draw ExprNode
//void DrawExprTree(const ExprNode*, const unsigned int);
//求表达式的值和绘图相关
double GetExprValue(ExprNode* );
//static void CalcCoord(ExprNode*,ExprNode*,double &,double &);
//extern HDC hdc;
void DrawHtml(std::string);
