import QtQuick
import QtQuick3D


CustomMaterial {
    id: root
    // external
    property color uDiffuse: "grey"
    property double iTime: 0.0

    // interior
    shadingMode: CustomMaterial.Shaded
    fragmentShader: "qrc:/shaders/basic.frag"
    vertexShader: "qrc:/shaders/sea.vert"

    NumberAnimation on iTime {
        running: true
        from: 0
        to: 100
        duration: 30000
        loops: Animation.Infinite
    }
}
