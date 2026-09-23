cmake_minimum_required(VERSION 3.16)

project(QuantumEngine
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS 
    Widgets 
    OpenGL 
    OpenGLWidgets
)

qt_standard_project_setup()

qt_add_executable(QuantumEngine
    src/main.cpp
    src/MainWindow.cpp
    src/QT_GLRenderer.cpp
)

target_include_directories(QuantumEngine
    PRIVATE
        include
)

target_link_libraries(QuantumEngine
    PRIVATE
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
)