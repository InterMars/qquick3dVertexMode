import QtQuick
import QtQuick3D
import DemoA

Model {
    id: root
    property bool isPicked: false
    onIsPickedChanged: {
        idMaterial.uDiffuse = isPicked ? "red" : "grey"
    }
    geometry: DynamicGeometry {}

    pickable: true
    NumberAnimation on eulerRotation.y {
        from: 0
        to: 360
        duration: 17000
        loops: Animation.Infinite
    }

    MorphTarget {
        id: morphtarget
        attributes: MorphTarget.Position | MorphTarget.Normal
        SequentialAnimation on weight {
            PauseAnimation { duration: 1000 }
            NumberAnimation { from: 0; to: 1; duration: 4000 }
            PauseAnimation { duration: 1000 }
            NumberAnimation { from: 1; to: 0; duration: 4000 }
            loops: Animation.Infinite
        }
    }
    morphTargets: [ morphtarget ]
    materials: PencilMaterial {
        id: idMaterial
        uDiffuse: "grey"
    }
}
