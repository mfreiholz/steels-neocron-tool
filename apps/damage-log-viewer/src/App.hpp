#pragma once
#include "ncloglib/DamageHit.hpp"
#include <QClipboard>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QMetaType>
#include <QObject>
#include <QRegularExpression>
#include <QSettings>
#include <QTextStream>
#include <QUrl>

class App : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString gameDirectoryPath READ getGameDirectoryPath WRITE setGameDirectoryPath NOTIFY gameDirectoryPathChanged)

public:
	App(QObject* parent = nullptr)
		: QObject(parent)
	{
		_gameDirectoryPath = _settings.value("gameDirectoryPath").toString();
	}

	~App() override
	{
		_settings.setValue("gameDirectoryPath", _gameDirectoryPath);
	}

	QString getGameDirectoryPath() const
	{
		return _gameDirectoryPath;
	}

	void setGameDirectoryPath(const QString& path)
	{
		if (path == _gameDirectoryPath)
			return;
		_gameDirectoryPath = path;
		emit gameDirectoryPathChanged();
	}

	Q_INVOKABLE QString urlLocalFile(const QUrl& url)
	{
		return url.toLocalFile();
	}

	Q_INVOKABLE QUrl urlFromPath(const QString& path)
	{
		return QUrl::fromLocalFile(path);
	}

	Q_INVOKABLE void copyToClipboard(const QVariant& v)
	{
		QGuiApplication::clipboard()->setText(v.toString());
	}

	Q_INVOKABLE void updateNeocronDirectoryFiles()
	{
		_damageLogFiles.clear();
		_characterLogFiles.clear();

		QDir dir(_gameDirectoryPath);
		if (dir.cd("logs"))
		{
			for (const QFileInfo& info : dir.entryInfoList())
			{
				if (info.isFile() && info.fileName().startsWith("Damage_"))
				{
					_damageLogFiles.append(info.absoluteFilePath());
				}
				else if (info.isFile() && QRegularExpression("(.*)_([0-9]{2})\\.log").match(info.fileName()).hasMatch())
				{
					_characterLogFiles.append(info.absoluteFilePath());
				}
			}
		}
		emit gameDirectoryFilesChanged(_damageLogFiles, _characterLogFiles);
	}

	Q_INVOKABLE QString readTextFileContents(const QString& path)
	{
		QFile f(path);
		if (!f.open(QFile::ReadOnly))
		{
			qDebug() << f.errorString();
			return QString();
		}
		QTextStream ts(&f);
		QString s = ts.readAll();
		f.close();
		return s;
	}

signals:
	void gameDirectoryPathChanged();
	void gameDirectoryFilesChanged(const QStringList& damageLogFiles, const QStringList& characterLogFiles);

private:
	QString _version;
	QString _gameDirectoryPath;

	QStringList _damageLogFiles;
	QStringList _characterLogFiles;

	QSettings _settings;
};
Q_DECLARE_METATYPE(nclog::DamageHit)
