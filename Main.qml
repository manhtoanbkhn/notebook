import QtQuick
import "qml"

Window {
    visible: true
    title: qsTr("My Note")

    onClosing: (close) => {
           console.log("on Closing " + loader.source.toString())
           if (loader.source.toString() === "qml/MainWindow.qml") {
               return
           }
           close.accepted = false
           if (loader.item.content !== "" || loader.item.title !== "") {
               let note = note_creator.create()
               note.content = loader.item.content
               note.title = loader.item.title
               note.time = note_controller.getTimeNowStr()
               note_controller.finishEditingNote(note)
           }
           loader.source = "qml/MainWindow.qml"
    }

    Loader {
        id: loader
        anchors.fill: parent
    }

    Connections {
        target: note_controller
        function onCreateEditingNote (note_index) {
            loader.setSource("qml/Note.qml", {"note_index": note_index})
        }
    }

    Component.onCompleted: loader.source = "qml/MainWindow.qml"

}
