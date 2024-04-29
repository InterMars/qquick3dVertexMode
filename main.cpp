#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick3D/qquick3d.h>
#include <QQmlContext>
  #include <QDir>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat::setDefaultFormat(QQuick3D::idealSurfaceFormat());
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/DemoA/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.addImportPath("qrc:/models");
    engine.rootContext()->setContextProperty("currentDir", app.applicationDirPath());
    engine.load(url);

    return app.exec();
}
