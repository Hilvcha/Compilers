#include"parser.h"
#include<cstdarg>
std::ifstream input;

void Program();
void Statement();
void OriginStatement();
void RotStatement();
void ScaleStatement();
void ForStatement();

ExprNode * MakeExprNode(Token_Type opcode, ...);
ExprNode * Expression();
ExprNode * Term();
ExprNode * Factor();
ExprNode * Component();
ExprNode * Atom();

inline
void FetchToken() {
	token = GetToken(input);
}
inline
void TokenMatch(Token_Type op) {

	if (op != token.type) {
		throw Syntax_error("未预期的终结符", LineNo, token);
	}
	//switch (token.type)
	//{
	//case ORIGIN:std::cout << "ORIGIN ";
	//	break;
	//case SCALE:	std::cout << "SCALE ";
	//	break;
	//case ROT:	std::cout << "ROT ";
	//	break;
	//case IS:	std::cout << "IS ";
	//	break;
	//case TO:	std::cout << "TO ";
	//	break;
	//case STEP:	std::cout << "STEP ";
	//	break;
	//case DRAW:	std::cout << "DRAW ";
	//	break;
	//case FOR:	std::cout << "FOR ";
	//	break;
	//case FROM:	std::cout << "FROM ";
	//	break;
	//case T:		std::cout << "T ";
	//	break;
	//case SEMICO:std::cout << "; " << std::endl;
	//	break;
	//case L_BRACKET:	std::cout << "( ";
	//	break;
	//case R_BRACKET:	std::cout << ") ";
	//	break;
	//case COMMA:		std::cout << ", ";
	//	break;
	//case NONTOKEN:	std::cout << "end ";
	//	break;
	//default:
	//	break;
	//}
	FetchToken();
}

void Parser(std::string filepath) {
	input.open(filepath);
	FetchToken();
	Program();
	input.close();
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
		std::cout << "ORIGIN ";
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
		throw Syntax_error("未能识别的 Statement!", LineNo, token);
	}
	std::cout << "out Statement" << std::endl;
}
void OriginStatement() {
	std::cout << "OriStatement" << std::endl;
	ExprNode *xPtr, *yPtr;
	TokenMatch(IS);
	TokenMatch(L_BRACKET);
	xPtr=Expression();
	TokenMatch(COMMA);
	yPtr=Expression();
	TokenMatch(R_BRACKET);
	std::cout << "out OriStatement" << std::endl;
}
void RotStatement() {
	std::cout << "RotStatement" << std::endl;
	ExprNode *rotPtr;
	TokenMatch(IS);
	rotPtr=Expression();
	std::cout << "out RotStatement" << std::endl;
	DrawExprTree(rotPtr, 0);
}

void ScaleStatement() {
	std::cout << "ScaleStatement" << std::endl;
	ExprNode *xPtr, *yPtr;
	TokenMatch(IS);
	TokenMatch(L_BRACKET);
	xPtr=Expression();
	TokenMatch(COMMA);
	yPtr=Expression();
	TokenMatch(R_BRACKET);
	std::cout << "out ScaleStatement" << std::endl;
}
void ForStatement() {
	std::cout << "ForStatement" << std::endl;
	ExprNode *fromPtr, *toPtr, *stepPtr, *xPtr, *yPtr;
	TokenMatch(T);
	TokenMatch(FROM);	fromPtr = Expression();
	TokenMatch(TO);		toPtr = Expression();
	TokenMatch(STEP);		stepPtr=Expression();
	TokenMatch(DRAW);
	TokenMatch(L_BRACKET);	xPtr=Expression();
	TokenMatch(COMMA);		yPtr=Expression();
	TokenMatch(R_BRACKET);
	std::cout << "out ForStatement" << std::endl;
}
double Parameter=233;
ExprNode *MakeExprNode (Token_Type opcode, ...){
	ExprNode *ExprPtr = new(ExprNode);
	ExprPtr->OpCode = opcode;
	va_list ArgPtr;
	va_start(ArgPtr, opcode);
	switch (opcode)
	{
	case CONST_ID:
		ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr, double);
		break;
	case T:
		ExprPtr->Content.CaseParmPtr = &Parameter;
		break;
	case FUNC:
		ExprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(ArgPtr, FuncPtr);
		ExprPtr->Content.CaseFunc.Child = (ExprNode*)va_arg(ArgPtr, ExprNode*);
		break;
	default:
		ExprPtr->Content.CaseOperator.Left = (ExprNode*)va_arg(ArgPtr, ExprNode*);
		ExprPtr->Content.CaseOperator.Right = (ExprNode*)va_arg(ArgPtr, ExprNode*);
		break;
	}
	va_end(ArgPtr);
	return ExprPtr;

}

