cmake_minimum_required(VERSION 3.16)

project(DemoA VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Core Gui Quick Quick3D REQUIRED)
find_package(Qt6 6.2 COMPONENTS ShaderTools REQUIRED)
qt_add_executable(appDemoA
    main.cpp
    dynamicgeometry.h dynamicgeometry.cpp
)

qt_add_qml_module(appDemoA
    URI DemoA
    VERSION 1.0
    QML_FILES main.qml
    SOURCES dynamicgeometry.h dynamicgeometry.cpp
    RESOURCES shaders/pencil.frag
    QML_FILES PencilMaterial.qml
    QML_FILES DynamicModel.qml
)

qt6_add_resources(appDemoA "appDemoA"
    PREFIX
        "/"
    FILES
        "main.qml"
)
#qt6_add_shaders(appDemoA "appDemoA_shader"
#    PREFIX
#        "/shaders"
#    FILES
#        shaders/tree.vsh
#        shaders/tree.fsh
#)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appDemoA PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDemoA
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDemoA PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Quick3D
)
include(GNUInstallDirs)
install(TARGETS appDemoA
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
