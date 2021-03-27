import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
	id: root
	readonly property color areaBackgroundColor: "#093d18"
	readonly property color areaBorderColor: "#bbefb3"
	readonly property color areaHighlightBackgroundColor: "#60c950"
	readonly property color areaHighlightBorderColor: "#bbefb3"

	ColumnLayout {
		anchors.fill: parent

		Rectangle {
			id: headShape
			Layout.fillHeight: true
			Layout.fillWidth: true
			color: areaBackgroundColor
			border.width: 1
			border.color: areaBorderColor
		}
		Rectangle {
			id: torseShape
			Layout.fillHeight: true
			Layout.fillWidth: true
			color: areaBackgroundColor
			border.width: 1
			border.color: areaBorderColor
		}
		Rectangle {
			id: legsShape
			Layout.fillHeight: true
			Layout.fillWidth: true
			color: areaBackgroundColor
			border.width: 1
			border.color: areaBorderColor
		}
	}
}
