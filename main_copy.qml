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

        PointLight {
            position: Qt.vector3d(-15, 50, 15)
            eulerRotation: Qt.vector3d(0, -3, 0)
        }
        PerspectiveCamera {
            id: idFllowCamera

            x: idBoat.x
            y: idBoat.y + 20
            z: idBoat.z - 65.4595

            eulerRotation.z: 0
            eulerRotation.y: 180
            eulerRotation.x: -5

            // eulerRotation.z: idBoat.eulerRotation.z
            // eulerRotation.y: idBoat.eulerRotation.y + 180
            // eulerRotation.x: idBoat.eulerRotation.x -5
        }
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
            id: idPalnet
            opacity: 1.0
            property double r: 10
            property double theta: 0.0
            property double phi: 0.0
            // property scale name: value
            objectName: "modela"
            z: 0
            pickable: true
            scale: Qt.vector3d(r, r, r)
        }

        Arrow {
            id: idArrow
            scale: Qt.vector3d(5.0, 5.0, 5.0)
        }
        Boat_sail_a {
            id: idBoat
            position: Qt.vector3d(0, idPalnet.r*10, 0)
            scale: Qt.vector3d(5.0, 5.0, 5.0)
            eulerRotation: COMMON.getPose(idPalnet.position, idBoat.position)
        }
        RandomInstancing {
            id: instancing
            instanceCount: 3
            scale: InstanceRange {
                property int boundsDiameter: 10
                from: Qt.vector3d(boundsDiameter, boundsDiameter, boundsDiameter);
                to: Qt.vector3d(boundsDiameter, boundsDiameter, boundsDiameter)

            }
            rotation: InstanceRange {
                property int boundsDiameter: 25
                from: Qt.vector3d(boundsDiameter, boundsDiameter, boundsDiameter);
                to: Qt.vector3d(boundsDiameter, boundsDiameter, boundsDiameter)
            }
            position: InstanceRange {
                property int boundsDiameter: 10

                from: Qt.vector3d(boundsDiameter, 0.5*boundsDiameter, 0.5*boundsDiameter);
                to: Qt.vector3d(2.0+boundsDiameter, -0.5*boundsDiameter, -0.5*boundsDiameter)
            }
            color: InstanceRange {
                from: Qt.hsva(0, 0.1, 0.8, 1)
                to: Qt.hsva(1, 0.3, 1, 1)
            }
        }
        RuntimeLoader {
            id: importNode
            source: "qrc:/models/arrow/Arrow.qml"
            instancing: instancing
        }
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
    Shortcut {
        sequence: "Shift+2"
        onActivated: {
            view.camera = idFllowCamera
        }
    }
    Shortcut {
        sequence: "Shift+3"
        onActivated: {
            view.camera = idOrbitCamera
        }
    }
    Shortcut {
        sequence: "w"
        onActivated: {
            var rotate = COMMON.getRotationAngle(idBoat.position, idPalnet.r*10)
            if(idBoat.position.y < 0){
                rotate.x += 0.01;
            }else{
                rotate.x -= 0.01;
            }
            COMMON.setThetaOffset(90)
            console.log(idBoat.eulerRotation)
            var newpos = COMMON.angleToPosition(rotate.x, rotate.y, idPalnet.r*10)
            // var neweul = Qt.vector3d(0, 0, 0)
            idBoat.position = newpos
        }
    }
    Shortcut {
        sequence: "s"
        onActivated: {
            var rotate = COMMON.getRotationAngle(idBoat.position, idPalnet.r*10)
            if(idBoat.position.y < 0){
                rotate.x -= 0.01;
            }else{
                rotate.x += 0.01;
            }
            COMMON.setThetaOffset(90)
            console.log(idBoat.position)
            var newpos = COMMON.angleToPosition(rotate.x, rotate.y, idPalnet.r*10)
            // var neweul = Qt.vector3d(0, 0, 0)
            idBoat.position = newpos
        }
    }
    Shortcut {
        sequence: "a"
        onActivated: {
            var rotate = COMMON.getRotationAngle(idBoat.position, idPalnet.r*10)

            // COMMON.setPhiOffset(90)
            // COMMON.setThetaOffset(90)
            if(idBoat.position.x < -100){
                rotate.y -= 0.01;
            }else{
                rotate.y += 0.01;
            }
            console.log(idBoat.position)
            var newpos = COMMON.angleToPosition(rotate.x, rotate.y, idPalnet.r*10)
            idBoat.position = newpos
        }
    }
    Shortcut {
        sequence: "d"
        onActivated: {
            var rotate = COMMON.getRotationAngle(idBoat.position, idPalnet.r*10)
            if(idBoat.position.x < -100){
                rotate.y += 0.01;
            }else{
                rotate.y -= 0.01;
            }
            COMMON.setPhiOffset(90)
            console.log(idBoat.position)
            var newpos = COMMON.angleToPosition(rotate.x, rotate.y, idPalnet.r*10)
            // var neweul = Qt.vector3d(0, 0, 0)
            idBoat.position = newpos
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
            var eul = COMMON.getPose(idPalnet.position, pos)
            // if(result.objectHit && (result.objectHit.objectName === "plane_xoy")) {
                // idArrow.position = pos
                // idArrow.eulerRotation = eul



            var rotate = COMMON.getRotationAngle(idBoat.position, idPalnet.r*10)
            console.log(rotate)
            idBoat.position = pos
            idBoat.eulerRotation = eul
            // }
        }
    }
}
