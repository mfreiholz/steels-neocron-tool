#pragma once
#include <iostream>
#include <ncloglib/DamageLog.hpp>

/*

| INDEX | PLAYER | HITZONE | PART_COUNT | PART_INDEX | DAMAGE | DMG_TYPE | HITZONE | DETAIL
-----------------------------------------------------------------------------
| 0     | Local  | ALL     | 2          | 0          | 73.889 | Energie  | 0       |

*/
class DamageLogCsvWriter
{
	std::ostream _out;

public:
	DamageLogCsvWriter(std::ostream&& out)
		: _out(std::move(out))
	{}

	~DamageLogCsvWriter()
	{
		_out.close();
	}

	void writeHeader()
	{
		_out
			<< "INDEX" << ","
			<< "PLAYER" << ","
			<< "HITZONE" << ","
			<< "PART_COUNT" << ","
			<< "PLAYER" << ","

			<< "PART_INDEX" << ","
			<< "DAMAGE" << ","
			<< "DAMAGE" << ","

			_out << "X";
	}

	void writeRow(const DamageHit& dmgHit)
	{

	}
};