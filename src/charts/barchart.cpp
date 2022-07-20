#include "barchart.h"
#include "chartfactory.h"

#include <QDebug>
#include <QJsonObject>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSet>
#include <limits>

using BasedCharts::Charts::AbstractChart;
using BasedCharts::Charts::BarChart;
using BasedCharts::Models::ChartDataModel;
using QtCharts::QBarCategoryAxis;
using QtCharts::QBarSeries;
using QtCharts::QVBarModelMapper;

const QString BarChart::TYPE("bar");

__attribute__((unused)) bool BarChart::isRegisteredWithFactory =
    ChartFactory::registerClass(BarChart::TYPE, &BarChart::instantiate);

BarChart::BarChart(QWidget* parent, ChartDataModel* model)
    : AbstractChart(parent, model, false, true, true, true),
      series(new QBarSeries(chart)),
      modelMapper(new QVBarModelMapper(this)) {
    modelMapper->setFirstBarSetColumn(0);
    modelMapper->setLastBarSetColumn(model->columnCount() - 1);
    modelMapper->setSeries(series);
    modelMapper->setModel(model);
    chart->addSeries(series);
    model->setMinimumColumnCount(1);
    model->setProtectedColumnCount(0);

    AbstractChart::postInit();
}

double BarChart::getYMinValue() const {
    double min = std::numeric_limits<double>::max();
    auto sets = series->barSets();
    for (auto set = sets.cbegin(); set != sets.cend(); set++) {
        for (int i = 0; i < (*set)->count(); i++) {
            auto value = (*set)->at(i);
            if (value < min) {
                min = value;
            }
        }
    }
    return min;
}

double BarChart::getYMaxValue() const {
    double max = std::numeric_limits<double>::min();
    auto sets = series->barSets();
    for (auto set = sets.cbegin(); set != sets.cend(); set++) {
        for (int i = 0; i < (*set)->count(); i++) {
            auto value = (*set)->at(i);
            if (value > max) {
                max = value;
            }
        }
    }
    return max;
}

void BarChart::modelResized() {
    modelMapper->setLastBarSetColumn(model->columnCount() - 1);
    syncColorsToModel();
}
BarChart* BarChart::instantiate(QWidget* parent, ChartDataModel* model) {
    return new BarChart(parent, model);
}
void BarChart::updateLabels() {
    QBarCategoryAxis* xAxis =
        qobject_cast<QBarCategoryAxis*>(chart->axes(Qt::Horizontal).at(0));

    for (int i = 0; i < xAxis->count(); ++i) {
        xAxis->replace(xAxis->at(i),
                       model->headerData(i, Qt::Vertical).toString());
    }
}

QColor BarChart::getColorForSeries(int index) const {
    return series  // clazy:exclude=detaching-temporary
        ->barSets()[index]
        ->color();
}

void BarChart::setColorForSeries(int index, QColor color) {
    series->barSets()[index]->setColor(  // clazy:exclude=detaching-temporary
        color);
    emit modified();
}

int BarChart::seriesCount() const {
    return model->columnCount();
}

int BarChart::getColumnForSeries(int index) const {
    return index;
}
bool BarChart::needsEvenColumns() const {
    return false;
}
const QString& BarChart::type() const {
    return TYPE;
}
