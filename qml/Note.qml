import QtQuick 2.15
import QtQuick.Controls

Item {
    property string content: note_content.text
    property string title: note_title_text.text
    property var note_index
    Rectangle {
        id: note_title
        x: 0
        y: 0
        width: parent.width
        height: 80
        border {
            width: 0.5
            color: "green"
        }

        TextInput {
            id: note_title_text
            anchors.fill: parent
            text: note_list.headerData(note_index.row, Qt.Vertical)
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font {
                pixelSize: Math.min(parent.height, parent.width) * 0.8
            }
            color: "blue"
            onTextChanged: note_controller.saveEditingNote(text, note_content.text)
        }
    }

    TextEdit {
        id: note_content
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.topMargin: 85

        text: note_list.data(note_index, Qt.DisplayRole)
        font {
            pixelSize: 30
        }
        clip: true
        wrapMode: Text.WordWrap
        inputMethodHints: Qt.ImhMultiLine
        onTextChanged: {
            note_controller.saveEditingNote(note_title_text.text, text)
        }

        // activeFocusOnPress: false
        // cursorVisible: true

    }
}
