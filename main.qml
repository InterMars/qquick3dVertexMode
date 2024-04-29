import QtQuick
import QtQuick3D
import QtQuick.Window
import QtQuick3D.Helpers
import QtQuick3D.AssetUtils
import DemoA
import "./models/sail"
import "./models/arrow"
import "./qml/common.js" as COMMON
Window {
    id: idwindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    // Sphere {
    //     id: view
    // }

    View3D {
        id: view
        anchors.fill: parent
        renderMode: View3D.Underlay
        camera: idOrbitCamera
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

        // PointLight {
        //     position: Qt.vector3d(-15, 50, 15)
        //     NumberAnimation on eulerRotation.y {
        //                   from: 0
        //                   to: 360
        //                   duration: 17000
        //                   loops: Animation.Infinite
        //               }
        //     eulerRotation: Qt.vector3d(0, -3, 0)
        // }

        Node {
            id: orbitCameraNode
            PerspectiveCamera {
                id: idOrbitCamera
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
                pickable: false
                scale: Qt.vector3d(10.0, 10.0, 10.0)
            }
        }
        OrbitCameraController {
            id: orbitController
            origin: orbitCameraNode
            camera: idOrbitCamera
        }

        DynamicModel {
            id: idBaseModel
            scale: Qt.vector3d(10.0, 10.0, 10.0)
        }
        Boat_sail_a {
            id: idBoat
            position: Qt.vector3d(0, 10, 0)
            scale: Qt.vector3d(5.0, 5.0, 5.0)
            // eulerRotation: COMMON.getPose(idBaseModel.position, idBoat.position)
        }
    }

    // NumberAnimation on eulerRotation.y {
    //     running: false
    //     from: 0
    //     to: 360
    //     duration: 17000
    //     loops: Animation.Infinite
    // }
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
    Shortcut {
        sequence: "w"
        onActivated: {
            idBoat.x ++
            var pos = idBoat.position
            var ix = pos.x / 20*2
            var iz = pos.z / 10*2
            console.log(pos)
            console.log(ix, iz)
            idBoat.y += Math.sin(Math.PI * ix) * Math.cos(Math.PI * iz)
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

            // var rotate = COMMON.getRotationAngle(idBoat.position, 10)
            // console.log(pos)
            var ix = pos.x / 228
            var iz = pos.z / 114
            pos.y += Math.sin(Math.PI * ix) * Math.cos(Math.PI * iz)
            // console.log(idBaseModel.boundMax)
            idBoat.position = pos
        }
    }
}



