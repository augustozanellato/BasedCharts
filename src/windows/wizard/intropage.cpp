#include "intropage.h"
#include "chartwizard.h"

#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QVariant>

using BasedCharts::Windows::Wizard::ChartWizard;
using BasedCharts::Windows::Wizard::IntroPage;

IntroPage::IntroPage(QWidget* parent) : QWizardPage(parent) {
    setTitle("Introduction");

    auto topLabel = new QLabel(
        "This wizard will help you create a new chart; "
        "or load a chart you already made.",
        this);
    topLabel->setWordWrap(true);

    auto newRadioBtn = new QRadioButton("&Create a new chart", this);
    registerField("create", newRadioBtn);
    newRadioBtn->setChecked(true);

    auto openRadioBtn = new QRadioButton("&Open an existing chart", this);
    registerField("open", openRadioBtn);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(topLabel);
    layout->addWidget(newRadioBtn);
    layout->addWidget(openRadioBtn);
    setLayout(layout);
}
int IntroPage::nextId() const {
    return field("open").toBool() ? ChartWizard::PAGE_OPEN
                                  : ChartWizard::PAGE_NEW;
}
