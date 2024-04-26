import QtQuick
import QtQuick3D
import QtQuick.Window
import QtQuick3D.Helpers
import QtQuick3D.AssetUtils

import DemoA

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
            id: env
            property alias wfenable: env.debugSettings.wireframeEnabled
            debugSettings.wireframeEnabled: false
            backgroundMode: SceneEnvironment.SkyBox
            lightProbe: Texture {
                textureData: ProceduralSkyTextureData{  }
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
                z: 500
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
                scale: Qt.vector3d(10.0, 10.0, 10.0)
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
//            z: 50
            objectName: "modelb"
        }
//        CubeModel {
//            id: idCube
//        }
    }
    Shortcut {
        sequence: "Esc"
        onActivated: {
            Qt.quit()
        }
    }
    Shortcut {
        sequence: "Shift+1"
        onActivated: {
            env.wfenable = !env.wfenable
        }
    }
    MouseArea {
        id: idmouse
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        anchors.fill: view
        onPressed: (mouse)=> {
            if(mouse.button === Qt.RightButton){
                orbitController.mouseEnabled = false
                idtimer_mousefollow.start()
            }

        }
        onReleased: (mouse)=> {
            if(mouse.button === Qt.RightButton){
                orbitController.mouseEnabled = true
                idtimer_mousefollow.stop()
            }
        }
    }
    Timer {
        id: idtimer_mousefollow
        interval: 10
        repeat: true
        running: false
        onTriggered: {
            var result = view.pick(idmouse.mouseX, idmouse.mouseY)
            var pos = Qt.vector3d(result.scenePosition.x.toFixed(2),
                                  result.scenePosition.y.toFixed(2),
                                  result.scenePosition.z.toFixed(2))
            console.log(pos)
            if(result.objectHit && (result.objectHit.objectName === "plane_xoy")) {
                idModelA.position = pos
            }
        }
    }
}
