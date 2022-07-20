#include "detailspage.h"

#include "charts/barchart.h"
#include "charts/linechart.h"
#include "charts/scatterchart.h"
#include "charts/splinechart.h"
#include "chartwizard.h"

#include <QFormLayout>

using BasedCharts::Charts::AbstractChart;
using BasedCharts::Charts::BarChart;
using BasedCharts::Charts::LineChart;
using BasedCharts::Charts::ScatterChart;
using BasedCharts::Charts::SplineChart;
using BasedCharts::Models::ChartDataModel;
using BasedCharts::Windows::Wizard::DetailsPage;

DetailsPage::DetailsPage(QWidget* parent)
    : QWizardPage(parent),
      titleEdit(new QLineEdit),
      rowSpinbox(new QSpinBox),
      columnSpinbox(new QSpinBox) {
    setTitle("Chart Details");
    setSubTitle(
        "Please set the chart details. Don't worry, you can also edit those "
        "later.");
    auto layout = new QFormLayout;

    layout->addRow("Title", titleEdit);
    connect(titleEdit, &QLineEdit::textChanged, this,
            [this]() { chart->setTitle(titleEdit->text()); });

    rowSpinbox->setMinimum(1);
    rowSpinbox->setMaximum(std::numeric_limits<int>::max());
    connect(rowSpinbox, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [this](int value) { model->setRowCount(value); });
    layout->addRow("Row count", rowSpinbox);

    columnSpinbox->setMaximum(std::numeric_limits<int>::max());
    connect(columnSpinbox, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [this](int value) {
                if (chart->needsEvenColumns() && value % 2 == 1) {
                    columnSpinbox->setValue(value + 1);
                }
                model->setColumnCount(columnSpinbox->value());
            });
    layout->addRow("Column count", columnSpinbox);

    setLayout(layout);
}

int DetailsPage::nextId() const {
    return -1;
}
void DetailsPage::cleanupPage() {
    QWizardPage::cleanupPage();
    static_cast<ChartWizard*>(wizard())->clearResult();
    delete chart;
    delete model;
    chart = nullptr;
    model = nullptr;
}

AbstractChart* DetailsPage::getChart() const {
    if (field("chartType.bar").toBool()) {
        return new BarChart(wizard(), model);
    } else if (field("chartType.line").toBool()) {
        return new LineChart(wizard(), model);
    } else if (field("chartType.scatter").toBool()) {
        return new ScatterChart(wizard(), model);
    } else if (field("chartType.spline").toBool()) {
        return new SplineChart(wizard(), model);
    }
    qDebug() << "no chart type selected in wizard";
    return nullptr;
}

void DetailsPage::initializePage() {
    QWizardPage::initializePage();
    model = new ChartDataModel(this);
    chart = getChart();
    static_cast<ChartWizard*>(wizard())->setResult({model, chart, {}});

    titleEdit->setText(chart->getTitle());

    rowSpinbox->setValue(model->rowCount());

    columnSpinbox->setValue(model->columnCount());
    columnSpinbox->setMinimum(model->getMinimumColumnCount());
    columnSpinbox->setSingleStep(chart->needsEvenColumns() ? 2 : 1);
}
