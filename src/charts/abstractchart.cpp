#include "abstractchart.h"
#include "utils/exceptions.h"
#include "utils/themes.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

using BasedCharts::Charts::AbstractChart;
using BasedCharts::Charts::AxisType;
using BasedCharts::Charts::validateAxisType;
using BasedCharts::Models::ChartDataModel;
using BasedCharts::Utils::Themes;
using BasedCharts::Utils::Exceptions::DeserializationException;
using QtCharts::QAbstractAxis;
using QtCharts::QBarCategoryAxis;
using QtCharts::QChart;
using QtCharts::QChartView;
using QtCharts::QValueAxis;

const int AbstractChart::MIN_TICKS = 2;
const int AbstractChart::MAX_TICKS = 10;
const int AbstractChart::MIN_MINOR_TICKS = 0;
const int AbstractChart::MAX_MINOR_TICKS = 4;

AbstractChart::AbstractChart(QWidget* parent,
                             ChartDataModel* model,
                             bool allowXEdit,
                             bool allowYEdit,
                             bool allowRowHeaderEdit,
                             bool allowColumnHeaderEdit)
    : QWidget(parent),
      model(model),
      allowXEdit(allowXEdit),
      allowYEdit(allowYEdit),
      allowRowHeaderEdit(allowRowHeaderEdit),
      allowColumnHeaderEdit(allowColumnHeaderEdit) {
    QChartView* chartView = new QChartView(chart);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle(title);
    chartView->setRenderHint(QPainter::Antialiasing);
    auto layout = new QGridLayout();
    layout->addWidget(chartView, 0, 0);
    setLayout(layout);
}

void AbstractChart::scaleX() {
    auto abstractXAxis = chart->axes(Qt::Horizontal).at(0);
    if (abstractXAxis->type() == QAbstractAxis::AxisTypeValue) {
        QValueAxis* xAxis = qobject_cast<QValueAxis*>(abstractXAxis);
        auto min = getXAxisMin();
        auto max = getXAxisMax();
        if (std::abs(min - max) < 0.01) {
            max += 1;
        }
        xAxis->setRange(xType == AxisType::MANUAL ? min : std::min(min, .0),
                        max);
        if (xType == AxisType::NICE) {
            xAxis->applyNiceNumbers();
            manualMinX = xAxis->min();
            manualMaxX = xAxis->max();
        } else if (xType == AxisType::AUTO) {
            manualMinX = min;
            manualMaxX = max;
        }
        xAxis->setTickCount(xTicks);
        xAxis->setMinorTickCount(xMinorTicks);
    } else if (abstractXAxis->type() == QAbstractAxis::AxisTypeBarCategory) {
        QBarCategoryAxis* xAxis =
            qobject_cast<QBarCategoryAxis*>(abstractXAxis);

        xAxis->setRange(xAxis->at(0), xAxis->at(xAxis->count() - 1));
    } else {
        qDebug() << "unsupported x axis type" << abstractXAxis->type();
    }
}

void AbstractChart::scaleY() {
    auto abstractYAxis = chart->axes(Qt::Vertical).at(0);
    if (abstractYAxis->type() == QAbstractAxis::AxisTypeValue) {
        QValueAxis* yAxis = qobject_cast<QValueAxis*>(abstractYAxis);
        auto min = getYAxisMin();
        auto max = getYAxisMax();
        if (std::abs(min - max) < 0.01) {
            max += 1;
        }
        yAxis->setRange(yType == AxisType::MANUAL ? min : std::min(min, .0),
                        max);
        if (yType == AxisType::NICE) {
            yAxis->applyNiceNumbers();
            manualMinY = yAxis->min();
            manualMaxY = yAxis->max();
        } else if (yType == AxisType::AUTO) {
            manualMinY = min;
            manualMaxY = max;
        }
        yAxis->setTickCount(yTicks);
        yAxis->setMinorTickCount(yMinorTicks);
    } else {
        qDebug() << "unsupported y axis type" << abstractYAxis->type();
    }
}

void AbstractChart::themeUpdated(const QString& newTheme) {
    const auto seriesList = chart->series();
    for (const auto series : seriesList) {
        chart->removeSeries(series);
    }
    if (newTheme == Themes::DARK) {
        chart->setTheme(QChart::ChartTheme::ChartThemeDark);
    } else if (newTheme == Themes::LIGHT) {
        chart->setTheme(QChart::ChartTheme::ChartThemeLight);
    } else {
        qDebug() << "unsupported chart theme" << newTheme
                 << "falling back to light";
        chart->setTheme(QChart::ChartTheme::ChartThemeLight);
    }

    const auto chartAxes = chart->axes();
    for (const auto series : seriesList) {
        chart->addSeries(series);
        for (const auto& axis : chartAxes) {
            series->attachAxis(axis);
        }
    }
}

