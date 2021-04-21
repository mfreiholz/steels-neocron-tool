#pragma once
#include <QList>
#include <QAbstractListModel>
#include "DamageLogEntryInfo.hpp"

class DamageLogListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	DamageLogListModel(QObject* parent = nullptr)
		: QAbstractListModel(parent)
	{}

	~DamageLogListModel() override
	{}

	Q_INVOKABLE void add()
	{
	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		Q_UNUSED(parent);
		return 0;
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
	{
		return QVariant();
	}

private:
	QList<DamageLogEntryInfo> _entries;
};
