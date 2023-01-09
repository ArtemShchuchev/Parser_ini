#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>
#include <variant>

#include "..\SecondFunk\SecondaryFunction.h"

// ������ ��� ��� �������� "variant"
using parseVar_t = std::variant<int, double, std::string>;

class Parser
{
protected:
private:
	std::string fileName;			// ��� �����
	std::string varName, secName;	// ������� ���������� � ������
	std::vector<std::string> buf;

	void deleteSpace(std::string& str);

	// �������������� ������ � ����� ���� <T>
	template <typename T>
	T fromChar(std::string s);
	// ������������� ��� std::string
	template <>
	std::string fromChar(std::string s);
	// ������ ������ � �������
	void printBuf(std::string_view s);
	// ������ ������, �������� ������ ���������� ������� ����������
	void someCleanBuf();

public:
	// ��������� rvalue, ���� "������" (��������� ������)
	Parser(std::string&& _fileName);
	// ��������� lvalue, ���� ����������
	Parser(std::string& _fileName);
	// ���������� ����� � ����� ��� ���� <parseVar_t>
	friend std::ostream& operator<< (std::ostream& out, const parseVar_t& var);

	template <typename T>
	parseVar_t get_value(std::string _findStr);

	std::string getSection();
	std::string getVarName();
};



// �������������� ������ � ����� ���� <T>
template<typename T>
inline T Parser::fromChar(std::string s)
{
	T value{};
	auto [ptr, ec] { std::from_chars(s.data(), s.data() + s.size(), value) };
	if (ec == std::errc()) return value;
	else if (ec == std::errc::invalid_argument)
	{
		someCleanBuf(); // ������ ������, �������� ������ ���������� ������� ����������
		printBuf("������� �������� ����������...");
		throw std::runtime_error("(fromChar) ���������� <" + secName + '.' + varName + "> �� �������� �����!");
	}
	else if (ec == std::errc::result_out_of_range)
	{
		someCleanBuf(); // ������ ������, �������� ������ ���������� ������� ����������
		printBuf("������� �������� ����������...");
		throw std::out_of_range("(fromChar) �������� ���������� <" + secName + '.' + varName + "> ��������� ��������!");
	}
	else throw std::runtime_error("(fromChar) �� ��������� ������! ���������� <" + secName + '.' + varName + ">!");
}
// ������������� ��� std::string
template <>
std::string Parser::fromChar(std::string s)
{
	return std::move(s);
}

