#ifndef DYNAMICGEOMETRY_H
#define DYNAMICGEOMETRY_H

#include <QtQuick3D/qquick3d.h>
#include <QtQuick3D/qquick3dgeometry.h>

#include <QVector3D>
#include <QtCore/QList>

class DynamicGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DynamicGeometry)
    Q_PROPERTY(int gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged FINAL)

public:
    explicit DynamicGeometry(QQuick3DObject *parent = nullptr);

    int gridSize() { return m_gridSize; }
    void setGridSize(int gridSize);

    int mode() {    return m_mode; }
    void setMode(int mode);
signals:
    void gridSizeChanged();
    void modeChanged();

private:
    void calculateGeometry();
    void calculateColumnGeometry();
    void calculateTriangleGeometry();
    void updateData();
    void updateDataB();

    QList<QVector3D> m_positions;
    QList<QVector3D> m_normals;

    QList<QVector3D> m_targetPositions;
    QList<QVector3D> m_targetNormals;

    QList<quint32> m_indexes;

    QByteArray m_vertexBuffer;
    QByteArray m_indexBuffer;

    int m_gridSize = 50;
    QVector3D m_boundsMin;
    QVector3D m_boundsMax;

    int m_mode = 1;
};

#endif // DYNAMICGEOMETRY_H
