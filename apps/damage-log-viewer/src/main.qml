import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.12
import "qrc:/qml/pages"
import "qrc:/qml"

ApplicationWindow {
	id: window
	width: 1024
	height: 768
	visible: true
	title: Qt.application.displayName + " - " + Qt.application.version + " - " + Qt.application.organization
	onActiveFocusItemChanged: console.log("onActiveFocusItemChanged()", activeFocusItem)

	header: Rectangle {
		height: 40
		color: Universal.accent
		RowLayout {
			anchors.fill: parent
			Button {
				id: backButton
				Layout.fillHeight: true
				enabled: stack.depth > 1
				icon.source: "qrc:/res/arrow_back_ios_new-white-24dp.svg"
				onClicked: stack.pop()
			}
			Label {
				id: titleLabel
				Layout.fillHeight: true
				Layout.fillWidth: true
				text: window.title
				horizontalAlignment: Qt.AlignHCenter
				verticalAlignment: Qt.AlignVCenter
			}
			Button {
				id: infoButton
				Layout.fillHeight: true
				enabled: true
				icon.source: "qrc:/res/info-white-24dp.svg"
				onClicked: stack.pop()
			}
		}
	}

	StackView {
		id: stack
		anchors.fill: parent
		initialItem: compSelectFile
	}

	Loader {
		id: loader
		focus: true
	}

	Component {
		id: compSelectFile
		Item {
			ColumnLayout {
				anchors.centerIn: parent
				CheckBox {
					id: watch
					text: "Auto Reload File"
				}
				Button {
					text: "Select *.log file"
					onClicked: {
						fileDialog.open()
					}
				}
			}
			FileDialog {
				id: fileDialog
				selectExisting: true
				selectFolder: false
				selectMultiple: false
				visible: false
				folder: "file:///D:/Projects/NeocronLogParser"
				onAccepted: {
					if (fileUrl.length <= 0) {
						return
					}
					loader.setSource("qrc:/qml/pages/DamageLogViewPage.qml", { logFilePath: app.urlLocalFile(fileDialog.fileUrl), autoReload: watch.checked })
					stack.push(loader)
				}
			}
		}
	}
}

//	footer: Rectangle {
//		color: "transparent"
//		height: 40

//		RowLayout {
//			anchors.fill: parent
//			spacing: 0
//			Button {
//				Layout.fillHeight: true
//				Layout.fillWidth: true
//				icon.source: "qrc:/res/folder_open-white-24dp.svg"
//				text: "File Selection"
//				onClicked: {
//					stack.pop()
//				}
//			}
//			Button {
//				Layout.fillHeight: true
//				Layout.fillWidth: true
//				icon.source: "qrc:/res/assessment-white-24dp.svg"
//				text: "Damage Log"
//				enabled: logFilePath.length > 0
//				onClicked: {
//				}
//			}
//		}
//	}

//	Drawer {
//		id: drawer
//		width: window.width * 0.66
//		height: window.height

//		Label {
//			anchors.centerIn: parent
//			text: "Content goes here!"
//		}
//	}

//	Component {
//		id: compDamageLogViewPage
//		DamageLogViewPage {
//			focus: true
//			logFilePath: window.logFilePath
//			autoReload: window.autoReload
//		}
//	}
