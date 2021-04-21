import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Universal 2.12
import Qt.labs.qmlmodels 1.0

import mf.nc.DamageHitInfo 1.0
import mf.nc.DamageHitInfoTableModel 1.0

import "../"

Item {
	id: root

	property QtObject damageHitInfo

	readonly property int outerRectBorderWidth: 1
	readonly property color outerRectBorderColor: "#aaaaaa"
	readonly property int innerRectBorderWidth: 0
	readonly property color innerRectBorderColor: "#666666"

	onDamageHitInfoChanged: {
		damageHitInfoModel.setDamageHitInfo(damageHitInfo)
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
			topMargin: columnsHeader.implicitHeight + tableView.rowSpacing - 1
			leftMargin: rowsHeader.implicitWidth + tableView.columnSpacing - 1
			model: damageHitInfoModel
			columnSpacing: 5
			rowSpacing: 5
			clip: true
			columnWidthProvider: function (column) {
				var w = (tableView.width - tableView.leftMargin) / tableView.model.columnCount()
				if (tableView.columnSpacing > 0) {
					var spacingWidth = tableView.columnSpacing * (tableView.model.columnCount(
																	  ) - 1)
					w -= spacingWidth / tableView.model.columnCount()
				}
				return w
			}
			rowHeightProvider: function (row) {
				var h = (tableView.height - tableView.topMargin) / tableView.model.rowCount()
				if (tableView.rowSpacing > 0) {
					var spacingHeight = tableView.rowSpacing * (tableView.model.rowCount(
																	) - 1)
					h -= spacingHeight / tableView.model.rowCount()
				}
				return h
			}
			onWidthChanged: {
				tableView.forceLayout()
			}
			onHeightChanged: {
				tableView.forceLayout()
			}
			delegate: DelegateChooser {
				// Row: Raw damage
				DelegateChoice {
					row: 0
					delegate: Rectangle {
						color: row > 4 ? (!model.damage ? "transparent" : Universal.accent) : (!model.damage ? "transparent" : "#2b5640")
						border.width: outerRectBorderWidth
						border.color: outerRectBorderColor
						DamageHitInfoTableCellLabel {
							text: model.damage === undefined ? "" : model.damage
							hint: "Incoming raw damage to player"
						}
					}
				}
				// Row: Result
				DelegateChoice {
					row: 5
					delegate: Rectangle {
						color: (!model.damage ? "transparent" : Universal.accent)
						border.width: outerRectBorderWidth
						border.color: outerRectBorderColor
						RowLayout {
							id: firstRow
							anchors {
								top: parent.top
								right: parent.right
								left: parent.left
							}
							height: parent.height / 2
							spacing: 0
							Rectangle {
								Layout.fillWidth: true
								Layout.fillHeight: true
								border.width: innerRectBorderWidth
								border.color: innerRectBorderColor
								color: "transparent"
								DamageHitInfoTableCellLabel {
									text: model.damage === undefined ? "" : model.damage
									hint: "Final incoming damage"
									font.bold: true
								}
							}
						}
						RowLayout {
							anchors.top: firstRow.bottom
							anchors.left: parent.left
							anchors.right: parent.right
							anchors.bottom: parent.bottom
							spacing: 0
							Rectangle {
								Layout.fillWidth: true
								Layout.fillHeight: true
								border.width: innerRectBorderWidth
								border.color: innerRectBorderColor
								color: "transparent"
								DamageHitInfoTableCellLabel {
									text: model.damageReduction
										  === undefined ? "" : model.damageReduction
									hint: "Final damage reduction"
									font.pointSize: tableViewFontMetrics.font.pointSize * 0.9
								}
							}
							Rectangle {
								Layout.fillWidth: true
								Layout.fillHeight: true
								border.width: innerRectBorderWidth
								border.color: innerRectBorderColor
								color: "transparent"
								DamageHitInfoTableCellLabel {
									text: model.damageReductionPercentage
										  === undefined ? "" : model.damageReductionPercentage + "%"
									hint: "Final damage reduction in %"
									font.pointSize: tableViewFontMetrics.font.pointSize * 0.9
								}
							}
						}
					}
				}
				// Row: All others (resists)
				DelegateChoice {
					delegate: Rectangle {
						color: (!model.display ? "transparent" : "#2b5640")
						border.width: outerRectBorderWidth
						border.color: outerRectBorderColor
						RowLayout {
							id: defaultFirstRow
							anchors.top: parent.top
							anchors.left: parent.left
							anchors.right: parent.right
							height: parent.height / 2
							spacing: 0
							Rectangle {
								Layout.fillWidth: true
								Layout.fillHeight: true
								border.width: innerRectBorderWidth
								border.color: innerRectBorderColor
								color: "transparent"
								DamageHitInfoTableCellLabel {
									text: model.damageReductionFromPrevious
										  === undefined ? "" : model.damageReductionFromPrevious
									hint: "Calculated damage reduction by "
										  + tableView.model.headerData(
											  row, Qt.Vertical)
								}
							}
							Rectangle {
								Layout.fillWidth: true
								Layout.fillHeight: true
								border.width: innerRectBorderWidth
								border.color: innerRectBorderColor
								color: "transparent"
								DamageHitInfoTableCellLabel {
									text: model.damageReductionPercentageFromPrevious
										  === undefined ? "" : model.damageReductionPercentageFromPrevious + "%"
									hint: "Calculated damage reduction by  "
										  + tableView.model.headerData(
											  row, Qt.Vertical) + " in %"
								}
							}
						}
						RowLayout {
							anchors.top: defaultFirstRow.bottom
							anchors.left: parent.left
							anchors.right: parent.right
							anchors.bottom: parent.bottom
							anchors.topMargin: -1
							spacing: 0
							Rectangle {
								Layout.fillWidth: true
								Layout.fillHeight: true
								border.width: innerRectBorderWidth
								border.color: innerRectBorderColor
								color: "transparent"
								DamageHitInfoTableCellLabel {
									text: model.damage === undefined ? "" : model.damage
									hint: "Remaining damage after reduction of "
										  + tableView.model.headerData(
											  row, Qt.Vertical)
									font.pointSize: tableViewFontMetrics.font.pointSize * 0.9
								}
							}
							Rectangle {
								Layout.fillWidth: true
								Layout.fillHeight: true
								border.width: innerRectBorderWidth
								border.color: innerRectBorderColor
								color: "transparent"
								DamageHitInfoTableCellLabel {
									text: model.damageReduction
										  === undefined ? "" : model.damageReduction
									hint: "Accumulated damage reduction after "
										  + tableView.model.headerData(
											  row, Qt.Vertical)
									font.pointSize: tableViewFontMetrics.font.pointSize * 0.9
								}
							}
							Rectangle {
								Layout.fillWidth: true
								Layout.fillHeight: true
								border.width: innerRectBorderWidth
								border.color: innerRectBorderColor
								color: "transparent"
								DamageHitInfoTableCellLabel {
									text: model.damageReductionPercentage
										  === undefined ? "" : model.damageReductionPercentage + "%"
									hint: "Accumulated damage reduction after "
										  + tableView.model.headerData(
											  row, Qt.Vertical) + " in %"
									font.pointSize: tableViewFontMetrics.font.pointSize * 0.9
								}
							}
						}
					}
				} // DelgateChoice
			}

			//			MouseArea {
			//				anchors.fill: parent
			//				enabled: !!model.display
			//				onClicked: {
			//					app.copyToClipboard(model.display)
			//				}
			//			}
			FontMetrics {
				id: tableViewFontMetrics
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
							text: tableView.model.headerData(modelData,
															 Qt.Horizontal)
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
						color: index > 4 ? "#666666" : "#333333"
						border.width: 1
						border.color: "#aaaaaa"
						Label {
							anchors.fill: parent
							anchors.leftMargin: 5
							verticalAlignment: Qt.AlignVCenter
							text: tableView.model.headerData(modelData,
															 Qt.Vertical)
						}
					}
				}
			}
		}
	}
}
