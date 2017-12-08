#pragma once
#include"scanner.h"
#include<iostream>
#include<fstream>
std::ifstream input;
inline
void FetchToken() {
	token = GetToken(input);
}
inline
void TokenMatch(Token_Type op) {
	
	if (op != token.type) {
		throw Syntax_error("未预期的终结符",LineNo,token);
	}
	FetchToken( );
	switch (op)
	{
	case ORIGIN:std::cout << "ORIGIN " ;
		break;
	case SCALE:	std::cout << "SCALE " ;
		break;
	case ROT:	std::cout << "ROT " ;
		break;
	case IS:	std::cout << "IS " ;
		break;
	case TO:	std::cout << "TO " ;
		break;
	case STEP:	std::cout << "STEP " ;
		break;
	case DRAW:	std::cout << "DRAW " ;
		break;
	case FOR:	std::cout << "FOR " ;
		break;
	case FROM:	std::cout << "FROM " ;
		break;
	case T:		std::cout << "T " ;
		break;
	case SEMICO:std::cout << "; " << std::endl;
		break;
	case L_BRACKET:	std::cout << "( " ;
		break;
	case R_BRACKET:	std::cout << ") " ;
		break;
	case COMMA:		std::cout << ", " ;
		break;
	case NONTOKEN:	std::cout << "end " ;
		break;
	default:
		break;
	}
}

void Parser(std::string filepath) {
	input.open(filepath);
	FetchToken( );
	Program();
}

void Program() {
	std::cout << "Program" << std::endl;
	while (token.type != NONTOKEN) {
		Statement();
		TokenMatch(SEMICO);
	}
	std::cout << "out Program" << std::endl;
}
void Statement() {
	std::cout << "Statement" << std::endl;
	if (token.type == ORIGIN) {
		FetchToken();
		OriginStatement();
	}
	else if (token.type == SCALE) {
		FetchToken();
		ScaleStatement();
	}
	else if (token.type == ROT) {
		FetchToken();
		RotStatement();
	}
	else if (token.type == FOR) {
		FetchToken();
		ForStatement();
	}
	else {
		throw Syntax_error("未能识别的 Statement", LineNo,token);
	}
	std::cout << "out Statement" << std::endl;
}
void OriginStatement() {
	std::cout << "OriStatement" << std::endl;
	TokenMatch(IS);
	TokenMatch(L_BRACKET);
	Expression();
	TokenMatch(COMMA);
	Expression();
	TokenMatch(R_BRACKET);
	std::cout << "out OriStatement" << std::endl;
}
void RotStatement() {
	std::cout << "RotStatement" << std::endl;
	TokenMatch(IS);
	Expression();
	std::cout << "out RotStatement" << std::endl;
}

void ScaleStatement(){
	std::cout << "ScaleStatement" << std::endl;
	TokenMatch(IS);
	TokenMatch(L_BRACKET);
	Expression();
	TokenMatch(COMMA);
	Expression();
	TokenMatch(R_BRACKET);
	std::cout << "out ScaleStatement" << std::endl;
}
void ForStatement() {
	std::cout << "ForStatement" << std::endl;
	TokenMatch(T);
	TokenMatch(FROM);		Expression();
	TokenMatch(TO);			Expression();
	TokenMatch(STEP);		Expression();
	TokenMatch(DRAW);
	TokenMatch(L_BRACKET);	Expression();
	TokenMatch(COMMA);		Expression();
	TokenMatch(R_BRACKET);
	std::cout << "out ForStatement" << std::endl;
}

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

ExprNode * Expression() {
	Term();
	while (token.type == PLUS || token.type == MINUS) {
		FetchToken();
		Term();
	}
}
ExprNode * Term() {
	Factor();
	while (token.type == MUL || token.type == DIV) {
		FetchToken();
		Factor();
	}
}
ExprNode * Factor() {
	while (token.type == PLUS || token.type == MINUS) {
		FetchToken();
	}//尾递归
	Component();
}
ExprNode * Component() {
	Atom();
	while (token.type == POWER) {
		FetchToken();
		Component();
	}

}
ExprNode * Atom() {
	if (token.type == CONST_ID) {
		//处理
		FetchToken();
	}
	else if (token.type == T) {
		FetchToken();
	}
	else if (token.type == FUNC) {
		FetchToken();
		TokenMatch(L_BRACKET); Expression(); TokenMatch(R_BRACKET);
	}
	else if (token.type == L_BRACKET) {
		Expression();
		TokenMatch(R_BRACKET);
	}
	else {
		throw Syntax_error("错误的 atom!", LineNo,token);
	}
}
//struct ExprNode