set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_COMPILER clazy)
find_package(Qt5 COMPONENTS
        Core
        Widgets
        Charts
        REQUIRED
)

add_library(${PROJECT_NAME}_assets
        ../assets/themes/dark/dark.qrc
        ../assets/themes/light/light.qrc
        ../assets/basedcharts.qrc
)

add_library(${PROJECT_NAME}_lib
        charts/abstractchart.h
        charts/abstractchart.cpp
        charts/barchart.h
        charts/barchart.cpp
        charts/chartfactory.h
        charts/chartfactory.cpp
        charts/linechart.h
        charts/linechart.cpp
        charts/scatterchart.h
        charts/scatterchart.cpp
        charts/splinechart.h
        charts/splinechart.cpp
        charts/xychart.h
        charts/xychart.cpp
        models/chartdatamodel.h
        models/chartdatamodel.cpp
        utils/exceptions.h
        utils/themeableaction.h
        utils/themeableaction.cpp
        utils/themes.h
        utils/themes.cpp
        utils/vector2d.h
        views/mainview.h
        views/mainview.cpp
        windows/editchartdialog.h
        windows/editchartdialog.cpp
        windows/editcolorsdialog.h
        windows/editcolorsdialog.cpp
        windows/mainwindow.h
        windows/mainwindow.cpp
        windows/wizard/chartwizard.h
        windows/wizard/chartwizard.cpp
        windows/wizard/detailspage.h
        windows/wizard/detailspage.cpp
        windows/wizard/intropage.h
        windows/wizard/intropage.cpp
        windows/wizard/newpage.h
        windows/wizard/newpage.cpp
        windows/wizard/openpage.h
        windows/wizard/openpage.cpp
)
target_link_libraries(${PROJECT_NAME}_lib
        Qt5::Core
        Qt5::Widgets
        Qt5::Charts
)

add_executable(${PROJECT_NAME}
        main.cpp
)
target_link_libraries(${PROJECT_NAME}
        ${PROJECT_NAME}_lib
        ${PROJECT_NAME}_assets
        Qt5::Core
        Qt5::Widgets
        Qt5::Charts
)