#include "SecondaryFunction.h"

// заголовок
void printHeader()
{
	using namespace std;
	setlocale(LC_ALL, "Russian");   // задаём русский текст
	system("chcp 1251");            // настраиваем кодировку консоли
	SetConsoleTitleA("Парсер INI-файлов");
	std::system("cls");
	consoleCol(11);
	cout << "Курсовой проект «Парсер INI-файлов»\n"
		<< "-----------------------------------\n" << endl;
	consoleCol();
}

void consoleCol(WORD color) // по умолчанию белый на черном фоне
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
