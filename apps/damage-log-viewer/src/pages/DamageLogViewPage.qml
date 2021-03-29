import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.12

import mf.nc.DamageHitInfoListModel 1.0
import mf.nc.DamageLogLoader 1.0

import "qrc:/qml/components"

Item {
	id: root

	property string logFilePath
	property bool autoReload
	property QtObject selectedDamageHitInfo

	focus: true
	Keys.onUpPressed: listView.decrementCurrentIndex()
	Keys.onDownPressed: listView.incrementCurrentIndex()

	Component.onCompleted: {
		if (logFilePath.length > 0) {
			loader.start()
		}
	}

	DamageLogLoader {
		id: loader
		logFilePath: root.logFilePath
		watch: root.autoReload
		onErrorOccurred: {
			console.log("DamageLogLoader::onErrorOccurred()")
			errorDialog.text = errorString
			errorDialog.open()
		}
		onFileSizeChanged: {
			console.log("DamageLogLoader::onFileSizeChanged()")
			logGroupBox.title = "Damage Logs (size=" + fileSize + " bytes)"
		}
		onNewLog: {
			console.log("DamageLogLoader::onNewLog(): ", damageHit)
			damageHitInfoModel.add(damageHit)
			listView.currentIndex = listView.count - 1
		}
		onFinished: {
			console.log("DamageLogLoader::onFinished()")
		}
	}

	DamageHitInfoListModel {
		id: damageHitInfoModel
	}

	RowLayout {
		height: parent.height
		width: parent.width

		Pane {
			id: leftPane
			Layout.fillHeight: true
			Layout.preferredWidth: 0.75 * parent.width

			ColumnLayout {
				height: parent.height
				width: parent.width

				GroupBox {
					Layout.fillHeight: true
					Layout.fillWidth: true
					title: "Details"

					DamageHitInfoTable {
						id: damageHitInfoTable
						anchors.fill: parent
					}
				}

				GroupBox {
					id: logGroupBox
					Layout.fillHeight: true
					Layout.fillWidth: true
					title: "Damage Log List"

					ListView {
						id: listView
						anchors.fill: parent
						clip: true
						currentIndex: -1
						model: damageHitInfoModel
						highlightFollowsCurrentItem: true
						highlightMoveDuration: 150
						onCurrentIndexChanged: {
							var currObj = damageHitInfoModel.get(currentIndex)
							damageHitInfoTable.damageHitInfo = currObj
							playerHitZones.hitZones = currObj.hitZones
						}
						delegate: RowLayout {
							width: listView.width
							Label {
								Layout.fillWidth: true
								padding: 3
								text: model.title
								MouseArea {
									anchors.fill: parent
									onClicked: {
										listView.currentIndex = index
									}
								}
							}
						}
						highlight: Rectangle {
							color: Universal.accent
						}
					}
				}
			}
		}

		Pane {
			id: rightPane
			Layout.fillHeight: true
			Layout.preferredWidth: 0.25 * parent.width

			PlayerHitZones {
				id: playerHitZones
				height: parent.height
				width: parent.width
			}
		}
	}

	MessageDialog {
		id: errorDialog
		title: "Error"
		text: ""
		onAccepted: {
		}
	}
}
