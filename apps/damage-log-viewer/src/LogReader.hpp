#pragma once
#include <fstream>
#include <iostream>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDeadlineTimer>
#include <atomic>
#include <ncloglib/Parser.hpp>
#include <ncloglib/DamageHit.hpp>
#include "DamageHitInfo.hpp"

/*!
	Reads from log file until the object is destroyed.
	Caller can pause reading from file.
*/
class LogReader : public QThread
{
	Q_OBJECT
	Q_PROPERTY(QString logFilePath READ getLogFilePath WRITE setLogFilePath NOTIFY logFilePathChanged)
	Q_PROPERTY(qint64 fileSize READ getFileSize NOTIFY fileSizeChanged)
	Q_PROPERTY(bool paused READ isPaused WRITE setPause NOTIFY pausedChanged)

public:
	LogReader(QObject* parent = nullptr)
		: QThread(parent)
		, _stop(false)
		, _paused(true)
	{}

	~LogReader() override
	{
		stop();
	}

	QString getLogFilePath() const
	{
		QMutexLocker l(&_mutex);
		return _logFilePath;
	}

	void setLogFilePath(const QString& path)
	{
		QMutexLocker l(&_mutex);
		if (path == _logFilePath)
			return;
		l.unlock();

		stop();

		l.relock();
		_logFilePath = path;
		l.unlock();

		emit logFilePathChanged(path);
	}

	qint64 getFileSize() const
	{
		QMutexLocker l(&_mutex);
		return _fileSize;
	}

	bool isPaused() const
	{
		QMutexLocker l(&_mutex);
		return _paused;
	}

	void setPause(bool pause)
	{
		QMutexLocker l(&_mutex);
		if (pause == _paused)
		{
			return;
		}
		_paused = pause;
		if (!_paused)
		{
			_pauseCondition.wakeAll();
		}
		l.unlock();
		emit pausedChanged(pause);
	}

	void run()
	{
		nclog::Parser parser(
			// on character system info
			[]() {
			},
			// on damage hit
			[&](std::unique_ptr<nclog::DamageHit> v)
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
			setFileSize(fileSize);

			if (offset < 0 || offset > fileSize)
				in.seekg(0, std::ios::beg);
			else
				in.seekg(offset);
			offset = fileSize;
			std::cout << "offset: " << offset << std::endl;

			parser.parseStream(in);
			in.close();

			// Need pause?
			QMutexLocker l(&_mutex);
			while (_paused && !_stop)
			{
				_pauseCondition.wait(&_mutex, QDeadlineTimer(std::chrono::milliseconds(1000)));
			}
			l.unlock();
			QThread::currentThread()->msleep(500);
		}
	}

	static void declareQtTypes()
	{
		qRegisterMetaType<nclog::DamageHit>("nclog::DamageHit");
		qmlRegisterType<LogReader>("mf.nc.DamageLogLoader", 1, 0, "DamageLogLoader");
	}

protected:
	void setFileSize(qint64 fileSize)
	{
		QMutexLocker l(&_mutex);
		if (_fileSize == fileSize)
			return;
		_fileSize = fileSize;
		l.unlock();
		emit fileSizeChanged(fileSize);
	}

	void stop()
	{
		_stop = true;
		if (isRunning())
			wait();
		_stop = false;
	}

signals:
	void errorOccurred(const QString& errorString);
	void logFilePathChanged(const QString& path);
	void pausedChanged(bool paused);
	void fileSizeChanged(qint64 fileSize);
	void newLog(nclog::DamageHit damageHit);

private:
	std::atomic<bool> _stop;

	mutable QMutex _mutex;
	QString _logFilePath;
	qint64 _fileSize = 0;

	// pause based attributes.
	QWaitCondition _pauseCondition;
	bool _paused = false;
};