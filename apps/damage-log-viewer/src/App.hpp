#pragma once
#include <QObject>
#include <QCoreApplication>
#include <QMetaType>
#include <QUrl>
#include <ncloglib/DamageLog.hpp>

class App : public QObject
{
	Q_OBJECT

public:
	App(QObject* parent = nullptr)
		: QObject(parent)
	{}
	
	~App() override
	{}

	Q_INVOKABLE QString urlLocalFile(const QUrl& url)
	{
		return url.toLocalFile();
	}

private:
	QString _version;
};
Q_DECLARE_METATYPE(DamageHit)