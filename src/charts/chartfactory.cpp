#include "chartfactory.h"
#include "utils/exceptions.h"

#include <QFile>
#include <QJsonDocument>
#include <utility>

using BasedCharts::Charts::AbstractChart;
using BasedCharts::Charts::ChartFactory;
using BasedCharts::Models::ChartDataModel;
using BasedCharts::Utils::Exceptions::DeserializationException;

ChartFactory* ChartFactory::factory = nullptr;

ChartFactory* ChartFactory::instance() {
    if (factory == nullptr) {
        factory = new ChartFactory();
    }
    return factory;
}

bool ChartFactory::internalRegisterClass(const QString& name,
                                         ChartFactory::factory_type type) {
    if (factoryTypes.contains(name)) {
        qDebug() << "class" << name << "already registered";
        return false;
    }
    factoryTypes[name] = std::move(type);
    return true;
}

bool ChartFactory::registerClass(const QString& name,
                                 ChartFactory::factory_type type) {
    return instance()->internalRegisterClass(name, std::move(type));
}

AbstractChart* ChartFactory::internalDeserialize(QWidget* parent,
                                                 const QJsonObject& json,
                                                 ChartDataModel* model) {
    if (!json["type"].isString()) {
        return nullptr;
    }
    auto type = json["type"].toString();
    AbstractChart* chart = nullptr;
    try {
        if (factoryTypes.contains(type)) {
            chart = factoryTypes[type](parent, model);
        } else {
            throw DeserializationException("Invalid chart type");
        }
        chart->deserialize(json);
    } catch (const DeserializationException& e) {
        if (chart != nullptr) {
            delete chart;
        }
        throw;
    }
    return chart;
}

AbstractChart* ChartFactory::deserialize(QWidget* parent,
                                         const QJsonObject& json,
                                         ChartDataModel* model) {
    return instance()->internalDeserialize(parent, json, model);
}

ChartFactory::load_result ChartFactory::load(QWidget* parent,
                                             const QString& filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        throw DeserializationException(QString("Cannot read file %1:\n%2.")
                                           .arg(filename, file.errorString()));
    }

    const auto jsonDoc = QJsonDocument::fromJson(file.readAll());
    const auto json = jsonDoc.object();
    if (json.isEmpty()) {
        throw DeserializationException("Empty chart file!");
    }
    ChartDataModel* model = nullptr;
    AbstractChart* chart;
    try {
        model = ChartDataModel::deserialize(parent, json);
        chart = deserialize(parent, json, model);
    } catch (const DeserializationException& e) {
        if (model != nullptr) {
            delete model;
        }
        throw;
    }
    return {model, chart};
}
