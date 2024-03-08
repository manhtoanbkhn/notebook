import QtQuick 2.15
import QtQuick.Controls

Item {
    property string content: note_content.text
    property string title: note_title_text.text
    property var note_index
    Rectangle {
        id: note_title
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
        }
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
        }
    }

    TextInput {
        id: note_content
        anchors {
            left: parent.left
            top: note_title.bottom
            right: parent.right
            bottom: parent.bottom
            leftMargin: 5
        }

        text: note_list.data(note_index, Qt.DisplayRole)
        font {
            pixelSize: 30
        }
        clip: true
        wrapMode: Text.WordWrap

    }


}
