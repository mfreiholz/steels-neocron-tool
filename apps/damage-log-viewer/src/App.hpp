#pragma once
#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include <QMetaType>
#include <QUrl>
#include <QGuiApplication>
#include <QClipboard>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <ncloglib/DamageHit.hpp>

class App : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString gameDirectoryPath READ getGameDirectoryPath WRITE setGameDirectoryPath NOTIFY gameDirectoryPathChanged)

public:
	App(QObject* parent = nullptr)
		: QObject(parent)
	{}

	~App() override
	{}

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
				else if (info.isFile() && QRegExp("(.*)_([0-9]{2})\\.log").exactMatch(info.fileName()))
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
};
Q_DECLARE_METATYPE(nclog::DamageHit)