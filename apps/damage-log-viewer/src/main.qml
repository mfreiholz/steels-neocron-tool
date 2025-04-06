import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls.Universal
import Qt.labs.qmlmodels
import "qrc:/qml/welcome"

ApplicationWindow {
	id: window
	width: 1280
	height: 800
	visible: true

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
				text: Qt.application.displayName + " " + Qt.application.version
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
					stack.push("qrc:/qml/about/AboutPage.qml")
				}
			}
		}
	}

	StackView {
		id: stack
		anchors.fill: parent
		initialItem: Welcome {
			Connections {
				function onPushed(item, properties) {
					stack.push(item, properties)
				}
			}
		}
	}
}