void AbstractChart::postInit() {
    connect(model, &ChartDataModel::rowsInserted, this,
            &AbstractChart::modelResized);
    connect(
        model, &ChartDataModel::rowsInserted, this,
        [this](const QModelIndex&, int first, int last) {
            auto abstractXAxis = chart->axes(Qt::Horizontal).at(0);
            if (abstractXAxis->type() == QAbstractAxis::AxisTypeBarCategory) {
                // in an ideal world this should be done by the Qt mapper but
                // whatever
                QBarCategoryAxis* xAxis =
                    qobject_cast<QBarCategoryAxis*>(abstractXAxis);

                for (int i = first; i <= last; ++i) {
                    auto category =
                        model->headerData(i, Qt::Vertical).toString();
                    if (i == xAxis->count()) {
                        xAxis->append(category);
                    } else {
                        xAxis->insert(i, category);
                    }
                }
            }
            scaleX();
        });
    connect(model, &ChartDataModel::columnsInserted, this,
            &AbstractChart::modelResized);
    connect(model, &ChartDataModel::columnsInserted, this,
            &AbstractChart::scaleY);
    connect(model, &ChartDataModel::rowsRemoved, this,
            &AbstractChart::modelResized);
    connect(
        model, &ChartDataModel::rowsRemoved, this,
        [this](const QModelIndex&, int first, int last) {
            auto abstractXAxis = chart->axes(Qt::Horizontal).at(0);
            if (abstractXAxis->type() == QAbstractAxis::AxisTypeBarCategory) {
                QBarCategoryAxis* xAxis =
                    qobject_cast<QBarCategoryAxis*>(abstractXAxis);

                for (int i = first; i <= last; ++i) {
                    xAxis->remove(xAxis->at(first));
                }
            } else {
                scaleX();
            }
        });
    connect(model, &ChartDataModel::columnsRemoved, this,
            &AbstractChart::modelResized);
    connect(model, &ChartDataModel::columnsRemoved, this,
            &AbstractChart::scaleY);
    connect(model, &ChartDataModel::dataChanged, this,
            &AbstractChart::dataChanged);
    connect(model, &ChartDataModel::headerDataChanged, this,
            &AbstractChart::updateLabels);
    chart->createDefaultAxes();
    syncColorsToModel();
    modelResized();
    scaleX();
    scaleY();
    updateLabels();
}

QJsonObject serializeAxis(const AbstractChart& chart, bool x) {
    QJsonObject axis;
    axis["type"] = static_cast<int>(x ? chart.getXType() : chart.getYType());
    axis["ticks"] = x ? chart.getXTicks() : chart.getYTicks();
    axis["minorTicks"] = x ? chart.getXMinorTicks() : chart.getYMinorTicks();
    axis["manualMin"] = x ? chart.getManualMinX() : chart.getManualMinY();
    axis["manualMax"] = x ? chart.getManualMaxX() : chart.getManualMaxY();
    axis["label"] = x ? chart.getXLabel() : chart.getYLabel();
    return axis;
}

void AbstractChart::serialize(QJsonObject& json) const {
    json["title"] = title;
    QJsonObject axes;
    if (allowXEdit) {
        axes["x"] = serializeAxis(*this, true);
    }
    if (allowYEdit) {
        axes["y"] = serializeAxis(*this, false);
    }
    json["axes"] = axes;
    internalSerialize(json);
}

const QString& AbstractChart::getTitle() const {
    return title;
}

void AbstractChart::setTitle(const QString& newTitle) {
    title = newTitle;
    chart->setTitle(title);
    emit titleChanged();
}

double AbstractChart::getXAxisMin() const {
    return xType == AxisType::MANUAL ? manualMinX : getXMinValue();
}

double AbstractChart::getYAxisMin() const {
    return yType == AxisType::MANUAL ? manualMinY : getYMinValue();
}

double AbstractChart::getXAxisMax() const {
    return xType == AxisType::MANUAL ? manualMaxX : getXMaxValue();
}

double AbstractChart::getYAxisMax() const {
    return yType == AxisType::MANUAL ? manualMaxY : getYMaxValue();
}

AxisType AbstractChart::getXType() const {
    return xType;
}

AxisType AbstractChart::getYType() const {
    return yType;
}

void AbstractChart::setXType(AxisType type) {
    xType = type;
    scaleX();
    emit modified();
}

void AbstractChart::setYType(AxisType type) {
    yType = type;
    scaleY();
    emit modified();
}

int AbstractChart::getXTicks() const {
    return xTicks;
}

int AbstractChart::getYTicks() const {
    return yTicks;
}

void AbstractChart::setXTicks(int ticks) {
    xTicks = ticks;
    scaleX();
    emit modified();
}

void AbstractChart::setYTicks(int ticks) {
    yTicks = ticks;
    scaleY();
    emit modified();
}

int AbstractChart::getXMinorTicks() const {
    return xMinorTicks;
}

int AbstractChart::getYMinorTicks() const {
    return yMinorTicks;
}

void AbstractChart::setXMinorTicks(int ticks) {
    xMinorTicks = ticks;
    scaleX();
    emit modified();
}

