#include "xychart.h"
#include "utils/exceptions.h"

#include <QJsonObject>

using BasedCharts::Charts::XYChart;
using BasedCharts::Models::ChartDataModel;
using BasedCharts::Utils::Exceptions::DeserializationException;
using QtCharts::QVXYModelMapper;

XYChart::XYChart(QWidget* parent, Models::ChartDataModel* model)
    : AbstractChart(parent, model, true, true, false, true) {
    model->setMinimumColumnCount(2);
    model->setProtectedColumnCount(independentX ? 0 : 1);
}
void XYChart::initializeMappers() {
    disconnect(model, &ChartDataModel::dataChanged, this,
               &XYChart::dataChanged);
    for (int i = 0; i < seriesCount(); i++) {
        int yCol = getColumnForSeries(i);
        int xCol = getXColumnForSeries(i);
        auto s = getSeries();
        s->setParent(this);
        chart->addSeries(s);
        series.append(s);
        auto mapper = new QVXYModelMapper(this);
        mapper->setXColumn(xCol);
        mapper->setYColumn(yCol);
        mapper->setSeries(s);
        mapper->setModel(model);
        mappers.append(mapper);
        model->setHeaderData(xCol, Qt::Horizontal, "X");
    }
    connect(model, &ChartDataModel::dataChanged, this, &XYChart::dataChanged);
    model->setProtectedColumnCount(independentX ? 0 : 1);
}
int XYChart::getXColumnForSeries(int index) const {
    return independentX ? index * 2 : 0;
}
QColor XYChart::getColorForSeries(int index) const {
    return series[index]->color();
}
void XYChart::setColorForSeries(int index, QColor color) {
    series[index]->setColor(color);
}
int XYChart::seriesCount() const {
    return independentX ? model->columnCount() / 2 : model->columnCount() - 1;
}
int XYChart::getColumnForSeries(int index) const {
    return independentX ? index * 2 + 1 : 1 + index;
}
void XYChart::internalSerialize(QJsonObject& json) const {
    json["independentX"] = independentX;
}
double XYChart::getXMinValue() const {
    double min = std::numeric_limits<double>::max();
    for (const auto& item : series) {
        for (int i = 0; i < item->count(); i++) {
            const auto& point = item->at(i);
            if (point.x() < min) {
                min = point.x();
            }
        }
    }
    return min;
}
double XYChart::getYMinValue() const {
    double min = std::numeric_limits<double>::max();
    for (const auto& item : series) {
        for (int i = 0; i < item->count(); i++) {
            const auto& point = item->at(i);
            if (point.y() < min) {
                min = point.y();
            }
        }
    }
    return min;
}
double XYChart::getXMaxValue() const {
    double max = std::numeric_limits<double>::min();
    for (const auto& item : series) {
        for (int i = 0; i < item->count(); i++) {
            const auto& point = item->at(i);
            if (point.x() > max) {
                max = point.x();
            }
        }
    }
    return max;
}
double XYChart::getYMaxValue() const {
    double max = std::numeric_limits<double>::min();
    for (const auto& item : series) {
        for (int i = 0; i < item->count(); i++) {
            const auto& point = item->at(i);
            if (point.y() > max) {
                max = point.y();
            }
        }
    }
    return max;
}
void XYChart::updateLabels() {
    for (int i = 0; i < series.count(); i++) {
        auto s = series.at(i);
        s->setName(model->headerData(getColumnForSeries(i), Qt::Horizontal)
                       .toString());
    }
}
bool XYChart::needsEvenColumns() const {
    return independentX;
}
void XYChart::postInit() {
    initializeMappers();
    connect(model, &ChartDataModel::rowsInserted, this,
            &XYChart::rowCountUpdated);
    connect(model, &ChartDataModel::rowsRemoved, this,
            &XYChart::rowCountUpdated);
    connect(model, &ChartDataModel::columnsAboutToBeRemoved, this,
            [this](const QModelIndex&, int first, int last) {
                int seriesIdx = getSeriesFromColumn(first);
                for (int i = first; i <= last; i += (independentX ? 2 : 1)) {
                    auto removedMapper = mappers[seriesIdx];
                    mappers.remove(seriesIdx);
                    removedMapper->setModel(nullptr);
                    removedMapper->setSeries(nullptr);
                    removedMapper->deleteLater();
                    auto removedSeries = series[seriesIdx];
                    series.remove(seriesIdx);
                    chart->removeSeries(removedSeries);
                    removedSeries->deleteLater();
                }
                const int newSeriesCount =
                    seriesCount() -
                    ((last - first) / (independentX ? 2 : 1) + 1);
                for (int i = seriesIdx; i < newSeriesCount; i++) {
                    mappers[i]->setXColumn(getXColumnForSeries(i));
                    mappers[i]->setYColumn(getColumnForSeries(i));
                }
            });
    connect(model, &ChartDataModel::columnsInserted, this,
            [this](const QModelIndex&, int first, int last) {
                disconnect(model, &ChartDataModel::dataChanged, this,
                           &XYChart::dataChanged);
                for (int colIdx = first; colIdx <= last;
                     colIdx += (independentX ? 2 : 1)) {
                    int seriesIdx = getSeriesFromColumn(colIdx);
                    auto mapper = new QVXYModelMapper(this);
                    auto s = getSeries();
                    s->setParent(this);
                    chart->addSeries(s);
                    mappers.insert(seriesIdx, mapper);
                    series.insert(seriesIdx, s);
                    const int xCol = getXColumnForSeries(seriesIdx);
                    const int yCol = getColumnForSeries(seriesIdx);
                    model->setHeaderData(xCol, Qt::Horizontal, "X");
                    mapper->setXColumn(xCol);
                    mapper->setYColumn(yCol);
                    if (independentX) {
                        model->setColumnColor(xCol, s->color());
                    }
                    model->setColumnColor(yCol, s->color());
                    mapper->setSeries(s);
                    mapper->setModel(model);
                    attachSeries(s);
                }
                for (int i = getSeriesFromColumn(last) + 1; i < seriesCount();
                     i++) {
                    mappers[i]->setXColumn(getXColumnForSeries(i));
                    mappers[i]->setYColumn(getColumnForSeries(i));
                }
                connect(model, &ChartDataModel::dataChanged, this,
                        &XYChart::dataChanged);
                updateLabels();
            });
    AbstractChart::postInit();
}
void XYChart::deserialize(const QJsonObject& obj) {
    AbstractChart::deserialize(obj);
    if (!obj["independentX"].isBool()) {
        throw DeserializationException("Missing independentX field");
    }
    setIndependentX(obj["independentX"].toBool());
}
void XYChart::rowCountUpdated() {
    int rowCount = model->rowCount();
    for (auto& item : mappers) {
        item->setRowCount(rowCount);
    }
}
int XYChart::getSeriesFromColumn(int index) const {
    if (independentX) {
        return index / 2;
    } else {
        return index - 1;
    }
}
void XYChart::setIndependentX(bool x) {
    if (x && model->columnCount() % 2 != 0) {
        qDebug() << "adding column to respect even restrictions";
        model->insertColumn(model->columnCount());
    }
    independentX = x;
    for (auto& item : mappers) {
        item->setModel(nullptr);
        item->setSeries(nullptr);
        item->deleteLater();
    }
    mappers.clear();
    for (auto& item : series) {
        chart->removeSeries(item);
        item->deleteLater();
    }
    series.clear();
    initializeMappers();
    for (auto& s : series) {
        attachSeries(s);
    }
    syncColorsToModel();
    dataChanged({}, {}, {Qt::DisplayRole});
}
void XYChart::attachSeries(QtCharts::QXYSeries*& s) const {
    for (auto& axis : chart->axes()) {
        s->attachAxis(axis);
    }
}
void XYChart::syncColorsToModel() const {
    for (int i = 0; i < seriesCount(); i++) {
        if (independentX) {
            model->setColumnColor(getXColumnForSeries(i), getColorForSeries(i));
        }
        model->setColumnColor(getColumnForSeries(i), getColorForSeries(i));
    }
    if (!independentX) {
        model->clearColumnColor(0);
    }
}
bool XYChart::hasIndependentX() const {
    return independentX;
}
