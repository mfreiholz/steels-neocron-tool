import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal

Label {
	id: root

	property string hint: ""

	anchors.fill: parent
	horizontalAlignment: Qt.AlignHCenter
	verticalAlignment: Qt.AlignVCenter
	color: hh.hovered ? "#000000" : Universal.foreground

	ToolTip.text: root.hint
	ToolTip.visible: hh.hovered && text.length > 0

	HoverHandler {
		id: hh
	}
}