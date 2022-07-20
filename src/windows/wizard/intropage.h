#pragma once

#include <QWizardPage>

namespace BasedCharts {
namespace Windows {
namespace Wizard {
class IntroPage : public QWizardPage {
    Q_OBJECT
public:
    IntroPage(QWidget* parent);
    int nextId() const override;
};
}  // namespace Wizard
}  // namespace Windows
}  // namespace BasedCharts