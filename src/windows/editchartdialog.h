#pragma once

#include "charts/abstractchart.h"
#include "models/chartdatamodel.h"

#include <QDialog>
#include <QWidget>

namespace BasedCharts {
namespace Windows {
class EditChartDialog : public QDialog {
    Q_OBJECT
    void createHeaderEdit(int layoutRow, int layoutColumn, bool isRowHeader);
    void createAxisEdit(int layoutRow, int layoutColumn, bool isXAxis);
    Models::ChartDataModel* model;
    QGridLayout* layout;
    Charts::AbstractChart* chart;

public:
    EditChartDialog(QWidget* parent,
                    Models::ChartDataModel* model,
                    Charts::AbstractChart* chart);
};
}  // namespace Windows
}  // namespace BasedCharts