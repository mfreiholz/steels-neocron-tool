import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import "qrc:/qml/pages"

ApplicationWindow {
	id: window
	width: 1024
	height: 768
	visible: true
	title: Qt.application.displayName + " - " + Qt.application.version + " - " + Qt.application.organization
	onActiveFocusItemChanged: console.log("onActiveFocusItemChanged()", activeFocusItem)

	property var logFilePath: ""
	property var autoReload: false

	Drawer {
		id: drawer
		width: window.width * 0.66
		height: window.height

		Label {
			anchors.centerIn: parent
			text: "Content goes here!"
		}
	}

	StackView {
		id: stack
		anchors.fill: parent
		initialItem: compSelectFile
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
					window.logFilePath = app.urlLocalFile(fileDialog.fileUrl)
					window.autoReload = watch.checked
					stack.push(compDamageLogViewPage)
				}
			}
		}
	}

	Component {
		id: compDamageLogViewPage
		DamageLogViewPage {
			focus: true
			logFilePath: window.logFilePath
			autoReload: window.autoReload
		}
	}
}
