import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

ApplicationWindow {
    visible: true
    width: 400
    height: 200
    title: "App"

    FileDialog {
        id: fileDialog
        title: "Selecciona archivo KML"
        folder: shortcuts.home
        nameFilters: ["Archivos KML (*.kml)"]
        onAccepted: {
            if (fileDialog.fileUrls.length > 0) {
                var selectedFileUrl = fileDialog.fileUrls[0].toString();
                LogicKml.parseKMLFile(selectedFileUrl);
            } else {
                console.log("No se seleccionó ningún archivo KML.");
            }
        }
    }

    Button {
        text: "Seleccionar archivo KML"
        anchors.centerIn: parent
        onClicked: {
            fileDialog.open()
        }
    }
}
