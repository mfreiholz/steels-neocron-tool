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
	width: 1280
	height: 800
	visible: true
	title: Qt.application.displayName + " - " + Qt.application.version
	onActiveFocusItemChanged: console.log("ApplicationWindow::onActiveFocusItemChanged()", activeFocusItem)

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
				color: "black"
			}
			Button {
				id: infoButton
				Layout.fillHeight: true
				enabled: true
				icon.source: "qrc:/res/info-white-24dp.svg"
				onClicked: {
					stack.push("qrc:/qml/pages/AboutPage.qml")
				}
			}
		}
	}

	StackView {
		id: stack
		anchors.fill: parent
		Component.onCompleted: {
			stack.push(startupTilesMenuComp)
			//stack.push("qrc:/qml/pages/DamageLogViewPage.qml")
			//stack.push("qrc:/qml/pages/AboutPage.qml")
		}
	}

	Component {
		id: startupTilesMenuComp
		Item {
			ColumnLayout {
				anchors.centerIn: parent
				height: 400
				width: 500
				Loader {
					Layout.fillHeight: true
					Layout.fillWidth: true
					sourceComponent: characterLogTileComp
				}
			}
		}
	}

	Component {
		id: characterLogTileComp
		Rectangle {
			anchors.fill: parent
			color: Universal.baseLowColor
			ColumnLayout {
				spacing: 10
				anchors {
					fill: parent
					margins: 10
				}
				Label {
					Layout.fillHeight: true
					Layout.fillWidth: true
					textFormat: Text.MarkdownText
					wrapMode: Text.Wrap
					text: "
# Character Log
Open your character log and analyze your resist values.

__Example file location:__ `$NEOCRON/logs/<YourCharacter>_00.log`

---

## How to enable logs?
Neocron doesn't write the required *.log files by default,
but you can enable logging in your game's configuration file.

- Open file `$NEOCRON/neocron.ini`
- Append `ENABLELOG = \"TRUE\"` to the end of file.
- Save file & Start game
"
				}
				Button {
					Layout.fillWidth: true
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
					stack.push("qrc:/qml/pages/DamageLogViewPage.qml", { logFilePath: app.urlLocalFile(fileDialog.fileUrl) })
				}
			}
		}
	}
}