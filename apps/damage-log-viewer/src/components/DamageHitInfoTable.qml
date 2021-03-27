import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.qmlmodels 1.0

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

	// all existing damage types
	// used to build header row
	ListModel {
		id: damageTypesModel
		ListElement {
			damageType: DamageTypeInfo.Fire
			title: "Fire"
		}
		ListElement {
			damageType: DamageTypeInfo.Energy
			title: "Energy"
		}
		ListElement {
			damageType: DamageTypeInfo.XRay
			title: "X-Ray"
		}
		ListElement {
			damageType: DamageTypeInfo.Poison
			title: "Poison"
		}
		ListElement {
			damageType: DamageTypeInfo.Force
			title: "Force"
		}
		ListElement {
			damageType: DamageTypeInfo.Pierce
			title: "Pierce"
		}
	}

	DamageHitInfoTableModel {
		id: damageHitInfoModel
	}

	Rectangle {
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
				color: !model.display ? "#111111" : "darkred"
				border.width: 1
				border.color: "#aaaaaa"
				Label {
					anchors.centerIn: parent
					text: !model.display ? "-" : model.display
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
						width: 100
						height: tableView.rowHeightProvider(modelData)
						color: "#333333"
						border.width: 1
						border.color: "#aaaaaa"
						Label {
							anchors.centerIn: parent
							text: tableView.model.headerData(modelData, Qt.Vertical)
						}
					}
				}
			}
		}
	}
}

//		// damage
//		Repeater {
//			model: damageTypesModel.count
//			Rectangle {
//				Layout.fillHeight: true
//				Layout.fillWidth: true
//				color: "darkred"
//				border.width: 1
//				border.color: "#aaaaaa"
//				Label {
//					anchors.centerIn: parent
//					text: getDamage(damageTypesModel.get(index).damageType)
//				}
//			}
//		}

//		Repeater {
//			model: gridLayout.columns * (gridLayout.rows - 2)
//			Rectangle {
//				Layout.fillHeight: true
//				Layout.fillWidth: true
//				color: "green"
//				border.width: 1
//				border.color: "#aaaaaa"
//			}
//		}

/*
	TableView {
		id: view
		anchors.fill: parent
		clip: true
		reuseItems: false // forces table to destroy delegates
		columnSpacing: 3
		rowSpacing: 3
		boundsBehavior: Flickable.StopAtBounds

		model: TableModel {
			TableModelColumn { display: "type" }
			TableModelColumn { display: "damage" }
			TableModelColumn { display: "psi_resist_percent" }
			TableModelColumn { display: "shield_resist_percent" }
			TableModelColumn { display: "armor_resist_percent" }
			TableModelColumn { display: "skill_resist_percent" }
			TableModelColumn { display: "summary_resist_percent" }
			TableModelColumn { display: "damage_real" }
			rows: [
				{
					type: "fire",
					damage: 100.0,
					psi_resist_percent: 12.34,
					shield_resist_percent: 0.0,
					armor_resist_percent: 0.0,
					skill_resist_percent: 0.0,
					summary_resist_percent: 0.0,
					damage_real: 0.0
				},
				{
					type: "xray",
					damage: 100.0,
					psi_resist_percent: 12.34,
					shield_resist_percent: 0.0,
					armor_resist_percent: 0.0,
					skill_resist_percent: 0.0,
					summary_resist_percent: 0.0,
					damage_real: 0.0
				},
				{
					type: "poison",
					damage: 100.0,
					psi_resist_percent: 12.34,
					shield_resist_percent: 0.0,
					armor_resist_percent: 0.0,
					skill_resist_percent: 0.0,
					summary_resist_percent: 0.0,
					damage_real: 0.0
				}
			]
		}
		delegate: DelegateChooser {
			DelegateChoice {
				delegate: Label {
					width: 200 //(view.width / view.columns)// - (view.columnSpacing * (1 + view.columns))
					text: model.display
					horizontalAlignment: Qt.AlignHCenter
				}
			}
		}
	}
*/

	//	function has(list, item) {
	//		for (var i = 0; i < list.length; i++) {
	//			if (damageHitInfo.damageTypes[i] === damageType)
	//				return true
	//		}
	//		return false
	//	}

	//	function hasDamageType(damageType) {
	//		if (!damageHitInfo) {
	//			return false
	//		}
	//		for (var i = 0; i < damageHitInfo.damageTypes.length; i++) {
	//			if (damageHitInfo.damageTypes[i] === damageType)
	//				return true
	//		}
	//		return false
	//	}

	//	function getDamage(damageType) {
	//		return root.damageHitInfo ? root.damageHitInfo.getDamageForType(damageType) : "-"
	//	}
//}

// header for resist types
//Repeater {
//	model: damageTypesModel
//	Rectangle {
//		Layout.row: 0
//		Layout.column: index
//		Layout.fillHeight: false
//		Layout.fillWidth: true
//		Layout.preferredHeight: 30
//		color: "#333333"
//		border.width: 1
//		border.color: "#aaaaaa"
//		Label {
//			anchors.centerIn: parent
//			text: title
//		}
//	}
//}