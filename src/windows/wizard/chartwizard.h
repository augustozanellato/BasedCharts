#pragma once

#include <charts/abstractchart.h>
#include <models/chartdatamodel.h>

#include <QWizard>

namespace BasedCharts {
namespace Windows {
namespace Wizard {
class ChartWizard : public QWizard {
public:
    using result_type =
        std::tuple<Models::ChartDataModel*, Charts::AbstractChart*, QString>;

private:
    Q_OBJECT
    const bool exitOnCancel;
    result_type result{nullptr, nullptr, {}};

    void clearResult();
    void setResult(const result_type& newResult);

protected:
    void closeEvent(QCloseEvent* event) override;

public:
    enum {
        PAGE_INTRO,
        PAGE_OPEN,
        PAGE_NEW,
        PAGE_CHART_DETAILS,
    };
    ChartWizard(QWidget* parent, bool exitOnCancel);

    bool hasValidResult() const;
    result_type getResult() const;

    friend class OpenPage;
    friend class DetailsPage;
};
}  // namespace Wizard
}  // namespace Windows
}  // namespace BasedCharts