void AbstractChart::setYMinorTicks(int ticks) {
    yMinorTicks = ticks;
    scaleY();
    emit modified();
}
double AbstractChart::getManualMinX() const {
    return manualMinX;
}
double AbstractChart::getManualMinY() const {
    return manualMinY;
}
double AbstractChart::getManualMaxX() const {
    return manualMaxX;
}
double AbstractChart::getManualMaxY() const {
    return manualMaxY;
}
void AbstractChart::setManualMinX(double minX) {
    manualMinX = minX;
    scaleX();
    emit modified();
}
void AbstractChart::setManualMinY(double minY) {
    manualMinY = minY;
    scaleY();
    emit modified();
}
void AbstractChart::setManualMaxX(double maxX) {
    manualMaxX = maxX;
    scaleX();
    emit modified();
}
void AbstractChart::setManualMaxY(double maxY) {
    manualMaxY = maxY;
    scaleY();
    emit modified();
}
void AbstractChart::syncColorsToModel() const {
    for (int i = 0; i < seriesCount(); ++i) {
        model->setColumnColor(getColumnForSeries(i), getColorForSeries(i));
    }
}
void AbstractChart::syncColorsFromModel() {
    for (int i = 0; i < seriesCount(); ++i) {
        setColorForSeries(i, model->getColumnColor(getColumnForSeries(i)));
    }
}

int checkIntInRange(const QJsonObject& json,
                    const QString&& key,
                    int min,
                    int max,
                    const QString&& error) {
    if (!json[key].isDouble()) {
        throw DeserializationException(error);
    }
    auto value = json[key].toInt();
    if (value < min || value > max) {
        throw DeserializationException(error);
    }
    return value;
}

void deserializeAxis(AbstractChart& chart, const QJsonObject& axis, bool x) {
    if (!axis["type"].isDouble() || !validateAxisType(axis["type"].toInt())) {
        throw DeserializationException("Invalid axis type");
    }
    auto axisType = static_cast<AxisType>(axis["type"].toInt());

    auto ticks =
        checkIntInRange(axis, "ticks", AbstractChart::MIN_TICKS,
                        AbstractChart::MAX_TICKS, "Invalid axis tick count");
    auto minorTicks = checkIntInRange(
        axis, "minorTicks", AbstractChart::MIN_MINOR_TICKS,
        AbstractChart::MAX_MINOR_TICKS, "Invalid axis minor tick count");
    if (!axis["manualMin"].isDouble()) {
        throw DeserializationException("Invalid axis min specification");
    }
    auto manualMin = axis["manualMin"].toDouble();
    if (!axis["manualMax"].isDouble()) {
        throw DeserializationException("Invalid axis max specification");
    }
    auto manualMax = axis["manualMax"].toDouble();
    if (manualMax <= manualMin) {
        throw DeserializationException("Invalid axis range specification");
    }
    if (!axis["label"].isString() && !axis["label"].isNull()) {
        throw DeserializationException("Invalid axis label");
    }
    auto label = axis["label"].toString();
    if (x) {
        chart.setXType(axisType);
        chart.setXTicks(ticks);
        chart.setXMinorTicks(minorTicks);
        chart.setManualMinX(manualMin);
        chart.setManualMaxX(manualMax);
        chart.setXLabel(label);
    } else {
        chart.setYType(axisType);
        chart.setYTicks(ticks);
        chart.setYMinorTicks(minorTicks);
        chart.setManualMinY(manualMin);
        chart.setManualMaxY(manualMax);
        chart.setYLabel(label);
    }
}

void AbstractChart::deserialize(const QJsonObject& obj) {
    syncColorsFromModel();
    setTitle(obj["title"].toString());
    if (obj["axes"].isObject()) {
        auto axisSpec = obj["axes"].toObject();
        if (allowXEdit) {
            if (axisSpec["x"].isObject()) {
                deserializeAxis(*this, axisSpec["x"].toObject(), true);
            } else {
                throw DeserializationException(
                    "Missing or invalid required X axis definition");
            }
        }
        if (allowYEdit) {
            if (axisSpec["y"].isObject()) {
                deserializeAxis(*this, axisSpec["y"].toObject(), false);
            } else {
                throw DeserializationException(
                    "Missing or invalid required Y axis definition");
            }
        }
    }
}
void AbstractChart::dataChanged(const QModelIndex&,
                                const QModelIndex&,
                                const QVector<int>& roles) {
    if (roles.contains(Qt::DisplayRole)) {
        scaleX();
        scaleY();
    }
}
void AbstractChart::setXLabel(const QString& label) {
    xLabel = label;
    chart->axes(Qt::Horizontal).at(0)->setTitleText(label);
}
void AbstractChart::setYLabel(const QString& label) {
    yLabel = label;
    chart->axes(Qt::Vertical).at(0)->setTitleText(label);
}

const QString& AbstractChart::getXLabel() const {
    return xLabel;
}

const QString& AbstractChart::getYLabel() const {
    return yLabel;
}
void AbstractChart::internalSerialize(QJsonObject& json) const {
    json["type"] = type();
}

bool BasedCharts::Charts::validateAxisType(int type) {
    switch (static_cast<AxisType>(type)) {
        case AxisType::AUTO:
        case AxisType::NICE:
        case AxisType::MANUAL:
            return true;
        default:
            return false;
    }
}