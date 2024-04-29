#include "dynamic_geometry.h"

#include <QRandomGenerator>
#include <qmath.h>

struct Vertex {
    QVector3D position;
    QVector3D normal;

    QVector3D targetPosition;
    QVector3D targetNormal;

    QVector2D texCoord;
};
// Q_STATIC_ASSERT((sizeof(Vertex)/16)*16 == sizeof(Vertex)); // must be  4-float aligned
Q_STATIC_ASSERT((sizeof(Vertex)/8)*8 == sizeof(Vertex));

DynamicGeometry::DynamicGeometry(QQuick3DObject* parent) : QQuick3DGeometry(parent)
{
    m_mode = 3;
    m_tim.setInterval(300);

    connect(&m_tim, &QTimer::timeout, this, [=](){
        static float seed;
        seed = (seed > 100)?0:(seed+0.5);
        this->qmlUpdateData(seed);

    });
    if(m_mode == 1 || m_mode == 3){
        m_tim.start();
    }
    updateData();
}

DynamicGeometry::~DynamicGeometry()
{
    m_tim.stop();
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
void DynamicGeometry::setMode(int mode)
{
    if(m_mode == mode)  return;

    m_mode = mode;
    emit modeChanged();

    updateData();
    update();
}
void DynamicGeometry::updateData(){
    clear();
    calculateGeometry();

    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0,
                 QQuick3DGeometry::Attribute::ComponentType::F32Type);
    addAttribute(QQuick3DGeometry::Attribute::NormalSemantic, 3 * sizeof(float),
                 QQuick3DGeometry::Attribute::ComponentType::F32Type);

    addAttribute(QQuick3DGeometry::Attribute::TargetPositionSemantic, 6 * sizeof(float),
                 QQuick3DGeometry::Attribute::ComponentType::F32Type);
    addAttribute(QQuick3DGeometry::Attribute::TargetNormalSemantic, 9 * sizeof(float),
                 QQuick3DGeometry::Attribute::ComponentType::F32Type);

    addAttribute(QQuick3DGeometry::Attribute::TexCoordSemantic, 12 * sizeof(float),
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
        v.texCoord = m_texcoord[i];
    }

    setStride(sizeof(Vertex));
    setVertexData(m_vertexBuffer);
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    setBounds(boundsMin, boundsMax);

    m_indexBuffer = QByteArray(reinterpret_cast<char *>(m_indexes.data()), m_indexes.size() * sizeof(quint32));
    setIndexData(m_indexBuffer);
}

void DynamicGeometry::calculateGeometry(){
    float dw = 20.0; // width/2
    float dh = 10.0;
    int iw = m_gridSize;
    int ih = m_gridSize;
    float wf = dw * 2 / iw; //factor grid coord => position
    float hf = dh * 2 / ih;

    m_positions.clear();
    m_indexes.clear();
    m_targetPositions.clear();
    m_targetNormals.clear();
    m_texcoord.clear();

    constexpr float maxFloat = std::numeric_limits<float>::max();
    boundsMin = QVector3D(maxFloat, maxFloat, maxFloat);
    boundsMax = QVector3D(-maxFloat, -maxFloat, -maxFloat);

    const float R = 10;
    for (int iy = 0; iy < ih; ++iy) {
        for (int ix = 0; ix < iw; ++ix) {
            float theta = M_PI * iy / (ih - 1); //  [0, π]
            float phi = 2 * M_PI * ix / (iw - 1); // [0, 2π]

            float x = R * sin(theta) * cos(phi);
            float y = R * sin(theta) * sin(phi);
            float z = R * cos(theta);
            QVector3D normal = QVector3D(2*x, 2*y, z).normalized();
            if(m_mode == 1){
                float T = 2.0f;
                float aphi = phi*M_PI*T+m_seed;
                float atheta = theta*M_PI*T+m_seed;
                QVector3D wave = QVector3D( R + sin(atheta) * cos(aphi),
                                           R + cos(atheta) * sin(aphi),
                                           R + sin(atheta));

                x += normal.x() * wave.x();
                y += normal.y() * wave.y();
                z += normal.z() * wave.z();
            }
            if(m_mode == 2){
                theta = 2 * M_PI * iy / (ih - 1);
                x = R * (16 * pow(sin(theta), 3));
                y = R * (13 * cos(theta) - 5 * cos(2 * theta) - 2 * cos(3 * theta) - cos(4 * theta));
                z = R * (sin(phi) * cos(theta));
            }
            float u = phi / (2 * M_PI); // 水平方向映射到 [0, 1]
            float v = theta / M_PI;     // 垂直方向映射到 [0, 1]
            m_texcoord.append({u, v});

            QVector3D targetPosition;
            QVector3D targetNormal;

            if(m_mode == 3){
                x = ix * wf - dw;
                z = iy * hf - dh;
                // float T = 1.0;
                // float a = phi*T+m_seed;
                // float b = theta*T+m_seed;
                // y = 2 * qCos(a) + 2 * qSin(b) + 3.0;
                // normal = QVector3D(2 * qSin(a) + 2 * qCos(b) + 0.234, 2, 0);
                y = 6.0;
                normal = QVector3D(0.234, 2, 0);

                if (iy >= ih-2)
                    normal = {0, 0, 1};
                else if (iy <= 1)
                    normal = {0, 0, -1};

                float u = x/dw;
                float v = z/dh;
                m_texcoord.append({u, v});
            }
            float tx = x * 1.2;
            float tz = z * 1.2;

            constexpr float R = 16;

            if (tx*tx + tz*tz < R*R) {
                float ty = 0.4f * qSqrt(R*R - tx*tx - tz*tz);
                targetPosition = {tx, ty, tz};
                targetNormal = targetPosition;
            } else {
                targetPosition = {tx, -3, tz};
                targetNormal = {0, 1, 0};

            }

            if (ix == 0 || iy == 0 || ix == iw-1 || iy == ih-1) {
                int iix = qMin(iw-2, qMax(1, ix));
                int iiy = qMin(ih-2, qMax(1, iy));
                x = iix * wf - dw;
                z = iiy * hf - dh;
                y = -3.0;
                targetPosition.setY(-3);
            }
            m_positions.append(QVector3D(x, y, z));
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

void DynamicGeometry::qmlUpdateData(double seed){
    m_seed = seed;
    updateData();
    update();
}
