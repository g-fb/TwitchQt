cmake_minimum_required(VERSION 3.14)

project(TwitchQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(TwitchQt INTERFACE)
add_library(TwitchQt::TwitchQt ALIAS TwitchQt)

target_sources(TwitchQt
    INTERFACE
        src/json.hpp
        src/twitchapi.cpp src/twitchapi.hpp
        src/twitchbadge.hpp
        src/twitchbadgereply.cpp src/twitchbadgereply.hpp
        src/twitchemote.cpp src/twitchemote.hpp
        src/twitchemotereply.cpp src/twitchemotereply.hpp
        src/twitchgame.hpp
        src/twitchgamereply.cpp src/twitchgamereply.hpp
        src/twitchreply.cpp src/twitchreply.hpp
        src/twitchstream.hpp
        src/twitchstreamreply.cpp src/twitchstreamreply.hpp
        src/twitchuser.hpp
        src/twitchuserreply.cpp src/twitchuserreply.hpp
        src/twitchvideo.hpp
        src/twitchvideoreply.cpp src/twitchvideoreply.hpp
        src/json/json.hpp)

target_include_directories(TwitchQt
    INTERFACE
        $<BUILD_INTERFACE:${TwitchQt_SOURCE_DIR}/TwitchQt>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/TwitchQt>)
