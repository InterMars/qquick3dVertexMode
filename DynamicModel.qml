import QtQuick
import QtQuick3D
import DemoA

Model {
    id: root
    pickable: true
    // 1: sphere, other: cube
    property alias effect: idMaterial.uEffect
    property alias mode: idGeometry.mode
    property alias frozen: idMaterial.uFrozen

    property alias aniStatus: idAni.running
    property double waveHeight: 4.0
    onWaveHeightChanged: {
        if(idGeometry.mode === 1) {
            if(Math.abs(waveHeight) > 20) {
                waveHeight = waveHeight>0?20.0:-20.0
            }
            if(waveHeight <= 20 && waveHeight >= -20){
                idMaterial.uHeight = waveHeight
            }else {
                idMaterial.uHeight = 20.0
            }
        }else {
//            console.log(waveHeight)
//            if(Math.abs(waveHeight) > 4.0){
//                idMaterial.uHeight = waveHeight>0?4.0:-5.0
//            }else {
                idMaterial.uHeight = waveHeight
//            }
        }

    }

    geometry: DynamicGeometry {
        id: idGeometry
        mode: 0
    }

    materials: CustomMaterial {
        id: idMaterial
        property color uDiffuse: "grey"
        property double uTime: 0.0
        property int uMode: idGeometry.mode
        property double uHeight: 0.0
        property int uEffect: 0
        property bool uFrozen: true

        //
        shadingMode: CustomMaterial.Shaded
        fragmentShader: "qrc:/shaders/water.frag"
        vertexShader: "qrc:/shaders/water.vert"
        SequentialAnimation on uTime {
            id: idAni
            running: true
            loops: Animation.Infinite
            NumberAnimation {
                from: 0.0
                to: 100.0
                duration: 10000
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                from: 100.0
                to: 0.0
                duration: 10000
                easing.type: Easing.InOutQuad
            }
        }
    }
}
