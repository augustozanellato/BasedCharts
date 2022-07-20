#include "newpage.h"
#include "windows/mainwindow.h"

#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QRadioButton>

using BasedCharts::Windows::MainWindow;
using BasedCharts::Windows::Wizard::NewPage;

NewPage::NewPage(QWidget* parent)
    : QWizardPage(parent), gridLayout(new QGridLayout) {
    setTitle("New Chart");
    setSubTitle("Please select the chart type you want to create.");
    createChartRadio("Bar", "chartType.bar", ":/icons/chart-type-bar.png", 0, 0,
                     true);
    createChartRadio("Scatter", "chartType.scatter",
                     ":/icons/chart-type-scatter.png", 1, 0);
    createChartRadio("Line", "chartType.line", ":/icons/chart-type-line.png", 0,
                     1);
    createChartRadio("Spline", "chartType.spline",
                     ":/icons/chart-type-spline.png", 1, 1);
    setLayout(gridLayout);
}
void NewPage::createChartRadio(const QString& name,
                               const QString& fieldName,
                               const QString& imagePath,
                               int layoutX,
                               int layoutY,
                               bool checked) {
    auto radioLayout = new QVBoxLayout;
    QPixmap image(MainWindow::themeFileSelector.select(imagePath));
    auto imageLabel = new QLabel;
    imageLabel->setScaledContents(true);
    imageLabel->setPixmap(image);
    imageLabel->setMaximumSize(80, 80);
    radioLayout->addWidget(imageLabel);
    auto radioBtn = new QRadioButton(name);
    radioBtn->setChecked(checked);

    registerField(fieldName, radioBtn);
    radioLayout->addWidget(radioBtn, 0, Qt::AlignmentFlag::AlignCenter);
    gridLayout->addLayout(radioLayout, layoutX, layoutY,
                          Qt::AlignmentFlag::AlignCenter);
}
