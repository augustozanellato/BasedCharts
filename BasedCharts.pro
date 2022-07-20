TEMPLATE = app
TARGET = BasedCharts
INCLUDEPATH += src/
QT += charts
OBJECTS_DIR = build/qmake
RCC_DIR = build/qmake/rcc
MOC_DIR = build/qmake/moc
QMAKE_CFLAGS += -Wall -Wno-unknown-pragmas
QMAKE_CXXFLAGS += -Wall -Wno-unknown-pragmas
QMAKE_CXXFLAGS_RELEASE += -DQT_RELEASE -DNDEBUG
HEADERS += src/charts/abstractchart.h \
           src/charts/barchart.h \
           src/charts/chartfactory.h \
           src/charts/linechart.h \
           src/charts/scatterchart.h \
           src/charts/splinechart.h \
           src/charts/xychart.h \
           src/models/chartdatamodel.h \
           src/utils/exceptions.h \
           src/utils/themeableaction.h \
           src/utils/themes.h \
           src/utils/vector2d.h \
           src/views/mainview.h \
           src/windows/editchartdialog.h \
           src/windows/editcolorsdialog.h \
           src/windows/mainwindow.h \
           src/windows/wizard/chartwizard.h \
           src/windows/wizard/detailspage.h \
           src/windows/wizard/intropage.h \
           src/windows/wizard/newpage.h \
           src/windows/wizard/openpage.h
SOURCES += src/main.cpp \
           src/charts/abstractchart.cpp \
           src/charts/barchart.cpp \
           src/charts/chartfactory.cpp \
           src/charts/linechart.cpp \
           src/charts/scatterchart.cpp \
           src/charts/splinechart.cpp \
           src/charts/xychart.cpp \
           src/models/chartdatamodel.cpp \
           src/utils/themeableaction.cpp \
           src/utils/themes.cpp \
           src/views/mainview.cpp \
           src/windows/editchartdialog.cpp \
           src/windows/editcolorsdialog.cpp \
           src/windows/mainwindow.cpp \
           src/windows/wizard/chartwizard.cpp \
           src/windows/wizard/detailspage.cpp \
           src/windows/wizard/intropage.cpp \
           src/windows/wizard/newpage.cpp \
           src/windows/wizard/openpage.cpp
RESOURCES += assets/basedcharts.qrc \
             assets/themes/dark/dark.qrc \
             assets/themes/light/light.qrc
