import QtQuick 2.0


Item {
  anchors.fill: parent
  QmlMitkRenderWindowItem {

     anchors.fill: parent
     Rectangle {
       color: Qt.rgba(1, 1, 1, 0.0)
       radius: 1; border.width: 3; border.color: "magenta";
       anchors.fill: parent
  }
}

