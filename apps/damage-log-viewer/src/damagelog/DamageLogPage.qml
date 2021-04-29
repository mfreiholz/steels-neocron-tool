import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15
import mf.nc.DamageLogPageBackend 1.0
import mf.nc.DamageLogFileReader 1.0
import mf.nc.DamageMeterModel 1.0

Item {
	id: root
	property string logFilePath
	property QtObject selectedEntry

	DamageLogPageBackend {
		id: backend
	}

	DamageMeterModel {
		id: model
	}

	DamageLogFileReader {
		id: loader
		logFilePath: root.logFilePath
		paused: !liveUpdateCheckBox.checked
		onLogFilePathChanged: {
			console.log("DamageLogFileReader::onLogFilePathChanged()", path)
			if (path.length > 0) {
				loader.start()
			}
		}
		onFileSizeChanged: {
			console.log("DamageLogFileReader::onFileSizeChanged()", fileSize)
		}
		onNewLog: {
			model.add(entry, false)
		}
		onFileEnd: {
			console.log("DamageLogFileReader::onFileEnd(): ", offset)
			model.updateRowStates()
		}
		onErrorOccurred: {
			console.log("DamageLogFileReader::onErrorOccurred()", errorString)
			//			errorDialog.text = errorString
			//			errorDialog.open()
		}
		onFinished: {
			console.log("DamageLogFileReader::onFinished()")
		}
	}

	Frame {
		id: leftArea
		anchors {
			top: parent.top
			right: rightArea.left
			bottom: parent.bottom
			left: parent.left
		}
		ListView {
			id: listView
			anchors.fill: parent
			clip: true
			currentIndex: -1
			model: model
			spacing: 2
			delegate: RowLayout {
				width: listView.width
				Item {
					Layout.fillWidth: true
					Layout.preferredHeight: 35
					Rectangle {
						anchors.top: parent.top
						anchors.bottom: parent.bottom
						anchors.left: parent.left
						width: parent.width / 100.0 * model.percent
						color: "#ff9900"
					}
					RowLayout {
						anchors.fill: parent
						spacing: 5
						Label {
							Layout.alignment: Qt.AlignCenter
							text: model.who
						}
						Item {
							Layout.fillHeight: true
							Layout.fillWidth: true
						}
						Label {
							Layout.alignment: Qt.AlignCenter
							text: formatNumber(model.summary)
							font: fixedFont
						}
						Label {
							//Layout.alignment: Qt.AlignCenter
							Layout.preferredWidth: 100
							horizontalAlignment: Qt.AlignRight
							text: model.percentFormatted + "%"
							font: fixedFont
						}
					}
				}
			}
		}
	}

	Frame {
		id: rightArea
		anchors {
			top: parent.top
			right: parent.right
			bottom: parent.bottom
			left: undefined
		}
		width: 300

		ColumnLayout {
			anchors.fill: parent
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
		}
	}

	// Helper functions

	function formatNumber(val) {
		return val.toFixed(3)
	}
}
