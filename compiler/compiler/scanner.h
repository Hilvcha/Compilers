#pragma once
#include<iostream>
#include<cmath>
#include<string>
#include<cctype>


enum Token_Type {
	ORIGIN,SCALE,ROT,IS,				//保留字
	TO,STEP,DRAW,FOR,FROM,  
	T,									//传入参数
	SEMICO=';',	L_BRACKET='(',	R_BRACKET=')',	COMMA=',',	//分隔符
	PLUS='+',	MINUS='-',		MUL='*',		DIV='/',	POWER,			//运算符
	FUNC,							    //调用的函数
	CONST_ID,						   //常数
	NONTOKEN,							//空记号
	ERRTOKEN,                          //出错记号
	END
};

struct Token
{
	Token_Type type;
	std::string literal_value;
	double value;
	typedef double Func(double);
	Func *FuncPtr;
	
	Token() = default;
	Token(Token_Type a,std::string b,double c,Func d):type(a),literal_value(b),value(c),FuncPtr(d){}
	Token(const Token& other) {
		type = other.type;
		literal_value = other.literal_value;
		value = other.value;
		FuncPtr = other.FuncPtr;
	}
};
//Token curr_tok { SEMICO, ";", 0.0, nullptr };


static Token TokenTab[] =
{
	{ CONST_ID,	"PI",		3.1415926,	nullptr	 },
	{ CONST_ID,	"E",		2.71828,	nullptr	 },
	{ T,		"T",		0.0,		nullptr  },
	{ FUNC,     "SIN",		0.0,        std::sin },
	{ FUNC,     "COS",		0.0,        std::cos },
	{ FUNC,     "TAN",		0.0,        std::tan },
	{ FUNC,     "LN",		0.0,        std::log },
	{ FUNC,		"EXP",		0.0,        std::exp },
	{ FUNC,     "SQRT",		0.0,		std::sqrt},
	{ ORIGIN,	"ORIGIN",	0.0,		nullptr	 },
	{ SCALE,    "SCALE",    0.0,        nullptr	 },
	{ ROT,		"ROT",		0.0,        nullptr  },
	{ IS,		"IS",		0.0,        nullptr  },
	{ FOR,		"FOR",		0.0,        nullptr  },
	{ FROM,		"FROM",		0.0,        nullptr  },
	{ TO,		"TO",		0.0,        nullptr  },
	{ STEP,     "STEP",		0.0,		nullptr  },
	{ DRAW,     "DRAW",		0.0,        nullptr  },
};

extern unsigned int LineNo;
extern Token token;
Token GetToken(std::istream &);
//void InitScanner(const std::string);