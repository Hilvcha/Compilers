#include<fstream>
#include<iostream>
#include<cstdio>
#include<string>
#include"scanner.h"
int _main() {
	std::string file_path;
	std::cout << "please input your file path" << std::endl;
	std::cin >> file_path;
	std::ifstream infile(file_path);
	std::ofstream outfile("e:\\out.txt");
	struct Token token;
	char head[][10] = { "�Ǻ�����","�ַ���","����ֵ","����ָ��","������" };
	printf("\t%s\t%s\t%s\t%s\t%s\n",head[0],head[1],head[2],head[3],head[4]);
	printf("======================================================================\n");
	while (token = GetToken(infile), token.type != NONTOKEN) {
		outfile << int(token.type) << std::ends << token.literal_value << std::ends << token.value << std::endl;
		printf("\t%d\t%s\t%lf\t%p\n", token.type, token.literal_value.c_str(), token.value, token.FuncPtr);
	}
	getchar();
	getchar();
	return 0;
}