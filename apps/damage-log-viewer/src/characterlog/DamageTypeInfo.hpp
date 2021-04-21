#pragma once
#include <QDebug>
#include <QVector>
#include <QObject>
#include <QMetaType>
#include <QQmlApplicationEngine>

class DamageTypeInfo : public QObject
{
	Q_OBJECT

public:
	enum class Type { Fire, Energy, XRay, Poison, Force, Piercing, UnknownDamageType };
	Q_ENUM(Type)

	enum class ResistSource { Psi, Shield, Armor, Skill, UnknownResistSource };
	Q_ENUM(ResistSource)

	DamageTypeInfo(QObject* parent = nullptr)
		: QObject(parent)
	{}

	static void declareQtTypes()
	{
		qRegisterMetaType<DamageTypeInfo::Type>();
		qRegisterMetaType<DamageTypeInfo::ResistSource>();
		qmlRegisterType<DamageTypeInfo>("mf.nc.Types", 1, 0, "DamageTypeInfo");
	}

	static Type typeFromString(const QString& typeString)
	{
		static QVector<QString> fireStrings = { "fire", "feuer" };
		static QVector<QString> energyStrings = { "energy", "energie" };
		static QVector<QString> xrayStrings = { "xray", "strahlung" };
		static QVector<QString> poisonStrings = { "poison", "gift" };
		static QVector<QString> forceStrings = { "force", "hieb" };
		static QVector<QString> pierceStrings = { "piercing", "stich" };

		const QString s = typeString.toLower().trimmed();
		Type t = Type::UnknownDamageType;
		if (fireStrings.contains(s))
			t = Type::Fire;
		else if (energyStrings.contains(s))
			t = Type::Energy;
		else if (xrayStrings.contains(s))
			t = Type::XRay;
		else if (poisonStrings.contains(s))
			t = Type::Poison;
		else if (forceStrings.contains(s))
			t = Type::Force;
		else if (pierceStrings.contains(s))
			t = Type::Piercing;
		else
			qDebug() << "Unknown DamageType" << typeString;
		return t;
	}

	static QString typeAsString(Type damageType)
	{
		switch (damageType)
		{
		case Type::Fire:
			return QStringLiteral("fire");
		case Type::Energy:
			return QStringLiteral("energy");
		case Type::XRay:
			return QStringLiteral("xray");
		case Type::Poison:
			return QStringLiteral("poison");
		case Type::Force:
			return QStringLiteral("force");
		case Type::Piercing:
			return QStringLiteral("piercing");
		}
		return QString();
	}

	static ResistSource sourceFromString(const QString& sourceString)
	{
		static QString psiMatch("psi");
		static QString shieldMatch("shield");
		static QString armorMatch("armor");
		static QString skillMatch("skill");

		const QString s = sourceString.toLower().trimmed();
		ResistSource rs = ResistSource::UnknownResistSource;
		if (s.contains(psiMatch))
			rs = ResistSource::Psi;
		else if (s.contains(shieldMatch))
			rs = ResistSource::Shield;
		else if (s.contains(armorMatch))
			rs = ResistSource::Armor;
		else if (s.contains(skillMatch))
			rs = ResistSource::Skill;
		else
			qDebug() << "Unknown ResistSource" << sourceString;
		return rs;
	}
};
Q_DECLARE_METATYPE(DamageTypeInfo::Type)
Q_DECLARE_METATYPE(DamageTypeInfo::ResistSource)