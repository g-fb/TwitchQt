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

find_package(ECM 5.66 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(FeatureSummary)
include(GenerateExportHeader)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.hpp *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

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
