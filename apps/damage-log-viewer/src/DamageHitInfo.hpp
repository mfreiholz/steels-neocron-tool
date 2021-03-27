#pragma once
#include <optional>
#include <QObject>
#include <QMetaType>
#include <QQmlApplicationEngine>
#include <ncloglib/DamageLog.hpp>
#include "DamageTypeInfo.hpp"

// DamageHitInfo holds information about a single hit/shot/...
class DamageHitInfo : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString hitZone READ getHitZone CONSTANT)
	Q_PROPERTY(QList<DamageTypeInfo::Type> damageTypes READ getDamageTypes CONSTANT)

public:
	DamageHitInfo(QObject* parent = nullptr)
		: QObject(parent)
	{}

	~DamageHitInfo() override
	{}

	void setDamageHit(const DamageHit& dmgHit)
	{
		_h = dmgHit;

		for (size_t i = 0; i < _h.damageParts.size(); i++)
		{
			auto s = QString::fromStdString(_h.damageParts.at(i).damageType);
			auto t = DamageTypeInfo::typeFromString(s);
			if (t == DamageTypeInfo::Type::UnknownDamageType)
			{
				qDebug() << "Unknown damage type in log, we may need a translation for it?" << s;
				continue;
			}
			_damageTypes.append(t);
		}
	}

	const DamageHit& getDamageHit() const
	{
		return _h;
	}

	QString getHitZone() const
	{
		return QString::fromStdString(_h.hitZone);
	}

	QList<DamageTypeInfo::Type> getDamageTypes() const
	{
		return _damageTypes;
	}

	Q_INVOKABLE double getDamageForType(DamageTypeInfo::Type dmgType) const
	{
		auto p = [&dmgType](const DamagePart& dmgPart) {
			auto t = DamageTypeInfo::typeFromString(QString::fromStdString(dmgPart.damageType));
			return t == dmgType;
		};
		auto it = std::find_if(_h.damageParts.begin(), _h.damageParts.end(), p);
		if (it == _h.damageParts.end())
		{
			return 0.0;
		}
		return it->value;
	}

	std::optional<std::vector<DamagePart>::const_iterator> findDamagePartByType(DamageTypeInfo::Type dmgType) const
	{
		auto p = [&dmgType](const DamagePart& dmgPart) {
			auto t = DamageTypeInfo::typeFromString(QString::fromStdString(dmgPart.damageType));
			return t == dmgType;
		};
		auto it = std::find_if(_h.damageParts.begin(), _h.damageParts.end(), p);
		return it == _h.damageParts.end() ? std::nullopt : std::optional(it);
	}

	std::optional<std::vector<DamagePart::Detail>::const_iterator> findResistSourceDetail(const std::vector<DamagePart::Detail>& details, DamageTypeInfo::ResistSource resistSource) const
	{
		auto p = [&resistSource](const DamagePart::Detail& detail) {
			auto rs = DamageTypeInfo::sourceFromString(QString::fromStdString(detail.source));
			return rs == resistSource;
		};
		auto it = std::find_if(details.begin(), details.end(), p);
		return it == details.end() ? std::nullopt : std::optional(it);
	}

	static void declareQtTypes()
	{
		qRegisterMetaType<DamageHitInfo*>();
		qRegisterMetaType<DamageTypeInfo::Type>();
		qmlRegisterType<DamageHitInfo>("mf.nc.DamageHitInfo", 1, 0, "DamageHitInfo");
	}

private:
	DamageHit _h;
	QList<DamageTypeInfo::Type> _damageTypes;
};