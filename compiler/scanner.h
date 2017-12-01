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
	double (*FuncPtr)(double);
};
//Token curr_tok { SEMICO, ";", 0.0, nullptr };


static Token TokenTab[] =
{
	{ CONST_ID,	"PI",		3.1415926,	nullptr},
	{ CONST_ID,	"E",		2.71828,	nullptr},
	{ T,		"T",		0.0,		nullptr  },
	{ FUNC,     "SIN",		0.0,        std::sin },
	{ FUNC,     "COS",		0.0,        std::cos },
	{ FUNC,     "TAN",		0.0,        std::tan },
	{ FUNC,     "LN",		0.0,        std::log },
	{ FUNC,		"EXP",		0.0,        std::exp },
	{ FUNC,     "SQRT",		0.0,		std::sqrt},
	{ ORIGIN,	"OPIGIN",	0.0,		nullptr	 },
	{ SCALE,    "SCALE",    0.0,        nullptr	 },
	{ ROT,		"ROT",		0.0,        nullptr  },
	{ IS,		"IS",		0.0,        nullptr  },
	{ FOR,		"FOR",		0.0,        nullptr  },
	{ FROM,		"FROM",		0.0,        nullptr  },
	{ TO,		"TO",		0.0,        nullptr  },
	{ STEP,     "STEP",		0.0,		nullptr  },
	{ DRAW,     "DRAW",		0.0,        nullptr  },
	{ ERRTOKEN, " ",		0.0,		nullptr	 },
	{ END,      "",         0.0,        nullptr  }
};
inline
void skip_to_line_end(std::istream &input) {
	char ch;
	do{
		input.get(ch);
	} while (ch!='\n'&&!input.fail());
}
Token GetToken(std::istream &input) {
	Token token;
	char ch;
	std::string literal_value;
	double num_value;
	while (input.get(ch), !input.eof()) {//skip
		switch (ch) {
		case ' ':
		case '\n':
		case '\t':
			continue;
		default:input.unget(); break;
		}
	}
	if (input.eof()) {
		token.type = NONTOKEN;
		return token;
	}
	input.get(ch);
	switch (ch) {
	//单个的
	case '+':
	case '(':
	case ')':
	case ';':
	case ',':
		token.type = Token_Type(ch);
		return token;
	//两个的符号
	case '*':
		if (input.get(ch), ch == '*') {
			token.type = POWER;
			return token;
		}
		input.unget();
		token.type = MUL;
		return token;
	case '/':
		input.get(ch);
		if (ch == '/') {
			skip_to_line_end(input);
			return token;
		}
		input.unget();
		token.type = MUL;
		return token;
	case '-':
		input.get(ch);
			if (ch == '-') {
				skip_to_line_end(input);
				return token;
			}
		input.unget();
		token.type = MUL;
		return token;
	//数值
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
		input.unget();
		input >> num_value;
		token.type = CONST_ID;
		token.value = num_value;
		return token;
	//字符串值
	}
	//return curr_tok = TokenTab[19];
}