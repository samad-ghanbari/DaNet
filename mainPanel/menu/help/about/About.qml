import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Page {
    anchors.fill: parent
    Image
    {
        id: backgroundImage
        source: "qrc:/Image/bg.jpg"
        anchors.fill: parent
        opacity: 1
    }

    function funcHovered()
    {
        if(verTxt.hovered)
        {
            verTxt.font.bold = true
            verTxt.scale = 1.1
            verTxt.color = "midnightblue"
        }
        else
        {
            verTxt.font.bold = false
            verTxt.scale = 1
            verTxt.color = "indigo"
        }
    }

    function closeFunc()
    {
        closeAnimation.start();
        aboutClass.closeDialog();
    }


    SequentialAnimation
    {
        id: closeAnimation

        NumberAnimation {
            target: closeBtnId
            property: "opacity"
            duration: 500
            easing.type: Easing.InOutQuad
            from: 1
            to: 0
        }

        NumberAnimation { target: logoImage; property: "rotation"; from: 0; to: 360; duration: 1000 }


        NumberAnimation {
            target: mainRecId
            property: "opacity"
            duration: 500
            easing.type: Easing.InOutQuad
            from: 1
            to: 0
        }


        NumberAnimation {
            target: splashId
            property: "scale"
            duration: 1500
            easing.type: Easing.InOutQuad
            from: 1
            to: 30
        }

    }

        ColumnLayout
        {
            anchors.fill: parent
            spacing: 5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Image {
                id: splashId
                source: "qrc:/Image/splash.png"
                Layout.preferredWidth: 260
                Layout.preferredHeight: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                opacity: 1
            }
            Rectangle
            {
                id: mainRecId
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#88DDCCFF"
                radius: 5
                border.width: 1
                border.color: "#555"


                ColumnLayout
                {
                    anchors.fill: parent
                    spacing: 5
                    TextArea
                    {
                        id: infoText
                        opacity: 1
                        Layout.fillWidth: true
                        color: "mediumvioletred"
                        font.pixelSize: 16
                        wrapMode: TextArea.WordWrap
                        text: "This program has been developed in Data Network Planning Department Of Telecommunication Company Of Tehran (TCT) based on GCC 9.3, 64 bit.\nAll rights reserved."
                        hoverEnabled: true
                        onHoveredChanged: infoText.font.bold=(hovered)? true : false;
                    }

                    RowLayout
                    {
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: 64
                        spacing: 10

                        height: 64
                        TextArea {
                            id: verTxt
                            verticalAlignment: Qt.AlignVCenter
                            text: " DaNet Version " + aboutClass.getVersion();
                            Layout.preferredHeight: 64
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            color: "indigo"
                            hoverEnabled: true
                            onHoveredChanged: {funcHovered()}
                        }

                        Image {
                            id: logoImage
                            source: "qrc:/Image/danetIcon.png"
                            Layout.preferredHeight: 64
                            Layout.preferredWidth: 64
                            Layout.alignment: Qt.AlignRight
                        }
                    }

                    TextArea {
                        id: contactTxt
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignBottom
                        color: "purple"
                        text: "Developer Samad Ghanbari\nContact:\nS.Ghanbari@TCI.ir\nGhanbari.Samad@Gmail.com"
                        font.pixelSize: 16
                        hoverEnabled: true
                        scale: 1
                        onHoveredChanged: contactTxt.font.bold=(hovered)? true : false;
                    }
                }
            }

            Button {
                id: closeBtnId
                text: "Close"
                font.bold: true
                Layout.preferredHeight: 50
                Layout.preferredWidth:  100
                Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter

                background: Rectangle {
                    id: closeBtnBackId
                    implicitWidth: 100
                    implicitHeight: 50
                    color: closeBtnId.down ? "#FAA" : "#FEE"
                    border.width: 1
                    border.color:"#f99"
                }
                onClicked:
                {
                    closeFunc();
                }
                hoverEnabled: true
                onHoveredChanged: closeBtnBackId.color= hovered? "#fde"  : "#fee";

            }
        }
    }
