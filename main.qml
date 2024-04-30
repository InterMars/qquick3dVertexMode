import QtQuick
import QtQuick3D
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
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
    title: qsTr("Demo")

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
                id: idGrid
                visible: false
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
            ambientColor: Qt.rgba(0.3, 0.3, 0.7, 0.2)
        }

        PointLight {
            position: Qt.vector3d(-15, 50, 15)
            eulerRotation: Qt.vector3d(0, -3, 0)
        }
        Node {
            id: orbitCameraNode
            PerspectiveCamera {
                id: idOrbitCamera
                z: 700
                y: 230
                eulerRotation.x: -10
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
            objectName: "idPalnet"
            z: 0
            pickable: true
            scale: Qt.vector3d(r, r, r)


            onModeChanged: {
                if(mode === 1) {
                    idPalnet.waveHeight = 0.0
                    sliderWave.value = 0.0
                }else{
                    idPalnet.waveHeight = 2.0
                    sliderWave.value = 2.0
                }
            }
        }
    }
    ColumnLayout {
        Label {
            text: "Use mouse to navigate"
            font.bold: true
        }

        CheckBox {
            id: boxReference
            text: "Enable Grid or Wire"
            checked: false
            focusPolicy: Qt.NoFocus
            onCheckedChanged: {
                if(checked) {
                    boxGrid.checked = true
                }else{
                    boxGrid.checked = checked
                    boxWireFrame.checked = checked
                }
            }
        }
        RowLayout {
            RadioButton {
                id: boxGrid
                text: "Grid"
                checked: false
                enabled: boxReference.checked
                focusPolicy: Qt.NoFocus
                onCheckedChanged: {
                    idGrid.visible = checked
                }
            }
            RadioButton {
                id: boxWireFrame
                text: "Wireframe"
                checked: false
                enabled: boxReference.checked
                focusPolicy: Qt.NoFocus
                onCheckedChanged: {
                    env.wfenable = checked
                }
            }
        }

        RowLayout {
            ButtonGroup {
                buttons: [ radioSphere, radioCube ]
            }

            RadioButton {
                id: radioCube
                text: "Cube"
                checked: true
                focusPolicy: Qt.NoFocus
                onCheckedChanged: { idPalnet.mode = checked?0:1 }
            }
            RadioButton {
                id: radioSphere
                text: "Sphere"
                checked: true
                focusPolicy: Qt.NoFocus
                onCheckedChanged: { idPalnet.mode = checked?1:0 }
            }
        }
        ColumnLayout {
            CheckBox {
                id: boxStop
                text: "Enable Move"
                checked: true
                focusPolicy: Qt.NoFocus
                onCheckedChanged: {
                    idPalnet.aniStatus = checked
                }
            }
            Label {
                text: "Wave adjust"
            }
            Slider {
                id: sliderWave
                from: (idPalnet.mode === 1)?(-20.0):-4.0
                to: (idPalnet.mode === 1)?(20.0):6.0
                stepSize: 0.01
                value: 0.0
                focusPolicy: Qt.NoFocus
                onValueChanged: {
                    idPalnet.waveHeight = value
                }
            }
            ColumnLayout {
                visible: radioCube.checked

                CheckBox {
                    id: boxFrozen
                    text: "Enable Frozen Effect"
                    checked: true
                    focusPolicy: Qt.NoFocus
                    onCheckedChanged: {
                        idPalnet.frozen = checked
                    }
                }
                RadioButton {
                    id: boxEffectNormal
                    text: "Effect Normal"
                    checked: true
                    enabled: radioCube.checked
                    focusPolicy: Qt.NoFocus
                    onCheckedChanged: {
                        idPalnet.effect = 0
                    }
                }
                RadioButton {
                    id: boxEffect1
                    text: "EffectA"
                    checked: false
                    enabled: radioCube.checked
                    focusPolicy: Qt.NoFocus
                    onCheckedChanged: {
                        idPalnet.effect = 1
                    }
                }
                RadioButton {
                    id: boxEffect2
                    text: "EffectB"
                    checked: false
                    enabled: radioCube.checked
                    focusPolicy: Qt.NoFocus
                    onCheckedChanged: {
                        idPalnet.effect = 2
                    }
                }
            }
        }
    }
    Shortcut {
        sequence: "Esc"
        onActivated: {
            Qt.quit()
        }
    }

}



