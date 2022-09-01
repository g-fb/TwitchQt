/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "twitchgamereply.hpp"

#include <QJsonArray>

Twitch::Game gameFromJson(const QJsonObject& game)
{
    return Twitch::Game {
        game["id"].toString(),
        game["name"].toString(),
        game["box_art_url"].toString()
    };
}

namespace Twitch {
void GameReply::parseData(const QJsonObject &json)
{
    if (json.find("data") != json.end()) {
        const auto& data = json["data"].toObject();
        if (!data.isEmpty()) {
            m_data.setValue(gameFromJson(data));
        }
    } else {
        // ??
    }
}

void GamesReply::parseData(const QJsonObject &json)
{
    Games games;
    if (json.find("data") != json.end()) {
        const auto& data = json["data"].toArray();
        for (const auto& game : data) {
            games.push_back(gameFromJson(game.toObject()));
        }
    } else {
        // ??
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
