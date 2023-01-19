#include "SecondFunk/SecondaryFunction.h"
#include "Parse/Parser.h"


int main(int argc, char** argv)
{
	printHeader("Курсовой проект \"Парсер INI-файлов\"");
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
	catch (const std::exception& err)
	{
		consoleCol(col::br_red);
		std::cout << "\nОшибка типа: " << typeid(err).name() << "\n";
		std::cout << err.what() << "\n";
		consoleCol(col::cancel);
	}

	return 0;
}
