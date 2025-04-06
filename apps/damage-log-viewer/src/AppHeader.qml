import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
	id: root
	color: "#111111"
	property alias title: titleLabel.text

	RowLayout {
		anchors.fill: parent

		Label {
			id: titleLabel
			Layout.fillWidth: true
			horizontalAlignment: Qt.AlignHCenter
		}
	}
}
