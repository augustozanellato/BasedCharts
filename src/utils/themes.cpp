#include "themes.h"
#include <QDebug>

using BasedCharts::Utils::ThemePaths;
using BasedCharts::Utils::Themes;

const QString Themes::DARK = "dark";
const QString Themes::LIGHT = "light";

const QString ThemePaths::DARK = ":themes/dark/style.qss";
const QString ThemePaths::LIGHT = ":themes/light/style.qss";

const QString& BasedCharts::Utils::getThemePath(const QString& theme) {
    if (theme == Themes::DARK) {
        return ThemePaths::DARK;
    } else if (theme == Themes::LIGHT) {
        return ThemePaths::LIGHT;
    } else {
        qDebug() << "Invalid theme" << theme << "falling back to light";
        return ThemePaths::LIGHT;
    }
}