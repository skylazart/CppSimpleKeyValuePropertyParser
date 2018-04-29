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
		PropertyParser(std::string& filename)
		{
			std::ifstream ifs(filename);
			if (!ifs.is_open())
			{
				throw std::runtime_error("Error opening property file");
			}

			std::string str((std::istreambuf_iterator<char>(ifs)),
							std::istreambuf_iterator<char>());

			std::istringstream ss {str};

			parser(ss);
		}

		PropertyParser(std::string str)
		{
			std::istringstream ss {str};
			parser(ss);
		}

	private:

		bool parser(std::istream& in)
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
						parse(acc);
						acc.clear();
					}
					else
					{
						parse(line);
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
					parse(acc + line);
					acc.clear();
				}
				else
				{
					parse(line);
				}
			}

			return true;
		}

		void parseCommand(const std::string& s)
		{

		}

		void parse(const std::string& s)
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

			auto it = keys.find(key);
			if (it != keys.end())
				std::cout << it->first << '=' << it->second << "\n";
		}

		std::string escapeChars(std::string& value)
		{
			return value;
		}

		std::string replaceVars(std::string& s)
		{
			std::string res;

			size_t idx = 0;
			
			while (idx < s.length() - 3) {
				auto begin = s.find_first_of("$(", idx);
				if (begin == s.npos)
					break;

				auto end = s.find(')', begin + 2);
				if (end == s.npos)
					break;

				begin += 2;

				auto k = s.substr(begin, (end - begin));
				res += s.substr(idx, (begin - begin));

				auto v = keys.find(k);
				if (v != keys.end())
					res += v->second;

				idx = end + 1;
			}

			res += s.substr(idx);
			
			return res;
		}

		
		std::unordered_map<std::string, std::string>::const_iterator begin() const
		{
			return keys.begin();
		}

		std::unordered_map<std::string, std::string>::const_iterator end() const
		{
			keys.end();
		}


	private:
		std::unordered_map<std::string, std::string> keys;
	};
}

#endif // !__PROPERTY_PARSER_H__

