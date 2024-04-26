import QtQuick
import QtQuick3D


CustomMaterial {
    id: root
    // external
    property color uDiffuse: "grey"
    property double uTime: 0.0

    // interior
    shadingMode: CustomMaterial.Shaded
    fragmentShader: "shaders/sea.frag"
//    vertexShader: "shaders/sea.vert"
    NumberAnimation on uTime {
        running: false
        from: 0
        to: 360
        duration: 17000
        loops: Animation.Infinite
    }
}
