/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHBADGE_HPP
#define TWITCHBADGE_HPP

#include <QString>
#include <QMap>

namespace Twitch {
struct Badge {
    struct Version {
        QString m_id;
        QString m_imageUrl;
        QString m_description;
        QString m_title;
    };
    QString m_setId;
    QVector<Version> m_versions;
};
using Badges = QVector<Badge>;
}

Q_DECLARE_METATYPE(Twitch::Badge);

#endif // TWITCHBADGE_HPP
