/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHSTREAM_HPP
#define TWITCHSTREAM_HPP

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QList>

namespace Twitch
{
/// Models
struct Stream {
    enum class StreamType { No, Live, Vodcast };
    QString m_id;
    QString m_userId;
    QString m_userLogin;
    QString m_userName;
    QString m_gameId;
    QString m_gameName;
    QList<QString> m_communityIds;
    StreamType m_type;
    QString m_title;
    int m_viewerCount{-1};
    QDateTime m_startedAt;
    QString m_language;
    QString m_thumbnailUrl;
    bool m_isMature{false};
};

using Streams = QList<Twitch::Stream>;
}

Q_DECLARE_METATYPE(Twitch::Stream)
Q_DECLARE_METATYPE(Twitch::Streams)

#endif // TWITCHSTREAM_HPP
