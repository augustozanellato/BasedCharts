#include "editcolorsdialog.h"

#include <QColorDialog>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>

using BasedCharts::Charts::AbstractChart;
using BasedCharts::Models::ChartDataModel;
using BasedCharts::Windows::EditColorsDialog;
EditColorsDialog::EditColorsDialog(QWidget* parent,
                                   ChartDataModel* model,
                                   AbstractChart* chart)
    : QDialog(parent),
      model(model),
      layout(new QVBoxLayout(this)),
      chart(chart) {
    layout->addWidget(new QLabel("Series colors", this),
                      Qt::AlignmentFlag::AlignLeft);
    auto colorList = new QListWidget(this);

    for (int i = 0; i < chart->seriesCount(); i++) {
        auto item = new QListWidgetItem(colorList, i);
        item->setBackground(chart->getColorForSeries(i));
        item->setText(
            model->headerData(chart->getColumnForSeries(i), Qt::Horizontal)
                .toString());
        colorList->addItem(item);
    }

    connect(colorList, &QListWidget::itemClicked, this,
            [this, chart, model](QListWidgetItem* item) {
                int series = item->type();
                auto color = QColorDialog::getColor(
                    chart->getColorForSeries(series), this, "Pick a color");
                if (color.isValid()) {
                    model->setColumnColor(chart->getColumnForSeries(series),
                                          color);
                    chart->setColorForSeries(series, color);
                    item->setBackground(color);
                }
            });

    layout->addWidget(colorList);

    auto close = new QPushButton("Close", this);
    close->setAutoDefault(false);
    connect(close, &QPushButton::clicked, this, &EditColorsDialog::accept);
    layout->addWidget(close, 0, Qt::AlignmentFlag::AlignRight);

    setLayout(layout);
}
