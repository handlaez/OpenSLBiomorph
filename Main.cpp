#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "src/BiomorphController.hpp"
#include "src/BiomorphImage.hpp"
#include "src/BiomorphParameters.hpp"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    qRegisterMetaType<BiomorphParameters>();
    qRegisterMetaType<BiomorphImage>();

    BiomorphController biomorph;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("biomorph", &biomorph);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.loadFromModule("Biomorph", "Main");

    return app.exec();
}