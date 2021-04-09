#pragma once
#include <string>
#include <iostream>

namespace nclog
{
	class LineParser
	{
	public:
		bool parseNextLine(std::istream& in, std::string& line)
		{
			while (in.good())
			{
				char c;
				if (!in.get(c).good())
					return false;
				line += c;

				switch (c)
				{
				case '\n':
					return true;
				}
			}
			return false;
		}
	};
}