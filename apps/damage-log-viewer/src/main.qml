import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.12
import "qrc:/qml/welcome"

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