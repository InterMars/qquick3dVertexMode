#ifndef DYNAMIC_GEOMETRY_H
#define DYNAMIC_GEOMETRY_H

#include <QtQuick3D/qquick3d.h>
#include <QtQuick3D/qquick3dgeometry.h>

#include <QVector3D>
#include <QtCore/QList>
#include <QTimer>

class DynamicGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DynamicGeometry)
    Q_PROPERTY(int gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(QVector3D boundMax READ boundMax NOTIFY boundMaxChanged FINAL)

public:
    explicit DynamicGeometry(QQuick3DObject *parent = nullptr);
    virtual ~DynamicGeometry();
    int gridSize() { return m_gridSize; }
    void setGridSize(int gridSize);

    int mode() {    return m_mode; }
    void setMode(int mode);

    QVector3D boundMax(){   return this->boundsMax; }
signals:
    void gridSizeChanged();
    void modeChanged();
    void boundMaxChanged();
public slots:
    void qmlUpdateData(double seed);
private:
    void calculateGeometry();
    void calculateColumnGeometry();
    void calculateTriangleGeometry();

    void updateData();
    QList<QVector3D> m_positions;
    QList<QVector2D> m_texcoord;
    QList<QVector3D> m_normals;

    QList<QVector3D> m_targetPositions;
    QList<QVector3D> m_targetNormals;

    QList<quint32> m_indexes;    

    QByteArray m_vertexBuffer;
    QByteArray m_texcoordBuffer;
    QByteArray m_indexBuffer;

    int m_gridSize = 40;

    QVector3D boundsMin;
    QVector3D boundsMax;

    int m_mode;
    double m_seed;

    QTimer m_tim;
};

#endif // DYNAMIC_GEOMETRY_H
