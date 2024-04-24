import QtQuick
import QtQuick3D


CustomMaterial {
    id: root
    // external
    property color uDiffuse: "grey"

    // interior
    shadingMode: CustomMaterial.Shaded
    fragmentShader: "shaders/pencil.frag"
}
