#pragma once
#include <QString>
#include <QList>
#include <QAbstractListModel>
#include <QQmlEngine>
#include <ncloglib/DamageLogParser.hpp>
#include "DamageLogEntryInfo.hpp"

class DamageMeterModel : public QAbstractListModel
{
	Q_OBJECT

	class RowItem
	{
	public:
		double percent = 0.0; // Updated from model.

		RowItem(const QString& who)
			: _who(who)
		{}

		QString getWho() const
		{
			return _who;
		}

		double getSummary() const
		{
			return _sum;
		}

		void addEntry(const nclog::DamageLogEntry& entry)
		{
			_entries.append(entry);

			// update stats
			auto energyOpt = entry.getValue("Energy");
			if (energyOpt.has_value())
			{
				_sum += std::stod(energyOpt.value());
			}
		}

	private:
		QString _who;
		double _sum = 0.0;
		QList<nclog::DamageLogEntry> _entries;
	};

public:
	enum Role
	{
		ObjectRole = Qt::UserRole + 1,
		WhoRole,
		SummaryRole,
		Percent,
		PercentFormatted
	};

	DamageMeterModel(QObject* parent = nullptr)
		: QAbstractListModel(parent)
	{}

	~DamageMeterModel() override
	{}

	static void declareQtTypes()
	{
		qmlRegisterType<DamageMeterModel>("mf.nc.DamageMeterModel", 1, 0, "DamageMeterModel");
	}

	Q_INVOKABLE void add(const nclog::DamageLogEntry& entry, bool notifyChanges = false)
	{
		// find matching row item
		QString entryWho = QString::fromStdString(entry.who);
		int index = -1;
		for (int i = 0; i < _rows.count(); i++)
		{
			if (_rows[i].getWho() == entryWho)
			{
				index = i;
				break;
			}
		}
		// new "who"
		if (index < 0)
		{
			if (notifyChanges)
				beginInsertRows(QModelIndex(), rowCount(), rowCount());
			RowItem item(entryWho);
			item.addEntry(entry);
			_rows.append(item);
			if (notifyChanges)
				endInsertRows();
		}
		// existing "who"
		else
		{
			_rows[index].addEntry(entry);
			if (notifyChanges)
				emit dataChanged(this->index(index), this->index(index));
		}
	}

	Q_INVOKABLE void updateRowStates()
	{
		beginResetModel();
		auto itMax = std::max_element(std::begin(_rows), std::end(_rows), [](const auto& a, const auto& b){
			return a.getSummary() < b.getSummary();
		});
		std::for_each(std::begin(_rows), std::end(_rows), [&itMax](RowItem& row){
			row.percent = (100.0 / itMax->getSummary()) * row.getSummary();
		});
		endResetModel();
	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		Q_UNUSED(parent);
		return _rows.count();
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
	{
		if (!isValidIndex(index))
			return QVariant();

		const RowItem& row = _rows[index.row()];
		switch (role)
		{
			case ObjectRole:
			{
				break;
			}
			case Qt::DisplayRole:
			case WhoRole:
			{
				return row.getWho();
			}
			case SummaryRole:
			{
				return row.getSummary();
			}
			case Percent:
			{
				return row.percent;
			}
			case PercentFormatted:
			{
				return QString::number(row.percent, 'f', 1);
			}
		}
		return QVariant();
	}

	QHash<int, QByteArray> roleNames() const override
	{
		QHash<int, QByteArray> h;
		h[Qt::DisplayRole] = "display";
		h[ObjectRole] = "object";
		h[WhoRole] = "who";
		h[SummaryRole] = "summary";
		h[Percent] = "percent";
		h[PercentFormatted] = "percentFormatted";
		return h;
	}

protected:
	bool isValidIndex(const QModelIndex& index) const
	{
		return index.row() >= 0 && index.row() < _rows.count();
	}

private:
	QList<RowItem> _rows;
};