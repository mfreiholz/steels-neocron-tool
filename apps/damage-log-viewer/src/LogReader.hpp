#pragma once
#include <fstream>
#include <iostream>
#include <QThread>
#include <QMutex>
#include <atomic>
#include <ncloglib/Parser.hpp>
#include <ncloglib/DamageLog.hpp>
#include "DamageHitInfo.hpp"

class LogReader : public QThread
{
	Q_OBJECT
	Q_PROPERTY(QString logFilePath READ getLogFilePath WRITE setLogFilePath NOTIFY logFilePathChanged)
	Q_PROPERTY(bool watch READ getWatch WRITE setWatch NOTIFY watchChanged)

public:
	LogReader(QObject* parent = nullptr)
		: QThread(parent)
		, _stop(false)
	{}

	~LogReader() override
	{
		stop();
	}

	QString getLogFilePath() const
	{
		return _logFilePath;
	}

	void setLogFilePath(const QString& path)
	{
		if (path == _logFilePath)
			return;
		stop();
		_logFilePath = path;
		emit logFilePathChanged(_logFilePath);
	}

	bool getWatch() const
	{
		return _watch;
	}

	void setWatch(bool onoff)
	{
		if (_watch == onoff)
			return;
		_watch = onoff;
		emit watchChanged(_watch);
	}

	void stop()
	{
		_stop = true;
		if (isRunning())
			wait();
		_stop = false;
	}

	void run()
	{
		Parser parser(
			// on character system info
			[]() {
			},
			// on damage hit
			[&](std::unique_ptr<DamageHit> v)
			{
				emit newLog(*v);
			}
		);
		std::streampos offset = -1;
		while (!_stop)
		{
			std::ifstream in(_logFilePath.toStdString(), std::ios_base::binary);
			if (!in)
			{
				_stop = true;
				emit errorOccurred("Can't open file: " + _logFilePath);
				break;
			}

			in.seekg(0, std::ios::end);
			const auto fileSize = in.tellg();
			emit fileSizeChanged(fileSize);

			if (offset < 0 || offset > fileSize)
				in.seekg(0, std::ios::beg);
			else
				in.seekg(offset);
			offset = fileSize;
			//std::cout << "offset: " << offset << std::endl;

			parser.parseStream(in);
			in.close();

			if (!_watch)
			{
				_stop = true;
				break;
			}
			QThread::currentThread()->msleep(500);
		}
	}

	static void declareQtTypes()
	{
		qRegisterMetaType<DamageHit>("DamageHit");
		qmlRegisterType<LogReader>("mf.nc.DamageLogLoader", 1, 0, "DamageLogLoader");
	}

signals:
	void errorOccurred(const QString& errorString);
	void logFilePathChanged(const QString& path);
	void watchChanged(bool enabled);
	void fileSizeChanged(qint64 fileSize);
	void newLog(DamageHit damageHit);

private:
	std::atomic<bool> _stop;
	QMutex _mutex;
	QString _logFilePath;
	bool _watch = false;
};