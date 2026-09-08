import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    id: window

    width: 1100
    height: 750
    visible: true
    title: "Biomorph Generator"

    minimumWidth: 900
    minimumHeight: 650

    function fixRatio() {
        let cx = (biomorph.xmin + biomorph.xmax) / 2.0;
        let cy = (biomorph.ymin + biomorph.ymax) / 2.0;
        
        let w = Math.abs(biomorph.xmax - biomorph.xmin);
        let h = Math.abs(biomorph.ymax - biomorph.ymin);
        
        // Pick the larger dimension so we don't crop out what you're seeing
        let size = Math.max(w, h); 
        
        biomorph.xmin = cx - size / 2.0;
        biomorph.xmax = cx + size / 2.0;
        biomorph.ymin = cy - size / 2.0;
        biomorph.ymax = cy + size / 2.0;
        
        biomorph.generatePreview();
    }

    function pan(dx, dy) {
        let w = biomorph.xmax - biomorph.xmin;
        let h = biomorph.ymax - biomorph.ymin;
        
        biomorph.xmin += w * dx;
        biomorph.xmax += w * dx;
        biomorph.ymin += h * dy;
        biomorph.ymax += h * dy;
        
        biomorph.generatePreview();
    }

    function zoom(factor) {
        let cx = (biomorph.xmin + biomorph.xmax) / 2.0;
        let cy = (biomorph.ymin + biomorph.ymax) / 2.0;
        let w = biomorph.xmax - biomorph.xmin;
        let h = biomorph.ymax - biomorph.ymin;
        
        biomorph.xmin = cx - (w * factor) / 2.0;
        biomorph.xmax = cx + (w * factor) / 2.0;
        biomorph.ymin = cy - (h * factor) / 2.0;
        biomorph.ymax = cy + (h * factor) / 2.0;
        
        biomorph.generatePreview();
    }

    FileDialog {
        id: exportDialog

        title: "Export Biomorph"
        fileMode: FileDialog.SaveFile

        nameFilters: [
            "PNG image (*.png)",
            "All files (*)"
        ]

        defaultSuffix: "png"

        onAccepted: {
            biomorph.exportImage(selectedFile)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 16


            // Parameter panel

            ScrollView {
                Layout.preferredWidth: 360
                Layout.fillHeight: true
                clip: true

                ColumnLayout {
                    width: parent.width
                    spacing: 12

                    Label {
                        text: "Biomorph Parameters"
                        font.pixelSize: 22
                        font.bold: true
                    }

                    GroupBox {
                        title: "Domain"
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 12

                            GroupBox {
                                title: "Navigation"
                                Layout.fillWidth: true

                                GridLayout {
                                    anchors.fill: parent
                                    columns: 3
                                    columnSpacing: 8
                                    rowSpacing: 8

                                    // Top Row
                                    Button {
                                        text: "Zoom In"
                                        Layout.fillWidth: true
                                        enabled: !biomorph.generating
                                        onClicked: window.zoom(0.8) 
                                    }
                                    Button {
                                        text: "Up"
                                        Layout.fillWidth: true
                                        enabled: !biomorph.generating
                                        onClicked: window.pan(0, 0.1) 
                                    }
                                    Button {
                                        text: "Zoom Out"
                                        Layout.fillWidth: true
                                        enabled: !biomorph.generating
                                        onClicked: window.zoom(1.25) 
                                    }

                                    // Bottom Row
                                    Button {
                                        text: "Left"
                                        Layout.fillWidth: true
                                        enabled: !biomorph.generating
                                        onClicked: window.pan(-0.1, 0)
                                    }
                                    Button {
                                        text: "Down"
                                        Layout.fillWidth: true
                                        enabled: !biomorph.generating
                                        onClicked: window.pan(0, -0.1)
                                    }
                                    Button {
                                        text: "Right"
                                        Layout.fillWidth: true
                                        enabled: !biomorph.generating
                                        onClicked: window.pan(0.1, 0)
                                    }
                                }
                            }

                            GridLayout {
                                // Replaced anchors.fill: parent with Layout properties
                                Layout.fillWidth: true
                                columns: 2
                                columnSpacing: 10
                                rowSpacing: 8

                                Label { text: "X min" }

                                TextField {
                                    Layout.fillWidth: true
                                    text: biomorph.xmin
                                    validator: DoubleValidator {}
                                    onEditingFinished: {
                                        biomorph.xmin = Number(text)
                                    }
                                }

                                Label { text: "X max" }

                                TextField {
                                    Layout.fillWidth: true
                                    text: biomorph.xmax
                                    validator: DoubleValidator {}
                                    onEditingFinished: {
                                        biomorph.xmax = Number(text)
                                    }
                                }

                                Label { text: "Y min" }

                                TextField {
                                    Layout.fillWidth: true
                                    text: biomorph.ymin
                                    validator: DoubleValidator {}
                                    onEditingFinished: {
                                        biomorph.ymin = Number(text)
                                    }
                                }

                                Label { text: "Y max" }

                                TextField {
                                    Layout.fillWidth: true
                                    text: biomorph.ymax
                                    validator: DoubleValidator {}
                                    onEditingFinished: {
                                        biomorph.ymax = Number(text)
                                    }
                                }

                                Button {
                                    text: "Fix 1:1 Ratio"
                                    Layout.fillWidth: true
                                    enabled: !biomorph.generating
                                    onClicked: window.fixRatio()
                                }
                            }
                        }
                    }

                    GroupBox {
                        title: "Parameters"
                        Layout.fillWidth: true

                        GridLayout {
                            anchors.fill: parent
                            columns: 2
                            columnSpacing: 10
                            rowSpacing: 8

                            Label { text: "Alpha" }

                            TextField {
                                Layout.fillWidth: true
                                text: biomorph.alpha

                                validator: DoubleValidator {}

                                onEditingFinished: {
                                    biomorph.alpha = Number(text)
                                }
                            }

                            Label { text: "Beta" }

                            TextField {
                                Layout.fillWidth: true
                                text: biomorph.beta

                                validator: DoubleValidator {}

                                onEditingFinished: {
                                    biomorph.beta = Number(text)
                                }
                            }

                            Label { text: "K" }

                            SpinBox {
                                Layout.fillWidth: true
                                from: 1
                                to: 1000
                                value: biomorph.K

                                onValueModified: {
                                    biomorph.K = value
                                }
                            }

                            Label { text: "R" }

                            TextField {
                                Layout.fillWidth: true
                                text: biomorph.R

                                validator: DoubleValidator {
                                    bottom: 0
                                }

                                onEditingFinished: {
                                    biomorph.R = Number(text)
                                }
                            }

                            Label { text: "Frequency" }

                            TextField {
                                Layout.fillWidth: true
                                text: biomorph.freq

                                validator: DoubleValidator {}

                                onEditingFinished: {
                                    biomorph.freq = Number(text)
                                }
                            }

                            Label { text: "Switch" }

                            CheckBox {
                                checked: biomorph.useSwitch

                                onToggled: {
                                    biomorph.useSwitch = checked
                                }
                            }
                        }
                    }

                    GroupBox {
                        title: "Growth Constant 1"
                        Layout.fillWidth: true

                        GridLayout {
                            anchors.fill: parent
                            columns: 2
                            columnSpacing: 10
                            rowSpacing: 8

                            Label { text: "X" }

                            TextField {
                                Layout.fillWidth: true
                                text: biomorph.gc1x

                                validator: DoubleValidator {}

                                onEditingFinished: {
                                    biomorph.gc1x = Number(text)
                                }
                            }

                            Label { text: "Y" }

                            TextField {
                                Layout.fillWidth: true
                                text: biomorph.gc1y

                                validator: DoubleValidator {}

                                onEditingFinished: {
                                    biomorph.gc1y = Number(text)
                                }
                            }
                        }
                    }

                    GroupBox {
                        title: "Growth Constant 2"
                        Layout.fillWidth: true

                        GridLayout {
                            anchors.fill: parent
                            columns: 2
                            columnSpacing: 10
                            rowSpacing: 8

                            Label { text: "X" }

                            TextField {
                                Layout.fillWidth: true
                                text: biomorph.gc2x

                                validator: DoubleValidator {}

                                onEditingFinished: {
                                    biomorph.gc2x = Number(text)
                                }
                            }

                            Label { text: "Y" }

                            TextField {
                                Layout.fillWidth: true
                                text: biomorph.gc2y

                                validator: DoubleValidator {}

                                onEditingFinished: {
                                    biomorph.gc2y = Number(text)
                                }
                            }
                        }
                    }
                }
            }


            // Preview

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                color: "#202020"
                radius: 4

                Image {
                    id: previewImage

                    anchors.centerIn: parent

                    width: Math.min(parent.width - 20, parent.height - 20)
                    height: width

                    fillMode: Image.PreserveAspectFit
                    cache: false

                    source: ""

                    asynchronous: true

                    onStatusChanged: {
                        if (status === Image.Error) {
                            console.log("Failed to load preview:", source)
                        }
                    }
                }

                Label {
                    anchors.centerIn: parent

                    visible: previewImage.source === ""

                    text: "No preview"
                    color: "#aaaaaa"
                    font.pixelSize: 18
                }

                BusyIndicator {
                    anchors.centerIn: parent

                    running: biomorph.generating
                    visible: running
                }
            }
        }


        // Bottom controls

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Label {
                text: biomorph.status
                Layout.fillWidth: true
            }

            Button {
                text: "Preview"

                enabled: !biomorph.generating

                onClicked: {
                    biomorph.generatePreview()
                }
            }

            Button {
                text: "Export PNG"

                enabled: !biomorph.generating

                onClicked: {
                    exportDialog.open()
                }
            }
        }
    }


    // Controller signals

    Connections {
        target: biomorph

        function onPreviewReady(url) {
            // Force QML to reload the same temporary file.
            previewImage.source = ""
            previewImage.source = url
        }
    }
}