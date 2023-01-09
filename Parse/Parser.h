#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>
#include <variant>

#include "..\SecondFunk\SecondaryFunction.h"

// создал тип для возврата "variant"
using parseVar_t = std::variant<int, double, std::string>;

class Parser
{
protected:
private:
	std::string fileName;			// имя файла
	std::string varName, secName;	// искомые переменная и секция
	std::vector<std::string> buf;

	void deleteSpace(std::string& str);

	// преобразование строки в число типа <T>
	template <typename T>
	T fromChar(std::string s);
	// специализация для std::string
	template <>
	std::string fromChar(std::string s);
	// печать буфера в консоль
	void printBuf(std::string_view s);
	// убираю лишние, остаются строки содержащие искомую переменную
	void someCleanBuf();

public:
	// принимает rvalue, типа "строка" (временный объект)
	Parser(std::string&& _fileName);
	// принимает lvalue, типа переменная
	Parser(std::string& _fileName);
	// перегружаю вывод в поток для типа <parseVar_t>
	friend std::ostream& operator<< (std::ostream& out, const parseVar_t& var);

	template <typename T>
	parseVar_t get_value(std::string _findStr);

	std::string getSection();
	std::string getVarName();
};



// преобразование строки в число типа <T>
template<typename T>
inline T Parser::fromChar(std::string s)
{
	T value{};
	auto [ptr, ec] { std::from_chars(s.data(), s.data() + s.size(), value) };
	if (ec == std::errc()) return value;
	else if (ec == std::errc::invalid_argument)
	{
		someCleanBuf(); // убираю лишние, остаются строки содержащие искомую переменную
		printBuf("Текущее значение переменной...");
		throw std::runtime_error("(fromChar) Переменная <" + secName + '.' + varName + "> не содержит числа!");
	}
	else if (ec == std::errc::result_out_of_range)
	{
		someCleanBuf(); // убираю лишние, остаются строки содержащие искомую переменную
		printBuf("Текущее значение переменной...");
		throw std::out_of_range("(fromChar) Значение переменной <" + secName + '.' + varName + "> превышает диапазон!");
	}
	else throw std::runtime_error("(fromChar) Не известная ошибка! Переменная <" + secName + '.' + varName + ">!");
}
// специализация для std::string
template <>
std::string Parser::fromChar(std::string s)
{
	return std::move(s);
}

