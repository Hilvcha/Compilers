#include"scanner.h"
#include "error.h"
inline
void skip_to_line_end(std::istream &input) {
	char ch;
	do {
		input.get(ch);
	} while (ch != '\n' && !input.fail());
}

Scan::Token Scan::token {Scan::ERRTOKEN,"",0.0,nullptr };


unsigned int Scan::LineNo=1;

std::ostream& Scan::operator<<(std::ostream &out, const Scan::Token &t) {
	out << int(t.type) << ' ' << t.literal_value << ' ' << t.value << ' ' << t.FuncPtr;
	return out;
}

Scan::Token Scan::GetToken(std::istream &input) {
	Scan::Token tmp{ Scan::ERRTOKEN,"",0.0,nullptr };
	char ch;
	std::string literal_value;
	double num_value;
	while (input.get(ch), !input.eof()) {//skip
		if (input.fail()) {
			literal_value.push_back(toupper(ch));
			throw Error::ID_error("文件存在非法字符，读取失败！", LineNo, literal_value.c_str());
		}
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
		tmp.type = Scan::NONTOKEN;
		return tmp;
	}
	input.get(ch);
	switch (ch) {
		//单个的
	case '+':
		tmp.literal_value = "+"; tmp.type = Scan::Token_Type(ch); return tmp;//两个的符号
	case '(':
		tmp.literal_value = "("; tmp.type = Scan::Token_Type(ch); return tmp;//两个的符号
	case ')':
		tmp.literal_value = ")"; tmp.type = Scan::Token_Type(ch); return tmp;//两个的符号
	case ',':
		tmp.literal_value = ","; tmp.type = Scan::Token_Type(ch); return tmp;//两个的符号
	case ';':
		LineNo++;
		tmp.literal_value = ";"; tmp.type = Scan::SEMICO;
		return tmp;
	case '*':
		if (input.get(ch), ch == '*') {
			tmp.type = Scan::POWER;
			return tmp;
		}
		input.unget();
		tmp.literal_value = "*"; tmp.type = Scan::MUL;
		return tmp;
	case '/':
		input.get(ch);
		if (ch == '/') {
			skip_to_line_end(input);
			return GetToken(input);
		}
		input.unget();
		tmp.literal_value = "/"; tmp.type = Scan::DIV;
		return tmp;
	case '-':
		input.get(ch);
		if (ch == '-') {
			skip_to_line_end(input);
			return GetToken(input);
		}
		input.unget();
		tmp.literal_value = "-"; tmp.type = Scan::MINUS;
		return tmp;
		//数值
	case '0':	case '1':	case '2':	case '3':
	case '4':	case '5':	case '6':	case '7':
	case '8':	case '9':	case '.':
		input.unget();
		input >> num_value;
		tmp.literal_value = "C_NUM"; tmp.type = Scan::CONST_ID;
		tmp.value = num_value;
		return tmp;
		//字符串值
	default:
		if (isalpha(ch)) {
			literal_value = toupper(ch);//表中都是大写
			while (input.get(ch) && isalnum(ch)) literal_value.push_back(toupper(ch));
			input.unget();
			for (auto t : Scan::TokenTab) {
				if (t.literal_value == literal_value) {
					return tmp = t;
				}
			}
			throw Error::ID_error("未定义 ID!",LineNo,literal_value.c_str());
		}
		literal_value.push_back(toupper(ch));
		throw Error::ID_error("未定义 token!",LineNo,literal_value.c_str());
	}
	//return curr_tok = TokenTab[19];
}