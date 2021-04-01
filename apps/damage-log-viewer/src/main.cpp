#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "App.hpp"
#include "LogReader.hpp"
#include "HitZoneInfo.hpp"
#include "DamageTypeInfo.hpp"
#include "DamageHitInfo.hpp"
#include "DamageHitInfoListModel.hpp"
#include "DamageHitInfoTableModel.hpp"
#include "pages/DamageLogViewPageBackend.hpp"

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
	DamageLogViewPageBackend::declareQtTypes();

	// setup QML engine and context
	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("app", &myApp);

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