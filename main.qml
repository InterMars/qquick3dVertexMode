import QtQuick
import QtQuick3D
import QtQuick.Window
import QtQuick3D.Helpers
import QtQuick3D.AssetUtils


Window {
    id: idwindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


    View3D {
        id: view
        anchors.fill: parent
        renderMode: View3D.Underlay
        camera: orbitCamera
        environment: SceneEnvironment {
            debugSettings.wireframeEnabled: false
            backgroundMode: SceneEnvironment.SkyBox
            lightProbe: Texture {
                textureData: ProceduralSkyTextureData{}
            }
            InfiniteGrid {
                visible: true
                gridInterval: 100
            }
        }
        DirectionalLight {
            eulerRotation.x: -10
            NumberAnimation on eulerRotation.y {
                from: 0
                to: 360
                duration: 17000
                loops: Animation.Infinite
            }
            ambientColor: Qt.rgba(0.3, 0.3, 0.3, 1.0)
        }

        PointLight {
            position: Qt.vector3d(-15, 50, 15)
            eulerRotation: Qt.vector3d(0, -3, 0)
        }
        Node {
            id: orbitCameraNode
            PerspectiveCamera {
                id: orbitCamera
                z: 200
            }
            PerspectiveCamera {
                id: unuseCamera
                z: 200
            }
        }
        OrbitCameraController {
            id: orbitController
            origin: orbitCameraNode
            camera: orbitCamera
        }

        DynamicModel {
            id: idModelA
            objectName: "modela"

            z: 50
        }
        DynamicModel {
            id: idModelB
            z: 50
            objectName: "modelb"
        }

        Model {
            id: idPlaneXoY
            source: "#Rectangle"
            objectName: "plane_xoy"
            materials: PrincipledMaterial {
                baseColor: "red"
            }
            opacity: 0.0
            pickable: true

            scale: Qt.vector3d(100.0, 100.0, 100.0)
        }
        Item {
            CustomMaterial {
                id: idPencilMaterial
                shadingMode: CustomMaterial.Shaded
                fragmentShader: "shaders/pencil.frag"
                property color uDiffuse: "grey"
            }
            PrincipledMaterial {
                id: idNormalMaterial
                baseColor: "#af4f1f"
                roughness: 0.3
                specularAmount: 0.6
            }
        }
    }
    Shortcut {
        sequence: "Esc"
        onActivated: {
            Qt.quit()
        }
    }
    MouseArea {
        id: idmouse
        anchors.fill: view
        onPressed: {
            orbitController.camera = unuseCamera
            var result = view.pick(mouseX, mouseY)
            var pos = Qt.vector3d(result.scenePosition.x.toFixed(2),
                                  result.scenePosition.y.toFixed(2),
                                  result.scenePosition.z.toFixed(2))
            console.log(pos)
            if(result.objectHit) {
                var objhit = result.objectHit
                console.log(objhit.objectName)
                idModelA.position = pos
            }
        }
        onReleased: {
            orbitController.camera = orbitCamera
        }
    }
}
