#
# SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>
#
# SPDX-License-Identifier: MIT
#

add_library(TwitchQt)
add_library(TwitchQt::TwitchQt ALIAS TwitchQt)

set_target_properties(TwitchQt
    PROPERTIES
        VERSION     ${TWITCHQT_VERSION}
        SOVERSION   ${TWITCHQT_SOVERSION}
        EXPORT_NAME TwitchQt
)

target_sources(TwitchQt
    PRIVATE
        badge.hpp
        badgereply.hpp badgereply.cpp
        channelreply.hpp channelreply.cpp
        emote.hpp
        emotereply.hpp emotereply.cpp
        game.hpp
        gamereply.hpp gamereply.cpp
        reply.hpp reply.cpp
        stream.hpp
        streamreply.hpp streamreply.cpp
        twitchqt.hpp twitchqt.cpp
        user.hpp
        userreply.hpp userreply.cpp
        video.hpp
        videoreply.hpp videoreply.cpp
)

ecm_generate_export_header(TwitchQt
    BASE_NAME TwitchQt
    VERSION 1.0.0
    USE_VERSION_HEADER
)

target_include_directories(TwitchQt
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/TwitchQt>")

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

install(TARGETS TwitchQt EXPORT TwitchQtTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/twitchqt_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/twitchqt_version.h
    ${TwitchQt_HEADERS}
    badge.hpp
    badgereply.hpp
    channelreply.hpp
    emote.hpp
    emotereply.hpp
    game.hpp
    gamereply.hpp
    reply.hpp
    stream.hpp
    streamreply.hpp
    twitchqt.hpp
    user.hpp
    userreply.hpp
    video.hpp
    videoreply.hpp
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/TwitchQt COMPONENT Devel
)

install(FILES
    "${PROJECT_BINARY_DIR}/TwitchQtConfig.cmake"
    "${PROJECT_BINARY_DIR}/TwitchQtConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT   TwitchQtTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE         TwitchQtTargets.cmake NAMESPACE TwitchQt::
)

