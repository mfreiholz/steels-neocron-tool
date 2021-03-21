#include <iostream>
#include <ncloglib/Parser.hpp>

#include <fstream>
#include <thread>
#include <chrono>

void print_help()
{
	std::cout << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << std::endl;
	std::cout << '\t' << "damage-hit-logger.exe <file> [-w]" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << std::endl;
	std::cout << '\t' << "-w" << '\t' << "Watch file" << std::endl;
	std::cout << std::endl;
}

bool file_exists(const std::string& filepath)
{
	std::ifstream ifs(filepath);
	return ifs.good();
}

bool has_arg(int argc, char** argv, const std::string& s)
{
	return true;
}

struct opts_t
{
	std::string filepath;
	bool watch = false;
};

/*
	Help:
	damage-hit-logger.exe <file> [-w]

	Examples:
	damage-hit-logger.exe "D:/Projects/NeocronLogParser/Mr Steel_01.log"
*/
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		print_help();
		return 1;
	}

	opts_t opts;
	opts.filepath = std::string(argv[1]);
	opts.watch = has_arg(argc, argv, "-w");
	if (!file_exists(opts.filepath))
	{
		std::cerr << "can not open file (check file path!): " << opts.filepath << std::endl;
		return 2;
	}

	std::cout << "----------------------------" << std::endl;
	std::cout << "damage-hit-logger; version=1.0.0; author=Manuel Freiholz <mfreiholz@gmail.com>;" << std::endl;
	std::cout << "file=" << opts.filepath << ";" << std::endl;
	std::cout << "----------------------------" << std::endl;

	Parser parser(
		// on character system info
		[]() {
		},
		// on damage hit
		[](std::unique_ptr<DamageHit> v)
		{
			std::cout << "DamageHit: " << v->hitZone << std::endl;
		}
	);

	bool stop = false;
	std::streampos offset = -1;
	while (!stop)
	{
		std::ifstream in(opts.filepath, std::ios_base::binary);
		in.seekg(0, std::ios::end);
		const auto fileSize = in.tellg();

		if (offset < 0)
			in.seekg(0, std::ios::beg);
		else
			in.seekg(offset);
		offset = fileSize;
		std::cout << "offset: " << offset << std::endl;

		parser.parseStream(in);
		in.close();

		if (!opts.watch)
		{
			stop = true;
			break;
		}
		std::cout << "Wait 1s for log file updates..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
	return 0;
}