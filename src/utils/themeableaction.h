#pragma once

#include <QAction>
#include <QFileSelector>
#include <QObject>
#include <QString>

namespace BasedCharts {
namespace Utils {

class ThemeableAction : public QAction {
    Q_OBJECT
public:
    template <typename Func>
    static ThemeableAction* create(
        const typename QtPrivate::FunctionPointer<Func>::Object* sender,
        Func signal,
        const QString& iconPath,
        const QString& text,
        QObject* parent = nullptr) {
        ThemeableAction* act = new ThemeableAction(iconPath, text, parent);
        act->connect(sender, signal, act, &ThemeableAction::themeUpdated);
        return act;
    }

private slots:

    void themeUpdated();

private:
    ThemeableAction(const QString& iconPath,
                    const QString& text,
                    QObject* parent = nullptr);

    const QString iconPath;
};

}  // namespace Utils
}  // namespace BasedCharts