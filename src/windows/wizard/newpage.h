#pragma once

#include <QGridLayout>
#include <QWizardPage>

namespace BasedCharts {
namespace Windows {
namespace Wizard {
class NewPage : public QWizardPage {
    Q_OBJECT

    QGridLayout* gridLayout;

    void createChartRadio(const QString& name,
                          const QString& fieldName,
                          const QString& imagePath,
                          int layoutX,
                          int layoutY,
                          bool checked = false);

public:
    NewPage(QWidget* parent);
};
}  // namespace Wizard
}  // namespace Windows
}  // namespace BasedCharts