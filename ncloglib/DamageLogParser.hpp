#pragma once
#include "LineParser.hpp"
#include <assert.h>
#include <atomic>
#include <functional>
#include <istream>
#include <memory>
#include <optional>
#include <regex>
#include <string>
#include <utility>
#include <vector>

namespace nclog
{
	/*
		Example:
		```
		DAMAGEINFO - Time 1652.204 Damage (Outcast Soldier): WeaponID 1609, DmgID 3809, DmgFac 0.398, Energy 39.765, Effect 2.768, Height 51.000
		DamageCause: Melee!
		DamageType: Direct!
		INS - ENR: 39.765
		DOT - ENR: 2.768, DoT: 4002, Duration: 8.000
		```
	*/
	class DamageLogEntry
	{
	public:
		// "time" is a floating point number in log file, but since we can not
		// exactly compare floating point values in code we transform it to an
		// integer (*1000).
		std::string time;
		std::string who;
		std::vector<std::pair<std::string, std::string>> values; // WeaponID=1609; DmgID=???; DmgFac=0.0; ...
		std::string damageCause; // Heavy!, Melee!, ...
		std::string damageType; // Collision!, Over Time!, ...
		std::vector<std::pair<std::string, std::string>> details;

		std::optional<std::string> getValue(const std::string& key) const
		{
			auto it = std::find_if(std::begin(values), std::end(values), [&key](const auto& v) {
				return v.first.compare(key) == 0;
			});
			return (it != std::end(values)) ? std::make_optional(it->second) : std::nullopt;
		}
	};

	/*
		DamageLogParser reads entries from a log file/input-stream.
	*/
	class DamageLogParser
	{
		DamageLogParser(const DamageLogParser&);

	public:
		using OnNewEntryFunc = std::function<void(std::unique_ptr<DamageLogEntry>)>;
		OnNewEntryFunc onNewEntryFunc;

		DamageLogParser() = default;

		~DamageLogParser() = default;

		void parseStream(std::istream& in)
		{
			while (in.good() && !_stop)
			{
				if (!_lineParser.parseNextLine(in, _line))
				{
					continue;
				}
				_lineParser.trimRight(_line);
				parseLine(_line);
				_line.clear();
			}
			finishEntry(); // eof forces finish of entry
		}

		void parseLine(const std::string& line)
		{
			auto pos = std::string::npos;
			std::smatch m;
			if (line.empty())
			{
				finishEntry();
			}
			else if ((pos = line.find("DAMAGEINFO - Time")) == 0)
			{
				finishEntry();
				assert(!_entry);
				_entry = std::make_unique<DamageLogEntry>();

				auto posBegin = std::string::npos;
				auto posEnd = std::string::npos;

				// time
				posBegin = 18;
				posEnd = line.find_first_of(' ', posBegin);
				assert(posEnd != std::string::npos);
				_entry->time = line.substr(posBegin, posEnd - posBegin);

				// who
				posBegin = line.find('(', posEnd);
				assert(posBegin != std::string::npos);
				posEnd = line.find(')', posBegin);
				assert(posEnd != std::string::npos);
				_entry->who = line.substr(posBegin + 1, posEnd - (posBegin + 1));

				// values
				posBegin = line.find(':', posEnd) + 1;
				posEnd = posBegin;
				for (;;)
				{
					// key
					posBegin = line.find_first_not_of(":, ", posEnd);
					if (posBegin == std::string::npos)
						break;
					posEnd = line.find(' ', posBegin);
					auto key = line.substr(posBegin, posEnd - posBegin);

					// value (can end with ',' or just end of line/string)
					posBegin = line.find_first_not_of(' ', posEnd);
					posEnd = line.find(',', posBegin);
					auto val = line.substr(posBegin, posEnd == std::string::npos ? std::string::npos : posEnd - posBegin);

					_entry->values.push_back(std::make_pair(key, val));
				}
			}
			else if (_entry && (pos = line.find("DamageCause: ")) == 0)
			{
				_entry->damageCause = line.substr(13, std::string::npos);
				//				std::cout << "damageCause: [" << _entry->damageCause << "]" << std::endl;
			}
			else if (_entry && (pos = line.find("DamageType: ")) == 0)
			{
				_entry->damageType = line.substr(12, std::string::npos);
				//				std::cout << "damageType: [" << _entry->damageType << "]" << std::endl;
			}
			else if (_entry && (pos = line.find(":")) != std::string::npos)
			{
				// matches lines like:
				// INS - ENR: 39.765
				// DOT - ENR: 2.768, DoT: 4002, Duration: 8.000
				// INS - FUL: -11.183
			}
		}

	protected:
		void finishEntry()
		{
			if (!_entry)
				return;
			if (onNewEntryFunc)
			{
				onNewEntryFunc(std::move(_entry));
			}
			else
				_entry.reset();
		}

	private:
		std::atomic<int> _stop = false;
		LineParser _lineParser;
		std::string _line;

		// the entry object the parser is currently working on
		std::unique_ptr<DamageLogEntry> _entry;
	};
}
