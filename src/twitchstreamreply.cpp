/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "twitchstreamreply.hpp"

#include <QJsonArray>

namespace Twitch {
void StreamReply::parseData(const QJsonObject& json)
{
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        if (!data.toObject().isEmpty()) {
            const auto& stream = data.toObject();

            QString typeStr = stream["type"].toString();
            Stream::StreamType type = Stream::StreamType::No;
            if (typeStr == "live")
                type = Stream::StreamType::Live;
            else if (typeStr == "vodcast")
                type = Stream::StreamType::Vodcast;
            QString startedAt = stream["started_at"].toString();

            m_data.setValue(Stream {
                                stream["id"].toString("-1"),
                                stream["user_id"].toString("-1"),
                                stream["user_name"].toString(),
                                stream["game_id"].toString("-1"),
                                stream["community_ids"].toString().split(",").toVector(),
                                type,
                                stream["title"].toString(),
                                stream["viewer_count"].toInt(-1),
                                QDateTime::fromString(startedAt, Qt::ISODate),
                                stream["language"].toString(),
                                stream["thumbnail_url"].toString()
                            });
        } else {
            // ???
        }
    }
}

void StreamsReply::parseData(const QJsonObject &json)
{
    Streams streams;
    if (json.find("data") != json.end()) {
        const auto& data = json["data"].toArray();
        for (const auto& stream : data) {
            auto streamObject = stream.toObject();
            QString typeStr = streamObject["type"].toString();

            Stream::StreamType type = Stream::StreamType::No;
            if (typeStr == "live")
                type = Stream::StreamType::Live;
            else if (typeStr == "vodcast")
                type = Stream::StreamType::Vodcast;
            QString startedAt = streamObject["started_at"].toString();

            streams.push_back(Stream {
                                  streamObject["id"].toString("-1"),
                                  streamObject["user_id"].toString("-1"),
                                  streamObject["user_name"].toString(),
                                  streamObject["game_id"].toString("-1"),
                                  streamObject["community_ids"].toString().split(",").toVector(),
                                  type,
                                  streamObject["title"].toString(),
                                  streamObject["viewer_count"].toInt(-1),
                                  QDateTime::fromString(startedAt, Qt::ISODate),
                                  streamObject["language"].toString(),
                                  streamObject["thumbnail_url"].toString()
                              });

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
