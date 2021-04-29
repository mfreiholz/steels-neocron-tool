import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Pane {
	id: root

	ColumnLayout {
		anchors.fill: parent

		Label {
			text: Qt.application.displayName + " (" + Qt.application.version + ")"
			font.pointSize: 24.0
		}
		Label {
			Layout.fillWidth: true
			Layout.fillHeight: true
			wrapMode: Text.Wrap
			textFormat: Text.MarkdownText
			text: app.readTextFileContents(":/res/AboutPage.md")
		}
	}
}
