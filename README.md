<!--
SPDX-FileCopyrightText: 2018 Kamil Jakubus

SPDX-License-Identifier: MIT
-->

# TwitchQt

_Fork of https://github.com/jkbz64/TwitchQt_

A simple Header-only Qt wrapper for the new Twitch.tv api(Helix) written using Qt5. It also supports TwitchEmotes, BTTV and FFZ.

## Requirements
#### Qt5
- Core
- Gui
- Network

## Usage
#### CMake
```
include(FetchContent)
...
FetchContent_Declare(
    TwitchQt
    GIT_REPOSITORY "https://github.com/g-fb/TwitchQt"
    GIT_TAG "commit hash"
    )
FetchContent_MakeAvailable(TwitchQt)
```
Or clone the repo in your project and add `add_subdirectory(path/to/TwitchQt)` in your CMakeLists.txt.

## Example
```cpp
#include <QCoreApplication>
#include <TwitchQt/Twitch>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    
    // Twitch::Api is aliased to New Twitch Api (Helix)
    Twitch::Api api("CLIENT-ID HERE");
    api.setBearerToken("BEARER-TOKEN HERE");

    // Note: For this endpoint stream has to be online to return the data! 
    auto reply = api.getStreamByName("forsen");
    a.connect(reply, &Twitch::Reply::finished, [&a, reply]() {
        if (reply->currentState() == Twitch::ReplyState::Success) {
            // Convert reply data to Stream struct
            auto stream = reply->data().value<Twitch::Stream>();
            qDebug() << stream.m_title; // Print title
            qDebug() << stream.m_viewerCount; // Print viewer count as int
            qDebug() << stream.m_startedAt.date(); // Print date
        }
        // Quit program after receiving reply
        a.quit();
    });

    a.exec();

    return 0;
}

```

## Features
Look at the [twitchapi.hpp](https://github.com/g-fb/TwitchQt/blob/master/src/twitchapi.hpp) file :)
