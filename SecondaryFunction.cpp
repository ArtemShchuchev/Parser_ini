#include "SecondaryFunction.h"

// ���������
void printHeader()
{
	using namespace std;
	setlocale(LC_ALL, "Russian");   // ����� ������� �����
	system("chcp 1251");            // ����������� ��������� �������
	SetConsoleTitleA("������ INI-������");
	std::system("cls");
	consoleCol(11);
	cout << "�������� ������ ������� INI-������\n"
		<< "-----------------------------------\n" << endl;
	consoleCol();
}

void consoleCol(WORD color) // �� ��������� ����� �� ������ ����
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
