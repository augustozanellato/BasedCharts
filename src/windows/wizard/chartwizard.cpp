#include "chartwizard.h"

#include "detailspage.h"
#include "intropage.h"
#include "newpage.h"
#include "openpage.h"

#include <QApplication>
#include <QCloseEvent>

using BasedCharts::Windows::Wizard::ChartWizard;
using BasedCharts::Windows::Wizard::IntroPage;
using BasedCharts::Windows::Wizard::NewPage;
using BasedCharts::Windows::Wizard::OpenPage;

ChartWizard::ChartWizard(QWidget* parent, bool exitOnCancel)
    : QWizard(parent), exitOnCancel(exitOnCancel) {
    setPage(PAGE_INTRO, new IntroPage(this));
    setPage(PAGE_NEW, new NewPage(this));
    setPage(PAGE_OPEN, new OpenPage(this));
    setPage(PAGE_CHART_DETAILS, new DetailsPage(this));
    setStartId(PAGE_INTRO);
    setWizardStyle(WizardStyle::ModernStyle);
    if (exitOnCancel) {
        setOption(QWizard::NoCancelButton);
    }
    setWindowTitle("Chart Wizard");
}
void ChartWizard::closeEvent(QCloseEvent* event) {
    if (exitOnCancel) {
        qApp->quit();
    }
    event->accept();
}

void ChartWizard::clearResult() {
    // There's no need to manually delete the results, they'll get deleted when
    // the wizard gets disposed
    result = {nullptr, nullptr, {}};
}
bool ChartWizard::hasValidResult() const {
    return std::get<0>(result) != nullptr && std::get<1>(result) != nullptr;
}
void ChartWizard::setResult(const result_type& newResult) {
    result = newResult;
}
ChartWizard::result_type ChartWizard::getResult() const {
    return result;
}
