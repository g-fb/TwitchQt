/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHBADGE_HPP
#define TWITCHBADGE_HPP

#include <QObject>
#include <QString>

namespace Twitch
{
struct Badge {
    struct Version {
        QString m_id;
        QString m_imageUrl;
        QString m_description;
        QString m_title;
    };
    QString m_setId;
    QList<Version> m_versions;
};
using Badges = QList<Badge>;
}

Q_DECLARE_METATYPE(Twitch::Badge)

#endif // TWITCHBADGE_HPP
