#pragma once
#include <QAbstractTableModel>
#include "DamageTypeInfo.hpp"
#include <optional>

class DamageHitInfoTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum class Rows : int
	{
		Damage,
		ByPsi,
		ByShields,
		ByArmor,
		BySkills,
		ResultReduction,
		ResultPercentage,
		ResultDamage
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
		_rowInfos
			<< RowInfo{ Rows::Damage, QStringLiteral("Damage") }
			<< RowInfo{ Rows::ByPsi, QStringLiteral("PSI (%)") }
			<< RowInfo{ Rows::ByShields, QStringLiteral("Shields (%)") }
			<< RowInfo{ Rows::ByArmor, QStringLiteral("Armor (%)") }
			<< RowInfo{ Rows::BySkills, QStringLiteral("Skills (%)") }
			<< RowInfo{ Rows::ResultReduction, QStringLiteral("Result Reduction") }
			<< RowInfo{ Rows::ResultPercentage, QStringLiteral("Result Damage (%)") }
			<< RowInfo{ Rows::ResultDamage, QStringLiteral("Result Damage") }
		;
	}

	~DamageHitInfoTableModel() override
	{}

	Q_INVOKABLE void setDamageHitInfo(DamageHitInfo* info)
	{
		beginResetModel();
		_dmgHitInfo = info;
		endResetModel();
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
			return _rowInfos.at(section).title;
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
		if (role != Qt::DisplayRole)
			return QVariant();

		const auto dmgType = _columns.at(index.column()).damageType;
		const auto rowType = static_cast<Rows>(index.row());

		// find damage part entry by damage type
		auto dmgPartOpt = _dmgHitInfo->findDamagePartByType(dmgType);
		if (!dmgPartOpt.has_value())
		{
			return QVariant();
		}

		switch (rowType)
		{
			case Rows::Damage:
			{
				return dmgPartOpt.value()->value;
			}
			case Rows::ByPsi:
			{
				auto it = _dmgHitInfo->findResistSourceDetail(dmgPartOpt.value()->details, DamageTypeInfo::ResistSource::Psi);
				return !it ? QVariant() : it.value()->reductionPercentage;
			}
			case Rows::ByShields:
			{
				auto it = _dmgHitInfo->findResistSourceDetail(dmgPartOpt.value()->details, DamageTypeInfo::ResistSource::Shield);
				return !it ? QVariant() : it.value()->reductionPercentage;
			}
			case Rows::ByArmor:
			{
				auto it = _dmgHitInfo->findResistSourceDetail(dmgPartOpt.value()->details, DamageTypeInfo::ResistSource::Armor);
				return !it ? QVariant() : it.value()->reductionPercentage;
			}
			case Rows::BySkills:
			{
				auto it = _dmgHitInfo->findResistSourceDetail(dmgPartOpt.value()->details, DamageTypeInfo::ResistSource::Skill);
				return !it ? QVariant() : it.value()->reductionPercentage;
			}
			case Rows::ResultDamage:
			{
				return dmgPartOpt.value()->result.value;
			}
			case Rows::ResultReduction:
			{
				return dmgPartOpt.value()->result.reduction;
			}
			case Rows::ResultPercentage:
			{
				return dmgPartOpt.value()->result.reductionPercentage;
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
		Rows row;
		QString title;
	};

	DamageHitInfo* _dmgHitInfo = nullptr;
	QList<ColumnInfo> _columns;
	QList<RowInfo> _rowInfos;
};