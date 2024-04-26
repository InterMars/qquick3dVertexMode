#include "dynamic_geometry.h"

#include <QRandomGenerator>
#include <qmath.h>

struct Vertex {
    QVector3D position;
    QVector3D normal;

    QVector3D targetPosition;
    QVector3D targetNormal;
};
Q_STATIC_ASSERT((sizeof(Vertex)/16)*16 == sizeof(Vertex));

DynamicGeometry::DynamicGeometry(QQuick3DObject* parent) : QQuick3DGeometry(parent)
{
    updateData();
}
void DynamicGeometry::setGridSize(int gridSize)
{
    if (m_gridSize == gridSize)
        return;

    m_gridSize = gridSize;
    emit gridSizeChanged();
    updateData();
    update();
}
void DynamicGeometry::setMode(int mode) {
    if(m_mode == mode)  return;

    m_mode = mode;
    emit modeChanged();

    updateData();
    update();
}
void DynamicGeometry::updateData(){
    clear();
    calculateGeometry();
//    calculateColumnGeometry();
//    calculateTriangleGeometry();

    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0,
                 QQuick3DGeometry::Attribute::ComponentType::F32Type);
    addAttribute(QQuick3DGeometry::Attribute::NormalSemantic, 3 * sizeof(float),
                 QQuick3DGeometry::Attribute::ComponentType::F32Type);

    addAttribute(QQuick3DGeometry::Attribute::TargetPositionSemantic, 6 * sizeof(float),
                 QQuick3DGeometry::Attribute::ComponentType::F32Type);
    addAttribute(QQuick3DGeometry::Attribute::TargetNormalSemantic, 9 * sizeof(float),
                 QQuick3DGeometry::Attribute::ComponentType::F32Type);

    addAttribute(QQuick3DGeometry::Attribute::IndexSemantic, 0,
                 QQuick3DGeometry::Attribute::ComponentType::U32Type);

    const int numVertexes = m_positions.size();
    m_vertexBuffer.resize(numVertexes * sizeof(Vertex));
    Vertex *vert = reinterpret_cast<Vertex *>(m_vertexBuffer.data());

    for (int i = 0; i < numVertexes; ++i) {
        Vertex &v = vert[i];
        v.position = m_positions[i];
        v.normal = m_normals[i];
        v.targetPosition = m_targetPositions[i];
        v.targetNormal = m_targetNormals[i];
    }

    setStride(sizeof(Vertex));
    setVertexData(m_vertexBuffer);
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    setBounds(boundsMin, boundsMax);

    m_indexBuffer = QByteArray(reinterpret_cast<char *>(m_indexes.data()), m_indexes.size() * sizeof(quint32));
    setIndexData(m_indexBuffer);
}