// ���� � ���������� ���������� � ����
template<typename T>
inline parseVar_t Parser::get_value(std::string _findStr)
{
	secName = std::move(_findStr);
	deleteSpace(secName);	// ������ ��������� ������� � ������ ������

	auto dotPos = secName.find('.');
	if (dotPos != std::string::npos)
	{
		varName = secName.substr(dotPos + 1);
		secName = secName.substr(0, dotPos);
	}
	else throw std::runtime_error("�������� get_value, ��������� ����� ����������� (������.����������)!");
	if (secName.empty())
		throw std::runtime_error("����� get_value �� ������� ��� ������!");
	if (varName.empty())
		throw std::runtime_error("����� get_value �� ������� ��� ����������!");


	bool notFindKey_F(true);		// ����: ��� ���������� �� �������
	bool notFindSection_F(true);	// ����: ������ �� �������
	std::string varValue;			// ������ ���������� �������� ����������

	std::ifstream file(fileName);
	if (file.is_open())
	{
		unsigned int lineNr{ 0 };
		if (!buf.empty()) buf.clear();
		std::string line;
		bool findVariable_f(false);	// ����: ������ ����������

		while (std::getline(file, line))
		{
			++lineNr;	// ������ �������, ������� � 1��
			if (line.empty()) continue;	// ������ ������, ��� ������ ������


			// ��� ������
			// ��� ������� ������ �����������
			const auto commentPos = line.find_first_of(';');
			// ��������� ������� ����� '[', ���� �� �����, �� 0, �.�. (-1 + 1)
			const auto openPos = line.find_first_of('[') + 1;
			// ���� '[' - ������� � ��������� ������ ������� ';'
			if (openPos != 0 && openPos <= commentPos)
			{
				// ��� ������� ']' ����� '[' + 1, �.�. ����� [] ���� ���� 1 ������
				const auto closePos = line.find_first_of(']', openPos + 1);

				// ���� ']' - ������� � ����� [] ���� ���� 1 ������
				// � ����������� ����������� �� ������ []
				if (closePos != std::string::npos && closePos < commentPos)
				{
					std::string section = line.substr(openPos, closePos - openPos);//(�������, ���-��)

					if (section == secName)	// ����� ������ ������
					{
						// ������� ����� ��� �������� ���������� �� ��������� ������
						if (!buf.empty() && notFindSection_F) buf.clear();
						findVariable_f = true;		// ������ ��� ����������
						notFindSection_F = false;	// ������ �������, ��������� ����
					}
					else
					{
						findVariable_f = false;		// � ���� ������ ���������� �� ����
						// ���� ������ ������ �� �����, �������� ������ ������ � �����
						if (notFindSection_F) buf.push_back(section);
					}
				}
				else
				{
					throw std::runtime_error("�� ������ ��������� � ������: "
						+ std::to_string(lineNr) + " -> " + line);
				}
				continue;	// �������� ������ ������ ���������� ��� ������, ������ ��� ������ ������
			}


			// ���� ������ �����, ��� ����������
			if (findVariable_f)
			{
				// �������� ������ ������
				auto str = "������: " + std::to_string(lineNr) + " ->" + line;
				buf.push_back(str); // �������� ������ � �������

				// ������ �����������
				line.erase(std::find(line.begin(), line.end(), ';'), line.end());

				// ��� ����������� '='
				const auto delimeterPos = line.find('=');
				if (delimeterPos != std::string::npos)
				{
					// ����������� '=' ������, ������ ��� ������ �������� ����������

					std::string key = line.substr(0, delimeterPos);	// ������ ���������� ��� ����������
					deleteSpace(key);	// ������ ��������� ������� � �����
					if (key == varName) // ���� ������������� ��������?
					{
						notFindKey_F = false;	// ���������� �������
						// ������ ������ �� '=' (�� �������) �� ����� ������
						varValue = line.substr(delimeterPos + 1);

						if (varValue.empty()) continue;	// ���������� �� ����� ��������
						// ��� 1�� ������ �� ' ' � �� '\t', � ������� �� �� ������ (____������)
						varValue = varValue.substr(varValue.find_first_not_of(" \t"));
						// ��� 1�� ������ �� ' ' � �� '\t' � ����� ������, � ������� �� �� ������ (������____)
						varValue = varValue.substr(0, varValue.find_last_not_of(" \t") + 1);
					}
				}
				else
				{
					// ��� ������ �� �������� ���������� ��� ��� ������ ����������
					// �������� ��� ������ � ������������, ������� ������ �����������
					line.erase(std::find(line.begin(), line.end(), ';'), line.end());
					deleteSpace(line);	// ����� �������� �������, ���������...
					if (line.empty())	// ������ - ��� ������ � ������������� ��� ������
					{
						buf.pop_back();	// ������ ������ ������
						continue;		// ���������
					}
					throw std::runtime_error("�� ������ ���������. " + buf.back());
				}
			}
		}
		file.close();	// �� ����� ������ �������
	}
	else throw std::runtime_error("�� ���� ������� ����: " + fileName);

	if (notFindSection_F)
	{
		printBuf("������ �������� ������...");
		throw std::runtime_error("����: " + fileName + " �� �������� ������: " + secName);
	}
	if (notFindKey_F)
	{
		printBuf("������ ���������� � ������: " + secName);
		throw std::runtime_error("������: " + secName + ", �� �������� ����������: " + varName + "!");
	}
	if (varValue.empty())
	{
		printBuf("������ ���������� � ������: " + secName);
		throw std::runtime_error("���������� " + secName + '.' + varName + " �� ����� ��������!");
	}

	// ��������� std::string, � ������� fromChar<T>
	// ������� � ���� �������, ������ ��������� �
	// std::variant<int, double, std::string>  -> parseVar_t
	return fromChar<T>(std::move(varValue));
}
