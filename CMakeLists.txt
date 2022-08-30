#
# SPDX-FileCopyrightText: 2021 George Florea Bănuș <georgefb899@gmail.com>
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.14)

project(TwitchQt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
include(FeatureSummary)
include(GenerateExportHeader)

find_package(QT NAMES Qt6 Qt5 COMPONENTS
    Core
    Gui
    Network
)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core
    Gui
    Network
)
set_package_properties(Qt${QT_VERSION_MAJOR}Core PROPERTIES TYPE REQUIRED)
set_package_properties(Qt${QT_VERSION_MAJOR}Gui PROPERTIES TYPE REQUIRED)
set_package_properties(Qt{QT_VERSION_MAJOR}Network PROPERTIES TYPE REQUIRED)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

add_subdirectory(src)
