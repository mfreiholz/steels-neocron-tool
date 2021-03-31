#pragma once
#include <QDebug>
#include <QObject>
#include <QMetaType>
#include <QQmlApplicationEngine>

class HitZoneInfo
{
	Q_GADGET

public:
	enum class HitZone { Head, Torso, Legs, All, Unknown };
	Q_ENUM(HitZone)

	static void declareQtTypes()
	{
		qRegisterMetaType<HitZoneInfo::HitZone>();
		qmlRegisterUncreatableType<HitZoneInfo>("mf.nc.Types", 1, 0, "HitZone", "Enum only!");
	}

	static HitZone hitZoneFromInt(int zone)
	{
		HitZone hz = HitZone::Unknown;
		switch (zone)
		{
		case 0:
			hz = HitZone::Head;
			break;
		case 1:
			hz = HitZone::Torso;
			break;
		case 2:
			hz = HitZone::Legs;
			break;
		case 3:
			hz = HitZone::All;
			break;
		default:
			qDebug() << "Unknown HitZone" << zone;
			break;
		}
		return hz;
	}

	static HitZone hitZoneFromString(const QString& zone)
	{
		const auto s = zone.toLower().trimmed();
		HitZone hz = HitZone::Unknown;
		if (s.compare(QStringLiteral("head")) == 0)
			hz = HitZone::Head;
		else if (s.compare(QStringLiteral("torso")) == 0)
			hz = HitZone::Torso;
		else if (s.compare(QStringLiteral("legs")) == 0)
			hz = HitZone::Legs;
		else if (s.compare(QStringLiteral("all")) == 0)
			hz = HitZone::All;
		else
			qDebug() << "Unknown HitZone" << zone;
		return hz;
	}
};
Q_DECLARE_METATYPE(HitZoneInfo::HitZone)