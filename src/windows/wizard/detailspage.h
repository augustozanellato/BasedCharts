#pragma once

#include "charts/abstractchart.h"
#include "models/chartdatamodel.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QWizardPage>

namespace BasedCharts {
namespace Windows {
namespace Wizard {
class DetailsPage : public QWizardPage {
    Q_OBJECT
    Models::ChartDataModel* model;
    Charts::AbstractChart* chart;
    QLineEdit* titleEdit;
    QSpinBox* rowSpinbox;
    QSpinBox* columnSpinbox;

public:
    DetailsPage(QWidget* parent);
    int nextId() const override;
    void cleanupPage() override;
    void initializePage() override;
    Charts::AbstractChart* getChart() const;
};
}  // namespace Wizard
}  // namespace Windows
}  // namespace BasedCharts