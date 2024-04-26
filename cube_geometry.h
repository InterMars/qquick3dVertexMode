#ifndef CUBE_GEOMETRY_H
#define CUBE_GEOMETRY_H

#include <QtQuick3D/qquick3d.h>
#include <QtQuick3D/qquick3dgeometry.h>

#include <QVector3D>
#include <QtCore/QList>

class CubeGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CubeGeometry)
    Q_PROPERTY(bool normals READ normals WRITE setNormals NOTIFY normalsChanged)
    Q_PROPERTY(float normalXY READ normalXY WRITE setNormalXY NOTIFY normalXYChanged)
    Q_PROPERTY(bool uv READ uv WRITE setUV NOTIFY uvChanged)
    Q_PROPERTY(float uvAdjust READ uvAdjust WRITE setUVAdjust NOTIFY uvAdjustChanged)

public:
    CubeGeometry();

    bool normals() const { return m_hasNormals; }
    void setNormals(bool enable);

    float normalXY() const { return m_normalXY; }
    void setNormalXY(float xy);

    bool uv() const { return m_hasUV; }
    void setUV(bool enable);

    float uvAdjust() const { return m_uvAdjust; }
    void setUVAdjust(float f);

signals:
    void normalsChanged();
    void normalXYChanged();
    void uvChanged();
    void uvAdjustChanged();

private:
    void updateData();
    void calculateGeometry();

    bool m_hasNormals = false;
    float m_normalXY = 0.0f;
    bool m_hasUV = false;
    float m_uvAdjust = 0.0f;

    int m_gridSize = 50;
    QVector3D m_boundsMin;
    QVector3D m_boundsMax;

    QList<QVector3D> m_positions;
    QList<QVector3D> m_normals;

    QList<QVector3D> m_targetPositions;
    QList<QVector3D> m_targetNormals;

    QList<quint32> m_indexes;

    QByteArray m_vertexBuffer;
    QByteArray m_indexBuffer;
};

#endif // CUBE_GEOMETRY_H
