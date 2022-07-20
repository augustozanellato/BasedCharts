#include "themeableaction.h"
#include "windows/mainwindow.h"

using BasedCharts::Utils::ThemeableAction;
using BasedCharts::Windows::MainWindow;

ThemeableAction::ThemeableAction(const QString& iconPath,
                                 const QString& text,
                                 QObject* parent)
    : QAction(text, parent), iconPath(iconPath) {}

void ThemeableAction::themeUpdated() {
    setIcon(QIcon(MainWindow::themeFileSelector.select(iconPath)));
}