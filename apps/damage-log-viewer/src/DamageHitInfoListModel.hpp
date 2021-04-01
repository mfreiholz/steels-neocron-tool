#pragma once
#include <QList>
#include <QObject>
#include <QAbstractListModel>
#include <QDebug>
#include <ncloglib/DamageHit.hpp>
#include "DamageHitInfo.hpp"

class DamageHitInfoListModel : public QAbstractListModel
{
	Q_OBJECT
	QList<DamageHitInfo*> _list;

public:
	enum Roles
	{
		ObjectRole = Qt::UserRole + 1,
		TitleRole
	};

	DamageHitInfoListModel(QObject* parent = nullptr)
		: QAbstractListModel(parent)
	{}

	~DamageHitInfoListModel() override = default;

	Q_INVOKABLE void add(DamageHit hit)
	{
		beginInsertRows(QModelIndex(), _list.count(), _list.count());
		auto info = new DamageHitInfo(this);
		info->setDamageHit(hit);
		_list.append(info);
		endInsertRows();
	}

	Q_INVOKABLE DamageHitInfo* get(int index) const
	{
		return _list.at(index);
	}

	Q_INVOKABLE QList<DamageHitInfo*> getItems() const
	{
		return _list;
	}

	QHash<int, QByteArray> roleNames() const
	{
		QHash<int, QByteArray> roles;
		roles[ObjectRole] = "object";
		roles[TitleRole] = "title";
		return roles;
	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		return _list.count();
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
	{
		if (index.row() < 0 || index.row() >= _list.count())
		{
			return QVariant();
		}
		const auto hit = _list.at(index.row());
		switch (role)
		{
			case ObjectRole:
			{
				return QVariant::fromValue(hit);
			}
			case TitleRole:
			{
				QStringList hitZones;
				for (auto hz : hit->getHitZones())
					hitZones.append(QMetaEnum::fromType<HitZoneInfo::HitZone>().valueToKey(static_cast<int>(hz)));

				QStringList dmgTypes;
				for (auto dt : hit->getDamageTypes())
					dmgTypes.append(QMetaEnum::fromType<DamageTypeInfo::Type>().valueToKey(static_cast<int>(dt)));

				return QStringLiteral("Hit #%1, Zones=%2; Types=%3")
					.arg(index.row() + 1, 4, 10, QLatin1Char('0'))
					.arg(hitZones.join(QChar(',')))
					.arg(dmgTypes.join(QChar(',')));
			}
		}
		return QVariant();
	}

	static void declareQtTypes()
	{
		qRegisterMetaType<QList<DamageHitInfo*>>("QList<DamageHitInfo*>");
		qmlRegisterType<DamageHitInfoListModel>("mf.nc.DamageHitInfoListModel", 1, 0, "DamageHitInfoListModel");
	}
};
