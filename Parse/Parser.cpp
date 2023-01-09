#include "Parser.h"

void Parser::deleteSpace(std::string& str)
{
	str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
}

// печать буфера в консоль
void Parser::printBuf(const std::string& s)
{
	consoleCol(14);
	std::cout<< "\n" << s << "\n";
	for (const auto& val : buf) std::cout << val << "\n";
	consoleCol();
}

// убираю лишние, остаютс€ строки содержащие искомую переменную
void Parser::someCleanBuf()
{
	for (auto it = buf.begin(); it != buf.end(); )
	{
		std::string line = *it;
		// удал€ю комментарии
		line.erase(std::find(line.begin(), line.end(), ';'), line.end());
		// удал€ю пробелы
		deleteSpace(line);

		if (line.find(varName) != std::string::npos) ++it;
		else it = buf.erase(it);
	}
}

Parser::Parser(std::string&& _fileName) : fileName(std::move(_fileName))
{
}

Parser::Parser(std::string& _fileName) : fileName(std::move(_fileName))
{
}

// возвращает им€ секции
std::string Parser::getSection()
{
	return secName;
}

// возвращает им€ переменной
std::string Parser::getVarName()
{
	return varName;
}

// перегружаю вывод в поток дл€ типа <parseVar_t>
std::ostream& operator<<(std::ostream& out, const parseVar_t& var)
{
	try
	{

	std::visit([&out](auto&& arg) { out << arg; }, var);
	}
	catch (const std::bad_variant_access& err)
	{
		consoleCol(12);
		std::cout << "\nќшибка! " << err.what() << "\n\n";
		consoleCol();
	}
	return out;
}
