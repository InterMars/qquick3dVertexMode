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
        source: "qrc:/models/arrow/maps/colormap.png"
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
        id: arrow
        source: "qrc:/models/arrow/meshes/arrow_mesh.mesh"
        materials: [
            colormap_material
        ]
    }

    // Animations:
}
