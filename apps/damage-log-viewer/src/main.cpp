#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include "App.hpp"
#include "characterlog/LogReader.hpp"
#include "characterlog/HitZoneInfo.hpp"
#include "characterlog/DamageTypeInfo.hpp"
#include "characterlog/DamageHitInfo.hpp"
#include "characterlog/DamageHitInfoListModel.hpp"
#include "characterlog/DamageHitInfoTableModel.hpp"
#include "characterlog/CharacterLogPageBackend.hpp"
#include "damagelog/DamageLogFileReader.hpp"
#include "damagelog/DamageLogPageBackend.hpp"
#include "damagelog/DamageMeterModel.hpp"

int main(int argc, char** argv)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QGuiApplication qtapp(argc, argv);
	qtapp.setOrganizationName("mfreiholz.de");
	qtapp.setOrganizationDomain("https://mfreiholz.de");
	qtapp.setApplicationName("Neocron Log Viewer");
	qtapp.setApplicationVersion("1.0.0-beta");

	App myApp;
	HitZoneInfo::declareQtTypes();
	DamageTypeInfo::declareQtTypes();
	DamageHitInfo::declareQtTypes();
	DamageHitInfoListModel::declareQtTypes();
	DamageHitInfoTableModel::declareQtTypes();
	LogReader::declareQtTypes();
	DamageLogFileReader::declareQtTypes();
	DamageLogViewPageBackend::declareQtTypes();
	DamageMeterModel::declareQtTypes();
	DamageLogPageBackend::declareQtTypes();

	const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

	// setup QML engine and context
	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("app", &myApp);
	engine.rootContext()->setContextProperty("fixedFont", fixedFont);

	const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &qtapp, [url](QObject* obj, const QUrl& objUrl)
		{
			if (!obj && url == objUrl)
			{
				QCoreApplication::exit(-1);
			}
		}, Qt::QueuedConnection);
	engine.load(url);
	return qtapp.exec();
}