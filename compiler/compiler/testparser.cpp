#include"parser.h"
#include"error.h"
int _main() {
	
	std::string file;
	std::string filepath;
	std::cout << "输入您要分析的代码文件地址:" << std::endl;
	std::cin >> filepath;
	std::cout << "输入你要输出的html文件地址:" << std::endl;
	//std::cin >> file;
	file = "e:\\out.html";
	try	{
		Parser(filepath);
		DrawHtml(file);
	}
	catch (const Error::Syntax_error& e){
		std::cout <<"Syntax error:"<< e.p <<std::endl<<"终止字符为："<< e.err_t << std::endl <<"语句行数："<< e.Line << std::endl;
	}
	catch (const Error::ID_error& t) {
		std::cout << "ID error:" << t.p << ' ' << t.name << std::endl<<"读取行数：" << t.Line << std::endl;
	}

	//test("abc");
	system("pause");
 	return 0;
}