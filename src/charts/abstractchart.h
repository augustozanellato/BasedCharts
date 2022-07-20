#pragma once

#include "models/chartdatamodel.h"

#include <QChart>
#include <QDebug>
#include <QGridLayout>
#include <QWidget>

namespace BasedCharts {
namespace Charts {

enum class AxisType {
    /// Sets axis min to either 0 or the minimum value in the charted data
    AUTO,
    /// Generally the same as AUTO but it'll probably set a lower value for
    /// min and a higher for max in order to get nicer looking axis ticks
    NICE,
    /// Uses custom values for axis bounds
    MANUAL,
};

bool validateAxisType(int type);

class ChartFactory;

class AbstractChart : public QWidget {
    Q_OBJECT

protected:
    AbstractChart(QWidget* parent,
                  Models::ChartDataModel* model,
                  bool allowXEdit,
                  bool allowYEdit,
                  bool allowRowHeaderEdit,
                  bool allowColumnHeaderEdit);

    Models::ChartDataModel* model;
    QtCharts::QChart* chart = new QtCharts::QChart();
    virtual void postInit();
    virtual void deserialize(const QJsonObject& obj);

private:
    QString title = "Untitled";
    AxisType xType = AxisType::NICE, yType = AxisType::NICE;
    int xTicks = 5, yTicks = 5;
    int xMinorTicks = 0, yMinorTicks = 0;
    double manualMinX, manualMinY, manualMaxX, manualMaxY;
    QString xLabel, yLabel;

    virtual double getXMinValue() const { return 0; };

    virtual double getYMinValue() const { return 0; };

    virtual double getXMaxValue() const { return 0; };

    virtual double getYMaxValue() const { return 0; };

    double getXAxisMin() const;
    double getYAxisMin() const;
    double getXAxisMax() const;
    double getYAxisMax() const;

    virtual void internalSerialize(QJsonObject& json) const;
    virtual const QString& type() const = 0;

public:
    static const int MIN_TICKS, MAX_TICKS, MIN_MINOR_TICKS, MAX_MINOR_TICKS;

    const bool allowXEdit, allowYEdit, allowRowHeaderEdit,
        allowColumnHeaderEdit;
    void serialize(QJsonObject& json) const;

    const QString& getTitle() const;
    void setTitle(const QString& newTitle);

    AxisType getXType() const;
    AxisType getYType() const;

    void setXType(AxisType type);
    void setYType(AxisType type);

    int getXTicks() const;
    int getYTicks() const;

    void setXTicks(int ticks);
    void setYTicks(int ticks);

    int getXMinorTicks() const;
    int getYMinorTicks() const;

    void setXMinorTicks(int ticks);
    void setYMinorTicks(int ticks);

    void setManualMinX(double minX);
    void setManualMinY(double minY);
    void setManualMaxX(double maxX);
    void setManualMaxY(double maxY);

    double getManualMinX() const;
    double getManualMinY() const;
    double getManualMaxX() const;
    double getManualMaxY() const;

    void setXLabel(const QString& label);
    void setYLabel(const QString& label);

    const QString& getXLabel() const;
    const QString& getYLabel() const;

    virtual void syncColorsToModel() const;
    void syncColorsFromModel();

    virtual QColor getColorForSeries(int index) const = 0;
    virtual void setColorForSeries(int index, QColor color) = 0;

    virtual int seriesCount() const = 0;
    virtual int getColumnForSeries(int index) const = 0;

    virtual bool needsEvenColumns() const = 0;

signals:
#pragma clang diagnostic push  // https://youtrack.jetbrains.com/issue/CPP-23495
#pragma ide diagnostic ignored "NotImplementedFunctions"
    void titleChanged();
    void modified();
#pragma clang diagnostic pop

private slots:

    virtual void modelResized(){};

    void scaleX();
    void scaleY();
    virtual void updateLabels() = 0;

protected slots:
    void dataChanged(const QModelIndex&,
                     const QModelIndex&,
                     const QVector<int>& roles);

public slots:

    void themeUpdated(const QString& newTheme);

    friend class ChartFactory;
};
}  // namespace Charts
}  // namespace BasedCharts