// ищет и возвращает переменную Т типа
template<typename T>
inline parseVar_t Parser::get_value(std::string _findStr)
{
	secName = std::move(_findStr);
	deleteSpace(secName);	// удаляю возможные пробелы в строке поиска

	auto dotPos = secName.find('.');
	if (dotPos != std::string::npos)
	{
		varName = secName.substr(dotPos + 1);
		secName = secName.substr(0, dotPos);
	}
	else throw std::runtime_error("Аргумент get_value, ожидается точка разделитель (секция.переменная)!");
	if (secName.empty())
		throw std::runtime_error("Метод get_value не получил имя секции!");
	if (varName.empty())
		throw std::runtime_error("Метод get_value не получил имя переменной!");


	bool notFindKey_F(true);		// флаг: имя переменной не найдено
	bool notFindSection_F(true);	// флаг: секция не найдена
	std::string varValue;			// строка содержащая значение переменной

	std::ifstream file(fileName);
	if (file.is_open())
	{
		unsigned int lineNr{ 0 };
		if (!buf.empty()) buf.clear();
		std::string line;
		bool findVariable_f(false);	// флаг: искать переменную

		while (std::getline(file, line))
		{
			++lineNr;	// считаю строчки, начиная с 1ой
			if (line.empty()) continue;	// пустые строки, там искать нечего


			// ищу секцию
			// ищу позицию начала комментария
			const auto commentPos = line.find_first_of(';');
			// следующая позиция после '[', если не нашел, то 0, т.к. (-1 + 1)
			const auto openPos = line.find_first_of('[') + 1;
			// если '[' - найдена и находится раньше символа ';'
			if (openPos != 0 && openPos <= commentPos)
			{
				// ищу позицию ']' после '[' + 1, т.е. между [] есть хоть 1 символ
				const auto closePos = line.find_first_of(']', openPos + 1);

				// если ']' - найдена и между [] есть хоть 1 символ
				// и комментарии расположены не внутри []
				if (closePos != std::string::npos && closePos < commentPos)
				{
					std::string section = line.substr(openPos, closePos - openPos);//(позиция, кол-во)

					if (section == secName)	// нашел нужную секцию
					{
						// готовлю буфер для хранения переменных из найденной секции
						if (!buf.empty() && notFindSection_F) buf.clear();
						findVariable_f = true;		// теперь ищу переменную
						notFindSection_F = false;	// секция найдена, сбрасываю флаг
					}
					else
					{
						findVariable_f = false;		// в этой секции переменные НЕ ищем
						// если нужную секцию не нашел, сохраняю список секций в файле
						if (notFindSection_F) buf.push_back(section);
					}
				}
				else
				{
					throw std::runtime_error("Не верный синтаксис в строке: "
						+ std::to_string(lineNr) + " -> " + line);
				}
				continue;	// нахожусь внутри строки содержащей имя секции, больше тут нечего искать
			}


			// если секцию нашел, ищу переменную
			if (findVariable_f)
			{
				// сохраняю строку секции
				auto str = "Строка: " + std::to_string(lineNr) + " ->" + line;
				buf.push_back(str); // сохраняю строку в векторе

				// удаляю комментарии
				line.erase(std::find(line.begin(), line.end(), ';'), line.end());

				// ищу разделитель '='
				const auto delimeterPos = line.find('=');
				if (delimeterPos != std::string::npos)
				{
					// разделитель '=' найден, значит эта строка содержит переменную

					std::string key = line.substr(0, delimeterPos);	// строка содержащая имя переменной
					deleteSpace(key);	// удаляю возможные пробелы в ключе
					if (key == varName) // ключ соответствует искомому?
					{
						notFindKey_F = false;	// переменная нашлась
						// создаю строку от '=' (не включая) до конца строки
						varValue = line.substr(delimeterPos + 1);

						if (varValue.empty()) continue;	// переменная не имеет значения
						// ищу 1ый символ не ' ' и не '\t', и вырезаю их из строки (____Строка)
						varValue = varValue.substr(varValue.find_first_not_of(" \t"));
						// ищу 1ый символ не ' ' и не '\t' с конца строки, и вырезаю их из строки (строкА____)
						varValue = varValue.substr(0, varValue.find_last_not_of(" \t") + 1);
					}
				}
				else
				{
					// эта строка не содержит переменной или тут ошибка синтаксиса
					// возможно это строка с комменариями, поэтому удаляю комментарии
					line.erase(std::find(line.begin(), line.end(), ';'), line.end());
					deleteSpace(line);	// также удудаляю пробелы, табуляцию...
					if (line.empty())	// значит - это строка с комментариями или пустая
					{
						buf.pop_back();	// убираю лишнюю строку
						continue;		// пропускаю
					}
					throw std::runtime_error("Не верный синтаксис. " + buf.back());
				}
			}
		}
		file.close();	// не очень нужная строчка
	}
	else throw std::runtime_error("Не могу открыть файл: " + fileName);

	if (notFindSection_F)
	{
		printBuf("Список найденых секций...");
		throw std::runtime_error("Файл: " + fileName + " не содержит секцию: " + secName);
	}
	if (notFindKey_F)
	{
		printBuf("Список переменных в секции: " + secName);
		throw std::runtime_error("Секция: " + secName + ", не содержит переменную: " + varName + "!");
	}
	if (varValue.empty())
	{
		printBuf("Список переменных в секции: " + secName);
		throw std::runtime_error("Переменная " + secName + '.' + varName + " не имеет значения!");
	}

	// перемещаю std::string, в функцию fromChar<T>
	// которая в свою очередь, отдает результат в
	// std::variant<int, double, std::string>  -> parseVar_t
	return fromChar<T>(std::move(varValue));
}
