#pragma once
#include <QCoreApplication>
#include <QMessageBox>
#include <QString>

namespace BasedCharts {
namespace Utils {
namespace Exceptions {

class DeserializationException {
    const QString error;

public:
    DeserializationException(const QString& error) : error(error) {}
    DeserializationException(const QString&& error) : error(std::move(error)) {}
    const QString& what() const { return error; }
    void showError(QWidget* parent) const {
        QMessageBox::critical(parent, QCoreApplication::applicationName(),
                              error);
    }
};
}  // namespace Exceptions
}  // namespace Utils
}  // namespace BasedCharts
