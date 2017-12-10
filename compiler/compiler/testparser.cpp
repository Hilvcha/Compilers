#include"parser.h"

int main() {
	std::cout << "please input your file path:" << std::endl;
	std::string filepath="e:\\in.txt";
	//std::cin >> filepath;
	try	{
		Parser(filepath);

	}
	catch (const Syntax_error& e){
		std::cout <<"Syntax error:"<< e.p <<std::endl<<"��ֹ�ַ�Ϊ��"<< e.err_t << std::endl <<"���������"<< e.Line << std::endl;
	}
	catch (const ID_error& t) {
		std::cout << "ID error:" << t.p << ' ' << t.name << std::endl<<"��ȡ������" << t.Line << std::endl;
	}
	system("pause");
 	return 0;
}