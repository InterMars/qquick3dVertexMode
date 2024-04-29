import QtQuick
import QtQuick3D
import DemoA

Model {
    id: root
    property bool isPicked: false
    pickable: true
    property alias boundMax: idGeometry.boundMax
    onIsPickedChanged: {
        idMaterial.uDiffuse = isPicked ? "red" : "grey"
    }
    geometry: DynamicGeometry {
        id: idGeometry
    }

    // MorphTarget {
    //     id: morphtarget
    //     attributes: MorphTarget.Position | MorphTarget.Normal
    //     SequentialAnimation on weight {
    //         PauseAnimation { duration: 1000 }
    //         NumberAnimation { from: 0; to: 1; duration: 4000 }
    //         PauseAnimation { duration: 1000 }
    //         NumberAnimation { from: 1; to: 0; duration: 4000 }
    //         loops: Animation.Infinite
    //     }
    // }
    // morphTargets: [ morphtarget ]
    // materials: DefaultMaterial {
    //     diffuseColor: "blue"
    // }
    materials: WaterMaterial {
        id: idMaterial
        uDiffuse: "grey"
    }

}
