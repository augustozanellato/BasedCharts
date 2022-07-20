#include "mainview.h"
#include "windows/editchartdialog.h"
#include "windows/editcolorsdialog.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFileDialog>
#include <QGuiApplication>
#include <QJsonObject>
#include <QMessageBox>
#include <QSaveFile>

using BasedCharts::Charts::AbstractChart;
using BasedCharts::Models::ChartDataModel;
using BasedCharts::Views::MainView;
using BasedCharts::Windows::EditChartDialog;
using BasedCharts::Windows::EditColorsDialog;

MainView::MainView(QWidget* parent, ChartDataModel* model, AbstractChart* chart)
    : QWidget(parent),
      gridLayout(new QGridLayout(this)),
      table(new QTableView(this)),
      tableModel(model),
      chart(chart) {
    setLayout(gridLayout);
    table->setModel(tableModel);
    table->setSelectionMode(QAbstractItemView::ContiguousSelection);
    gridLayout->addWidget(table, 0, 0);
    gridLayout->addWidget(chart, 0, 1);
    connect(tableModel, &ChartDataModel::dataChanged, this,
            &MainView::dataModified);
    connect(tableModel, &ChartDataModel::headerDataChanged, this,
            &MainView::dataModified);
    connect(chart, &AbstractChart::modified, this, &MainView::dataModified);
    connect(chart, &AbstractChart::titleChanged, this, &MainView::dataModified);
    connect(chart, &AbstractChart::titleChanged, this,
            [this]() { emit tabTextUpdated(this, this->chart->getTitle()); });
}

MainView::MainView(QWidget* parent,
                   Models::ChartDataModel* model,
                   Charts::AbstractChart* chart,
                   const QString& filePath)
    : MainView(parent, model, chart) {
    currentFilePath = filePath;
    modified = false;
}

void MainView::dataModified() {
    modified = true;
    emit updateModified(this, true);
}

bool MainView::saveTo(const QString& filePath) {
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(filePath);
    file.setDirectWriteFallback(true);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        QJsonObject json;
        chart->serialize(json);
        tableModel->serialize(json);
        out << QJsonDocument(json).toJson();
        if (!file.commit()) {
            errorMessage = QString("Cannot write file %1:\n%2.")
                               .arg(QDir::toNativeSeparators(filePath),
                                    file.errorString());
        }
    } else {
        errorMessage =
            QString("Cannot open file %1 for writing:\n%2.")
                .arg(QDir::toNativeSeparators(filePath), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, QCoreApplication::applicationName(),
                             errorMessage);
        return false;
    }

    currentFilePath = filePath;
    emit updateModified(this, false);
    emit pushStatusBarMessage("File saved", 2000);
    return true;
}

bool MainView::saveAs() {
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(".json");
    dialog.setNameFilter("JSON Files (*.json)");
    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }
    return saveTo(dialog.selectedFiles().constFirst());
}

bool MainView::save() {
    bool saved;
    if (currentFilePath.isEmpty()) {
        saved = saveAs();
    } else {
        saved = saveTo(currentFilePath);
    }
    if (saved) {
        modified = false;
    }
    return saved;
}

const ChartDataModel& MainView::model() const {
    return *tableModel;
}

bool MainView::isModified() const {
    return modified;
}

void MainView::addRow() {
    tableModel->insertRow(tableModel->rowCount());
}

void MainView::addColumn() {
    tableModel->insertColumns(tableModel->columnCount(),
                              chart->needsEvenColumns() ? 2 : 1);
}

const QString& MainView::getCurrentFilePath() const {
    return currentFilePath;
}

AbstractChart* MainView::getChart() const {
    return chart;
}

bool MainView::requestClose() {
    if (!modified) {
        return true;
    }
    const QMessageBox::StandardButton ret = QMessageBox::warning(
        this, QCoreApplication::applicationName(),
        "The document has been modified.\n"
        "Do you want to save your changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
        case QMessageBox::Save:
            return save();
        case QMessageBox::Cancel:
            return false;
        case QMessageBox::Discard:
        default:
            return true;
    }
}
void MainView::removeRow() {
    auto select = table->selectionModel();
    if (!select->hasSelection()) {
        tableModel->removeRow(tableModel->rowCount() - 1);
    } else {
        auto selectionRect = select->selection().constFirst();
        tableModel->removeRows(selectionRect.top(), selectionRect.height());
    }
}
void MainView::removeColumn() {
    auto select = table->selectionModel();
    int removeBegin = tableModel->columnCount() - 1;
    int removeEnd = tableModel->columnCount() - 1;
    if (select->hasSelection()) {
        auto selectionRect = select->selection().constFirst();
        removeBegin = selectionRect.left();
        removeEnd = selectionRect.right();
    }
    if (chart->needsEvenColumns()) {
        if (removeBegin % 2 == 1) {
            removeBegin--;
        }
        if (removeEnd % 2 == 0) {
            removeEnd++;
        }
    }
    tableModel->removeColumns(removeBegin, removeEnd - removeBegin + 1);
}
void MainView::editChart() {
    auto dialog = new EditChartDialog(this, tableModel, chart);
    dialog->exec();
    dialog->deleteLater();
}
void MainView::editColors() {
    auto dialog = new EditColorsDialog(this, tableModel, chart);
    dialog->exec();
    dialog->deleteLater();
}
