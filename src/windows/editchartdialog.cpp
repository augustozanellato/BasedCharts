#include "editchartdialog.h"
#include "charts/xychart.h"
#include "mainwindow.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>

using BasedCharts::Charts::AbstractChart;
using BasedCharts::Charts::AxisType;
using BasedCharts::Charts::XYChart;
using BasedCharts::Models::ChartDataModel;
using BasedCharts::Windows::EditChartDialog;
using BasedCharts::Windows::MainWindow;

EditChartDialog::EditChartDialog(QWidget* parent,
                                 ChartDataModel* model,
                                 AbstractChart* chart)
    : QDialog(parent), model(model), layout(new QGridLayout), chart(chart) {
    auto editChartTitleLayout = new QHBoxLayout;
    editChartTitleLayout->addWidget(new QLabel("Title", this));

    auto editChartTitle = new QLineEdit(chart->getTitle(), this);
    editChartTitleLayout->addWidget(editChartTitle);
    layout->addLayout(editChartTitleLayout, 0, 0, 1, 4);
    if (chart->allowRowHeaderEdit) {
        layout->addWidget(new QLabel("Rows", this), 1, 0);
        createHeaderEdit(2, 0, true);
    }
    if (chart->allowColumnHeaderEdit) {
        layout->addWidget(new QLabel("Columns", this), 1, 2);
        createHeaderEdit(2, 2, false);
    }

    if (chart->allowXEdit) {
        layout->addWidget(new QLabel("X Axis", this), 4, 0);
        createAxisEdit(5, 0, true);
    }
    if (chart->allowYEdit) {
        layout->addWidget(new QLabel("Y Axis", this), 4, 2);
        createAxisEdit(5, 2, false);
    }

    auto close = new QPushButton("Close", this);
    close->setAutoDefault(false);
    connect(close, &QPushButton::clicked, this, [chart, editChartTitle]() {
        chart->setTitle(editChartTitle->text());
    });
    connect(close, &QPushButton::clicked, this, &EditChartDialog::accept);
    layout->addWidget(close, 6, 3, Qt::AlignmentFlag::AlignRight);

    setLayout(layout);
}

QStringList getHeaders(ChartDataModel* model,
                       AbstractChart* chart,
                       bool isRow) {
    if (isRow) {
        return model->getRowHeaders();
    } else {
        QStringList headers;
        for (int i = 0; i < chart->seriesCount(); i++) {
            headers.append(
                model->headerData(chart->getColumnForSeries(i), Qt::Horizontal)
                    .toString());
        }
        return headers;
    }
}

/// Result occupies 2x2 starting from (layoutRow, layoutColumn)
/// \param layoutRow
/// \param layoutColumn
/// \param isRowHeader indicates if the created widget should edit rows or
/// columns
void EditChartDialog::createHeaderEdit(int layoutRow,
                                       int layoutColumn,
                                       bool isRowHeader) {
    auto listModel =
        new QStringListModel(getHeaders(model, chart, isRowHeader), this);
    auto listView = new QListView(this);
    listView->setModel(listModel);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    layout->addWidget(listView, layoutRow, layoutColumn, 1, 2);

    connect(listModel, &QAbstractItemModel::dataChanged, this,
            [this, listModel, isRowHeader](const QModelIndex& top,
                                           const QModelIndex& bottom) {
                for (int i = top.row(); i <= bottom.row(); i++) {
                    if (isRowHeader) {
                        model->setHeaderData(i, Qt::Vertical,
                                             listModel->index(i).data());
                    } else {
                        model->setHeaderData(chart->getColumnForSeries(i),
                                             Qt::Horizontal,
                                             listModel->index(i).data());
                    }
                }
            });
    auto editListButtonsLayout = new QHBoxLayout;
    auto addButton = new QPushButton(
        QIcon(MainWindow::themeFileSelector.select(":/icons/plus-circle.svg")),
        "", this);
    addButton->setAutoDefault(false);
    connect(addButton, &QPushButton::clicked, this,
            [this, listModel, isRowHeader]() {
                if (isRowHeader) {
                    model->insertRow(model->rowCount());
                } else {
                    model->insertColumns(model->columnCount(),
                                         chart->needsEvenColumns() ? 2 : 1);
                }
                listModel->setStringList(getHeaders(model, chart, isRowHeader));
            });
    editListButtonsLayout->addWidget(addButton);
    auto removeButton = new QPushButton(
        QIcon(MainWindow::themeFileSelector.select(":/icons/minus-circle.svg")),
        "", this);
    removeButton->setAutoDefault(false);
    connect(removeButton, &QPushButton::clicked, this,
            [listView, this, listModel, isRowHeader]() {
                int index =
                    (isRowHeader ? model->rowCount() : model->columnCount()) -
                    1;
                if (listView->selectionModel()->hasSelection()) {
                    index = listView->currentIndex().row();
                }

                if (isRowHeader) {
                    model->removeRow(index);
                } else {
                    if (chart->needsEvenColumns()) {
                        if (index % 2 == 1) {
                            index--;
                        }
                        model->removeColumns(index, 2);
                    } else {
                        model->removeColumn(index);
                    }
                }
                listModel->setStringList(getHeaders(model, chart, isRowHeader));
            });
    editListButtonsLayout->addWidget(removeButton);
    layout->addLayout(editListButtonsLayout, layoutRow + 1, layoutColumn, 1, 2);
}

