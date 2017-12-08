#include"scanner.h"
#include "error.h"
inline
void skip_to_line_end(std::istream &input) {
	char ch;
	do {
		input.get(ch);
	} while (ch != '\n' && !input.fail());
}
Token token;


unsigned int LineNo=1;


Token GetToken(std::istream &input) {
	Token token{ ERRTOKEN,"",0.0,nullptr };
	char ch;
	std::string literal_value;
	double num_value;
	while (input.get(ch), !input.eof()) {//skip
		switch (ch) {
		case ' ':
		case '\n':
		case '\t':
			continue;
		default: break;
		}
		input.unget(); break;
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
	case ',':
		token.type = Token_Type(ch);
		return token;//两个的符号
	case ';':
		LineNo++;
		token.type = SEMICO;
		return token;
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
			return GetToken(input);
		}
		input.unget();
		token.type = MUL;
		return token;
	case '-':
		input.get(ch);
		if (ch == '-') {
			skip_to_line_end(input);
			return GetToken(input);
		}
		input.unget();
		token.type = MUL;
		return token;
		//数值
	case '0':	case '1':	case '2':	case '3':
	case '4':	case '5':	case '6':	case '7':
	case '8':	case '9':	case '.':
		input.unget();
		input >> num_value;
		token.type = CONST_ID;
		token.value = num_value;
		return token;
		//字符串值
	default:
		if (isalpha(ch)) {
			literal_value = toupper(ch);//表中都是大写
			while (input.get(ch) && isalnum(ch)) literal_value.push_back(toupper(ch));
			input.unget();
			for (auto t : TokenTab) {
				if (t.literal_value == literal_value) {
					return token = t;
				}
			}
			throw ID_error("未定义 ID!",LineNo,literal_value.c_str());
		}
		literal_value.push_back(toupper(ch));
		throw ID_error("未定义 token!",LineNo,literal_value.c_str());
	}
	//return curr_tok = TokenTab[19];
}