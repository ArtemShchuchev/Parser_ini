#include "SecondaryFunction.h"

// заголовок
void printHeader()
{
	using namespace std;
	setlocale(LC_ALL, "ru_RU.UTF-8");				// задаем русский текст
	//system("chcp UTF-8");				// настраиваем кодировку консоли
	//system("chcp ANSI");				// настраиваем кодировку консоли
	//system("chcp 1251");				// настраиваем кодировку консоли
	//SetConsoleOutputCP(CP_UTF8);
	
	//SetConsoleTitleA("Парсер INI-файлов");
	std::system("cls");
	consoleCol(11);
	cout << "Курсовой проект \"Парсер INI-файлов\"\n"
		<< "-----------------------------------\n" << endl;
	consoleCol();
}

void consoleCol(WORD color) // по умолчанию белый на черном фоне
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
