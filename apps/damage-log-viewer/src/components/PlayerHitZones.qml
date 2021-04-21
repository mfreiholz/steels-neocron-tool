import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import mf.nc.Types 1.0

Item {
	id: root

	property var hitZones: []
	readonly property color textColor: "black"
	readonly property color areaBackgroundColor: "#093d18"
	readonly property color areaBorderColor: "#bbefb3"
	readonly property color areaHighlightBackgroundColor: "#60c950"
	readonly property color areaHighlightBorderColor: "#bbefb3"

	onHitZonesChanged: {
		headShape.color = checkHighlight(HitZone.Head) ? areaHighlightBackgroundColor : areaBackgroundColor
		torsoShape.color = checkHighlight(HitZone.Torso) ? areaHighlightBackgroundColor : areaBackgroundColor
		legsShape.color = checkHighlight(HitZone.Legs) ? areaHighlightBackgroundColor : areaBackgroundColor
	}

	// @note We need to convert the "hitZone" values to Number, before we check.
	// It seems to be a Qt bug that "HitZone.All" is a Number and not the type.
	function checkHighlight(zone) {
		for (var i = 0; i < hitZones.length; i++) {
			if (Number(hitZones[i]) === HitZone.All || Number(hitZones[i]) === zone) {
				return true;
			}
		}
		return false;
	}

	ColumnLayout {
		anchors.fill: parent
		anchors.margins: 5
		spacing: 5

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
