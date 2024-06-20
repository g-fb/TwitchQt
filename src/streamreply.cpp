/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "streamreply.hpp"

#include <QJsonArray>

Twitch::Stream streamFromJson(const QJsonObject &stream)
{
    QString typeStr = stream[u"type"_qs].toString();
    Twitch::Stream::StreamType type = Twitch::Stream::StreamType::No;
    if (typeStr == u"live"_qs) {
        type = Twitch::Stream::StreamType::Live;
    } else {
        type = Twitch::Stream::StreamType::Vodcast;
    }
    QString startedAt = stream[u"started_at"_qs].toString();

    return Twitch::Stream{stream[u"id"_qs].toString(u"-1"_qs),
                          stream[u"user_id"_qs].toString(u"-1"_qs),
                          stream[u"user_login"_qs].toString(),
                          stream[u"user_name"_qs].toString(),
                          stream[u"game_id"_qs].toString(u"-1"_qs),
                          stream[u"game_name"_qs].toString(),
                          stream[u"community_ids"_qs].toString().split(u","_qs).toList(),
                          type,
                          stream[u"title"_qs].toString(),
                          stream[u"viewer_count"_qs].toInt(-1),
                          QDateTime::fromString(startedAt, Qt::ISODate),
                          stream[u"language"_qs].toString(),
                          stream[u"thumbnail_url"_qs].toString(),
                          stream[u"is_mature"_qs].toBool(false)};
}

namespace Twitch
{
void StreamReply::parseData(const QJsonObject &json)
{
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
        if (!data.isEmpty()) {
            m_data.setValue(streamFromJson(data.first().toObject()));
        } else {
            // ???
        }
    }
}

void StreamsReply::parseData(const QJsonObject &json)
{
    Streams streams;
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
        for (const auto &stream : data) {
            streams.push_back(streamFromJson(stream.toObject()));

            m_combinedViewerCount += streams.back().m_viewerCount;
        }
    }
    m_data.setValue(streams);
}

int StreamsReply::combinedViewerCount() const
{
    return m_combinedViewerCount;
}

Twitch::Stream Twitch::StreamReply::stream()
{
    return m_data.value<Twitch::Stream>();
}

Twitch::Streams Twitch::StreamsReply::streams()
{
    return m_data.value<Twitch::Streams>();
}
}

#include "moc_streamreply.cpp"
