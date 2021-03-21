#include <iostream>

#include <fstream>
#include <thread>
#include <chrono>

static const char* damage_hit_block = "\
\r\n\
Character System: Acceleration 0.641 Speed 0.655 Jump 0.818\r\n\
Local Player: HitZone verification result TORSO\r\n\
\r\n\
Local Player:Damage() - Damage processing statistics!\r\n\
Damage: 109.862 Target Stich HitZone 1 - Part 0\r\n\
Damage: 64.748 (Reduction: 45.114 - 41.064 Percentage) - Damage reduced by player armor\r\n\
Damage: 64.714 (Reduction: 45.148 - 41.095 Percentage) - Damage reduced by player skills\r\n\
Results of this target: Damage 64.714 (Reduction: 45.148 - 41.095 Percentage) - ResistanceCap: 0.840!\r\n\
\r\n\
Local Player: HitZone verification result TORSO\r\n\
\r\n\
Local Player: Damage() - Damage processing statistics!\r\n\
Damage: 109.862 Target Stich HitZone 1 - Part 0\r\n\
Damage: 64.748 (Reduction: 45.114 - 41.064 Percentage) - Damage reduced by player armor\r\n\
Damage: 64.714 (Reduction: 45.148 - 41.095 Percentage) - Damage reduced by player skills\r\n\
Results of this target: Damage 64.714 (Reduction : 45.148 - 41.095 Percentage) - ResistanceCap: 0.840!\r\n\
\r\n\
Character System: Acceleration 0.641 Speed 0.655 Jump 0.818\r\n";

void print_help()
{
	std::cout << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << std::endl;
	std::cout << '\t' << ".exe <file>" << std::endl;
	std::cout << std::endl;
	//std::cout << "Options:" << std::endl;
	//std::cout << std::endl;
	//std::cout << '\t' << "-w" << '\t' << "Watch file" << std::endl;
	//std::cout << std::endl;
}

void write_damagehit(std::ostream& out)
{
	out << damage_hit_block;
	out.flush();
}

struct opts_t
{
	std::string filepath;
	std::chrono::milliseconds interval = std::chrono::milliseconds(250);
};

/*
	Help:
	.exe <file> [-w]

	Examples:
	.exe "D:/Projects/NeocronLogParser/Infinite.log"
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
	std::ofstream out(opts.filepath, std::ios_base::binary | std::ios_base::ate);
	if (!out)
	{
		std::cerr << "can not open file (check file path and permissions!): " << opts.filepath << std::endl;
		return 2;
	}

	std::cout << "----------------------------" << std::endl;
	std::cout << "infinite-damage-log-writer; version=1.0.0; author=Manuel Freiholz <mfreiholz@gmail.com>;" << std::endl;
	std::cout << "file=" << opts.filepath << ";" << std::endl;
	std::cout << "----------------------------" << std::endl;

	while (!out.bad())
	{
		write_damagehit(out);
		std::this_thread::sleep_for(opts.interval);
	}
	out.close();
	return 0;
}