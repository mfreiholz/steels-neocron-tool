import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Universal 2.12
import mf.nc.DamageTypeInfo 1.0
import mf.nc.DamageHitInfo 1.0
import mf.nc.DamageHitInfoTableModel 1.0

Item {
	id: root

	property QtObject damageHitInfo

	onDamageHitInfoChanged: {
		console.log("DamageHitInfo::onDamageHitInfoChanged() ", damageHitInfo)
		damageHitInfoModel.setDamageHitInfo(damageHitInfo)
		if (damageHitInfo) {
			console.log("hitZone=", damageHitInfo.hitZone)
			console.log("types=", damageHitInfo.damageTypes)
		}
	}

	DamageHitInfoTableModel {
		id: damageHitInfoModel
	}

	Rectangle {
		id: rect
		anchors.fill: parent
		color: "transparent"

		TableView {
			id: tableView
			anchors.fill: parent
			topMargin:  columnsHeader.implicitHeight + tableView.rowSpacing - 1
			leftMargin: rowsHeader.implicitWidth + tableView.columnSpacing - 1
			model: damageHitInfoModel
			columnSpacing: 5
			rowSpacing: 5
			clip: true
			columnWidthProvider: function(column) {
				var w = (tableView.width - tableView.leftMargin) / tableView.model.columnCount()
				if (tableView.columnSpacing > 0) {
					var spacingWidth = tableView.columnSpacing * (tableView.model.columnCount() - 1)
					w -= spacingWidth / tableView.model.columnCount()
				}
				return w
			}
			rowHeightProvider: function(row) {
				var h = (tableView.height - tableView.topMargin) / tableView.model.rowCount()
				if (tableView.rowSpacing > 0) {
					var spacingHeight = tableView.rowSpacing * (tableView.model.rowCount() - 1)
					h -= spacingHeight / tableView.model.rowCount()
				}
				return h
			}
			onWidthChanged: {
				tableView.forceLayout()
			}
			delegate: Rectangle {
				color: !model.display ? Universal.baseLowColor : Universal.altLowColor
				border.width: 1
				border.color: "#aaaaaa"
				Label {
					anchors.centerIn: parent
					text: !model.display ? "-" : model.display
				}
				MouseArea {
					anchors.fill: parent
					enabled: !!model.display
					onClicked: {
						app.copyToClipboard(model.display)
					}
				}
			}

			Row {
				id: columnsHeader
				y: tableView.contentY
				z: 2
				spacing: tableView.columnSpacing

				Repeater {
					model: tableView.model.columnCount()

					Rectangle {
						height: 30
						width: tableView.columnWidthProvider(modelData)
						color: "#333333"
						border.width: 1
						border.color: "#aaaaaa"
						Label {
							anchors.centerIn: parent
							text: tableView.model.headerData(modelData, Qt.Horizontal)
						}
					}
				}
			}

			Column {
				id: rowsHeader
				x: tableView.contentX
				z: 2
				spacing: tableView.rowSpacing

				Repeater {
					model: tableView.model.rowCount()

					Rectangle {
						width: 150
						height: tableView.rowHeightProvider(modelData)
						color: "#333333"
						border.width: 1
						border.color: "#aaaaaa"
						Label {
							anchors.fill: parent
							anchors.leftMargin: 5
							verticalAlignment: Qt.AlignVCenter
							text: tableView.model.headerData(modelData, Qt.Vertical)
						}
					}
				}
			}
		}
	}
}