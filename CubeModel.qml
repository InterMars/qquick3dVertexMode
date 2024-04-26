import QtQuick
import QtQuick3D
import DemoA

Model {
    scale: Qt.vector3d(100, 100, 100)
    geometry: CubeGeometry {
    }
    materials: PencilMaterial {
        id: idMaterial
        uDiffuse: "red"
    }
}
