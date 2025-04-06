import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
