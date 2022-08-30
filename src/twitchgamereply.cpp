/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#include "twitchgamereply.hpp"

namespace Twitch {
void GameReply::parseData(const JSON& json)
{
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        if (!data.empty()) {
            const auto& game = data.front();
            m_data.setValue(Game{
                game.value("id", QString("-1")),
                game.value("name", QString("ERROR")),
                game.value("box_art_url", QString("")) });
        } else {
            // ??
        }
    }
}

void GamesReply::parseData(const JSON& json)
{
    Games games;
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        for (const auto& game : data) {
            games.push_back({ game.value("id", QString("-1")),
                game.value("name", QString("ERROR")),
                game.value("box_art_url", QString("")) });
        }
    }
    m_data.setValue(games);
}

Twitch::Games Twitch::GamesReply::games()
{
    return m_data.value<Twitch::Games>();
}

void BoxArtReply::parseData(const QByteArray& data)
{
    m_data.setValue(QImage::fromData(data));
}
}
