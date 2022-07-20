#pragma once
#include "abstractchart.h"

#include <QtCharts/QBarSeries>
#include <QtCharts/QVBarModelMapper>

namespace BasedCharts {
namespace Charts {
class BarChart : public AbstractChart {
    Q_OBJECT
    __attribute__((unused)) static bool isRegisteredWithFactory;

    static BarChart* instantiate(QWidget* parent,
                                 Models::ChartDataModel* model);

public:
    static const QString TYPE;

    BarChart(QWidget* parent, Models::ChartDataModel* model);

    QColor getColorForSeries(int index) const override;
    void setColorForSeries(int index, QColor color) override;
    int seriesCount() const override;
    int getColumnForSeries(int index) const override;
    bool needsEvenColumns() const override;

private:
    QtCharts::QBarSeries* series;
    QtCharts::QVBarModelMapper* modelMapper;

    const QString& type() const override;

    double getYMinValue() const override;

    double getYMaxValue() const override;

    void modelResized() override;

    void updateLabels() override;
};
}  // namespace Charts
}  // namespace BasedCharts