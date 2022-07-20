#include <QtTest>
// NOLINTNEXTLINE
#define private public
// NOLINTNEXTLINE
#define protected public

#include "models/chartdatamodel.h"
#include "utils/exceptions.h"

#undef private
#undef protected

using BasedCharts::Models::ChartDataModel;
using BasedCharts::Utils::Exceptions::DeserializationException;

class ModelSerializationTest : public QObject {
    Q_OBJECT
public:
    virtual ~ModelSerializationTest() {}

private slots:
    __attribute__((unused)) void testSerialization() {
        ChartDataModel model(nullptr, 3, 4);
        QJsonObject json;
        model.serialize(json);
    }
    __attribute__((unused)) void testDeserializationEmptyJson() {
        QJsonObject json;
        QVERIFY_EXCEPTION_THROWN(ChartDataModel::deserialize(nullptr, json),
                                 DeserializationException);
    }
};

#include "model_serialization_test.moc"

QTEST_MAIN(ModelSerializationTest)