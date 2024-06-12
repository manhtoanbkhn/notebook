import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import model.notelist 1.0

Item {
    MessageDialog {
        property int index_note
        id: dialog
        text: qsTr("Delete this note?")
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onButtonClicked: function (button, role) {
            switch(button) {
            case MessageDialog.Ok:
                note_controller.remove(index_note)
                break;
            }
        }
    }

    ListView {
        id: list_view
        anchors.fill: parent
        model: note_list
        delegate: Button {
            id: a_note
            width: list_view.width
            text: note_list.headerData(index, Qt.Vertical, ModelNoteList.TitleRole)

            contentItem: Text {
                id: note_header
                text: a_note.text
                anchors.fill: parent
                font {
                    pixelSize: 40
                }
                anchors.leftMargin: 5
                color: "blue"
                verticalAlignment: Text.AlignTop
            }
            background: Rectangle {
                anchors.fill: parent
                opacity: enabled ? 1.0 : 0.3
                color: index % 2 ? "#f7f7e9" : "#f1fbf7"
                radius: 2
                Text {
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 5
                    text: TimeRole
                    color: "blue"
                    opacity: 0.4
                    font.pixelSize: 20
                }
            }
            onClicked: note_controller.setEditingNote(index)
            onPressAndHold: {
                dialog.index_note = index
                dialog.open()
            }
        }
    }

    Button {
        id: add_new_note_button
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: 80
        height: 80
        background: Rectangle {
            property color default_color:  Qt.rgba(0, 0.5, 0.7, 0.7)
            anchors.fill: parent
            radius: width / 2
            color: add_new_note_button.down ? Qt.darker(default_color) : default_color
            Text {
                anchors.centerIn: parent
                text: qsTr("+")
                font.pixelSize: 40
                font.weight: Font.ExtraLight
            }
        }
        onClicked: note_controller.createNewNote()
    }
}
