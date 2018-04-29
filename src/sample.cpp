/* 
 * C++ Simple property parser
 * Author: Felipe Cerqueira - skylazart@gmail.com
 */

#include <iostream>
#include <sstream>
#include <istream>
#include <iterator>
#include <unordered_map>
#include "property.h"

using namespace property;

int main(int argc, char *argv[])
{
	std::stringstream ss;

	ss << "option1 = 1" << "\n";
	ss << "option2 = 2" << "\n";
	ss << "option3=$(option1)+$(option2) = 3";

	std::istream& is = ss;

	PropertyParser properties(is);

	for (auto it = std::cbegin(properties); it != std::cend(properties); it++)
	{
		std::cout << it->first << " = " << it->second << "\n";
	}

	for (auto kv : properties.getMap())
	{
		std::cout << kv.first << " = " << kv.second << "\n";
	}

	std::cout << properties.valueOf("option3") << "\n";

	return 0;
}
