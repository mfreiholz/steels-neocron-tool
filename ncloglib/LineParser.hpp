#pragma once
#include <iostream>
#include <string>

namespace nclog
{
	class LineParser
	{
		// number of parsed lines
		size_t _lineCount = 0;

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
						_lineCount++;
						return true;
				}
			}
			return false;
		}

		void trimRight(std::string& line)
		{
			char trimchars[3] = {'\n', '\r', ' '};
			line.erase(line.find_last_not_of(trimchars, std::string::npos, sizeof(trimchars)) + 1);
		}

		size_t getLineCount() const
		{
			return _lineCount;
		}
	};
}
