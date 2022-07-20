#pragma once

#include "xychart.h"

#include <QScatterSeries>

namespace BasedCharts {
namespace Charts {
class ScatterChart : public XYChart {
    Q_OBJECT
    __attribute__((unused)) static bool isRegisteredWithFactory;

public:
    static const QString TYPE;
    ScatterChart(QWidget* parent, Models::ChartDataModel* model);
    const QString& type() const override;

private:
    static ScatterChart* instantiate(QWidget* parent,
                                     Models::ChartDataModel* model);
    QtCharts::QScatterSeries* getSeries() const override;
};
}  // namespace Charts
}  // namespace BasedCharts