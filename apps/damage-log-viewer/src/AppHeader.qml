import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

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
