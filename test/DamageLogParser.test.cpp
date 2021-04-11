#include <assert.h>
#include <fstream>
#include <ncloglib/DamageLogParser.hpp>

int main(int, char**)
{
	// open file
	std::ifstream in("D:/Projects/NeocronLogParser/Damage_00.log", std::ifstream::binary);
	const auto inExpectedCount = 1947;

	auto counter = 0;
	nclog::DamageLogParser parser;
	parser.onNewEntryFunc = [&counter](std::unique_ptr<nclog::DamageLogEntry>){
		counter++;
	};
	parser.parseStream(in);
	in.close();

	assert(counter == inExpectedCount);
	return 0;
}