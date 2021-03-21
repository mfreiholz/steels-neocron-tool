#pragma once
#include <vector>
#include <iostream>
#include "DamageType.hpp"
#include "PlayerType.hpp"
#include "HitZone.hpp"
#include "ResistSource.hpp"

/*
Local Player:Damage() - Damage processing statistics!

Damage: 18.178 Target Feuer HitZone 3 - Part 0
Damage: 12.724 (Reduction: 5.453 - 30.000 Percentage) - Damage caused by PSI
Damage: 7.635 (Reduction: 10.543 - 57.998 Percentage) - Damage reduced by shield
Damage: 4.467 (Reduction: 13.711 - 75.427 Percentage) - Damage reduced by player armor
Damage: 4.463 (Reduction: 13.714 - 75.445 Percentage) - Damage reduced by player skills
Results of this target: Damage 4.463 (Reduction: 13.714 - 75.445 Percentage) - ResistanceCap: 0.840!
*/

class DamagePart
{
public:
	class Detail
	{
	public:
		double value = 0.0;
		double reduction = 0.0;
		double reductionPercentage = 0.0;
		std::string causedOrReduced;
		std::string source;
	};

	class Result
	{
	public:
		double value = 0.0;
		double reduction = 0.0;
		double reductionPercentage = 0.0;
		double resistanceCap = 0.840;
	};

	std::string hitZone;
	int partIndex = 0;
	std::string damageType;
	double value = 0.0;
	std::vector<Detail> details;
	Result result;
};

class DamageHit
{
public:
	PlayerType player = PlayerType::Local;
	std::string hitZone;
	std::vector<DamagePart> damageParts;
};