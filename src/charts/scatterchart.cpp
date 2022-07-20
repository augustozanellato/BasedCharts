#include "scatterchart.h"
#include "chartfactory.h"

using BasedCharts::Charts::ScatterChart;
using BasedCharts::Models::ChartDataModel;
using QtCharts::QScatterSeries;

const QString ScatterChart::TYPE("scatter");

__attribute__((unused)) bool ScatterChart::isRegisteredWithFactory =
    ChartFactory::registerClass(ScatterChart::TYPE, &ScatterChart::instantiate);

ScatterChart::ScatterChart(QWidget* parent, ChartDataModel* model)
    : XYChart(parent, model) {
    postInit();
}

QScatterSeries* ScatterChart::getSeries() const {
    auto series = new QScatterSeries;
    series->setMarkerSize(series->markerSize() *
                          0.75);  // default point size is too big
    return series;
}

const QString& ScatterChart::type() const {
    return TYPE;
}

ScatterChart* ScatterChart::instantiate(QWidget* parent,
                                        ChartDataModel* model) {
    return new ScatterChart(parent, model);
}
