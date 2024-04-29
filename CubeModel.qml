import QtQuick
import QtQuick3D
import DemoA

Model {
    scale: Qt.vector3d(100, 100, 100)
    geometry: CubeGeometry {
    }
    materials: DefaultMaterial {
        id: idMaterial
        diffuseColor:  "red"
    }
}