void DynamicGeometry::calculateGeometry(){
    float dw = 10.0; // width/2
    float dh = 10.0;
    int iw = m_gridSize;
    int ih = m_gridSize;
    float wf = dw * 2 / iw; //factor grid coord => position
    float hf = dh * 2 / ih;

    m_positions.clear();
    m_indexes.clear();
    m_targetPositions.clear();
    m_targetNormals.clear();

    constexpr float maxFloat = std::numeric_limits<float>::max();
    boundsMin = QVector3D(maxFloat, maxFloat, maxFloat);
    boundsMax = QVector3D(-maxFloat, -maxFloat, -maxFloat);

    const float R = 16;
    float x = 0, y = 0, z = 0;
    for (int iy = 0; iy < ih; ++iy) {
        for (int ix = 0; ix < iw; ++ix) {
            float theta = M_PI * iy / (ih - 1); // 角度范围在 [0, π]
            float phi = 2 * M_PI * ix / (iw - 1); // 角度范围在 [0, 2π]

            float waveHeight = 1.0 ;
            float x = R * sin(theta) * cos(phi) + waveHeight;
            float y = R * sin(theta) * sin(phi) + waveHeight;
            float z = R * cos(theta) + waveHeight;

            QVector3D normal{2*x, 2*y, z};

            if (iy >= ih-2)
                normal = {0, 0, 1};
            else if (iy <= 1)
                normal = {0, 0, -1};


            float tx = ix * wf - dw;
            float tz = iy * hf - dh;
            float ty = 3.0;

            QVector3D targetPosition{tx, ty, tz};
            QVector3D targetNormal{0, 2, 0};

            if (ix == 0 || iy == 0 || ix == iw-1 || iy == ih-1) {
                targetPosition.setY(-3);
                targetNormal.setY(0);
            }

            m_positions.append({x, y, z});
            m_normals.append(normal.normalized());
            m_targetPositions.append(targetPosition);
            m_targetNormals.append(targetNormal.normalized());

            // Note: We only use the bounds of the target positions since they are strictly
            // bigger than the original
            boundsMin.setX(std::min(boundsMin.x(), targetPosition.x()));
            boundsMin.setY(std::min(boundsMin.y(), targetPosition.y()));
            boundsMin.setZ(std::min(boundsMin.z(), targetPosition.z()));

            boundsMax.setX(std::max(boundsMax.x(), targetPosition.x()));
            boundsMax.setY(std::max(boundsMax.y(), targetPosition.y()));
            boundsMax.setZ(std::max(boundsMax.z(), targetPosition.z()));
        }
    }

    for (int ix = 0; ix < iw - 1; ++ix) {
        for (int iy = 0; iy < ih - 1; ++iy) {
            int idx = ix + iy * ih;
            m_indexes << idx << idx + iw << idx + iw + 1
                      << idx << idx + iw + 1 << idx + 1;
        }
    }
}


void DynamicGeometry::calculateColumnGeometry(){
    float dw = 10.0; // width/2
    float dh = 10.0;
    int iw = m_gridSize;
    int ih = m_gridSize;
    float wf = dw * 2 / iw; //factor grid coord => position
    float hf = dh * 2 / ih;

    m_positions.clear();
    m_indexes.clear();
    m_targetPositions.clear();
    m_targetNormals.clear();

    constexpr float maxFloat = std::numeric_limits<float>::max();
    boundsMin = QVector3D(maxFloat, maxFloat, maxFloat);
    boundsMax = QVector3D(-maxFloat, -maxFloat, -maxFloat);

    // We construct a rectangular grid of iw times ih vertices;
    // ix and iy are indices into the grid. x, y, and z are the spatial
    // coordinates we calculate for each vertex. tx, ty, and tz are the
    // coordinates for the morph target.

    for (int iy = 0; iy < ih; ++iy) {
        for (int ix = 0; ix < iw; ++ix) {
            float x = ix * wf - dw;
            float z = iy * hf - dh;

            // The base shape is a cosine wave, and the corresponding normal
            // vectors are calculated from the partial derivatives.
            float y = 2 * qCos(x) + 3.0;
            QVector3D normal{2 * qSin(x), 2, 0};

            float tx = x * 1.2;
            float tz = z * 1.2;

            constexpr float R = 16;
            QVector3D targetPosition;
            QVector3D targetNormal;

            // The morph target shape is a hemisphere. Therefore we don't have
            // to do complex math to calculate the normal vector, since all vectors
            // from the center are normal to the surface of a sphere.
            if (tx*tx + tz*tz < R*R) {
    float ty = 0.4f * qSqrt(R*R - tx*tx - tz*tz);
    targetPosition = {tx, ty, tz};
    targetNormal = targetPosition;
            } else {
    targetPosition = {tx, -3, tz};
    targetNormal = {0, 1, 0};
            }

            // Finally, we make the outside edges of the shapes vertical, so they look nicer.
            if (ix == 0 || iy == 0 || ix == iw-1 || iy == ih-1) {
    int iix = qMin(iw-2, qMax(1, ix));
    int iiy = qMin(ih-2, qMax(1, iy));
    x = iix * wf - dw;
    z = iiy * hf - dh;
    y = -3.0;
    targetPosition.setY(-3);
            }

            if (iy >= ih-2)
    normal = {0, 0, 1};
            else if (iy <= 1)
    normal = {0, 0, -1};

            m_positions.append({x, y, z});
            m_normals.append(normal.normalized());

            m_targetPositions.append(targetPosition);
            m_targetNormals.append(targetNormal.normalized());

            // Note: We only use the bounds of the target positions since they are strictly
            // bigger than the original
            boundsMin.setX(std::min(boundsMin.x(), targetPosition.x()));
            boundsMin.setY(std::min(boundsMin.y(), targetPosition.y()));
            boundsMin.setZ(std::min(boundsMin.z(), targetPosition.z()));

            boundsMax.setX(std::max(boundsMax.x(), targetPosition.x()));
            boundsMax.setY(std::max(boundsMax.y(), targetPosition.y()));
            boundsMax.setZ(std::max(boundsMax.z(), targetPosition.z()));
        }
    }

    for (int ix = 0; ix < iw - 1; ++ix) {
        for (int iy = 0; iy < ih - 1; ++iy) {
            int idx = ix + iy * ih;
            m_indexes << idx << idx + iw << idx + iw + 1
                      << idx << idx + iw + 1 << idx + 1;
        }
    }
}

