#include<fstream>
#include<iostream>
#include<cstdio>
#include<string>
#include"scanner.h"
#include"error.h"
int main() {
	try {
		std::string file_path;
		std::cout << "please input your file path" << std::endl;
		std::cin >> file_path;
		std::ifstream infile(file_path);
		std::ofstream outfile("e:\\out.txt");
		Scan::Token token;
		char head[][10] = { "记号类型","字符串","常数值","函数指针","所在行" };
		printf("\t%s %s\t%s\t\t%s\t%s\n",head[0],head[1],head[2],head[3],head[4]);
		printf("======================================================================\n");
		while (token = Scan::GetToken(infile), token.type != Scan::NONTOKEN) {
			outfile << int(token.type) << std::ends << token.literal_value << std::ends << token.value << std::endl;
			printf("\t%d\t%s\t%lf\t%p\t%u\n", token.type, token.literal_value.c_str(), token.value, token.FuncPtr,Scan::LineNo);
		}
	}
	catch( const Error::ID_error t){
		std::cout << "ID error:" << t.p << ' ' << t.name << std::endl << "读取行数：" << t.Line << std::endl;
	}
	
	getchar();
	getchar();
	return 0;
}