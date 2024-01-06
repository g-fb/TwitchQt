/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "videoreply.hpp"

#include <QJsonArray>

Twitch::Video videoFromJson(const QJsonObject &video)
{
    QList<Twitch::MutedSegment> mutedSegmentsList;
    if (video.find(u"muted_segments"_qs) != video.end()) {
        const auto &mutedSegments = video[u"muted_segments"_qs].toArray();
        for (const auto &segment : mutedSegments) {
            auto segmentObject = segment.toObject();
            if (segmentObject.find(u"duration"_qs) == segmentObject.end()) {
                continue;
            }
            Twitch::MutedSegment ms;
            ms.duration = segmentObject[u"duration"_qs].toInt(-1);
            ms.offset = segmentObject[u"offset"_qs].toInt(-1);
            mutedSegmentsList.append(ms);
        }
    }

    QString typeStr = video[u"type"_qs].toString();
    Twitch::Video::VideoType type;
    if (typeStr == u"upload"_qs) {
        type = Twitch::Video::VideoType::Upload;
    } else if (typeStr == u"archive"_qs) {
        type = Twitch::Video::VideoType::Archive;
    } else {
        type = Twitch::Video::VideoType::Highlight;
    }

    QString createdAt = video[u"created_at"_qs].toString();
    QString publishedAt = video[u"published_at"_qs].toString();

    return Twitch::Video{video[u"id"_qs].toString(u"-1"_qs),
                         video[u"stream_id"_qs].toString(u"-1"_qs),
                         video[u"user_id"_qs].toString(u"-1"_qs),
                         video[u"user_login"_qs].toString(),
                         video[u"user_name"_qs].toString(),
                         video[u"title"_qs].toString(),
                         video[u"description"_qs].toString(),
                         QDateTime::fromString(createdAt, Qt::ISODate),
                         QDateTime::fromString(publishedAt, Qt::ISODate),
                         video[u"url"_qs].toString(),
                         video[u"thumbnail_url"_qs].toString(),
                         video[u"viewable"_qs].toString(),
                         video[u"view_count"_qs].toInt(-1),
                         video[u"language"_qs].toString(),
                         type,
                         video[u"duration"_qs].toString(),
                         mutedSegmentsList};
}

namespace Twitch
{
void VideoReply::parseData(const QJsonObject &json)
{
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
        if (!data.isEmpty()) {
            m_data.setValue(videoFromJson(data.first().toObject()));
        } else {
            // ???
        }
    }
}

void VideosReply::parseData(const QJsonObject &json)
{
    Videos videos;
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
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

#include "moc_videoreply.cpp"