ExprNode* Expression() {
	ExprNode *left,*right;
	Token_Type t_tmp;
	left=Term();
	while (token.type == PLUS || token.type == MINUS) {
		t_tmp = token.type;
		FetchToken();
		right=Term();
		left=MakeExprNode(t_tmp, left, right); //left传完值左节点值后就可存放根节点
	}
	return left;
}
ExprNode * Term() {
	ExprNode *left, *right;
	Token_Type t_tmp;
	left=Factor();
	while (token.type == MUL || token.type == DIV) {
		t_tmp = token.type;
		FetchToken();
		right=Factor();
		left = MakeExprNode(t_tmp, left, right);
	}
	return left;
}
ExprNode * Factor() {
	ExprNode *left, *right;
	if (token.type == PLUS) {
		TokenMatch(PLUS);
		right = Factor();
	}
	else if (token.type == MINUS) {
		TokenMatch(MINUS);
		right = Factor();
		left = MakeExprNode(CONST_ID, 0);
		right = MakeExprNode(MINUS, left, right);
	}
	else {
		right=Component();
	}
	return right;
}
ExprNode * Component() {
	ExprNode *left, *right;
	left=Atom();
	while (token.type == POWER) {
		TokenMatch(POWER);
		right=Component();
		left = MakeExprNode(POWER, left, right);
	}
	return left;
}
ExprNode * Atom() {
	ExprNode *p;
	if (token.type == CONST_ID) {
		p=MakeExprNode(CONST_ID, token.value);
		TokenMatch(CONST_ID);
	}
	else if (token.type == T) {
		p = MakeExprNode(T);
		TokenMatch(T);
	}
	else if (token.type == FUNC) {
		Token tmp = token;
		TokenMatch(FUNC);
		TokenMatch(L_BRACKET);
		p=Expression(); 
		p = MakeExprNode(FUNC, tmp.FuncPtr, p);
		TokenMatch(R_BRACKET);
	}
	else if (token.type == L_BRACKET) {
		FetchToken();
		p=Expression();
		TokenMatch(R_BRACKET);
	}
	else {
		throw Syntax_error("错误的 atom!", LineNo, token);
	}
	return p;
}
inline 
void DrawSpace(const unsigned int layer,const unsigned int space) {
	for (size_t i = 0; i < layer; i++){
		for (size_t j = 0; j < space; j++){
			std::cout << ' ';
		}
	}
}

void DrawExprTree(const ExprNode* root,const unsigned int layer) {
		DrawSpace(layer, 1);
		switch (root->OpCode){
			case PLUS:
				std::cout << '+'<<std::endl;
				DrawExprTree(root->Content.CaseOperator.Left, layer + 1);
				DrawExprTree(root->Content.CaseOperator.Right, layer + 1);
				break;
			case MINUS://有可能是负号，需要判断左节点是不是0
				std::cout << '-' << std::endl;
				if (root->Content.CaseOperator.Left->OpCode == CONST_ID) {
					if (std::abs(root->Content.CaseOperator.Left->Content.CaseConst) <= 1e-15) {
						DrawExprTree(root->Content.CaseOperator.Right, layer + 1);
						break;
					}
				}
				DrawExprTree(root->Content.CaseOperator.Left, layer + 1);
				DrawExprTree(root->Content.CaseOperator.Right, layer + 1);
				break;
			case MUL:
				std::cout << '*' << std::endl;
				DrawExprTree(root->Content.CaseOperator.Left, layer + 1);
				DrawExprTree(root->Content.CaseOperator.Right, layer + 1);
				break;
			case DIV:
				std::cout << '/' << std::endl;
				DrawExprTree(root->Content.CaseOperator.Left, layer + 1);
				DrawExprTree(root->Content.CaseOperator.Right, layer + 1);
				break;
			case FUNC:
				for (auto a : TokenTab) {
					if (a.FuncPtr == root->Content.CaseFunc.MathFuncPtr) {
						std::cout << a.literal_value << std::endl;
						DrawExprTree(root->Content.CaseFunc.Child, layer + 1);
					}
				}
				break;
			case CONST_ID:
				std::cout << root->Content.CaseConst << std::endl;
				break;
			case T://T的值有问题
				std::cout << root->Content.CaseParmPtr <<std::endl;
				break;
			case POWER:
				std::cout << "**" << std::endl;
				DrawExprTree(root->Content.CaseOperator.Left,layer+1);
				DrawExprTree(root->Content.CaseOperator.Right,layer+1);
			default:
				break;
		}
}