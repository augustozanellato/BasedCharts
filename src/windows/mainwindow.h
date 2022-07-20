#pragma once

#include "utils/themeableaction.h"
#include "views/mainview.h"
#include "wizard/chartwizard.h"

#include <QMainWindow>
#include <QSessionManager>
#include <QTabWidget>
#include <QtWidgets>

namespace BasedCharts {
namespace Windows {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

    void loadFile(const QString& fileName);

    static QFileDialog* getOpenDialog(QWidget* parent);

    static QFileSelector themeFileSelector;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:

    void open();

    void save();

    void saveAs();

    void about();

    void updateModified(QWidget* tab, bool modified);

    void addRow();

    void addColumn();

    void removeRow();

    void removeColumn();

    void editChart();

    void editColors();

    void commitData(QSessionManager&);

    void newFile(bool exitOnCancel = false);

    void tabCloseRequested(int index);

    void updateTabs();

    void tabChanged();

    void tabTextUpdated(QWidget* tab, const QString&);

    void setTheme(QAction* action);

    void wizardFinished();

#pragma clang diagnostic push
#pragma ide diagnostic ignored \
    "HidingNonVirtualFunction"  // this is intentional
    void setWindowTitle(const QString& title);
#pragma clang diagnostic pop

    void applyTheme();

signals:

#pragma clang diagnostic push  // https://youtrack.jetbrains.com/issue/CPP-23495
#pragma ide diagnostic ignored "NotImplementedFunctions"
    void themeChanged(const QString& theme);
#pragma clang diagnostic pop

private:
    bool requestClose();

    void createActions();

    void createStatusBar();

    void readSettings();

    void writeSettings();

    Utils::ThemeableAction* createThemeableAction(const QString& iconPath,
                                                  const QString& text,
                                                  QMenu* menu = nullptr,
                                                  QToolBar* toolbar = nullptr);

    template <typename Func>
    Utils::ThemeableAction* createThemeableAction(
        const QString& iconPath,
        const QString& text,
        const typename QtPrivate::FunctionPointer<Func>::Object* receiver,
        Func slot,
        QMenu* menu = nullptr,
        QToolBar* toolbar = nullptr);

    int initNewTab(Views::MainView* tab);

    QTabWidget* tabs;
    Wizard::ChartWizard* lastWizard;

    QString currentTheme;
};
}  // namespace Windows
}  // namespace BasedCharts
