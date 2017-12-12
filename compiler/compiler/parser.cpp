#include"parser.h"
#include<cstdarg>
#include<vector>
#include"error.h"
#include<windows.h>
#define FOR_SIZE 100
std::ifstream input;
//绘图相关
void DrawLoop(double start, double to, double step, ExprNode * xp, ExprNode * yp);
void CalcCoord(ExprNode *xp, ExprNode *yp, double &x_v, double &y_v);
void DrawDot(unsigned long, unsigned long);
//void DrawLoop(double, double, double, ExprNode*, ExprNode*,HDC);
double Parameter = 0;//T
double Origin_x = 0.0, Origin_y = 0.0;//平移
double Rot_ang = 0.0;//旋转角度
double Scale_x = 1, Scale_y = 1;//比例因子
int ForCnt = 0;
struct Point{
	double x;
	double y;
	Point(double x1,double y1):x(x1),y(y1){}
};
std::ostream& operator<<(std::ostream &out, const Point &t) {
	out << t.x << ' ' << t.y << ' ' ;
	return out;
}
std::vector<Point> *ForPtr[FOR_SIZE];//暂存每个for语句的生成坐标

//parser递归子程序
void Program();
void Statement();
void OriginStatement();
void RotStatement();
void ScaleStatement();
void ForStatement();

