import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.12

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
		Frame {
			anchors.fill: parent
			anchors.margins: 5
			RowLayout {
				anchors.fill: parent
				Label {
					Layout.fillWidth: false
					text: "Game Directory: "
				}
				Label {
					Layout.fillWidth: true
					text: app.gameDirectoryPath
				}
				ToolButton {
					Layout.fillWidth: false
					text: "Change..."
					onClicked: {
						gameDirFileDialog.open()
					}
				}
			}
		}
		FileDialog {
			id: gameDirFileDialog
			visible: false
			selectExisting: true
			selectFolder: true
			selectMultiple: false
			onAccepted: {
				console.log(gameDirFileDialog.fileUrl)
				var folderPath = app.urlLocalFile(gameDirFileDialog.fileUrl)
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
			Row {
				id: tileRow
				height: parent.height
				spacing: 5

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
