import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Universal

import mf.nc.DamageHitInfoListModel 1.0
import mf.nc.DamageLogLoader 1.0
import mf.nc.CharacterLogPageBackend 1.0

import "qrc:/qml/components"

Item {
	id: root

	property string logFilePath
	property QtObject selectedDamageHitInfo

	focus: true
	Keys.onUpPressed: listView.decrementCurrentIndex()
	Keys.onDownPressed: listView.incrementCurrentIndex()

	CharacterLogPageBackend {
		id: backend
	}

	DamageLogLoader {
		id: loader
		logFilePath: root.logFilePath
		paused: !liveUpdateCheckBox.checked
		onLogFilePathChanged: function (path) {
			if (path.length > 0) {
				loader.start()
			}
		}
		onFileSizeChanged: {
		}
		onNewLog: function (damageHit) {
			damageHitInfoModel.add(damageHit)
			if (autoSelectCurrent.checked) {
				listView.currentIndex = listView.count - 1
			}
		}
		onErrorOccurred: {
			errorDialog.text = errorString
			errorDialog.open()
		}
		onFinished: {
		}
	}

	DamageHitInfoListModel {
		id: damageHitInfoModel
	}


	/*
		Table + Hit Zones
	*/
	Item {
		id: areaTop
		anchors {
			top: parent.top
			right: parent.right
			bottom: undefined
			left: parent.left
		}
		height: parent.height * 0.7

		DamageHitInfoTable {
			id: damageHitInfoTable
			anchors {
				top: parent.top
				right: playerHitZones.left
				bottom: parent.bottom
				left: parent.left
				margins: 5
			}
		}

		PlayerHitZones {
			id: playerHitZones
			anchors {
				top: parent.top
				right: parent.right
				bottom: parent.bottom
				left: undefined
			}
			width: 200
		}
	}


	/*
		Hit list + Options
	*/
	Item {
		id: areaBottom
		anchors {
			top: areaTop.bottom
			right: parent.right
			bottom: parent.bottom
			left: parent.left
		}

		Rectangle {
			id: areaBottomLeft
			color: "transparent"
			border.color: Universal.accent
			anchors {
				top: parent.top
				right: areaBottomRight.left
				bottom: parent.bottom
				left: parent.left
				margins: 5
			}
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

		ColumnLayout {
			id: areaBottomRight
			anchors {
				top: parent.top
				right: parent.right
				bottom: parent.bottom
				left: undefined
				margins: 5
			}
			width: 200
			Label {
				Layout.fillWidth: true
				text: "Size: " + loader.fileSize + " bytes"
			}
			CheckBox {
				Layout.fillWidth: true
				id: autoSelectCurrent
				text: "Auto select newest log"
				checked: true
			}
			CheckBox {
				Layout.fillWidth: true
				id: liveUpdateCheckBox
				text: "Live update"
				checked: false
			}
			Item {
				Layout.fillHeight: true
				Layout.fillWidth: true
			}
			Button {
				Layout.fillWidth: true
				id: exportCsvButton
				text: "Export CSV"
				onClicked: {
					backend.writeCSV(damageHitInfoModel.getItems())
				}
			}
		}
	}
}
