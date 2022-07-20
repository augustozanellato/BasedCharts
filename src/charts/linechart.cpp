#include "linechart.h"
#include "chartfactory.h"

#include <QLineSeries>

using BasedCharts::Charts::LineChart;
using BasedCharts::Models::ChartDataModel;
using QtCharts::QLineSeries;

const QString LineChart::TYPE("line");

__attribute__((unused)) bool LineChart::isRegisteredWithFactory =
    ChartFactory::registerClass(LineChart::TYPE, &LineChart::instantiate);

LineChart::LineChart(QWidget* parent, ChartDataModel* model)
    : XYChart(parent, model) {
    postInit();
}

QLineSeries* LineChart::getSeries() const {
    return new QLineSeries();
}

const QString& LineChart::type() const {
    return TYPE;
}

LineChart* LineChart::instantiate(QWidget* parent, ChartDataModel* model) {
    return new LineChart(parent, model);
}