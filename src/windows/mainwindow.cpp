#include "mainwindow.h"
#include "charts/abstractchart.h"
#include "charts/chartfactory.h"
#include "models/chartdatamodel.h"
#include "utils/exceptions.h"
#include "utils/themeableaction.h"
#include "utils/themes.h"
#include "views/mainview.h"
#include "windows/wizard/chartwizard.h"

#include <QDesktopWidget>

using BasedCharts::Charts::AbstractChart;
using BasedCharts::Charts::ChartFactory;
using BasedCharts::Utils::getThemePath;
using BasedCharts::Utils::ThemeableAction;
using BasedCharts::Utils::Themes;
using BasedCharts::Utils::Exceptions::DeserializationException;
using BasedCharts::Views::MainView;
using BasedCharts::Windows::MainWindow;
using BasedCharts::Windows::Wizard::ChartWizard;

QFileSelector MainWindow::themeFileSelector;

MainWindow::MainWindow() : tabs(new QTabWidget(this)) {
    setCentralWidget(tabs);

    connect(tabs->tabBar(), &QTabBar::tabCloseRequested, this,
            &MainWindow::tabCloseRequested);
    connect(tabs->tabBar(), &QTabBar::tabCloseRequested, this,
            &MainWindow::updateTabs);

    connect(tabs, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);

    readSettings();

    createActions();
    createStatusBar();

    applyTheme();

    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest, this,
            &MainWindow::commitData);

    setUnifiedTitleAndToolBarOnMac(true);

    QTimer::singleShot(0, this, [this] { newFile(true); });
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (requestClose()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::wizardFinished() {
    if (lastWizard != nullptr) {
        if (lastWizard->hasValidResult()) {
            auto result = lastWizard->getResult();
            auto model = std::get<0>(result);
            auto chart = std::get<1>(result);

            // we need to take the ownership of the returned objects to prevent
            // them from being deleted when the wizard is disposed
            model->setParent(this);
            chart->setParent(this);

            MainView* view;
            auto loadedFilePath = std::get<2>(result);
            if (loadedFilePath.isNull()) {
                view = new MainView(this, model, chart);
            } else {
                view = new MainView(this, model, chart, loadedFilePath);
            }
            tabs->setCurrentIndex(initNewTab(view));
        }
        lastWizard->deleteLater();
        lastWizard = nullptr;
    }
}

void MainWindow::newFile(bool exitOnCancel) {
    lastWizard = new ChartWizard(this, exitOnCancel);
    lastWizard->setModal(true);
    lastWizard->show();
    connect(lastWizard, &ChartWizard::finished, this,
            &MainWindow::wizardFinished);
}

void MainWindow::open() {
    QFileDialog* dialog = getOpenDialog(this);
    if (dialog->exec() == QDialog::Accepted) {
        loadFile(dialog->selectedFiles().constFirst());
    }
    dialog->deleteLater();
}

QFileDialog* MainWindow::getOpenDialog(QWidget* parent) {
    auto dialog = new QFileDialog(parent);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setDefaultSuffix(".json");
    dialog->setNameFilter("JSON Files (*.json)");
    return dialog;
}

void MainWindow::save() {
    static_cast<MainView*>(tabs->currentWidget())->save();
}

void MainWindow::saveAs() {
    static_cast<MainView*>(tabs->currentWidget())->saveAs();
}

void MainWindow::about() {
    QMessageBox::about(this, "About BasedCharts",
                       "<b>BasedCharts</b> was developed by Augusto Zanellato "
                       "during the academic year 2021/22 as the project for "
                       "the OOP course held in UniPD");
}

void MainWindow::updateModified(QWidget* tab, bool modified) {
    if (tab == tabs->currentWidget()) {
        setWindowModified(modified);
    }
}

void MainWindow::addRow() {
    static_cast<MainView*>(tabs->currentWidget())->addRow();
}

void MainWindow::addColumn() {
    static_cast<MainView*>(tabs->currentWidget())->addColumn();
}

void MainWindow::createActions() {
    auto addTabBtn = new QToolButton(tabs);
    addTabBtn->setText("+");
    tabs->setTabPosition(QTabWidget::TabPosition::North);
    tabs->setCornerWidget(addTabBtn, Qt::Corner::TopLeftCorner);
    connect(addTabBtn, &QToolButton::clicked, this, &MainWindow::newFile);

    QMenu* fileMenu = menuBar()->addMenu("&File");
    QToolBar* fileToolBar = addToolBar("File");
    fileToolBar->setObjectName("FileToolbar");

    QAction* newAct = createThemeableAction(":/icons/new-chart.svg", "&New",
                                            fileMenu, fileToolBar);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip("Create a new chart");
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    QAction* openAct =
        createThemeableAction(":/icons/open.svg", "&Open...", this,
                              &MainWindow::open, fileMenu, fileToolBar);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip("Open an existing chart");

    QAction* saveAct =
        createThemeableAction(":/icons/save.svg", "&Save", this,
                              &MainWindow::save, fileMenu, fileToolBar);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip("Save the chart to disk");

    QAction* saveAsAct =
        createThemeableAction(":/icons/save-as.svg", "Save &As...", this,
                              &MainWindow::saveAs, fileMenu);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip("Save the chart under a new name");

    fileMenu->addSeparator();

    QAction* exitAct = createThemeableAction(":/icons/exit.svg", "E&xit", this,
                                             &QWidget::close, fileMenu);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip("Exit the application");

    QMenu* editMenu = menuBar()->addMenu("&Edit");
    QToolBar* editToolBar = addToolBar("Edit");
    editToolBar->setObjectName("EditToolbar");

    QAction* addRowAct =
        createThemeableAction(":/icons/table-row-plus.svg", "Add &Row", this,
                              &MainWindow::addRow, editMenu, editToolBar);
    addRowAct->setStatusTip("Add a new row to the table");
    QAction* addColAct = createThemeableAction(
        ":/icons/table-column-plus.svg", "Add &Column", this,
        &MainWindow::addColumn, editMenu, editToolBar);
    addColAct->setStatusTip("Add a new column to the table");

    QAction* removeRowAct = createThemeableAction(
        ":/icons/table-row-remove.svg", "Remove Row", this,
        &MainWindow::removeRow, editMenu, editToolBar);
    removeRowAct->setStatusTip("Remove a row from the table");

    QAction* removeColAct = createThemeableAction(
        ":/icons/table-column-remove.svg", "Remove Column", this,
        &MainWindow::removeColumn, editMenu, editToolBar);
    removeColAct->setStatusTip("Remove a column from the table");

    QAction* editTableAct =
        createThemeableAction(":/icons/table-cog.svg", "Edit chart", this,
                              &MainWindow::editChart, editMenu, editToolBar);
    editTableAct->setStatusTip(
        "Customize chart properties such as row and column labels, axis labels "
        "and scaling properties");

    QAction* editColorsAct =
        createThemeableAction(":/icons/palette.svg", "Edit chart colors", this,
                              &MainWindow::editColors, editMenu, editToolBar);
    editColorsAct->setStatusTip("Customize chart colors");

    QMenu* viewMenu = menuBar()->addMenu("&View");
    QMenu* themeMenu = viewMenu->addMenu("&Theme");
    connect(this, &MainWindow::themeChanged, this, [themeMenu]() {
        themeMenu->setIcon(QIcon(MainWindow::themeFileSelector.select(
            ":/icons/theme-selection.svg")));
    });

    QActionGroup* themeGroup = new QActionGroup(this);
    themeGroup->setExclusive(true);
    themeGroup->addAction("Dark");
    themeGroup->addAction("Light");
    connect(themeGroup, &QActionGroup::triggered, this, &MainWindow::setTheme);
    connect(themeGroup, &QActionGroup::triggered, this,
            &MainWindow::applyTheme);
    themeMenu->addActions(themeGroup->actions());

    QMenu* helpMenu = menuBar()->addMenu("&Help");
    QAction* aboutAct = helpMenu->addAction("&About", this, &MainWindow::about);
    aboutAct->setStatusTip("Show the application's About box");

    QAction* aboutQtAct =
        helpMenu->addAction("About &Qt", qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip("Show the Qt library's About box");
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("Ready");
}

void MainWindow::readSettings() {
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    const QByteArray geometry =
        settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        // QDesktopWidget is deprecated but its replacement isn't available in
        // Qt 5.9
        const QRect availableGeometry =
            QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
    const QByteArray state =
        settings.value("state", QByteArray()).toByteArray();
    if (!state.isEmpty()) {
        restoreState(state);
    }
    currentTheme = settings.value("theme", Themes::LIGHT).toString();
}

void MainWindow::setTheme(QAction* action) {
    currentTheme = action->text().toLower();
}

void MainWindow::applyTheme() {
    QFile themeFile(getThemePath(currentTheme));
    themeFile.open(QFile::ReadOnly | QFile::Text);
    qApp->setStyleSheet(QTextStream(&themeFile).readAll());
    themeFile.close();
    QStringList theme;
    theme.append(currentTheme);
    themeFileSelector.setExtraSelectors(theme);
    emit themeChanged(currentTheme);
}

void MainWindow::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    settings.setValue("theme", currentTheme);
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

void MainWindow::loadFile(const QString& fileName) {
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    try {
        auto result = ChartFactory::load(this, fileName);
        tabs->setCurrentIndex(initNewTab(new MainView(
            this, std::get<0>(result), std::get<1>(result), fileName)));
        statusBar()->showMessage("File loaded", 2000);
    } catch (const DeserializationException& e) {
        e.showError(this);
    }

    QGuiApplication::restoreOverrideCursor();
}

void MainWindow::commitData(QSessionManager& manager) {
    if (manager.allowsInteraction()) {
        if (!requestClose()) {
            manager.cancel();
        }
    } else {
        for (int i = 0; i < tabs->count(); i++) {
            auto tab = static_cast<MainView*>(tabs->widget(i));
            if (tab->isModified()) {
                tab->save();
            }
        }
    }
}

void MainWindow::tabCloseRequested(int index) {
    MainView* view = static_cast<MainView*>(tabs->widget(index));
    if (view->requestClose()) {
        tabs->removeTab(index);
    }
}

void MainWindow::updateTabs() {
    tabs->setTabsClosable(tabs->count() > 1);
}

void MainWindow::tabChanged() {
    MainView* view = static_cast<MainView*>(tabs->currentWidget());
    setWindowTitle(view->getChart()->getTitle());
    setWindowFilePath(view->getCurrentFilePath());
    updateModified(view, view->isModified());
}

void MainWindow::tabTextUpdated(QWidget* tab, const QString& text) {
    tabs->setTabText(tabs->indexOf(tab), text);
    if (tab == tabs->currentWidget()) {
        setWindowTitle(text);

        setWindowFilePath(static_cast<MainView*>(tab)->getCurrentFilePath());
    }
}
bool MainWindow::requestClose() {
    bool result = true;
    for (int i = 0; i < tabs->count(); i++) {
        result &= static_cast<MainView*>(tabs->widget(i))->requestClose();
    }
    return result;
}

int MainWindow::initNewTab(MainView* tab) {
    tab->getChart()->themeUpdated(currentTheme);
    connect(tab, &MainView::updateModified, this, &MainWindow::updateModified);
    connect(this, &MainWindow::themeChanged, tab->getChart(),
            &AbstractChart::themeUpdated);
    connect(tab, &MainView::tabTextUpdated, this, &MainWindow::tabTextUpdated);

    int tabIdx = tabs->addTab(tab, tab->getChart()->getTitle());
    updateTabs();
    return tabIdx;
}
void MainWindow::removeRow() {
    static_cast<MainView*>(tabs->currentWidget())->removeRow();
}
void MainWindow::removeColumn() {
    static_cast<MainView*>(tabs->currentWidget())->removeColumn();
}
ThemeableAction* MainWindow::createThemeableAction(const QString& iconPath,
                                                   const QString& text,
                                                   QMenu* menu,
                                                   QToolBar* toolbar) {
    auto action = ThemeableAction::create(this, &MainWindow::themeChanged,
                                          iconPath, text, this);
    if (menu != nullptr) {
        menu->addAction(action);
    }
    if (toolbar != nullptr) {
        toolbar->addAction(action);
    }
    return action;
}
template <typename Func>
ThemeableAction* MainWindow::createThemeableAction(
    const QString& iconPath,
    const QString& text,
    const typename QtPrivate::FunctionPointer<Func>::Object* receiver,
    Func slot,
    QMenu* menu,
    QToolBar* toolbar) {
    auto action = createThemeableAction(iconPath, text);
    connect(action, &QAction::triggered, receiver, slot);
    if (menu != nullptr) {
        menu->addAction(action);
    }
    if (toolbar != nullptr) {
        toolbar->addAction(action);
    }
    return action;
}

void MainWindow::editChart() {
    static_cast<MainView*>(tabs->currentWidget())->editChart();
}

void MainWindow::editColors() {
    static_cast<MainView*>(tabs->currentWidget())->editColors();
}

void MainWindow::setWindowTitle(const QString& title) {
    QWidget::setWindowTitle(QString("[*]%1").arg(title));
}
