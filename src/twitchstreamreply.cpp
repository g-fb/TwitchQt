/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "twitchstreamreply.hpp"

#include <QJsonArray>

Twitch::Stream streamFromJson(const QJsonObject &stream)
{
    QString typeStr = stream["type"].toString();
    Twitch::Stream::StreamType type = Twitch::Stream::StreamType::No;
    if (typeStr == "live") {
        type = Twitch::Stream::StreamType::Live;
    } else {
        type = Twitch::Stream::StreamType::Vodcast;
    }
    QString startedAt = stream["started_at"].toString();

    return Twitch::Stream{stream["id"].toString("-1"),
                          stream["user_id"].toString("-1"),
                          stream["user_name"].toString(),
                          stream["game_id"].toString("-1"),
                          stream["community_ids"].toString().split(",").toVector(),
                          type,
                          stream["title"].toString(),
                          stream["viewer_count"].toInt(-1),
                          QDateTime::fromString(startedAt, Qt::ISODate),
                          stream["language"].toString(),
                          stream["thumbnail_url"].toString()};
}

namespace Twitch
{
void StreamReply::parseData(const QJsonObject &json)
{
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toArray();
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
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toArray();
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
