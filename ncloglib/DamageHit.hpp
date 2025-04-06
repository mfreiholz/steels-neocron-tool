#pragma once
#include <string>
#include <vector>

namespace nclog
{
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

		int hitZone = -1;
		int partIndex = -1;
		std::string damageType;
		double value = 0.0;
		std::vector<Detail> details;
		Result result;
	};

	class DamageHit
	{
	public:
		std::vector<DamagePart> damageParts;
	};
}
