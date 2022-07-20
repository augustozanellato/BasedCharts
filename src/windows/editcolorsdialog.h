#pragma once

#include "charts/abstractchart.h"
#include "models/chartdatamodel.h"

#include <QDialog>
#include <QWidget>

namespace BasedCharts {
namespace Windows {
class EditColorsDialog : public QDialog {
    Q_OBJECT
    Models::ChartDataModel* model;
    QVBoxLayout* layout;
    Charts::AbstractChart* chart;

public:
    EditColorsDialog(QWidget* parent,
                     Models::ChartDataModel* model,
                     Charts::AbstractChart* chart);
};
}  // namespace Windows
}  // namespace BasedCharts