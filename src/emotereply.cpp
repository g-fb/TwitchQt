/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "emotereply.hpp"

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
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
        for (const auto &emote : data) {
            auto emoteObject = emote.toObject();
            Emote e;
            e.m_id = emoteObject[u"id"_qs].toString();
            e.m_name = emoteObject[u"name"_qs].toString();
            e.m_images = emoteObject[u"images"_qs].toObject().toVariantMap();
            e.m_format = emoteObject[u"format"_qs].toArray().toVariantList();
            e.m_scale = emoteObject[u"scale"_qs].toArray().toVariantList();
            e.m_themeMode = emoteObject[u"theme_mode"_qs].toArray().toVariantList();
            emotes.insert(e.m_name, e);
        }
    }
    m_data.setValue(emotes);
}
}

#include "moc_emotereply.cpp"
