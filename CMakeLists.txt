cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(occt_part_positioner_6dof CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME} main.cpp
    core/ShapeModel.hpp
    core/ShapeModel.cpp
    core/TransformController.hpp
    core/TransformController.cpp
    ui/OcctQOpenGLWidgetViewer.hpp
    ui/OcctQOpenGLWidgetViewer.cpp
    infrastructure/OcctViewerManager.hpp
    infrastructure/OcctViewerManager.cpp
    ui/occt-qt-tools/OcctGlTools.h
    ui/occt-qt-tools/OcctGlTools.cpp
    ui/occt-qt-tools/OcctQtTools.h
    ui/occt-qt-tools/OcctQtTools.cpp
    ui/MainWindow.cpp
    ui/MainWindow.hpp
    ui/MainWindow.ui)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
option(ENABLE_TSAN "Enable Thread Sanitizer" OFF)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core OpenGL)
# ==========================================
# РУЧНАЯ НАСТРОЙКА OpenCASCADE (для Ubuntu)
# ==========================================
# 1. Указываем путь к заголовочным файлам (.hxx)
include_directories(/usr/include/opencascade)

# 2. Список библиотек OCCT. 
# CMake сам найдёт их в /usr/lib/x86_64-linux-gnu/, так как это системная папка.
# Добавь сюда TKSTEP, TKXCAF и т.д., если твой проект работает с импортом/экспортом файлов
set(OpenCASCADE_LIBRARIES
    TKernel TKMath 
    TKG2d TKG3d TKGeomBase TKBRep TKGeomAlgo TKTopAlgo 
    TKPrim TKBO TKBool TKHLR TKFillet TKOffset TKFeat TKMesh 
    TKService TKV3d TKOpenGl
)
# ==========================================
#find_package(OpenCASCADE REQUIRED COMPONENTS ModelingAlgorithms ModelingData FoundationClasses)
#find_package(OpenCASCADE REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt5::Core
	Qt5::Widgets
	Qt5::OpenGL
	${OpenCASCADE_LIBRARIES}
)

if(ENABLE_TSAN)
    message(STATUS "Thread Sanitizer ENABLED")
    # Флаги компиляции и линковки для TSan
    target_compile_options(main PRIVATE -fsanitize=thread -g -O1 -fno-omit-frame-pointer)
    target_link_options(main PRIVATE -fsanitize=thread)
else()
    message(STATUS "Thread Sanitizer DISABLED")
endif()