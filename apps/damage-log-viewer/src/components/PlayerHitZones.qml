import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
	id: root

	property string hitZone: ""
	readonly property color textColor: "black"
	readonly property color areaBackgroundColor: "#093d18"
	readonly property color areaBorderColor: "#bbefb3"
	readonly property color areaHighlightBackgroundColor: "#60c950"
	readonly property color areaHighlightBorderColor: "#bbefb3"

	onHitZoneChanged: {
		headShape.color = (hitZone == "ALL" || hitZone == "HEAD") ? areaHighlightBackgroundColor : areaBackgroundColor
		torsoShape.color = (hitZone == "ALL" || hitZone == "TORSO") ? areaHighlightBackgroundColor : areaBackgroundColor
		legsShape.color = (hitZone == "ALL" || hitZone == "LEGS") ? areaHighlightBackgroundColor : areaBackgroundColor
	}

	ColumnLayout {
		anchors.fill: parent

		Rectangle {
			id: headShape
			Layout.fillHeight: true
			Layout.fillWidth: true
			color: areaBackgroundColor
			border.width: 1
			border.color: areaBorderColor
			Label {
				anchors.centerIn: parent
				text: "Head"
			}
		}
		Rectangle {
			id: torsoShape
			Layout.fillHeight: true
			Layout.fillWidth: true
			color: areaBackgroundColor
			border.width: 1
			border.color: areaBorderColor
			Label {
				anchors.centerIn: parent
				text: "Torso"
			}
		}
		Rectangle {
			id: legsShape
			Layout.fillHeight: true
			Layout.fillWidth: true
			color: areaBackgroundColor
			border.width: 1
			border.color: areaBorderColor
			Label {
				anchors.centerIn: parent
				text: "Legs"
			}
		}
	}
}
