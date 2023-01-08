#include <iostream>
#include <any>
#include <typeinfo>

#include "SecondaryFunction.h"
#include "Parser.h"

template <class T>
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
		std::any varValue;
		

		{
			using myType_t = double;
			varValue = parser.get_value<myType_t>("Section1.var1");
			prinToCons<myType_t>(parser, varValue);


			std::cout << varValue.type().name() << "\n\n";
		}

		{
			using myType_t = int;
			varValue = parser.get_value<myType_t>("Section2.var1");
			prinToCons<myType_t>(parser, varValue);
		}

		{
			using myType_t = std::string;
			varValue = parser.get_value<myType_t>("Section2.var2");
			prinToCons<myType_t>(parser, varValue);
		}

		using myType_t = double;
		varValue = parser.get_value<myType_t>("Section1.var2");
		prinToCons<myType_t>(parser, varValue);
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

template <class T>
void prinToCons(Parser& par, const std::any& var)
{
	std::cout << "  <"
		<< par.getSection() << "."
		<< par.getVarName() << "="
		<< std::any_cast<T>(var) << ">\n";
}
