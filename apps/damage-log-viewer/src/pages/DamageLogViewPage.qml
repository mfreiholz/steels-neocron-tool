import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.12

import mf.nc.DamageHitInfoListModel 1.0
import mf.nc.DamageLogLoader 1.0
import mf.nc.DamageLogViewPageBackend 1.0

import "qrc:/qml/components"

Item {
	id: root

	property string logFilePath
	property QtObject selectedDamageHitInfo

	focus: true
	Keys.onUpPressed: listView.decrementCurrentIndex()
	Keys.onDownPressed: listView.incrementCurrentIndex()

	Component.onCompleted: {
		console.log("DamageLogViewPage::onCompleted()")
	}

	Component.onDestruction: {
		console.log("DamageLogViewPage::onDestruction()")
	}

	DamageLogViewPageBackend {
		id: backend
	}

	DamageLogLoader {
		id: loader
		logFilePath: root.logFilePath
		paused: !liveUpdateCheckBox.checked
		onLogFilePathChanged: {
			console.log("DamageLogLoader::onLogFilePathChanged()", path)
			if (path.length > 0) {
				loader.start()
			}
		}
		onFileSizeChanged: {
			console.log("DamageLogLoader::onFileSizeChanged()", fileSize)
		}
		onNewLog: {
			console.log("DamageLogLoader::onNewLog(): ", damageHit)
			damageHitInfoModel.add(damageHit)
			if (autoSelectCurrent.checked) {
				listView.currentIndex = listView.count - 1
			}
		}
		onErrorOccurred: {
			console.log("DamageLogLoader::onErrorOccurred()", errorString)
			errorDialog.text = errorString
			errorDialog.open()
		}
		onFinished: {
			console.log("DamageLogLoader::onFinished()")
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
				text: "Size: " + loader.fileSize+" bytes"
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


//	RowLayout {
//		y: parent.height
//		x: parent.width

//		Pane {
//			id: leftPane
//			Layout.fillHeight: true
//			Layout.preferredWidth: 0.75 * parent.width

//			ColumnLayout {
//				height: parent.height
//				width: parent.width

//				GroupBox {
//					Layout.fillHeight: true
//					Layout.fillWidth: true
//					title: "Details"


//				}

//				GroupBox {
//					id: logGroupBox
//					Layout.fillHeight: true
//					Layout.fillWidth: true
//					title: "Damage Log List (Count = "+damageHitInfoModel.rowCount()+"; File Size = "+loader.fileSize+" bytes)"

//					ColumnLayout {
//						anchors.fill: parent

//						ListView {
//							id: listView
//							Layout.fillHeight: true
//							Layout.fillWidth: true
//							clip: true
//							currentIndex: -1
//							model: damageHitInfoModel
//							highlightFollowsCurrentItem: true
//							highlightMoveDuration: 150
//							onCurrentIndexChanged: {
//								var currObj = damageHitInfoModel.get(currentIndex)
//								damageHitInfoTable.damageHitInfo = currObj
//								playerHitZones.hitZones = currObj.hitZones
//							}
//							delegate: RowLayout {
//								width: listView.width
//								Label {
//									Layout.fillWidth: true
//									padding: 3
//									text: model.title
//									MouseArea {
//										anchors.fill: parent
//										onClicked: {
//											listView.currentIndex = index
//										}
//									}
//								}
//							}
//							highlight: Rectangle {
//								color: Universal.accent
//							}
//						}

//						RowLayout {
//							Layout.fillWidth: true
//							Item {
//								Layout.fillWidth: true
//							}
//							CheckBox {
//								id: pauseCheckbox
//								text: "Live Update"
//								checked: false
//							}
//							CheckBox {
//								id: autoSelectCurrent
//								text: "Auto select newest log"
//								checked: false
//								onCheckedChanged: {
//									console.log("ASDFASDFASDF", checked)
//								}
//							}
//						}
//					}
//				}
//			}
//		}

//		Pane {
//			id: rightPane
//			Layout.fillHeight: true
//			Layout.preferredWidth: 0.25 * parent.width


//		}
//	}

//	MessageDialog {
//		id: errorDialog
//		title: "Error"
//		text: ""
//		onAccepted: {
//		}
//	}