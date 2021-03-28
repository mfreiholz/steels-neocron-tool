#pragma once
#include <QObject>
#include <QCoreApplication>
#include <QMetaType>
#include <QUrl>
#include <QGuiApplication>
#include <QClipboard>
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

	Q_INVOKABLE void copyToClipboard(const QVariant& v)
	{
		QGuiApplication::clipboard()->setText(v.toString());
	}

private:
	QString _version;
};
Q_DECLARE_METATYPE(DamageHit)