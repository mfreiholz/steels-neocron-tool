#pragma once
#include <QObject>
#include <ncloglib/DamageLogParser.hpp>

/*
*/
class DamageLogEntryInfo
{
	Q_GADGET
	Q_PROPERTY(QString who READ getWho CONSTANT)

public:
	void setEntry(const nclog::DamageLogEntry& entry)
	{
		_entry = entry;
	}

	QString getWho() const
	{
		return QString::fromStdString(_entry.who);
	}

private:
	nclog::DamageLogEntry _entry;
};
Q_DECLARE_METATYPE(DamageLogEntryInfo);