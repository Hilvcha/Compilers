#include"parser.h"
#include"error.h"
int _main() {
	
	std::string file;
	std::string filepath;
	std::cout << "������Ҫ�����Ĵ����ļ���ַ:" << std::endl;
	std::cin >> filepath;
	std::cout << "������Ҫ�����html�ļ���ַ:" << std::endl;
	//std::cin >> file;
	file = "e:\\out.html";
	try	{
		Parser(filepath);
		DrawHtml(file);
	}
	catch (const Error::Syntax_error& e){
		std::cout <<"Syntax error:"<< e.p <<std::endl<<"��ֹ�ַ�Ϊ��"<< e.err_t << std::endl <<"���������"<< e.Line << std::endl;
	}
	catch (const Error::ID_error& t) {
		std::cout << "ID error:" << t.p << ' ' << t.name << std::endl<<"��ȡ������" << t.Line << std::endl;
	}

	//test("abc");
	system("pause");
 	return 0;
}