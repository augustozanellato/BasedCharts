#include "openpage.h"

#include "charts/chartfactory.h"
#include "chartwizard.h"
#include "utils/exceptions.h"
#include "windows/mainwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

using BasedCharts::Charts::ChartFactory;
using BasedCharts::Utils::Exceptions::DeserializationException;
using BasedCharts::Windows::Wizard::OpenPage;

OpenPage::OpenPage(QWidget* parent) : QWizardPage(parent) {
    setTitle("Open Chart");
    setSubTitle("Please select the desired chart file");

    QVBoxLayout* layout = new QVBoxLayout(this);
    auto selectFileLayout = new QHBoxLayout;
    selectFileLayout->addWidget(new QLabel("Choose file"));

    auto filePath = new QLineEdit(this);
    filePath->setReadOnly(true);
    selectFileLayout->addWidget(filePath);
    auto browseBtn = new QPushButton("Browse", this);
    connect(browseBtn, &QPushButton::clicked, this, [filePath, this] {
        auto dialog = MainWindow::getOpenDialog(this);
        if (dialog->exec() == QDialog::Accepted) {
            QGuiApplication::setOverrideCursor(Qt::WaitCursor);
            auto selectedPath = dialog->selectedFiles().constFirst();
            filePath->setText(selectedPath);
            try {
                auto loadResult = ChartFactory::load(this, selectedPath);
                static_cast<ChartWizard*>(wizard())->setResult(
                    {std::get<0>(loadResult), std::get<1>(loadResult),
                     selectedPath});
            } catch (const DeserializationException& e) {
                e.showError(this);
            }
            QGuiApplication::restoreOverrideCursor();
            emit completeChanged();
        }
        dialog->deleteLater();
    });
    selectFileLayout->addWidget(browseBtn);

    layout->addLayout(selectFileLayout);

    setLayout(layout);
}
bool OpenPage::isComplete() const {
    return static_cast<ChartWizard*>(wizard())->hasValidResult();
}
void OpenPage::cleanupPage() {
    QWizardPage::cleanupPage();
    static_cast<ChartWizard*>(wizard())->clearResult();
}

int OpenPage::nextId() const {
    return -1;
}
