#include "SecondaryFunction.h"

// ���������
void printHeader()
{
	using namespace std;
	setlocale(LC_ALL, "");				// ������ ������� �����
	//system("chcp UTF-8");				// ����������� ��������� �������
	system("chcp ANSI");				// ����������� ��������� �������
	//system("chcp 1251");				// ����������� ��������� �������
	//SetConsoleOutputCP(CP_UTF8);
	
	//SetConsoleTitleA("������ INI-������");
	std::system("cls");
	consoleCol(11);
	cout << "�������� ������ \"������ INI-������\"\n"
		<< "-----------------------------------\n" << endl;
	consoleCol();
}

void consoleCol(WORD color) // �� ��������� ����� �� ������ ����
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
