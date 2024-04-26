#include "cube_geometry.h"

#include <QRandomGenerator>
#include <qmath.h>

CubeGeometry::CubeGeometry()
{
    updateData();
}

void CubeGeometry::setNormals(bool enable)
{
    if (m_hasNormals == enable)
        return;

    m_hasNormals = enable;
    emit normalsChanged();
    updateData();
    update();
}

void CubeGeometry::setNormalXY(float xy)
{
    if (m_normalXY == xy)
        return;

    m_normalXY = xy;
    emit normalXYChanged();
    updateData();
    update();
}

void CubeGeometry::setUV(bool enable)
{
    if (m_hasUV == enable)
        return;

    m_hasUV = enable;
    emit uvChanged();
    updateData();
    update();
}

void CubeGeometry::setUVAdjust(float f)
{
    if (m_uvAdjust == f)
        return;

    m_uvAdjust = f;
    emit uvAdjustChanged();
    updateData();
    update();
}

//! [update data]
void CubeGeometry::updateData()
{
    clear();
    calculateGeometry();

    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);

    if (m_hasNormals) {
        addAttribute(QQuick3DGeometry::Attribute::NormalSemantic,
                     3 * sizeof(float),
                     QQuick3DGeometry::Attribute::F32Type);
    }

    if (m_hasUV) {
        addAttribute(QQuick3DGeometry::Attribute::TexCoordSemantic,
                     m_hasNormals ? 6 * sizeof(float) : 3 * sizeof(float),
                     QQuick3DGeometry::Attribute::F32Type);
    }
    addAttribute(QQuick3DGeometry::Attribute::IndexSemantic, 0,
                 QQuick3DGeometry::Attribute::ComponentType::U32Type);

    constexpr int stride = 3 * sizeof(float);
    const int numVertexes = m_positions.size();
    m_vertexBuffer.resize(numVertexes * stride);
    QVector3D *p = reinterpret_cast<QVector3D *>(m_vertexBuffer.data());

    for (int i = 0; i < numVertexes; ++i) {
        *p++ = m_positions[i];
    }

    setStride(stride);
    setVertexData(m_vertexBuffer);
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    setBounds(m_boundsMin, m_boundsMax);

    m_indexBuffer = QByteArray(reinterpret_cast<char *>(m_indexes.data()), m_indexes.size() * sizeof(quint32));
    setIndexData(m_indexBuffer);

}

void CubeGeometry::calculateGeometry() {
    float dw = 1.0;
    float dh = 1.0;
    float dl = 1.0;
    int iw = 10;
    int ih = 10;
    int il = 10;
    float uw = (2*dw)/iw;
    float uh = (2*dh)/ih;
    float ul = (2*dl)/il;

    m_positions.clear();
    m_indexes.clear();
    m_targetPositions.clear();
    m_targetNormals.clear();

    constexpr float maxFloat = std::numeric_limits<float>::max();
    m_boundsMin = QVector3D(maxFloat, maxFloat, maxFloat);
    m_boundsMax = QVector3D(-maxFloat, -maxFloat, -maxFloat);

    for(int iy = 0; iy < ih; ++iy) {
        for(int ix = 0; ix < iw; ++ix) {
            for(int iz = 0; iz < il; ++iz) {
                float x = ix * uw - dw;
                float y = iy * uh - dh;
                float z = iz * ul - dl;

                // 添加左下角顶点
                m_positions.append({x, y, z});

                // 更新包围盒
                m_boundsMin.setX(std::min(m_boundsMin.x(), x));
                m_boundsMin.setY(std::min(m_boundsMin.y(), y));
                m_boundsMin.setZ(std::min(m_boundsMin.z(), z));
                m_boundsMax.setX(std::max(m_boundsMax.x(), x));
                m_boundsMax.setY(std::max(m_boundsMax.y(), y));
                m_boundsMax.setZ(std::max(m_boundsMax.z(), z));
            }
            // float x = ix * uw - dw;
            // float z = iy * uh - dh;
            // float y = qCos(x);
        }
    }

    for (int iz = 0; iz < il - 1; ++iz) {
        for (int iy = 0; iy < ih - 1; ++iy) {
            for (int ix = 0; ix < iw - 1; ++ix) {
                int idx = ix + iy * iw + iz * iw * ih;
                m_indexes << idx << idx + iw << idx + iw + 1
                          << idx << idx + iw + 1 << idx + 1;
            }
        }
    }
}
