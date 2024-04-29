import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources
    Texture {
        id: textures_colormap_png_texture
        magFilter: Texture.Nearest
        minFilter: Texture.Nearest
        generateMipmaps: true
        mipFilter: Texture.Nearest
        source: "qrc:/models/sail/maps/colormap.png"
    }
    PrincipledMaterial {
        id: colormap_material
        baseColorMap: textures_colormap_png_texture
        roughness: 1
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }

    // Nodes:
    Model {
        id: boat_sail_a
        source: "qrc:/models/sail/meshes/boat_sail_a_mesh.mesh"
        materials: [
            colormap_material
        ]
        Model {
            id: sail
            position: Qt.vector3d(-1.21281e-13, 0.8725, 0.575333)
            source: "qrc:/models/sail/meshes/sail_mesh.mesh"
            materials: [
                colormap_material
            ]
        }
    }

    // Animations:
}
