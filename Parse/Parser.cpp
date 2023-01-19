#include "Parser.h"

void Parser::deleteSpace(std::string& str)
{
	str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
}

// печать буфера в консоль
void Parser::printBuf(std::string_view s)
{
	consoleCol(col::br_yellow);
	std::cout<< "\n" << s << "\n";
	for (const auto& val : buf) std::cout << val << "\n";
	consoleCol(col::cancel);
}

// убираю лишние, остаются строки содержащие искомую переменную
void Parser::someCleanBuf()
{
	for (auto it = buf.begin(); it != buf.end(); )
	{
		std::string line = *it;
		// удаляю комментарии
		line.erase(std::find(line.begin(), line.end(), ';'), line.end());
		// удаляю пробелы
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

// возвращает имя секции
std::string Parser::getSection()
{
	return secName;
}

// возвращает имя переменной
std::string Parser::getVarName()
{
	return varName;
}

// перегружаю вывод в поток для типа <parseVar_t>
std::ostream& operator<<(std::ostream& out, const parseVar_t& var)
{
	std::visit([&out](auto&& arg) { out << arg; }, var);
	return out;
}
