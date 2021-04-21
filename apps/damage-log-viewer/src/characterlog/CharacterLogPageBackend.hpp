#pragma once
#include <QObject>
#include <QQmlEngine>
#include <QDir>
#include <QDateTime>
#include <QFile>
#include "../characterlog/DamageHitInfo.hpp"
#include "../CsvWriter.hpp"

// @todo rename
class DamageLogViewPageBackend : public QObject
{
	Q_OBJECT

public:
	DamageLogViewPageBackend(QObject* parent = nullptr)
	{}

	~DamageLogViewPageBackend() override = default;

	Q_INVOKABLE void writeCSV(const QList<DamageHitInfo*>& hits)
	{
		QDateTime now = QDateTime::currentDateTime();
		QString filePath = QDir::homePath() + QString("/NeocronLogViewerExport_%1.csv").arg(now.toString("yyyy-MM-dd_hh-mm-ss"));
		QFile f(filePath);
		if (!f.open(QIODevice::WriteOnly))
		{
			qDebug() << "Can not open file" << filePath << f.errorString();
			return;
		}

		CsvWriter writer(&f);

		QStringList headers;
		headers
			<< "DAMAGE_TYPE"
			<< "RAW_DAMAGE"
			<< "RAW_DAMAGE_HITZONE"

			<< "RESIST_PSI_DAMAGE"
			<< "RESIST_PSI_REDUCTION"
			<< "RESIST_PSI_REDUCTION_PERCENT"

			<< "RESIST_SHIELD_DAMAGE"
			<< "RESIST_SHIELD_REDUCTION"
			<< "RESIST_SHIELD_REDUCTION_PERCENT"

			<< "RESIST_ARMOR_DAMAGE"
			<< "RESIST_ARMOR_REDUCTION"
			<< "RESIST_ARMOR_REDUCTION_PERCENT"

			<< "RESIST_SKILL_DAMAGE"
			<< "RESIST_SKILL_REDUCTION"
			<< "RESIST_SKILL_REDUCTION_PERCENT"

			<< "FINAL_DAMAGE"
			<< "FINAL_DAMAGE_REDUCTION"
			<< "FINAL_DAMAGE_REDUCTION_PERCENT"
			;
		writer.writeHeaders(headers);

		for (DamageHitInfo* hit : hits)
		{
			for (const auto& hitPart : hit->getDamageHit().damageParts)
			{
				auto row = writer.createEmptyRow();
				row["DAMAGE_TYPE"] = DamageTypeInfo::typeAsString(DamageTypeInfo::typeFromString(QString::fromStdString(hitPart.damageType)));
				row["RAW_DAMAGE"] = QString::number(hitPart.value, 'f', 3);
				row["RAW_DAMAGE_HITZONE"] = QString::number(hitPart.hitZone);
				for (const auto& detail : hitPart.details)
				{
					auto rs = DamageTypeInfo::sourceFromString(QString::fromStdString(detail.source));
					switch (rs)
					{
						case DamageTypeInfo::ResistSource::Psi:
						{
							row["RESIST_PSI_DAMAGE"] = QString::number(detail.value, 'f', 3);
							row["RESIST_PSI_REDUCTION"] = QString::number(detail.reduction, 'f', 3);
							row["RESIST_PSI_REDUCTION_PERCENT"] = QString::number(detail.reductionPercentage, 'f', 3);
							break;
						}
						case DamageTypeInfo::ResistSource::Shield:
						{
							row["RESIST_SHIELD_DAMAGE"] = QString::number(detail.value, 'f', 3);
							row["RESIST_SHIELD_REDUCTION"] = QString::number(detail.reduction, 'f', 3);
							row["RESIST_SHIELD_REDUCTION_PERCENT"] = QString::number(detail.reductionPercentage, 'f', 3);
							break;
						}
						case DamageTypeInfo::ResistSource::Armor:
						{
							row["RESIST_ARMOR_DAMAGE"] = QString::number(detail.value, 'f', 3);
							row["RESIST_ARMOR_REDUCTION"] = QString::number(detail.reduction, 'f', 3);
							row["RESIST_ARMOR_REDUCTION_PERCENT"] = QString::number(detail.reductionPercentage, 'f', 3);
							break;
						}
						case DamageTypeInfo::ResistSource::Skill:
						{
							row["RESIST_SKILL_DAMAGE"] = QString::number(detail.value, 'f', 3);
							row["RESIST_SKILL_REDUCTION"] = QString::number(detail.reduction, 'f', 3);
							row["RESIST_SKILL_REDUCTION_PERCENT"] = QString::number(detail.reductionPercentage, 'f', 3);
							break;
						}
					}
				}
				row["FINAL_DAMAGE"] = QString::number(hitPart.result.value, 'f', 3);
				row["FINAL_DAMAGE_REDUCTION"] = QString::number(hitPart.result.reduction, 'f', 3);
				row["FINAL_DAMAGE_REDUCTION_PERCENT"] = QString::number(hitPart.result.reductionPercentage, 'f', 3);
				writer.writeRowWithHeaders(row);
			}
		}
		f.close();
	}

	static void declareQtTypes()
	{
		qmlRegisterType<DamageLogViewPageBackend>("mf.nc.CharacterLogPageBackend", 1, 0, "CharacterLogPageBackend");
	}
};