/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "twitchemotereply.hpp"

#include <QJsonArray>

namespace Twitch
{
Twitch::Emotes EmotesReply::emotes()
{
    return m_data.value<Twitch::Emotes>();
}

void EmotesReply::parseData(const QJsonObject &json)
{
    Twitch::Emotes emotes;
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toArray();
        for (const auto &emote : data) {
            auto emoteObject = emote.toObject();
            Emote e;
            e.m_id = emoteObject["id"].toString();
            e.m_name = emoteObject["name"].toString();
            e.m_images = emoteObject["images"].toObject().toVariantMap();
            e.m_format = emoteObject["format"].toArray().toVariantList();
            e.m_scale = emoteObject["scale"].toArray().toVariantList();
            e.m_themeMode = emoteObject["theme_mode"].toArray().toVariantList();
            emotes.insert(e.m_name, e);
        }
    }
    m_data.setValue(emotes);
}
}

#include "moc_twitchemotereply.cpp"