/// Result occupies 1x2 starting from (layoutRow, layoutColumn)
void EditChartDialog::createAxisEdit(int layoutRow,
                                     int layoutColumn,
                                     bool isXAxis) {
    auto formLayout = new QFormLayout;

    auto axisLabelEdit = new QLineEdit;
    axisLabelEdit->setText(isXAxis ? chart->getXLabel() : chart->getYLabel());
    connect(axisLabelEdit, &QLineEdit::textChanged, this,
            [this, isXAxis](const QString& value) {
                if (isXAxis) {
                    chart->setXLabel(value);
                } else {
                    chart->setYLabel(value);
                }
            });
    formLayout->addRow("Label", axisLabelEdit);
    auto axisTypeSelect = new QComboBox(this);
    axisTypeSelect->addItem("Nice", static_cast<int>(AxisType::NICE));
    axisTypeSelect->addItem("Auto", static_cast<int>(AxisType::AUTO));
    axisTypeSelect->addItem("Manual", static_cast<int>(AxisType::MANUAL));
    axisTypeSelect->setCurrentIndex(axisTypeSelect->findData(
        static_cast<int>(isXAxis ? chart->getXType() : chart->getYType())));
    connect(axisTypeSelect, QOverload<int>::of(&QComboBox::activated), this,
            [this, isXAxis, axisTypeSelect](int index) {
                auto type = static_cast<AxisType>(
                    axisTypeSelect->itemData(index).toInt());
                if (isXAxis) {
                    chart->setXType(type);
                } else {
                    chart->setYType(type);
                }
            });
    formLayout->addRow("Type", axisTypeSelect);

    auto axisTicks = new QSpinBox(this);
    axisTicks->setMinimum(AbstractChart::MIN_TICKS);
    axisTicks->setMaximum(AbstractChart::MAX_TICKS);
    axisTicks->setValue(isXAxis ? chart->getXTicks() : chart->getYTicks());
    connect(axisTicks, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [this, isXAxis](int newTicks) {
                if (isXAxis) {
                    chart->setXTicks(newTicks);
                } else {
                    chart->setYTicks(newTicks);
                }
            });
    formLayout->addRow("Ticks", axisTicks);

    auto axisMinorTicks = new QSpinBox(this);
    axisMinorTicks->setMinimum(AbstractChart::MIN_MINOR_TICKS);
    axisMinorTicks->setMaximum(AbstractChart::MAX_MINOR_TICKS);
    axisMinorTicks->setValue(isXAxis ? chart->getXMinorTicks()
                                     : chart->getYMinorTicks());
    connect(axisMinorTicks, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [this, isXAxis](int newTicks) {
                if (isXAxis) {
                    chart->setXMinorTicks(newTicks);
                } else {
                    chart->setYMinorTicks(newTicks);
                }
            });
    formLayout->addRow("Minor ticks", axisMinorTicks);

    auto axisManualMin = new QDoubleSpinBox(this);
    axisManualMin->setRange(std::numeric_limits<double>::min(),
                            std::numeric_limits<double>::max());
    axisManualMin->setValue(isXAxis ? chart->getManualMinX()
                                    : chart->getManualMinY());
    connect(axisManualMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [isXAxis, this](double value) {
                if (isXAxis) {
                    chart->setManualMinX(value);
                } else {
                    chart->setManualMinY(value);
                }
            });
    formLayout->addRow("Min", axisManualMin);
    auto axisManualMax = new QDoubleSpinBox(this);
    axisManualMax->setRange(std::numeric_limits<double>::min(),
                            std::numeric_limits<double>::max());
    axisManualMax->setValue(isXAxis ? chart->getManualMaxX()
                                    : chart->getManualMaxY());
    connect(axisManualMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [isXAxis, this](double value) {
                if (isXAxis) {
                    chart->setManualMaxX(value);
                } else {
                    chart->setManualMaxY(value);
                }
            });
    formLayout->addRow("Max", axisManualMax);
    auto updateMinMax = [this, isXAxis, axisManualMin, axisManualMax]() {
        auto min = isXAxis ? chart->getManualMinX() : chart->getManualMinY();
        auto max = isXAxis ? chart->getManualMaxX() : chart->getManualMaxY();
        auto isManualAxis = (isXAxis ? chart->getXType() : chart->getYType()) ==
                            AxisType::MANUAL;

        axisManualMin->setValue(min);
        axisManualMax->setValue(max);
        axisManualMin->setEnabled(isManualAxis);
        axisManualMax->setEnabled(isManualAxis);
    };
    connect(axisTypeSelect, QOverload<int>::of(&QComboBox::activated),
            updateMinMax);
    updateMinMax();

    if (isXAxis && dynamic_cast<XYChart*>(chart) != nullptr) {
        auto xyChart = static_cast<XYChart*>(chart);
        auto independentXCheckbox = new QCheckBox(this);
        independentXCheckbox->setChecked(xyChart->hasIndependentX());
        connect(independentXCheckbox, &QAbstractButton::toggled, xyChart,
                &XYChart::setIndependentX);
        formLayout->addRow("Independent X", independentXCheckbox);
    }

    layout->addLayout(formLayout, layoutRow, layoutColumn, 1, 2);
}