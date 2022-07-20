#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "windows/mainwindow.h"

using BasedCharts::Windows::MainWindow;

int main(int argc, char* argv[]) {
#ifdef QT_RELEASE
    qInstallMessageHandler(
        [](QtMsgType, const QMessageLogContext&, const QString&) {});
#endif
    Q_INIT_RESOURCE(basedcharts);
    Q_INIT_RESOURCE(dark);
    Q_INIT_RESOURCE(light);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Augusto Zanellato");
    QCoreApplication::setApplicationName("BasedCharts");
    QApplication app(argc, argv);
    MainWindow mainWin;
    mainWin.show();
    return QApplication::exec();
}
