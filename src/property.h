/*
 * C++ Simple property parser
 * Author: Felipe Cerqueira - skylazart@gmail.com
 */

#ifndef __PROPERTY_PARSER_H__
#define __PROPERTY_PARSER_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <algorithm>

namespace property
{
	class PropertyParser
	{
	public:
		PropertyParser(std::istream& is)
		{
			parseInputStream(is);
		}

	public:
		const std::unordered_map<std::string, std::string>& getMap() const
		{
			return keys;
		}

		std::string valueOf(const std::string& k) const
		{
			return keys.at(k);
		}

		const std::unordered_map<std::string, std::string>::const_iterator begin() const
		{
			return keys.cbegin();
		}

		const std::unordered_map<std::string, std::string>::const_iterator end() const
		{
			return keys.cend();
		}

	private:
		bool parseInputStream(std::istream& in);
		void parseLine(const std::string& s);
		void parseCommand(const std::string& s);

		std::string escapeChars(std::string& value);
		std::string replaceVars(std::string& s);
		
		

	private:
		std::unordered_map<std::string, std::string> keys;
	};

	// Implementantions

	bool PropertyParser::parseInputStream(std::istream& in)
	{
		std::string acc;
		std::string line;

		while (std::getline(in, line))
		{
			if (line[0] == '#')
				continue;

			if (line.length() > 1 && line.at(line.length() - 1) == '\r')
				line.pop_back();

			if (line.length() == 0)
			{
				if (acc.length() > 0)
				{
					acc += line;
					parseLine(acc);
					acc.clear();
				}
				else
				{
					parseLine(line);
				}
				continue;
			}

			if (line.at(line.length() - 1) == '\\')
			{
				line.pop_back();
				acc += line;
				continue;
			}

			if (!acc.empty())
			{
				parseLine(acc + line);
				acc.clear();
			}
			else
			{
				parseLine(line);
			}
		}

		return true;
	}

	void PropertyParser::parseLine(const std::string& s)
	{
		// Checking for commands
		if (s[0] == '!')
		{
			parseCommand(s);
			return;
		}

		auto idx = s.find_first_of('=');
		if (idx == s.npos)
		{
			// Error
			return;
		}

		auto idxFirstSpace = s.find_first_of(' ');
		std::string key = s.substr(0, std::min(idx, idxFirstSpace));

		idx = s.find_first_not_of('= ', idx + 1);
		if (idx == s.npos)
		{
			// Error
			return;
		}

		std::string value = s.substr(idx);
		keys[key] = replaceVars(value);
	}

	// TODO: Implement commands like !if !else !endif
	void PropertyParser::parseCommand(const std::string& s)
	{

	}

	// TODO: Discard '"' at the beginning/end. Interpret characters preceded by '\'
	std::string PropertyParser::escapeChars(std::string& value)
	{
		return value;
	}

	std::string PropertyParser::replaceVars(std::string& s)
	{
		std::string res;

		size_t idx = 0;

		while (idx < s.length() - 3) {
			auto beginIdx = s.find_first_of("$(", idx);
			if (beginIdx == s.npos)
				break;

			auto endIdx = s.find(')', beginIdx + 2);
			if (endIdx == s.npos)
				break;

			auto key = s.substr(beginIdx, (endIdx - beginIdx));
			key = key.substr(2);

			if (beginIdx > idx)
			{
				res += s.substr(idx, (beginIdx - idx));
			}

			auto value = keys.find(key);
			if (value != keys.end())
			{
				res += value->second;
			} 
			else
			{
				res += "(unknown)";
			}

			idx = endIdx + 1;
		}

		if (idx < s.length())
		{
			res += s.substr(idx);
		}

		return res;
	}
}

#endif // !__PROPERTY_PARSER_H__

