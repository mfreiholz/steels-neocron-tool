import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls.Universal

/*
	Shows Neocron directory selection + Tiles.
*/
Item {
	id: root
	signal pushed(string item, var properties)
	property int tileWidth: 400

	// Game Directory Selection
	Item {
		id: gameDirSelectionItem
		anchors {
			top: parent.top
			right: parent.right
			bottom: undefined
			left: parent.left
		}
		height: 80
		Item {
			anchors.fill: parent
			anchors.margins: 12
			RowLayout {
				anchors.fill: parent
				Label {
					Layout.fillWidth: false
					Layout.fillHeight: true
					padding: 5
					text: "Game Directory: "
					verticalAlignment: Qt.AlignVCenter
					background: Rectangle {
						color: Universal.baseLowColor
					}
					font.bold: true
				}
				Label {
					Layout.fillWidth: true
					Layout.fillHeight: true
					padding: 5
					text: app.gameDirectoryPath
					font: fixedFont
					verticalAlignment: Qt.AlignVCenter
					background: Rectangle {
						color: Universal.baseLowColor
					}
				}
				Button {
					Layout.fillWidth: false
					Layout.fillHeight: true
					text: "Refresh"
					icon.source: "qrc:/res/refresh_white_24dp.svg"
					display: AbstractButton.IconOnly
					onClicked: {
						app.updateNeocronDirectoryFiles()
					}
				}
				Button {
					Layout.fillWidth: false
					Layout.fillHeight: true
					text: "Change..."
					highlighted: true
					onClicked: {
						gameDirFileDialog.open()
					}
				}
			}
		}
		FolderDialog {
			id: gameDirFileDialog
			visible: false
			onAccepted: {
				var folderPath = app.urlLocalFile(gameDirFileDialog.selectedFolder)
				app.gameDirectoryPath = folderPath
			}
		}
	}

	// Tiles
	Item {
		id: tilesContainerItem
		anchors {
			top: gameDirSelectionItem.bottom
			right: parent.right
			bottom: parent.bottom
			left: parent.left
		}
		Pane {
			anchors.fill: parent
			leftPadding: 12
			rightPadding: 12
			topPadding: 0
			bottomPadding: 12
			Row {
				id: tileRow
				height: parent.height
				spacing: 10

				TileWithFiles {
					id: damageLogTile
					anchors.top: tileRow.top
					anchors.bottom: tileRow.bottom
					width: root.tileWidth
					filePaths: []
					text: app.readTextFileContents(":/res/DamageLogTile.md")
					Connections {
						function onFileSelected(filePath) {
							root.pushed("qrc:/qml/damagelog/DamageLogPage.qml", { logFilePath: filePath })
						}
					}
				}
				TileWithFiles {
					id: characterLogTile
					anchors.top: tileRow.top
					anchors.bottom: tileRow.bottom
					width: root.tileWidth
					filePaths: []
					text: app.readTextFileContents(":/res/CharacterLogTile.md")
					Connections {
						function onFileSelected(filePath) {
							root.pushed("qrc:/qml/characterlog/CharacterLogPage.qml", { logFilePath: filePath })
						}
					}
				}
			}
		}
	}

	// Global events
	Component.onCompleted: {
		if (app.gameDirectoryPath.length > 0) {
			app.updateNeocronDirectoryFiles()
		}
	}

	Connections {
		target: app
		function onGameDirectoryPathChanged() {
			app.updateNeocronDirectoryFiles()
		}
		function onGameDirectoryFilesChanged(damageLogFiles, characterLogFiles) {
			damageLogTile.filePaths = damageLogFiles
			characterLogTile.filePaths = characterLogFiles
		}
	}
}
