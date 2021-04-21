import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.12

Item {
	id: root
	property alias text: description.text
	property var filePaths: []
	signal fileSelected(string filePath)

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
				id: description
				Layout.fillHeight: true
				Layout.fillWidth: true
				textFormat: Text.MarkdownText
				wrapMode: Text.Wrap
				text: ""
			}
			Item {
				Layout.fillHeight: false
				Layout.fillWidth: true
				Layout.preferredHeight: 100
				Rectangle {
					anchors.fill: parent
					color: Universal.background
				}
				ListView {
					id: fileListView
					anchors.fill: parent
					clip: true
					model: filePaths
					delegate: Label {
						height: 20
						padding: 3
						width: fileListView.width
						text: modelData
					}
				}
			}
			Button {
				Layout.fillWidth: true
				text: "Select *.log file"
				onClicked: {
					fileDialog.open()
				}
			}
		}
	}

	FileDialog {
		id: fileDialog
		selectExisting: true
		selectFolder: false
		selectMultiple: false
		visible: false
		folder: app.urlFromPath(app.gameDirectoryPath)
		onAccepted: {
			if (fileUrl.length <= 0) {
				return
			}
			root.fileSelected(app.urlLocalFile(fileDialog.fileUrl))
		}
	}
}
