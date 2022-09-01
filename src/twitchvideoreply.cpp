/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "twitchvideoreply.hpp"

#include <QJsonArray>

Twitch::Video videoFromJson(const QJsonObject &video)
{
    QList<Twitch::MutedSegment> mutedSegmentsList;
    if (video.find("muted_segments") != video.end()) {
        const auto &mutedSegments = video["muted_segments"].toArray();
        for (const auto &segment : mutedSegments) {
            auto segmentObject = segment.toObject();
            if (segmentObject.find("duration") == segmentObject.end()) {
                continue;
            }
            Twitch::MutedSegment ms;
            ms.duration = segmentObject["duration"].toInt(-1);
            ms.offset = segmentObject["offset"].toInt(-1);
            mutedSegmentsList.append(ms);
        }
    }

    QString typeStr = video["type"].toString();
    Twitch::Video::VideoType type;
    if (typeStr == "upload") {
        type = Twitch::Video::VideoType::Upload;
    } else if (typeStr == "archive") {
        type = Twitch::Video::VideoType::Archive;
    } else {
        type = Twitch::Video::VideoType::Highlight;
    }

    QString createdAt = video["created_at"].toString();
    QString publishedAt = video["published_at"].toString();

    return Twitch::Video{video["id"].toString("-1"),
                         video["stream_id"].toString("-1"),
                         video["user_id"].toString("-1"),
                         video["user_login"].toString(),
                         video["user_name"].toString(),
                         video["title"].toString(),
                         video["description"].toString(),
                         QDateTime::fromString(createdAt, Qt::ISODate),
                         QDateTime::fromString(publishedAt, Qt::ISODate),
                         video["url"].toString(),
                         video["thumbnail_url"].toString(),
                         video["viewable"].toString(),
                         video["view_count"].toInt(-1),
                         video["language"].toString(),
                         type,
                         video["duration"].toString(),
                         mutedSegmentsList};
}

namespace Twitch
{
void VideoReply::parseData(const QJsonObject &json)
{
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toObject();
        if (!data.isEmpty()) {
            m_data.setValue(videoFromJson(data));
        } else {
            // ???
        }
    }
}

void VideosReply::parseData(const QJsonObject &json)
{
    Videos videos;
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toArray();
        for (const auto &video : data) {
            videos.push_back(videoFromJson(video.toObject()));
        }
    }
    m_data.setValue(videos);
}

int VideosReply::combinedViewerCount() const
{
    return m_combinedViewerCount;
}

Twitch::Video Twitch::VideoReply::video()
{
    return m_data.value<Twitch::Video>();
}

Twitch::Videos Twitch::VideosReply::videos()
{
    return m_data.value<Twitch::Videos>();
}
}
