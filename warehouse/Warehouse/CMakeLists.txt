cmake_minimum_required(VERSION 3.10)
project(SmartWarehouseSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    src/main.cpp
    src/models/Product.cpp
    src/models/User.cpp
    src/models/Admin.cpp
    src/models/Staff.cpp
    src/core/FileManager.cpp
    src/core/Warehouse.cpp
    src/gui/LoginWindow.cpp
    src/gui/DashboardWindow.cpp
    src/gui/LoginWindow.h
    src/gui/DashboardWindow.h
)

add_executable(WarehouseApp ${SOURCES})

target_link_libraries(WarehouseApp Qt5::Widgets)
