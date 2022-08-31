/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "twitchvideoreply.hpp"

#include <QJsonArray>

namespace Twitch {
void VideoReply::parseData(const QJsonObject &json)
{
    if (json.find("data") != json.end()) {
        const auto& data = json["data"];
        if (!data.toObject().isEmpty()) {
            const auto& video = data.toObject();

            QList<MutedSegment> mutedSegmentsList;
            if (video.find("muted_segments") != video.end()) {
                const auto& mutedSegments = video["muted_segments"].toArray();
                for (const auto& segment : mutedSegments) {
                    auto segmentObject = segment.toObject();
                    if (segmentObject.find("duration") == segmentObject.end()) {
                        continue;
                    }
                    MutedSegment ms;
                    ms.duration = segmentObject["duration"].toInt(-1);
                    ms.offset = segmentObject["offset"].toInt(-1);
                    mutedSegmentsList.append(ms);
                }
            }

            QString typeStr = video["type"].toString();
            Video::VideoType type;
            if (typeStr == "upload")
                type = Video::VideoType::Upload;
            else if (typeStr == "archive")
                type = Video::VideoType::Archive;
            else
                type = Video::VideoType::Highlight;

            QString createdAt = video["created_at"].toString();
            QString publishedAt = video["published_at"].toString();

            m_data.setValue(Video {
                                video["id"].toString("-1"),
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
                                mutedSegmentsList
                            });
        } else {
            // ???
        }
    }
}

void VideosReply::parseData(const QJsonObject& json)
{
    Videos videos;
    if (json.find("data") != json.end()) {
        const auto& data = json["data"].toArray();
        for (const auto& video : data) {
            auto videoObject = video.toObject();
            QList<MutedSegment> mutedSegmentsList;
            if (videoObject.find("muted_segments") != videoObject.end()) {
                const auto& mutedSegments = videoObject["muted_segments"].toArray();
                for (const auto& segment : mutedSegments) {
                    auto segmentObject = segment.toObject();
                    if (segmentObject.find("duration") == segmentObject.end()) {
                        continue;
                    }
                    MutedSegment ms;
                    ms.duration = segmentObject["duration"].toInt(-1);
                    ms.offset = segmentObject["offset"].toInt(-1);
                    mutedSegmentsList.append(ms);
                }
            }

            QString typeStr = videoObject["type"].toString();
            Video::VideoType type;
            if (typeStr == "upload")
                type = Video::VideoType::Upload;
            else if (typeStr == "archive")
                type = Video::VideoType::Archive;
            else
                type = Video::VideoType::Highlight;

            QString createdAt = videoObject["created_at"].toString();
            QString publishedAt = videoObject["published_at"].toString();

            videos.push_back(Video {
                                 videoObject["id"].toString("-1"),
                                 videoObject["stream_id"].toString("-1"),
                                 videoObject["user_id"].toString("-1"),
                                 videoObject["user_login"].toString(),
                                 videoObject["user_name"].toString(),
                                 videoObject["title"].toString(),
                                 videoObject["description"].toString(),
                                 QDateTime::fromString(createdAt, Qt::ISODate),
                                 QDateTime::fromString(publishedAt, Qt::ISODate),
                                 videoObject["url"].toString(),
                                 videoObject["thumbnail_url"].toString(),
                                 videoObject["viewable"].toString(),
                                 videoObject["view_count"].toInt(-1),
                                 videoObject["language"].toString(),
                                 type,
                                 videoObject["duration"].toString(),
                                 mutedSegmentsList
                             });
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
