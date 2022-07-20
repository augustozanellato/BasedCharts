#pragma once

#include "charts/abstractchart.h"
#include "models/chartdatamodel.h"

#include <QGridLayout>
#include <QJsonDocument>
#include <QTableView>
#include <QWidget>

namespace BasedCharts {
namespace Views {
class MainView : public QWidget {
    Q_OBJECT
public:
    MainView(QWidget* parent,
             Models::ChartDataModel* model,
             Charts::AbstractChart* chart);
    MainView(QWidget* parent,
             Models::ChartDataModel* model,
             Charts::AbstractChart* chart,
             const QString& filePath);

    void addRow();

    void addColumn();

    void removeRow();

    void removeColumn();

    bool save();

    bool saveAs();

    void editChart();

    void editColors();

    bool saveTo(const QString& filePath);

    const Models::ChartDataModel& model() const;

    bool isModified() const;

    const QString& getCurrentFilePath() const;

    bool requestClose();

    Charts::AbstractChart* getChart() const;

private:
    QGridLayout* gridLayout;
    QTableView* table;
    Models::ChartDataModel* tableModel;
    Charts::AbstractChart* chart;
    QString currentFilePath;
    bool modified = true;

signals:

#pragma clang diagnostic push  // https://youtrack.jetbrains.com/issue/CPP-23495
#pragma ide diagnostic ignored "NotImplementedFunctions"
    void updateModified(QWidget* tab, bool modified);

    void tabTextUpdated(QWidget* tab, const QString& text);

    void pushStatusBarMessage(const QString& message, unsigned int timeout);
#pragma clang diagnostic pop

private slots:
    void dataModified();
};
}  // namespace Views
}  // namespace BasedCharts