#pragma once
#include <QAbstractTableModel>
#include "DamageTypeInfo.hpp"
#include <optional>
#include <QHash>

class DamageHitInfoTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum Rows
	{
		Damage,
		ByPsi,
		ByShields,
		ByArmor,
		BySkills,
		ResultDamage
	};

	enum Role
	{
		DamageValueRole = Qt::UserRole +1,
		DamageReductionRole,
		DamageReductionPercentageRole,
		DamageReductionFromPreviousRole,
		DamageReductionPercentageFromPreviousRole
	};

	DamageHitInfoTableModel(QObject* parent = nullptr)
		: QAbstractTableModel(parent)
	{
		_columns
			<< ColumnInfo{ DamageTypeInfo::Type::Fire, QStringLiteral("Fire") }
			<< ColumnInfo{ DamageTypeInfo::Type::Energy, QStringLiteral("Energy") }
			<< ColumnInfo{ DamageTypeInfo::Type::XRay, QStringLiteral("XRay") }
			<< ColumnInfo{ DamageTypeInfo::Type::Poison, QStringLiteral("Poison") }
			<< ColumnInfo{ DamageTypeInfo::Type::Force, QStringLiteral("Force") }
			<< ColumnInfo{ DamageTypeInfo::Type::Pierce, QStringLiteral("Pierce") }
		;
		_rowInfos.insert(static_cast<int>(Rows::Damage), RowInfo{ QStringLiteral("Raw Damage") });
		_rowInfos.insert(static_cast<int>(Rows::ByPsi), RowInfo{ QStringLiteral("PSI Resist"), DamageTypeInfo::ResistSource::Psi });
		_rowInfos.insert(static_cast<int>(Rows::ByShields), RowInfo{ QStringLiteral("Shields Resist"), DamageTypeInfo::ResistSource::Shield });
		_rowInfos.insert(static_cast<int>(Rows::ByArmor), RowInfo{ QStringLiteral("Armor Resist"), DamageTypeInfo::ResistSource::Armor });
		_rowInfos.insert(static_cast<int>(Rows::BySkills), RowInfo{ QStringLiteral("Skills Resist"), DamageTypeInfo::ResistSource::Skill });
		_rowInfos.insert(static_cast<int>(Rows::ResultDamage), RowInfo{ QStringLiteral("Result") });
	}

	~DamageHitInfoTableModel() override
	{}

	Q_INVOKABLE void setDamageHitInfo(DamageHitInfo* info)
	{
		beginResetModel();
		_dmgHitInfo = info;
		endResetModel();
	}

	QHash<int, QByteArray> roleNames() const
	{
		QHash<int, QByteArray> h;
		h[static_cast<int>(Qt::DisplayRole)] = "display";
		h[static_cast<int>(Role::DamageValueRole)] = "damage";
		h[static_cast<int>(Role::DamageReductionRole)] = "damageReduction";
		h[static_cast<int>(Role::DamageReductionPercentageRole)] = "damageReductionPercentage";
		h[static_cast<int>(Role::DamageReductionFromPreviousRole)] = "damageReductionFromPrevious";
		h[static_cast<int>(Role::DamageReductionPercentageFromPreviousRole)] = "damageReductionPercentageFromPrevious";
		return h;
	}

	int columnCount(const QModelIndex& parent = QModelIndex()) const override
	{
		return _columns.count();
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
	{
		if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		{
			return _columns.at(section).title;
		}
		else if (orientation == Qt::Vertical && role == Qt::DisplayRole)
		{
			return _rowInfos[section].title;
		}
		return QVariant();
	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		return _rowInfos.size();
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
	{
		if (!_dmgHitInfo || index.row() < 0 || index.row() >= _rowInfos.size() || index.column() < 0 || index.column() >= _columns.count())
			return QVariant();
		if (role != Qt::DisplayRole && !(role > Qt::UserRole))
			return QVariant();

		const auto& columnInfo = _columns.at(index.column());
		const auto& rowInfo = _rowInfos[index.row()];
		const auto dmgPartOpt = _dmgHitInfo->findDamagePartByType(columnInfo.damageType);
		if (!dmgPartOpt.has_value())
		{
			return QVariant();
		}
		else if (index.row() == Rows::Damage)
		{
			return dmgPartOpt.value()->value;
		}
		else if (index.row() >= Rows::ByPsi && index.row() <= Rows::BySkills)
		{
			const auto it = _dmgHitInfo->findResistSourceDetail(dmgPartOpt.value()->details, rowInfo.resistSource);
			if (it)
			{
				switch (role)
				{
					case Role::DamageValueRole:
					{
						return QString::number(it.value()->value, 'f', 3);
					}
					case Role::DamageReductionRole:
					{
						return QString::number(it.value()->reduction, 'f', 3);
					}
					case Role::DamageReductionPercentageRole:
					{
						return QString::number(it.value()->reductionPercentage, 'f', 3);
					}
					case Role::DamageReductionFromPreviousRole:
					{
						const auto itPrev = _dmgHitInfo->findPreviousResistSourceDetail(dmgPartOpt.value()->details, rowInfo.resistSource);
						if (itPrev)
						{
							auto reduction = itPrev.value()->value - it.value()->value;
							return QString::number(reduction, 'f', 3);
						}
						else
						{
							auto reduction = dmgPartOpt.value()->value - it.value()->value;
							return QString::number(reduction, 'f', 3);
						}
						break;
					}
					case Role::DamageReductionPercentageFromPreviousRole:
					{
						const auto itPrev = _dmgHitInfo->findPreviousResistSourceDetail(dmgPartOpt.value()->details, rowInfo.resistSource);
						if (itPrev)
						{
							auto reduction = itPrev.value()->value - it.value()->value;
							reduction = 100.0 / itPrev.value()->value * reduction;
							return QString::number(reduction, 'f', 3);
						}
						else
						{
							auto reduction = dmgPartOpt.value()->value - it.value()->value;
							reduction = 100.0 / dmgPartOpt.value()->value * reduction;
							return QString::number(reduction, 'f', 3);
						}
						break;
					}
					default:
					{
						return QStringLiteral("Woot? Default");
					}
				}
			}
		}
		else if (index.row() == Rows::ResultDamage)
		{
			switch (role)
			{
				case Role::DamageValueRole:
				{
					return dmgPartOpt.value()->result.value;
				}
				case Role::DamageReductionRole:
				{
					return dmgPartOpt.value()->result.reduction;
				}
				case Role::DamageReductionPercentageRole:
				{
					return dmgPartOpt.value()->result.reductionPercentage;
				}
			}
		}
		return QVariant();
	}

	static void declareQtTypes()
	{
		qmlRegisterType<DamageHitInfoTableModel>("mf.nc.DamageHitInfoTableModel", 1, 0, "DamageHitInfoTableModel");
	}

private:
	struct ColumnInfo
	{
		DamageTypeInfo::Type damageType;
		QString title;
	};

	struct RowInfo
	{
		QString title;
		DamageTypeInfo::ResistSource resistSource = DamageTypeInfo::ResistSource::UnknownResistSource;
	};

	DamageHitInfo* _dmgHitInfo = nullptr;
	QList<ColumnInfo> _columns;
	QHash<int, RowInfo> _rowInfos;
};