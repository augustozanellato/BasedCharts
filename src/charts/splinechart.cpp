#include "splinechart.h"
#include "chartfactory.h"

using BasedCharts::Charts::SplineChart;
using BasedCharts::Models::ChartDataModel;
using QtCharts::QSplineSeries;

const QString SplineChart::TYPE("spline");

__attribute__((unused)) bool SplineChart::isRegisteredWithFactory =
    ChartFactory::registerClass(SplineChart::TYPE, &SplineChart::instantiate);

SplineChart::SplineChart(QWidget* parent, ChartDataModel* model)
    : XYChart(parent, model) {
    postInit();
}

QSplineSeries* SplineChart::getSeries() const {
    return new QSplineSeries;
}

const QString& SplineChart::type() const {
    return TYPE;
}

SplineChart* SplineChart::instantiate(QWidget* parent,
                                      Models::ChartDataModel* model) {
    return new SplineChart(parent, model);
}