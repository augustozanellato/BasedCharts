#pragma once
#include "abstractchart.h"
#include "models/chartdatamodel.h"

#include <QJsonObject>
#include <QMap>
#include <QString>
#include <QWidget>
#include <functional>

namespace BasedCharts {
namespace Charts {
class ChartFactory {
public:
    using factory_type =
        std::function<AbstractChart*(QWidget* parent,
                                     Models::ChartDataModel* model)>;
    using load_result = std::tuple<Models::ChartDataModel*, AbstractChart*>;

private:
    QMap<QString, factory_type> factoryTypes;
    static ChartFactory* factory;
    ChartFactory() = default;
    ~ChartFactory() = default;
    static ChartFactory* instance();
    bool internalRegisterClass(const QString& name, factory_type type);
    AbstractChart* internalDeserialize(QWidget* parent,
                                       const QJsonObject& json,
                                       Models::ChartDataModel* model);

public:
    ChartFactory(const ChartFactory&) = delete;
    ChartFactory& operator=(const ChartFactory&) = delete;

    static bool registerClass(const QString& name, factory_type type);

    static AbstractChart* deserialize(QWidget* parent,
                                      const QJsonObject& json,
                                      Models::ChartDataModel* model);

    static load_result load(QWidget* parent, const QString& filename);
};

}  // namespace Charts
}  // namespace BasedCharts