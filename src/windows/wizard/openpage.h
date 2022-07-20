#pragma once

#include "charts/abstractchart.h"
#include "models/chartdatamodel.h"

#include <QWizardPage>
#include <tuple>

namespace BasedCharts {
namespace Windows {
namespace Wizard {
class OpenPage : public QWizardPage {
    Q_OBJECT
public:
    OpenPage(QWidget* parent);
    int nextId() const override;
    bool isComplete() const override;
    void cleanupPage() override;
};
}  // namespace Wizard
}  // namespace Windows
}  // namespace BasedCharts