void DynamicGeometry::calculateTriangleGeometry(){
    float dw = 10.0; // width/2
    float dh = 10.0;
    int iw = m_gridSize;
    int ih = m_gridSize;

    // 清空之前的数据
    m_positions.clear();
    m_indexes.clear();
    m_targetPositions.clear();
    m_targetNormals.clear();

    const float R = 10.0;
    constexpr float maxFloat = std::numeric_limits<float>::max();
    boundsMin = QVector3D(maxFloat, maxFloat, maxFloat);
    boundsMax = QVector3D(-maxFloat, -maxFloat, -maxFloat);

    for(int iy = 0; iy < ih; ++iy) {
        for(int ix = 0; ix < iw; ++ix) {
            float x = ix;
            float y = qSqrt(R*R - ix*ix);
            float z = qSqrt(R*R - x*x - y*y);

            QVector3D normal{2 * qSin(x), 2, 0};

            if (iy >= ih-2)
                normal = {0, 0, 1};
            else if (iy <= 1)
                normal = {0, 0, -1};

            m_positions.append({x, y, z});
            m_normals.append(normal.normalized());
            m_targetPositions.append({x, y, z});
            m_targetNormals.append(normal.normalized());
        }
    }
//    int baseIndex = m_gridSize * 2;
//    for (int i = 0; i < m_gridSize - 1; ++i) {
//        m_indexes << baseIndex << i * 2 << (i + 1) * 2;
//        m_indexes << baseIndex + 1 << (i + 1) * 2 + 1 << i * 2 + 1;
//    }
//    m_indexes << baseIndex << (m_gridSize - 1) * 2 << 0;
//    m_indexes << baseIndex + 1 << (m_gridSize - 1) * 2 + 1 << 1;
//    for (int ix = 0; ix < iw - 1; ++ix) {
//        for (int iy = 0; iy < ih - 1; ++iy) {
//            int idx = ix + iy * ih;
//            m_indexes << idx << idx + iw << idx + iw + 1
//                      << idx << idx + iw + 1 << idx + 1;
//        }
//    }

}
void DynamicGeometry::updateDataB(){
    clear();

    constexpr auto randomFloat = [](const float lowest, const float highest) -> float {
        return lowest + QRandomGenerator::global()->generateDouble() * (highest - lowest);
    };
    constexpr int NUM_POINTS = 2000;
    constexpr int stride = 3 * sizeof(float);

    QByteArray vertexData;
    vertexData.resize(NUM_POINTS * stride);
    float *p = reinterpret_cast<float *>(vertexData.data());

    for (int i = 0; i < NUM_POINTS; ++i) {
        *p++ = randomFloat(-5.0f, +5.0f);
        *p++ = randomFloat(-5.0f, +5.0f);
        *p++ = 0.0f;
    }

    setVertexData(vertexData);
    setStride(stride);
    setBounds(QVector3D(-5.0f, -5.0f, 0.0f), QVector3D(+5.0f, +5.0f, 0.0f));

    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Points);

    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);
}
