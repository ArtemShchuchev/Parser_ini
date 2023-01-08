#include <iostream>
#include <any>
#include <typeinfo>

#include "SecondaryFunction.h"
#include "Parser.h"

void prinToCons(Parser&, const std::any&);

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
		Parser parser("../test2.ini");

		std::any varValue = parser.get_value<double>("Section1.var1");
		prinToCons(parser, varValue);

		varValue = parser.get_value<int>("Section2.var1");
		prinToCons(parser, varValue);

		varValue = parser.get_value<std::string>("Section2.var2");
		prinToCons(parser, varValue);

		varValue = parser.get_value<double>("Section1.var2");
		prinToCons(parser, varValue);
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

	return 0;
}

void prinToCons(Parser& par, const std::any& var)
{
	std::cout << "  <"
		<< par.getSection() << "."
		<< par.getVarName() << "=";
	
	if (var.type() == typeid(double)) std::cout << std::any_cast<double>(var);
	else if (var.type() == typeid(int)) std::cout << std::any_cast<int>(var);
	else if (var.type() == typeid(std::string)) std::cout << std::any_cast<std::string>(var);
	else
	{
		std::string err = "Функция: prinToCons, не хватает поддержки типа: ";
		err += var.type().name();
		throw std::runtime_error(err);
	}
	
	std::cout << ">\n";
}
