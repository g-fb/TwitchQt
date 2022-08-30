
add_library(TwitchQt)
add_library(TwitchQt::TwitchQt ALIAS TwitchQt)

target_sources(TwitchQt
    PRIVATE
        json.hpp
        twitchapi.cpp
        twitchbadgereply.cpp
        twitchemote.cpp
        twitchemotereply.cpp
        twitchgame.hpp
        twitchgamereply.cpp
        twitchreply.cpp
        twitchstreamreply.cpp
        twitchuserreply.cpp
        twitchvideoreply.cpp
)

generate_export_header(TwitchQt)

target_include_directories(TwitchQt
    INTERFACE
        $<BUILD_INTERFACE:${TwitchQt_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/src>)

target_link_libraries(TwitchQt
    PRIVATE
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Gui
        Qt${QT_MAJOR_VERSION}::Network
)
