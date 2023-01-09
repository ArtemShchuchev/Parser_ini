#include <iostream>

#include "SecondFunk/SecondaryFunction.h"
#include "Parse/Parser.h"


int main(int argc, char** argv)
{
	printHeader();
	/*
	if (argc != 4)
	{
		std::cout << "Использование: " << argv[0] << " <file.ini> <section> <parameter>" << std::endl;
		return 0;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << "Не могу открыть файл \"" << argv[1] << '\"' << std::endl;
		return 1;
	}
	*/
	try
	{
		// лямбда для вывода в консоль
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
		std::cout << "\nОшибка! " << err.what() << "\n\n";
		consoleCol();
	}
	catch (const std::bad_alloc& err)
	{
		// обработка ошибки выделения памяти
		consoleCol(12);
		std::cout << "\nОшибка! " << err.what() << "\n\n";
		consoleCol();
	}
	catch (const std::runtime_error& err)
	{
		consoleCol(12);
		std::cout << "\nОшибка! " << err.what() << "\n\n";
		consoleCol();
	}
	catch (const std::bad_variant_access& err)
	{
		consoleCol(12);
		std::cout << "\nОшибка! " << err.what() << "\n\n";
		consoleCol();
	}
	catch (...)
	{
		consoleCol(12);
		std::cout << "\nКакая-то ошибка!" << "\n\n";
		consoleCol();
	}

	return 0;
}
