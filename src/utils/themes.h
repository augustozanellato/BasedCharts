#pragma once

#include <QString>

namespace BasedCharts {
namespace Utils {
class Themes {
public:
    const static QString DARK;
    const static QString LIGHT;
};

class ThemePaths {
public:
    const static QString DARK;
    const static QString LIGHT;
};

extern const QString& getThemePath(const QString& theme);
}  // namespace Utils
}  // namespace BasedCharts