cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(occt_part_positioner_6dof CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(OpenCASCADE REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core OpenGL)

add_executable(${PROJECT_NAME} main.cpp
    core/ShapeModel.hpp
    core/ShapeModel.cpp
    core/TransformController.hpp
    core/TransformController.cpp
    ui/OcctQOpenGLWidgetViewer.hpp
    ui/OcctQOpenGLWidgetViewer.cpp
    infrastructure/SceneManager.hpp
    infrastructure/SceneManager.cpp
    ui/occt-qt-tools/OcctGlTools.h
    ui/occt-qt-tools/OcctGlTools.cpp
    ui/occt-qt-tools/OcctQtTools.h
    ui/occt-qt-tools/OcctQtTools.cpp
    ui/MainWindow.cpp
    ui/MainWindow.hpp
    ui/MainWindow.ui
    core/ModelEntity.hpp
    infrastructure/StepImporter.hpp
    infrastructure/StepImporter.cpp)

set(OpenCASCADE_USED_LIBRARIES
    TKernel TKMath 
    TKG2d TKG3d TKGeomBase TKBRep TKGeomAlgo TKTopAlgo 
    TKPrim TKBO TKBool TKHLR TKFillet TKOffset TKFeat TKMesh 
    TKService TKV3d TKOpenGl TKDESTEP 
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCASCADE_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt5::Core
	Qt5::Widgets
	Qt5::OpenGL
	${OpenCASCADE_USED_LIBRARIES}
)

option(ENABLE_ASAN "Enable Address Sanitizer" OFF)
if(ENABLE_ASAN)
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -g -O1)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()


option(ENABLE_TSAN "Enable Thread Sanitizer" OFF)
if(ENABLE_TSAN)
    message(STATUS "Thread Sanitizer ENABLED")
    # Флаги компиляции и линковки для TSan
    target_compile_options(main PRIVATE -fsanitize=thread -g -O1 -fno-omit-frame-pointer)
    target_link_options(main PRIVATE -fsanitize=thread)
else()
    message(STATUS "Thread Sanitizer DISABLED")
endif()