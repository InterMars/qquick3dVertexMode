#include "dynamicgeometry.h"

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
void DynamicGeometry::updateData(){
    clear();
    calculateGeometry();
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
    setBounds(m_boundsMin, m_boundsMax);

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
    m_boundsMin = QVector3D(maxFloat, maxFloat, maxFloat);
    m_boundsMax = QVector3D(-maxFloat, -maxFloat, -maxFloat);

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
            m_boundsMin.setX(std::min(m_boundsMin.x(), targetPosition.x()));
            m_boundsMin.setY(std::min(m_boundsMin.y(), targetPosition.y()));
            m_boundsMin.setZ(std::min(m_boundsMin.z(), targetPosition.z()));

            m_boundsMax.setX(std::max(m_boundsMax.x(), targetPosition.x()));
            m_boundsMax.setY(std::max(m_boundsMax.y(), targetPosition.y()));
            m_boundsMax.setZ(std::max(m_boundsMax.z(), targetPosition.z()));
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
    float radius = 5.0; // 圆柱体半径
    float height = 10.0; // 圆柱体高度
    int numSegments = m_gridSize; // 网格的段数

    m_positions.clear();
    m_indexes.clear();
    m_targetPositions.clear();
    m_targetNormals.clear();

    constexpr float maxFloat = std::numeric_limits<float>::max();
    m_boundsMin = QVector3D(maxFloat, maxFloat, maxFloat);
    m_boundsMax = QVector3D(-maxFloat, -maxFloat, -maxFloat);

    // 添加底部顶点
    m_positions.append({0.0, static_cast<float>(-height / 2.0), 0.0});
    m_normals.append({0.0, -1.0, 0.0});
    m_targetPositions.append({0.0, static_cast<float>(-height / 2.0), 0.0});
    m_targetNormals.append({0.0, -1.0, 0.0});
    m_boundsMin = { -radius, static_cast<float>(-height / 2.0), -radius };
    m_boundsMax = { radius, static_cast<float>(height / 2.0), radius };

    // 循环生成圆柱体的侧面顶点和侧面顶点索引
    for (int i = 0; i < numSegments; ++i) {
        float theta = static_cast<float>(i) / static_cast<float>(numSegments - 1) * 2.0 * M_PI;
        float x = radius * cos(theta);
        float z = radius * sin(theta);

        for (int j = 0; j < 2; ++j) { // 两个顶点分别在圆柱体的上下两端
            float y = (j == 0) ? -height / 2.0 : height / 2.0;

            m_positions.append({x, y, z});
            m_normals.append({x, 0, z}); // 圆柱体的法线和顶点在同一方向

            // 添加目标位置和目标法线，对于圆柱体，它们与顶点位置和法线相同
            m_targetPositions.append({x, y, z});
            m_targetNormals.append({x, 0, z});

            // 更新边界范围
            m_boundsMin.setX(std::min(m_boundsMin.x(), x));
            m_boundsMin.setY(std::min(m_boundsMin.y(), y));
            m_boundsMin.setZ(std::min(m_boundsMin.z(), z));

            m_boundsMax.setX(std::max(m_boundsMax.x(), x));
            m_boundsMax.setY(std::max(m_boundsMax.y(), y));
            m_boundsMax.setZ(std::max(m_boundsMax.z(), z));
        }
    }

    // 添加顶部顶点
    m_positions.append({0.0, static_cast<float>(height / 2.0), 0.0});
    m_normals.append({0.0, 1.0, 0.0});
    m_targetPositions.append({0.0, static_cast<float>(height / 2.0), 0.0});
    m_targetNormals.append({0.0, 1.0, 0.0});

    // 生成顶部和底部的顶点索引
    int baseIndex = numSegments * 2;
    for (int i = 0; i < numSegments - 1; ++i) {
        m_indexes << baseIndex << i * 2 << (i + 1) * 2;
        m_indexes << baseIndex + 1 << (i + 1) * 2 + 1 << i * 2 + 1;
    }
    m_indexes << baseIndex << (numSegments - 1) * 2 << 0;
    m_indexes << baseIndex + 1 << (numSegments - 1) * 2 + 1 << 1;
}

void DynamicGeometry::calculateTriangleGeometry(){
    // 清空之前的数据
    m_positions.clear();
    m_indexes.clear();
    m_targetPositions.clear();
    m_targetNormals.clear();

    // 定义三角形的三个顶点
    QVector3D vertex1(-5.0, 0.0, 5.0);
    QVector3D vertex2(5.0, 0.0, 5.0);
    QVector3D vertex3(5.0, 0.0, -5.0);
    QVector3D vertex4(-5.0, 0.0, -5.0);

    // 将三个顶点添加到顶点数组中
    m_positions.append(vertex1);
    m_positions.append(vertex2);
    m_positions.append(vertex3);
    m_positions.append(vertex4);

    // 添加目标位置和目标法线（此处与顶点位置和法线相同）
    m_targetPositions.append(vertex1);
    m_normals.append({0.0, 1.0, 0.0});
    m_targetPositions.append(vertex2);
    m_normals.append({0.0, 1.0, 0.0});
    m_targetPositions.append(vertex3);
    m_normals.append({0.0, 1.0, 0.0});
    m_targetPositions.append(vertex4);
    m_normals.append({0.0, 1.0, 0.0});

    m_targetNormals.append(QVector3D(0.0, 0.0, 1.0)); // 三角形的法线向量
    m_targetNormals.append(QVector3D(0.0, 0.0, 1.0)); // 三角形的法线向量
    m_targetNormals.append(QVector3D(0.0, 0.0, 1.0)); // 三角形的法线向量
    m_targetNormals.append(QVector3D(0.0, 0.0, 1.0)); // 三角形的法线向量

    // 生成顶点索引，按顺时针方向连接三个顶点
    m_indexes.append(0);
    m_indexes.append(1);
    m_indexes.append(2);
    m_indexes.append(0);
    m_indexes.append(2);
    m_indexes.append(3);

    // 更新边界范围
    m_boundsMin = QVector3D(0.0, 0.0, 0.0);
    m_boundsMax = QVector3D(1.0, 1.0, 0.0);
}

