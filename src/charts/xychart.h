#pragma once

#include "abstractchart.h"

#include <QVXYModelMapper>
#include <QVector>
#include <QXYSeries>

namespace BasedCharts {
namespace Charts {
class XYChart : public AbstractChart {
    Q_OBJECT

public:
    XYChart(QWidget* parent, Models::ChartDataModel* model);

    QColor getColorForSeries(int index) const override;
    void setColorForSeries(int index, QColor color) override;
    int seriesCount() const override;
    int getColumnForSeries(int index) const override;

    bool hasIndependentX() const;
    bool needsEvenColumns() const override;

protected:
    void postInit() override final;

private:
    QVector<QtCharts::QXYSeries*> series;
    QVector<QtCharts::QVXYModelMapper*> mappers;
    bool independentX = false;
    int getXColumnForSeries(int index) const;
    int getSeriesFromColumn(int index) const;

    void initializeMappers();
    virtual QtCharts::QXYSeries* getSeries() const = 0;

    void internalSerialize(QJsonObject& json) const override;
    void deserialize(const QJsonObject& obj) override;

    double getXMinValue() const override;

    double getXMaxValue() const override;

    double getYMinValue() const override;

    double getYMaxValue() const override;

    void updateLabels() override;

    void syncColorsToModel() const override;
private slots:
    void rowCountUpdated();
public slots:
    void setIndependentX(bool x);
    void attachSeries(QtCharts::QXYSeries*& s) const;
};
}  // namespace Charts
}  // namespace BasedCharts