#include <sstream>
#include <fstream>
#include <assert.h>
#include <ncloglib/LineParser.hpp>
#include <ncloglib/Parser.hpp>

void test_line_parser()
{
	const char* s = "\
Character System: Acceleration 0.641 Speed 0.655 Jump 0.818\r\n\
Character System: Acceleration 0.641 Speed 0.655 Jump 0.818\r\n\
\r\n\
\r\n\
asdf\r\n\
\r\n\
";
	const std::string str(s);
	std::stringstream ss(str);

	LineParser lp;
	int count = 0;
	while (!ss.eof())
	{
		std::string line;
		if (!lp.parseNextLine(ss, line))
			continue;
		count++;
	}
	assert(count == 6);
}

void test_log_parser()
{
}

int main(int argc, char **argv)
{
	test_line_parser();
	test_log_parser();
	return 0;
}