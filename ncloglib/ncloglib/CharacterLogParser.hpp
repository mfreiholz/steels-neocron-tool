#pragma once
#include <iostream>
#include <string>
#include <regex>
#include <functional>
#include <atomic>
#include <assert.h>
#include <ncloglib/LineParser.hpp>
#include <ncloglib/DamageHit.hpp>

namespace nclog
{
	class CharacterLogParser
	{
		using OnNewCharacterSystemFunc = std::function<void()>;
		using OnNewDamageHitFunc = std::function<void(std::unique_ptr<DamageHit>)>;

		const std::string LINE_CHARACTER_SYSTEM = "Character System:";
		const std::regex MATCH_RX_DAMAGE_HIT_PART_BEGIN = std::regex("^Damage: ([0-9\\.]+) Target ([A-Za-z]+) HitZone ([0-9]+) \\- Part ([0-9]+)$", std::regex::optimize | std::regex::ECMAScript);
		const std::regex MATCH_RX_DAMAGE_HIT_PART_DETAIL = std::regex("^Damage: ([0-9\\.]+) \\(Reduction: ([0-9\\.]+) - ([0-9\\.]+) Percentage\\) - Damage (caused|reduced) by ([A-Za-z ]+)$", std::regex::optimize | std::regex::ECMAScript);
		const std::regex MATCH_RX_DAMAGE_HIT_PART_END = std::regex("^Results of this target: Damage ([0-9\\.]+) \\(Reduction: ([0-9\\.]+) - ([0-9\\.]+) Percentage\\) \\- ResistanceCap: ([0-9\\.]+)!$", std::regex::optimize | std::regex::ECMAScript);

		OnNewCharacterSystemFunc _onNewCharacterSystem;
		OnNewDamageHitFunc _onNewDamageHit;

		std::atomic<int> _stop = false;
		LineParser _lineParser;
		std::string _line;

		int _lineCharacterSystemCount = 0;
		int _lineDamageHitCount = 0;

		std::unique_ptr<DamageHit> _dmgHit;

		CharacterLogParser(const CharacterLogParser&);

	public:
		CharacterLogParser(
			OnNewCharacterSystemFunc charSys,
			OnNewDamageHitFunc dmgHit
		)
			: _onNewCharacterSystem(charSys)
			, _onNewDamageHit(dmgHit)
		{
		}

		~CharacterLogParser() = default;

		void parseStream(std::istream& in)
		{
			while (in.good() && !_stop)
			{
				if (!_lineParser.parseNextLine(in, _line))
					continue;

				trimRight(_line);
				parseLine(_line);
				_line.clear();
			}
		}

		// parses a single line and updates internal states from it.
		void parseLine(const std::string& line)
		{
			size_t pos = -1;
			std::smatch m;
			if (line.empty())
			{
				return;
			}
			else if ((pos = line.find(LINE_CHARACTER_SYSTEM)) != std::string::npos)
			{
				// finish previous one.
				if (_dmgHit)
				{
					finishDamageHit();
				}
				_lineCharacterSystemCount += 1;
				if (_onNewCharacterSystem)
				{
					_onNewCharacterSystem();
				}
			}
			// begin of a new DamagePart
			else if (std::regex_match(line, m, MATCH_RX_DAMAGE_HIT_PART_BEGIN))
			{
				DamagePart dmgPart;
				dmgPart.value = std::stod(m.str(1));
				dmgPart.damageType = m.str(2);
				dmgPart.hitZone = std::stoi(m.str(3));
				dmgPart.partIndex = std::stoi(m.str(4));

				if (dmgPart.partIndex == 0)
				{
					if (_dmgHit)
					{
						finishDamageHit();
					}
					_dmgHit = std::make_unique<DamageHit>();
					_dmgHit->damageParts.push_back(dmgPart);
				}
				else if (dmgPart.partIndex > 0 && _dmgHit)
				{
					_dmgHit->damageParts.push_back(dmgPart);
				}
				else
				{
					std::cerr << "ERROR: Missing _dmgHit object! This should never happen!" << std::endl;
				}
			}
			// work on current "DamageHit"
			// @note if there is a "character:" between parts, we may loose data from current DamageHit.
			else if (_dmgHit)
			{
				if (std::regex_match(line, m, MATCH_RX_DAMAGE_HIT_PART_DETAIL))
				{
					assert(_dmgHit->damageParts.size() > 0);
					DamagePart::Detail detail;
					detail.value = std::stod(m.str(1));
					detail.reduction = std::stod(m.str(2));
					detail.reductionPercentage = std::stod(m.str(3));
					detail.causedOrReduced = m.str(4);
					detail.source = m.str(5);
					_dmgHit->damageParts.at(_dmgHit->damageParts.size() - 1).details.push_back(detail);
				}
				else if (std::regex_match(line, m, MATCH_RX_DAMAGE_HIT_PART_END))
				{
					assert(_dmgHit->damageParts.size() > 0);
					DamagePart::Result result;
					result.value = std::stod(m.str(1));
					result.reduction = std::stod(m.str(2));
					result.reductionPercentage = std::stod(m.str(3));
					result.resistanceCap = std::stod(m.str(4));
					_dmgHit->damageParts.at(_dmgHit->damageParts.size() - 1).result = result;
				}
			}
		}

		void stop()
		{
			_stop = true;
		}

		int getCharacterSystemMessageCount() const
		{
			return _lineCharacterSystemCount;
		}

		int getDamageHitMessageCount() const
		{
			return _lineDamageHitCount;
		}

	private:
		void trimRight(std::string& line)
		{
			char trimchars[3] = { '\n', '\r', ' ' };
			line.erase(line.find_last_not_of(trimchars, std::string::npos, sizeof(trimchars)) + 1);
		}

		void finishDamageHit()
		{
			assert(_dmgHit);
			if (_onNewDamageHit)
			{
				_onNewDamageHit(std::move(_dmgHit));
			}
			else
			{
				_dmgHit.reset();
			}
			assert(!_dmgHit);
		}
	};
}