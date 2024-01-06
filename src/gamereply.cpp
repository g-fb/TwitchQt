/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "gamereply.hpp"

#include <QJsonArray>

Twitch::Game gameFromJson(const QJsonObject &game)
{
    return Twitch::Game{game[u"id"_qs].toString(), game[u"name"_qs].toString(), game[u"box_art_url"_qs].toString()};
}

namespace Twitch
{
void GameReply::parseData(const QJsonObject &json)
{
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
        if (!data.isEmpty()) {
            m_data.setValue(gameFromJson(data.first().toObject()));
        }
    } else {
        // ??
    }
}

void GamesReply::parseData(const QJsonObject &json)
{
    Games games;
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
        for (const auto &game : data) {
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

void BoxArtReply::parseData(const QByteArray &data)
{
    m_data.setValue(QImage::fromData(data));
}
}

#include "moc_gamereply.cpp"
