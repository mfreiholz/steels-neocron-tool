#pragma once
#include <QList>
#include <QStringList>
#include <QHash>
#include <QIODevice>
#include <QTextStream>

class CsvWriter
{
	QIODevice* _device;
	QChar _quoteChar;
	QChar _separatorChar;
	QTextStream _textStream;

	QStringList _headers;

public:
	CsvWriter(QIODevice* out, QChar quote = QChar('\"'), QChar separator = QChar(','))
		: _device(out)
		, _quoteChar(quote)
		, _separatorChar(separator)
		, _textStream(_device)
	{
		_textStream.setCodec("UTF-8");
	}

	~CsvWriter()
	{
	}

	void writeHeaders(const QStringList& headers)
	{
		_headers = headers;
		_writeLine(_headers);
	}

	QHash<QString, QString> createEmptyRow() const
	{
		QHash<QString, QString> h;
		for (const auto& str : _headers)
		{
			h.insert(str, QString());
		}
		return h;
	}

	void writeRow(const QStringList& cells)
	{
		_writeLine(cells);
	}

	void writeRowWithHeaders(const QHash<QString, QString>& cells)
	{
		QStringList values;
		for (const auto& h : _headers)
		{
			if (!cells.contains(h))
				values.append(QString());
			else
				values.append(cells.value(h));
		}
		_writeLine(values);
	}

private:
	void _writeLine(const QStringList& cells)
	{
		bool begin = true;
		for (const auto& str : cells)
		{
			if (!begin)
				_textStream << _separatorChar;
			//_textStream << _quoteChar;
			_textStream << str;
			//_textStream << _quoteChar;
			begin = false;
		}
		_textStream << '\n';
	}
};