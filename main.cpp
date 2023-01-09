#include <iostream>

#include "SecondFunk/SecondaryFunction.h"
#include "Parse/Parser.h"


int main(int argc, char** argv)
{
	printHeader();
	/*
	if (argc != 4)
	{
		std::cout << "�������������: " << argv[0] << " <file.ini> <section> <parameter>" << std::endl;
		return 0;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << "�� ���� ������� ���� \"" << argv[1] << '\"' << std::endl;
		return 1;
	}
	*/
	try
	{
		// ������ ��� ������ � �������
		auto print_lambda = [](Parser& par, const parseVar_t& var)
		{
			std::cout << "  <"
				<< par.getSection() << "."
				<< par.getVarName() << "="
				<< var << ">\n";
		};

		Parser parser("../test2.ini");

		auto varValue = parser.get_value<double>("Section1.var1");
		print_lambda(parser, varValue);

		varValue = parser.get_value<int>("Section2.var1");
		print_lambda(parser, varValue);

		varValue = parser.get_value<std::string>("Section2.var2");
		print_lambda(parser, varValue);

		varValue = parser.get_value<double>("Section1.var2");
		print_lambda(parser, varValue);
	}
	catch (const std::out_of_range& err)
	{
		consoleCol(12);
		std::cout << "\n������! " << err.what() << "\n\n";
		consoleCol();
	}
	catch (const std::bad_alloc& err)
	{
		// ��������� ������ ��������� ������
		consoleCol(12);
		std::cout << "\n������! " << err.what() << "\n\n";
		consoleCol();
	}
	catch (const std::runtime_error& err)
	{
		consoleCol(12);
		std::cout << "\n������! " << err.what() << "\n\n";
		consoleCol();
	}
	catch (const std::bad_variant_access& err)
	{
		consoleCol(12);
		std::cout << "\n������! " << err.what() << "\n\n";
		consoleCol();
	}
	catch (...)
	{
		consoleCol(12);
		std::cout << "\n�����-�� ������!" << "\n\n";
		consoleCol();
	}

	return 0;
}
