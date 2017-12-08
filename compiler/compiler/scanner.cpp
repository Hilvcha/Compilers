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

std::ostream& operator<<(std::ostream &out, const Token &t) {
	out << int(t.type) << ' ' << t.literal_value << ' ' << t.value << ' ' << t.FuncPtr;
	return out;
}

Token GetToken(std::istream &input) {
	Token tmp{ ERRTOKEN,"",0.0,nullptr };
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
		tmp.type = NONTOKEN;
		return tmp;
	}
	input.get(ch);
	switch (ch) {
		//������
	case '+':
	case '(':
	case ')':
	case ',':
		tmp.type = Token_Type(ch);
		return tmp;//�����ķ���
	case ';':
		LineNo++;
		tmp.type = SEMICO;
		return tmp;
	case '*':
		if (input.get(ch), ch == '*') {
			tmp.type = POWER;
			return tmp;
		}
		input.unget();
		tmp.type = MUL;
		return tmp;
	case '/':
		input.get(ch);
		if (ch == '/') {
			skip_to_line_end(input);
			return GetToken(input);
		}
		input.unget();
		tmp.type = MUL;
		return tmp;
	case '-':
		input.get(ch);
		if (ch == '-') {
			skip_to_line_end(input);
			return GetToken(input);
		}
		input.unget();
		tmp.type = MUL;
		return tmp;
		//��ֵ
	case '0':	case '1':	case '2':	case '3':
	case '4':	case '5':	case '6':	case '7':
	case '8':	case '9':	case '.':
		input.unget();
		input >> num_value;
		tmp.type = CONST_ID;
		tmp.value = num_value;
		return tmp;
		//�ַ���ֵ
	default:
		if (isalpha(ch)) {
			literal_value = toupper(ch);//���ж��Ǵ�д
			while (input.get(ch) && isalnum(ch)) literal_value.push_back(toupper(ch));
			input.unget();
			for (auto t : TokenTab) {
				if (t.literal_value == literal_value) {
					return tmp = t;
				}
			}
			throw ID_error("δ���� ID!",LineNo,literal_value.c_str());
		}
		literal_value.push_back(toupper(ch));
		throw ID_error("δ���� token!",LineNo,literal_value.c_str());
	}
	//return curr_tok = TokenTab[19];
}