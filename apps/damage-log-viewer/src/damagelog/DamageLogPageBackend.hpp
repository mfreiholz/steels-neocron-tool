#pragma once
#include <QObject>
#include <QMetaType>
#include <QQmlEngine>
#include "DamageLogEntryInfo.hpp"

/*
*/
class DamageLogPageBackend : public QObject
{
	Q_OBJECT

public:
	DamageLogPageBackend(QObject* parent = nullptr)
		: QObject(parent)
	{}

	static void declareQtTypes()
	{
		qRegisterMetaType<DamageLogEntryInfo>();
		qmlRegisterType<DamageLogPageBackend>("mf.nc.DamageLogPageBackend", 1, 0, "DamageLogPageBackend");
	}
};