ExprNode * MakeExprNode(Scan::Token_Type opcode, ...);
ExprNode * Expression();
ExprNode * Term();
ExprNode * Factor();
ExprNode * Component();
ExprNode * Atom();
void DrawExprTree(const ExprNode* root, const unsigned int layer);
//词法分析辅助函数
void FetchToken() {
	Scan::token = Scan::GetToken(input);
}
inline
void TokenMatch(Scan::Token_Type op) {

	if (op != Scan::token.type) {
		throw Error::Syntax_error("未预期的终结符", Scan::LineNo, Scan::token);
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
//递归子程序实现
void Parser(std::string filepath) {
	input.open(filepath);
	if (input.fail()) {
		throw Error::ID_error("未打开文件",0,"!");
	}
	FetchToken();
	Program();
	input.close();
}

void Program() {
	std::cout << "Program" << std::endl;
	while (Scan::token.type != Scan::NONTOKEN) {
		Statement();
		TokenMatch(Scan::SEMICO);
	}
	std::cout << "out Program" << std::endl;
}
void Statement() {
	std::cout << "Statement" << std::endl;
	if (Scan::token.type == Scan::ORIGIN) {
		std::cout << "ORIGIN ";
		FetchToken();
		OriginStatement();
	}
	else if (Scan::token.type == Scan::SCALE) {
		FetchToken();
		ScaleStatement();
	}
	else if (Scan::token.type == Scan::ROT) {
		FetchToken();
		RotStatement();
	}
	else if (Scan::token.type == Scan::FOR) {
		FetchToken();
		ForStatement();
	}
	else {
		throw Error::Syntax_error("未能识别的 Statement!", Scan::LineNo, Scan::token);
	}
	std::cout << "out Statement" << std::endl;
}
void OriginStatement() {
	std::cout << "OriStatement" << std::endl;
	ExprNode *xPtr, *yPtr;
	TokenMatch(Scan::IS);
	TokenMatch(Scan::L_BRACKET);
	xPtr=Expression();
	DrawExprTree(xPtr, 0);

	TokenMatch(Scan::COMMA);
	yPtr=Expression();
	DrawExprTree(yPtr, 0);

	TokenMatch(Scan::R_BRACKET);
	std::cout << "out OriStatement" << std::endl;
	//draw
	Origin_x = GetExprValue(xPtr);
	Origin_y = GetExprValue(yPtr);
}
void RotStatement() {
	std::cout << "RotStatement" << std::endl;
	ExprNode *rotPtr;
	TokenMatch(Scan::IS);
	rotPtr=Expression();
	DrawExprTree(rotPtr, 0);
	std::cout << "out RotStatement" << std::endl;
	//
	Rot_ang = GetExprValue(rotPtr);
}

void ScaleStatement() {
	std::cout << "ScaleStatement" << std::endl;
	ExprNode *xPtr, *yPtr;
	TokenMatch(Scan::IS);
	TokenMatch(Scan::L_BRACKET);
	xPtr=Expression();
	DrawExprTree(xPtr, 0);
	TokenMatch(Scan::COMMA);
	yPtr=Expression();
	DrawExprTree(yPtr, 0);
	TokenMatch(Scan::R_BRACKET);
	std::cout << "out ScaleStatement" << std::endl;
	//
	Scale_x = GetExprValue(xPtr);
	Scale_y = GetExprValue(yPtr);
}
void ForStatement() {
	ForPtr[ForCnt] = new std::vector<Point>;
	std::cout << "ForStatement" << std::endl;
	ExprNode *fromPtr, *toPtr, *stepPtr, *xPtr, *yPtr;
	TokenMatch(Scan::T);
	TokenMatch(Scan::FROM);	fromPtr = Expression();
	TokenMatch(Scan::TO);		toPtr = Expression();
	TokenMatch(Scan::STEP);		stepPtr=Expression();
	TokenMatch(Scan::DRAW);
	TokenMatch(Scan::L_BRACKET);	xPtr=Expression();
	TokenMatch(Scan::COMMA);		yPtr=Expression();
	TokenMatch(Scan::R_BRACKET);
	std::cout << "out ForStatement" << std::endl;
	DrawLoop(GetExprValue(fromPtr), GetExprValue(toPtr), GetExprValue(stepPtr), xPtr, yPtr);
	ForCnt++;
}

//语法树实现
ExprNode *MakeExprNode (Scan::Token_Type opcode, ...){
	ExprNode *ExprPtr = new(ExprNode);
	ExprPtr->OpCode = opcode;
	va_list ArgPtr;
	va_start(ArgPtr, opcode);
	switch (opcode)
	{
	case Scan::CONST_ID:
		ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr, double);
		break;
	case Scan::T:
		ExprPtr->Content.CaseParmPtr = &Parameter;
		break;
	case Scan::FUNC:
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
	Scan::Token_Type t_tmp;
	left=Term();
	while (Scan::token.type == Scan::PLUS || Scan::token.type == Scan::MINUS) {
		t_tmp = Scan::token.type;
		FetchToken();
		right=Term();
		left=MakeExprNode(t_tmp, left, right); //left传完值左节点值后就可存放根节点
	}
	return left;
}
ExprNode * Term() {
	ExprNode *left, *right;
	Scan::Token_Type t_tmp;
	left=Factor();
	while (Scan::token.type == Scan::MUL || Scan::token.type == Scan::DIV) {
		t_tmp = Scan::token.type;
		FetchToken();
		right=Factor();
		left = MakeExprNode(t_tmp, left, right);
	}
	return left;
}
ExprNode * Factor() {
	ExprNode *left, *right;
	if (Scan::token.type == Scan::PLUS) {
		TokenMatch(Scan::PLUS);
		right = Factor();
	}
	else if (Scan::token.type == Scan::MINUS) {
		TokenMatch(Scan::MINUS);
		right = Factor();
		left = MakeExprNode(Scan::CONST_ID, 0);
		right = MakeExprNode(Scan::MINUS, left, right);
	}
	else {
		right=Component();
	}
	return right;
}
ExprNode * Component() {
	ExprNode *left, *right;
	left=Atom();
	while (Scan::token.type == Scan::POWER) {
		TokenMatch(Scan::POWER);
		right=Component();
		left = MakeExprNode(Scan::POWER, left, right);
	}
	return left;
}
ExprNode * Atom() {
	ExprNode *p;
	if (Scan::token.type == Scan::CONST_ID) {
		p=MakeExprNode(Scan::CONST_ID, Scan::token.value);
		TokenMatch(Scan::CONST_ID);
	}
	else if (Scan::token.type == Scan::T) {
		p = MakeExprNode(Scan::T);
		TokenMatch(Scan::T);
	}
	else if (Scan::token.type == Scan::FUNC) {
		Scan::Token tmp = Scan::token;
		TokenMatch(Scan::FUNC);
		TokenMatch(Scan::L_BRACKET);
		p=Expression(); 
		p = MakeExprNode(Scan::FUNC, tmp.FuncPtr, p);
		TokenMatch(Scan::R_BRACKET);
	}
	else if (Scan::token.type == Scan::L_BRACKET) {
		FetchToken();
		p=Expression();
		TokenMatch(Scan::R_BRACKET);
	}
	else {
		throw Error::Syntax_error("错误的 atom!", Scan::LineNo, Scan::token);
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
			case Scan::PLUS:
				std::cout << '+'<<std::endl;
				DrawExprTree(root->Content.CaseOperator.Left, layer + 1);
				DrawExprTree(root->Content.CaseOperator.Right, layer + 1);
				break;
			case Scan::MINUS://有可能是负号，需要判断左节点是不是0
				std::cout << '-' << std::endl;
				if (root->Content.CaseOperator.Left->OpCode == Scan::CONST_ID) {
					if (std::abs(root->Content.CaseOperator.Left->Content.CaseConst) <= 1e-15) {
						DrawExprTree(root->Content.CaseOperator.Right, layer + 1);
						break;
					}
				}
				DrawExprTree(root->Content.CaseOperator.Left, layer + 1);
				DrawExprTree(root->Content.CaseOperator.Right, layer + 1);
				break;
			case Scan::MUL:
				std::cout << '*' << std::endl;
				DrawExprTree(root->Content.CaseOperator.Left, layer + 1);
				DrawExprTree(root->Content.CaseOperator.Right, layer + 1);
				break;
			case Scan::DIV:
				std::cout << '/' << std::endl;
				DrawExprTree(root->Content.CaseOperator.Left, layer + 1);
				DrawExprTree(root->Content.CaseOperator.Right, layer + 1);
				break;
			case Scan::FUNC:
				for (auto a : Scan::TokenTab) {
					if (a.FuncPtr == root->Content.CaseFunc.MathFuncPtr) {
						std::cout << a.literal_value << std::endl;
						DrawExprTree(root->Content.CaseFunc.Child, layer + 1);
					}
				}
				break;
			case Scan::CONST_ID:
				std::cout << root->Content.CaseConst << std::endl;
				break;
			case Scan::T://T的值有问题
				std::cout << *root->Content.CaseParmPtr <<std::endl;
				break;
			case Scan::POWER:
				std::cout << "**" << std::endl;
				DrawExprTree(root->Content.CaseOperator.Left,layer+1);
				DrawExprTree(root->Content.CaseOperator.Right,layer+1);
			default:
				break;
		}
}

double GetExprValue(ExprNode * root){
	if (root == NULL) return 0.0;
	switch (root->OpCode){
	case Scan::PLUS:
		return GetExprValue(root->Content.CaseOperator.Left) + GetExprValue(root->Content.CaseOperator.Right);
	case Scan::MINUS:
		return GetExprValue(root->Content.CaseOperator.Left) - GetExprValue(root->Content.CaseOperator.Right);
	case Scan::MUL:
		return GetExprValue(root->Content.CaseOperator.Left) * GetExprValue(root->Content.CaseOperator.Right);
	case Scan::DIV:
		return GetExprValue(root->Content.CaseOperator.Left) / GetExprValue(root->Content.CaseOperator.Right);
	case Scan::FUNC:
		return root->Content.CaseFunc.MathFuncPtr(GetExprValue(root->Content.CaseFunc.Child));
	case Scan::CONST_ID:
		return root->Content.CaseConst;
	case Scan::T:
		return *(root->Content.CaseParmPtr);
	default:
		return 0.0;
	}
}

void CalcCoord(ExprNode *xp, ExprNode *yp, double &x_v, double &y_v){
	double local_x, local_y, temp;
	local_x = GetExprValue(xp);
	local_y = GetExprValue(yp);
	local_x *= Scale_x;
	local_y *= Scale_y;
	temp = local_x* cos(Rot_ang) + local_y* sin(Rot_ang);
	local_y = local_y* cos(Rot_ang) - local_y* sin(Rot_ang);
	local_x += Origin_x;
	local_y += Origin_y;
	x_v = local_x;
	y_v = local_y;
}

void DrawDot(unsigned long x, unsigned long y){
	ForPtr[ForCnt]->push_back(Point(x, y));
}

void DrawLoop(double start, double to, double step, ExprNode * xp, ExprNode *yp){

	double x, y;
	for (Parameter = start; Parameter <= to; Parameter += step) {
		CalcCoord(xp, yp, x, y);//算出循环的每一个x y的变幻后的值
		/*if (Parameter == start) {
			MoveToEx(hdc, int(x), int(y), NULL);
		}*/
		DrawDot((unsigned long)x,(unsigned long)y);
	//	SetPixel(hdc, int(x)+1000, int(y), 7);
	}
}

void DrawHtml(std::string filepath) {

	if (ForCnt == 0) {
		throw Error::Syntax_error("没有for语句进行绘制", Scan::LineNo, Scan::token);
	}
	std::ofstream HtmlOut(filepath);
	Point temp(0, 0);
	HtmlOut << "<svg width='800px' height='618px' style=\"border:10px solid rgb(17, 17, 219)\">" << std::endl;
	for (size_t i = 0; i < ForCnt; i++) {
		HtmlOut << "	<polyline points=\"";
		for (auto it = ForPtr[i]->begin(); it != ForPtr[i]->end(); it++) {
			HtmlOut << it->x << ',' << it->y << ' ';
			/*else if (it == ForPtr[i]->end() - 1) {
				HtmlOut << 'Z';
			}*/
		}
		HtmlOut << "\"style = \"fill:none;stroke:black;stroke-width:2\" />\n";
	}
	for (size_t i = 0; i < ForCnt; i++) {
		delete ForPtr[i];
	}
	HtmlOut << "</svg>";
}