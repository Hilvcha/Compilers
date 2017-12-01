#include<fstream>
#include<string>
#include"scanner.h"
int main() {
	std::string file_path;
	std::cout << "please input your file path" << std::endl;
	std::cin >> file_path;
	std::ifstream infile(file_path);
	
}