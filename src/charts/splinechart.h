#pragma once

#include "xychart.h"

#include <QSplineSeries>

namespace BasedCharts {
namespace Charts {
class SplineChart : public XYChart {
    Q_OBJECT
    __attribute__((unused)) static bool isRegisteredWithFactory;

public:
    static const QString TYPE;
    SplineChart(QWidget* parent, Models::ChartDataModel* model);
    const QString& type() const override;

private:
    static SplineChart* instantiate(QWidget* parent,
                                    Models::ChartDataModel* model);
    QtCharts::QSplineSeries* getSeries() const override;
};
}  // namespace Charts
}  // namespace BasedCharts