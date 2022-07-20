#pragma once
#include "xychart.h"

#include <QLineSeries>

namespace BasedCharts {
namespace Charts {
class LineChart : public XYChart {
    Q_OBJECT
    __attribute__((unused)) static bool isRegisteredWithFactory;

public:
    static const QString TYPE;
    LineChart(QWidget* parent, Models::ChartDataModel* model);

    const QString& type() const override;

private:
    static LineChart* instantiate(QWidget* parent,
                                  Models::ChartDataModel* model);
    QtCharts::QLineSeries* getSeries() const override;
};
}  // namespace Charts
}  // namespace BasedCharts