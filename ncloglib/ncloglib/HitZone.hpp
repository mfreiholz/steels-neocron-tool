#pragma once

enum class HitZone
{
	All,
	Head,
	Torso,
	Legs
};

HitZone HitZoneFromString(const std::string& s)
{
	if (s.compare("ALL") == 0)
	{
		return HitZone::All;
	}
	else if (s.compare("HEAD") == 0)
	{
		return HitZone::Head;
	}
	else if (s.compare("") == 0)
	{

	}
	return HitZone::All;
}