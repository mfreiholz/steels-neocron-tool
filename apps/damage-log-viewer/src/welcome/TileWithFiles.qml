import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls.Universal

Item {
	id: root
	property alias text: description.text
	property var filePaths: []
	signal fileSelected(string filePath)

	function prettyName(path) {
		var idx = path.lastIndexOf("/")
		return path.substr(idx + 1)
	}

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
					currentIndex: -1
					highlightFollowsCurrentItem: true
					highlightMoveDuration: 150
					onCurrentIndexChanged: {
					}
					delegate: Label {
						//height: 35
						padding: 3
						width: fileListView.width
						verticalAlignment: Qt.AlignVCenter
						text: prettyName(modelData)
						MouseArea {
							anchors.fill: parent
							onClicked: {
								fileListView.currentIndex = index
							}
							onDoubleClicked: {
								fileListView.currentIndex = index
								root.fileSelected(modelData)
							}
						}
					}
					highlight: Rectangle {
						color: Universal.accent
					}
				}
			}
			RowLayout {
				Layout.fillWidth: true
				Button {
					Layout.fillWidth: true
					highlighted: true
					enabled: fileListView.currentIndex >= 0
					text: "Open Selected"
					onClicked: {
						root.fileSelected(fileListView.model[fileListView.currentIndex])
					}
				}
				Button {
					Layout.fillWidth: false
					text: "..."
					onClicked: {
						fileDialog.open()
					}
				}
			}

		}
	}

	FileDialog {
		id: fileDialog
		fileMode: FileDialog.OpenFile
		//selectExisting: true
		//selectFolder: false
		//selectMultiple: false
		visible: false
		currentFolder: app.urlFromPath(app.gameDirectoryPath)
		onAccepted: {
			if (selectedFile.length <= 0) {
				return
			}
			root.fileSelected(app.urlLocalFile(fileDialog.selectedFile))
		}
	}
}
