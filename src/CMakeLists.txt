#
# SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>
#
# SPDX-License-Identifier: MIT
#

add_library(TwitchQt)
add_library(TwitchQt::TwitchQt ALIAS TwitchQt)

target_sources(TwitchQt
    PRIVATE
        channelreply.hpp channelreply.cpp
        badgereply.hpp badgereply.cpp
        emotereply.hpp emotereply.cpp
        game.hpp game.hpp
        gamereply.hpp gamereply.cpp
        reply.hpp reply.cpp
        streamreply.hpp streamreply.cpp
        twitchqt.hpp twitchqt.cpp
        userreply.hpp userreply.cpp
        videoreply.hpp videoreply.cpp
)

generate_export_header(TwitchQt)

target_include_directories(TwitchQt
    INTERFACE
        $<BUILD_INTERFACE:${TwitchQt_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/src>)

target_link_libraries(TwitchQt
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Network
)

ecm_generate_headers(TwitchQt_HEADERS
    HEADER_EXTENSION hpp
    HEADER_NAMES
        TwitchQt
    REQUIRED_HEADERS TwitchQt_HEADERS
)

install(TARGETS TwitchQt ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/twitchqt_export.h
        ${TwitchQt_HEADERS}
    DESTINATION
        ${KDE_INSTALL_LIBDIR}/TwitchQt COMPONENT Devel
)
